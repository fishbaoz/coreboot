#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


#if defined (__sun) && (defined(__i386) || defined(__amd64))
#  define MEM_DEV "/dev/xsvc"
#else
#  define MEM_DEV "/dev/mem"
#endif

int main(int argc, char* argv[])
{
	char* ptr;
	int fd_mem = -1;
	struct irq_routing_table* rt = NULL;
	void* bios_image = NULL;
	if ( argc > 1 )
	{
		/** there a paramater passed to the program, assume that it is a menory file */
		printf("Opening memory image file '%s'\n", argv[1]);
		/** Open the file */
		fd_mem = open(argv[1], O_RDONLY);
		if (fd_mem > 0) {
			/** get tyhe size of the file */
			int file_size = lseek(fd_mem, 0, SEEK_END);
			printf("Memory image '%i'\n", file_size);
			/** get a memory block for it. */
			bios_image = malloc(file_size);
			if (bios_image) {
				/** Fill the created buffer */
				lseek(fd_mem, 0, SEEK_SET);
				read(fd_mem, bios_image, file_size);
				/** set the pointer for the probe function */
				ptr = (char*)bios_image;
			} else {
				/* no memory available ? */
				perror("Failed to open imagefile\n");
				return (-3);
			}
		} else {
			/** An error occourd, just exit with a message */
			perror("Failed to open imagefile");
			return (-2);
		}
	} else {
		/** No paramaters means that the program will access the system memory */
		printf("Accessing memory\n");
		if (getuid()) {
			/** i'm not root message !!!! */
			fprintf(stderr, "Run me as root, I need access to " MEM_DEV ".\n");
		}
		/** open the system memory */
		fd_mem = open(MEM_DEV, O_RDONLY);
		if (fd_mem < 0) {
			/** could not open the system memory, exit with a message */
			perror("Could not open " MEM_DEV ":");
			exit(1);
		}
		printf("Probing PIRQ table in memory.\n");
		ptr = mmap(0, 0x10000, PROT_READ, MAP_SHARED, fd_mem, (off_t)0xf0000);
		if (ptr == MAP_FAILED) {
			/** could not map the system memory, exit with a message */
			perror("Mapping system memory failed: ");
			close(fd_mem);
			return (1);
		}
	}
	if (ptr) {
		/** now do the actual probe function */
		rt = probe_table(ptr);
		if (rt != NULL && bios_image == NULL) {
			/** when probing system memory we write the 'irq_tables.c' code file */
			printf("Creating irq_tables.c ...\n");
			code_gen("irq_tables.c", rt);
			printf("Done, you can move the file to the coreboot tree now.\n");
		}
	} else {
		printf("invalid memory pointer\n");
	}
	if (bios_image) {
		/** when we are reading from a file the memory must be released */
		free(bios_image);
	} else {
		/** when reading from the system memory, it must be unmapped */
		munmap(ptr, 0x10000);
	}
	/** Close the file handle */
	close(fd_mem);
	/** return 0 as OK ) */
	return 0;
}
