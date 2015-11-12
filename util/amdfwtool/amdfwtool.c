/*
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code
 * distributions retain the above copyright notice and this paragraph
 * in its entirety, and (2) distributions including binary code include
 * the above copyright notice and this paragraph in its entirety in
 * the documentation or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND
 * WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT
 * LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE.
 *
 */

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define MAX_PSP_DIRECTORY_SIZE 512
#define ALIGN(val, by) (((val) + (by)-1)&~((by)-1))

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

/*
 * Creates the OSI Fletcher checksum. See 8473-1, Appendix C, section C.3.
 * The checksum field of the passed PDU does not need to be reset to zero.
 *
 * The "Fletcher Checksum" was proposed in a paper by John G. Fletcher of
 * Lawrence Livermore Labs.  The Fletcher Checksum was proposed as an
 * alternative to cyclical redundancy checks because it provides error-
 * detection properties similar to cyclical redundancy checks but at the
 * cost of a simple summation technique.  Its characteristics were first
 * published in IEEE Transactions on Communications in January 1982.  One
 * version has been adopted by ISO for use in the class-4 transport layer
 * of the network protocol.
 *
 * This program expects:
 *    stdin:    The input file to compute a checksum for.  The input file
 *              not be longer than 256 bytes.
 *    stdout:   Copied from the input file with the Fletcher's Checksum
 *              inserted 8 bytes after the beginning of the file.
 *    stderr:   Used to print out error messages.
 */

uint32_t fletcher32 (const uint16_t *pptr, int length)
{

	uint32_t c0;
	uint32_t c1;
	uint32_t checksum;
	int index;

	c0 = 0xFFFF;
	c1 = 0xFFFF;

	printf("%x,%x, len=%x\n", pptr[0], pptr[1], length);
	for (index = 0; index < length; index++) {
		/*
		* Ignore the contents of the checksum field.
		*/
		c0 += *(pptr++);
		c1 += c0;
		if ((index % 360) == 0) {
			c0 = (c0 & 0xFFFF) + (c0 >> 16);	// Sum0 modulo 65535 + the overflow
			c1 = (c1 & 0xFFFF) + (c1 >> 16);	// Sum1 modulo 65535 + the overflow
		}

	}

	c0 = (c0 & 0xFFFF) + (c0 >> 16);	// Sum0 modulo 65535 + the overflow
	c1 = (c1 & 0xFFFF) + (c1 >> 16);	// Sum1 modulo 65535 + the overflow
	checksum = (c1 << 16) | c0;

	return checksum;
}

#define CONFIG_ROM_SIZE 0x800000
typedef enum _amd_fw_type {
	AMD_FW_PSP_PUBKEY = 0,
	AMD_FW_PSP_BOOTLOADER = 1,
	AMD_FW_PSP_SMU_FIRMWARE = 8,
	AMD_FW_PSP_RECOVERY = 3,
	AMD_FW_PSP_RTM_PUBKEY = 5,
	AMD_FW_PSP_SECURED_OS = 2,
	AMD_FW_PSP_NVRAM = 4,
	AMD_FW_PSP_SECURED_DEBUG = 9,
	AMD_FW_PSP_TRUSTLETS = 12,
	AMD_FW_PSP_TRUSTLETKEY = 13,
	AMD_FW_PSP_SMU_FIRMWARE2 = 18,
	AMD_PSP_FUSE_CHAIN = 11,
	AMD_FW_PSP_SMUSCS = 95,

	AMD_FW_IMC,
	AMD_FW_GEC,
	AMD_FW_XHCI,
} amd_fw_type;

typedef struct _amd_fw_entry {
	amd_fw_type type;
	char *filename;
	uint32_t offset;	/* TODO: delete */
} amd_fw_entry;

amd_fw_entry amd_psp_fw_table[] = {
	{ /*.offset = 0xff870100,*/ .type = AMD_FW_PSP_PUBKEY },
	{ /*.offset = 0xff870780,*/ .type = AMD_FW_PSP_BOOTLOADER },
	{ /*.offset = 0xff8a83a0,*/ .type = AMD_FW_PSP_SMU_FIRMWARE },
	{ /*.offset = 0xff876d80,*/ .type = AMD_FW_PSP_RECOVERY },
	{ /*.offset = 0xff8703c0,*/ .type = AMD_FW_PSP_RTM_PUBKEY },
	{ /*.offset = 0xff87c2a0,*/ .type = AMD_FW_PSP_SECURED_OS },
	{ /*.offset = 0xff898320,*/ .type = AMD_FW_PSP_NVRAM },
	{ /*.offset = 0xff8bbfc0,*/ .type = AMD_FW_PSP_SECURED_DEBUG },
	{ /*.offset = 0xff8bc380,*/ .type = AMD_FW_PSP_TRUSTLETS },
	{ /*.offset = 0xff8d2b20,*/ .type = AMD_FW_PSP_TRUSTLETKEY },
	{ /*.offset = 0xff8d2ee0,*/ .type = AMD_FW_PSP_SMU_FIRMWARE2 },
	{ /*.offset = 0xff8bb360,*/ .type = AMD_FW_PSP_SMUSCS },
	{ /*.offset = 0x0,       */ .type = AMD_PSP_FUSE_CHAIN },
};
amd_fw_entry amd_fw_table[] = {
	{ /*.offset = 0xff821060, */ .type = AMD_FW_XHCI },
	{ /*.offset = 0xff840000, */ .type = AMD_FW_IMC },
	{ /*.offset = 0,          */ .type = AMD_FW_GEC },
};

//uint32_t integerate_one(uint32_t pos, uint32_t *psp_entry, amd_fw_entry *table)
void fill_psp_head(uint32_t *pspdir, int count)
{
	pspdir[0] = 1347637284;//'PSP$';
	pspdir[2] = count;		/* size */
	pspdir[3] = 0;
	pspdir[1] = fletcher32((uint16_t *)&pspdir[1], (count *16 + 16)/2 - 2);
}
uint32_t integerate_one_fw(void *base, uint32_t pos, uint32_t *romsig, int i)
{
	int fd;
	struct stat fd_stat;
	printf("integrate %x, %s, pos=%x\n", amd_fw_table[i].type, amd_fw_table[i].filename, pos);

	if (amd_fw_table[i].filename != NULL) {
		fd = open (amd_fw_table[i].filename, O_RDONLY);
		fstat(fd, &fd_stat);

		//pos = amd_fw_table[i].offset - 0xFF800000; /* TODO: delete */
		//printf("amd_fw_table[i].offset=%x, pos=%x\n", amd_fw_table[i].offset, pos);

		switch (amd_fw_table[i].type) {
		case AMD_FW_IMC:
			romsig[1] = pos + 0xFF800000;
			break;
		case AMD_FW_GEC:
			romsig[2] = pos + 0xFF800000;
			break;
		case AMD_FW_XHCI:
			romsig[3] = pos + 0xFF800000;
			break;
		default:
			/* Error */
			break;
		}
		read (fd, base+pos, fd_stat.st_size);

		pos += fd_stat.st_size;
		pos = ALIGN(pos, 0x100);
		close (fd);
	}

	return pos;
}

uint32_t integerate_one_psp(void *base, uint32_t pos, uint32_t *pspdir, int i)
{
	int fd;
	struct stat fd_stat;

//	printf("integrate %x, %s, ", amd_psp_fw_table[i].type, amd_psp_fw_table[i].filename);
	pspdir[4+4*i+0] = amd_psp_fw_table[i].type;

	if (amd_psp_fw_table[i].type == AMD_PSP_FUSE_CHAIN) {
		pspdir[4+4*i+1] = 0xFFFFFFFF;
		pspdir[4+4*i+2] = 1;
		pspdir[4+4*i+3] = 0;
	} else if (amd_psp_fw_table[i].filename != NULL) {
		fd = open (amd_psp_fw_table[i].filename, O_RDONLY);
		fstat(fd, &fd_stat);
		pspdir[4+4*i+1] = fd_stat.st_size;
		//pos = amd_psp_fw_table[i].offset - 0xFF800000; /* TODO: delete */
		//printf("pos=%x\n", pos);
		pspdir[4+4*i+2] = pos + 0xFF800000;
		pspdir[4+4*i+3] = 0;

		read (fd, base+pos, fd_stat.st_size);

		pos += fd_stat.st_size;
		pos = ALIGN(pos, 0x100);
		close (fd);
	}
	return pos;
}

static const char *optstring  = "x:i:g:p:b:s:r:k:o:n:d:t:u:w:m:h";
static struct option long_options[] = {
	{"xhci",         required_argument, 0, 'x' },
	{"imc",          required_argument, 0, 'i' },
	{"gec",          required_argument, 0, 'g' },
	/* PSP */
	{"pubkey",       required_argument, 0, 'p' },
	{"bootloader",   required_argument, 0, 'b' },
	{"smufirmware",  required_argument, 0, 's' },
	{"recovery",     required_argument, 0, 'r' },
	{"rtmpubkey",    required_argument, 0, 'k' },
	{"secureos",     required_argument, 0, 'o' },
	{"nvram",        required_argument, 0, 'n' },
	{"securedebug",  required_argument, 0, 'd' },
	{"trustlets",    required_argument, 0, 't' },
	{"trustletkey",  required_argument, 0, 'u' },
	{"smufirmware2", required_argument, 0, 'w' },
	{"smuscs",       required_argument, 0, 'm' },

///	{"verbose",      no_argument,       0, 'v' },
	{"help",         no_argument,       0, 'h' },
	{NULL,           0,                 0,  0  }
};

void register_fw_filename(amd_fw_type type, char filename[])
{
	int i;

	for (i = 0; i < sizeof(amd_psp_fw_table)/sizeof(amd_fw_entry); i++) {
		if (amd_psp_fw_table[i].type == type) {
			amd_psp_fw_table[i].filename = filename;
			printf("[%d]=%s\n", type, filename);
		}
	}
	for (i = 0; i < sizeof(amd_fw_table)/sizeof(amd_fw_entry); i++) {
		if (amd_fw_table[i].type == type) {
			amd_fw_table[i].filename = filename;
			printf("[%d]=%s\n", type, filename);
		}
	}
}

int main(int argc, char **argv)
{
	int c, i;
	uint32_t checksum = 0xFFFFFFFF;
	struct stat filestat = {};
	int retcode = EINVAL;
	size_t filesize = 0;
	char debugoption[] = "--print";

	void *rom = NULL;
	uint32_t current;
	uint32_t *amd_romsig, *pspdir, *entry;
	uint32_t rombase=0xFF800000;

	int targetfd;

	rom = malloc(CONFIG_ROM_SIZE);
	memset (rom, 0xFF, CONFIG_ROM_SIZE);
	if (!rom) {
		//ERROR("Couldn't allocate memory for microcode update entries.\n");
		return 1;
	}
	amd_romsig = rom + 0x20000;
	amd_romsig[0] = 0x55AA55AA; /* romsig */
	amd_romsig[1] = 0;	    /* IMC */
	amd_romsig[2] = 0;	    /* GEC */
	amd_romsig[3] = 0;	    /* XHCI */
	amd_romsig[4] = 0;	    /* PSP */

	pspdir = rom + 0x70000;
	pspdir[0] = 1347637284;//'PSP$';
	pspdir[1] = 0;		/* fletcher */
	pspdir[2] = 0;		/* size */
	pspdir[3] = 0;

	current = 0x20100; /* TODO 0x70400 */
	entry = &pspdir[4];

	//current = integerate_one(current, pspdir, entryb,
	while (1) {
		int optindex = 0;

		c = getopt_long(argc, argv, optstring, long_options, &optindex);

		printf("option=%c,%x\n", c, c);
		if (c == -1)
			break;

		switch (c) {
		case 'x':
			register_fw_filename(AMD_FW_XHCI, optarg);
			break;
		case 'i':
			register_fw_filename(AMD_FW_IMC, optarg);
			break;
		case 'g':
			register_fw_filename(AMD_FW_GEC, optarg);
			break;
		case 'p':
			register_fw_filename(AMD_FW_PSP_PUBKEY, optarg);
			break;
		case 'b':
			register_fw_filename(AMD_FW_PSP_BOOTLOADER, optarg);
			break;
		case 's':
			register_fw_filename(AMD_FW_PSP_SMU_FIRMWARE, optarg);
			break;
		case 'r':
			register_fw_filename(AMD_FW_PSP_RECOVERY, optarg);
			break;
		case 'k':
			register_fw_filename(AMD_FW_PSP_RTM_PUBKEY, optarg);
			break;
		case 'o':
			register_fw_filename(AMD_FW_PSP_SECURED_OS, optarg);
			break;
		case 'n':
			register_fw_filename(AMD_FW_PSP_NVRAM, optarg);
			break;
		case 'd':
			register_fw_filename(AMD_FW_PSP_SECURED_DEBUG, optarg);
			break;
		case 't':
			register_fw_filename(AMD_FW_PSP_TRUSTLETS, optarg);
			break;
		case 'u':
			register_fw_filename(AMD_FW_PSP_TRUSTLETKEY, optarg);
			break;
		case 'w':
			register_fw_filename(AMD_FW_PSP_SMU_FIRMWARE2, optarg);
			break;
		case 'm':
			register_fw_filename(AMD_FW_PSP_SMUSCS, optarg);
			break;

//		case 'h':
//			usage(argv[0]);
//			return 1;
		case 'v':
//			verbose++;
			break;
		default:
			break;
		}
	}

	for (i=0; i< sizeof(amd_fw_table) / sizeof(amd_fw_entry); i ++) {
		current = integerate_one_fw(rom, current, amd_romsig, i);
		entry += 32;
	}

	current = ALIGN(current, 0x10000);
	//current = 0x70000;	/* TODO: Delete */
	pspdir = rom + current;
	amd_romsig[4] = current + 0xFF800000;
	current += 0x400;
	for (i=0; i< sizeof(amd_psp_fw_table) / sizeof(amd_fw_entry); i ++) {
		current = integerate_one_psp(rom, current, pspdir, i);
		entry += 32;
	}

	fill_psp_head(pspdir, i);
#if 0
	uint16_t buffer[MAX_PSP_DIRECTORY_SIZE / sizeof(uint16_t)];

	retcode = fstat(fileno(stdin), &filestat);
	filesize = filestat.st_size;
	if (retcode < 0) {
		perror("FLETCHER32");
		return errno;
	} else if (!((12 < filesize) && (filesize <= sizeof(buffer)))) {
		fprintf(stderr, "FLETCHER32: input file is not valid for this program.\n");
		return EINVAL;
	}
	retcode = read(fileno(stdin), (void *)buffer, filesize);
	if (retcode < 0) {
		perror("FLETCHER32");
		return errno;
	}

	checksum = fletcher32(&buffer[2], filesize/2 - 2);
	*((uint32_t *)& buffer[2]) = checksum;
#ifndef DEBUG
	if ((argc == 2) && (strcmp(argv[1], debugoption) == 0)) {
#endif
		fprintf(stderr, "Fletcher's Checksum: %x\n", checksum);
#ifndef DEBUG
	}
#endif

	retcode = write(fileno(stdout), buffer, filesize);
	if (retcode < 0) {
		perror("FLETCHER32");
		return errno;
	}
#endif
	targetfd = open("amdfw.rom", O_RDWR | O_CREAT, 0666);
	write(targetfd, amd_romsig, current - 0x20000);
//	write(targetfd, rom, CONFIG_ROM_SIZE);
	close(targetfd);
	free(rom);
	return 0;
}
