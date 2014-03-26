#include "mec1308.h"

static inline void mec1308_early_init(unsigned port)
{
	/* Enable SMSC UART 0 */
	device_t dev;

	dev = PNP_DEV (port, MEC1308_UART);
	pnp_set_logical_device(dev);
	pnp_set_enable(dev, 0);
	pnp_set_iobase(dev, PNP_IDX_IO0, CONFIG_TTYS0_BASE);
	pnp_set_enable(dev, 1);
}
