

#include <boot/coreboot_tables.h>
#include <console/console.h>	/* for __console definition */
#include <console/uart.h>
#include <drivers/uart/uart8250reg.h>

#define ACPI_MMIO_BASE 0xFED80000
#define AOAC_BASE 0x1E00
uintptr_t uart_platform_base(int idx)
{
	return (uintptr_t)(0xFEDC6000 + 0x2000 * (idx & 1));
}

unsigned int uart_platform_refclk(void)
{
	return 48000000;
}

void uart_fill_lb(void *data)
{
	struct lb_serial serial;
	serial.type = LB_SERIAL_TYPE_MEMORY_MAPPED;
	serial.baseaddr = uart_platform_base(CONFIG_UART_FOR_CONSOLE);
	serial.baud = default_baudrate();
	serial.regwidth = 1;
	lb_add_serial(&serial, data);

	lb_add_console(LB_TAG_CONSOLE_SERIAL8250MEM, data);
}
