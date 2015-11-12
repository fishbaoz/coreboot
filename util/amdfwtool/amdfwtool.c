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

	AMD_FW_XHCI,
	AMD_FW_IMC,
	AMD_FW_GEC,

} amd_fw_type;

typedef struct _amd_fw_entry {
	amd_fw_type type;
	char *filename;
} amd_fw_entry;

amd_fw_entry amd_fw_table[] = {
	{ .type = AMD_FW_PSP_PUBKEY },
	{ .type = AMD_FW_PSP_BOOTLOADER },
	{ .type = AMD_FW_PSP_SMU_FIRMWARE },
	{ .type = AMD_FW_PSP_RECOVERY },
	{ .type = AMD_FW_PSP_RTM_PUBKEY },
	{ .type = AMD_FW_PSP_SECURED_OS },
	{ .type = AMD_FW_PSP_NVRAM },
	{ .type = AMD_FW_PSP_SECURED_DEBUG },
	{ .type = AMD_FW_PSP_TRUSTLETS },
	{ .type = AMD_FW_PSP_TRUSTLETKEY },
	{ .type = AMD_FW_PSP_SMU_FIRMWARE2 },
	{ .type = AMD_PSP_FUSE_CHAIN },
	{ .type = AMD_FW_PSP_SMUSCS },

	{ .type = AMD_FW_XHCI },
	{ .type = AMD_FW_IMC },
	{ .type = AMD_FW_GEC },
};

uint32_t integerate_one(uint32_t pos, uint32_t *psp_entry, amd_fw_entry *table)
{
	int fd;
	struct stat fd_stat;

	fd = open (table->filename, O_RDONLY);
	fstat(fd, &fd_stat);
	psp_entry[0] = table->type;
	psp_entry[1] = fd_stat.st_size;
	psp_entry[2] = pos + 0xFF800000;
	psp_entry[3] = 0;

	pos += fd_stat.st_size;
	close (fd);

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

void register_fw_filename(amd_fw_entry *table, amd_fw_type type, char filename[])
{
	int i;

	for (i = 0; i < sizeof(amd_fw_table)/sizeof(amd_fw_entry); i++) {
		if (amd_fw_table[i].type == type) {
			amd_fw_table[i].filename = filename;
			printf("[%d]=%s\n", type, filename);
		}
	}

}

int main(int argc, char **argv)
{
	int c;
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

	current = 0x70100; /* TODO 0x70400 */
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
			register_fw_filename(amd_fw_table, AMD_FW_XHCI, optarg);
			break;
		case 'i':
			register_fw_filename(amd_fw_table, AMD_FW_IMC, optarg);
			break;
		case 'g':
			register_fw_filename(amd_fw_table, AMD_FW_GEC, optarg);
			break;
		case 'p':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_PUBKEY, optarg);
			break;
		case 'b':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_BOOTLOADER, optarg);
			break;
		case 's':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_SMU_FIRMWARE, optarg);
			break;
		case 'r':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_RECOVERY, optarg);
			break;
		case 'k':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_RTM_PUBKEY, optarg);
			break;
		case 'o':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_SECURED_OS, optarg);
			break;
		case 'n':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_NVRAM, optarg);
			break;
		case 'd':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_SECURED_DEBUG, optarg);
			break;
		case 't':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_TRUSTLETS, optarg);
			break;
		case 'u':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_TRUSTLETKEY, optarg);
			break;
		case 'w':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_SMU_FIRMWARE2, optarg);
			break;
		case 'm':
			register_fw_filename(amd_fw_table, AMD_FW_PSP_SMUSCS, optarg);
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

	current = integerate_one(current, entry, &amd_fw_table[0]);
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
//	write(targetfd, romsig, pos-romsig);
	write(targetfd, rom, CONFIG_ROM_SIZE);
	close(targetfd);
	free(rom);
	return 0;
}
