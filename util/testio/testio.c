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
	ptr = mmap(0, 0x4000, PROT_READ, MAP_SHARED, fd_mem, (off_t)0xf00d8000);
	if (ptr == MAP_FAILED) {
		/** could not map the system memory, exit with a message */
		perror("Mapping system memory failed: ");
		close(fd_mem);
		return (1);
	}
	if (ptr) {
		*(unsigned int *)ptr= 0x55aa55aa;
		printf("ptr=%x", *(unsigned int *)ptr);
	} else {
		printf("invalid memory pointer\n");
	}
	munmap(ptr, 0xf00d8000);
	/** Close the file handle */
	close(fd_mem);

	iopl(3);
	uart_init();
	uart_tx_byte('U');
	uart_tx_byte('U');
	uart_tx_byte('U');
	uart_tx_byte('U');
	/** return 0 as OK ) */
	return 0;
}
