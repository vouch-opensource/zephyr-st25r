#include "platform.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "rfal_nfc.h"

#define DT_DRV_COMPAT st_st25r

K_MUTEX_DEFINE(platform_st25r_comm_mutex);

void platform_st25r_protect_comm()
{
    k_mutex_lock(&platform_st25r_comm_mutex, K_FOREVER);
}

void platform_st25r_unprotect_comm()
{
    k_mutex_unlock(&platform_st25r_comm_mutex);
}

#define DW_INST DT_INST(0, st_st25r)

struct st25r_config {
	struct gpio_dt_spec gpio_irq;
//	struct gpio_dt_spec gpio_reset;
//	struct gpio_dt_spec gpio_wakeup;
//	struct gpio_dt_spec gpio_spi_pol;
//	struct gpio_dt_spec gpio_spi_pha;
};

static const struct st25r_config conf = {
	.gpio_irq = GPIO_DT_SPEC_GET_OR(DW_INST, irq_gpios, {0}),
//	.gpio_reset = GPIO_DT_SPEC_GET_OR(DW_INST, reset_gpios, {0}),
//	.gpio_wakeup = GPIO_DT_SPEC_GET_OR(DW_INST, wakeup_gpios, {0}),
//	.gpio_spi_pol = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pol_gpios, {0}),
//	.gpio_spi_pha = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pha_gpios, {0}),
};

gpio_pin_t platform_st25r_int_pin()
{
    return conf.gpio_irq.pin;
}

const struct device *platform_st25r_int_port()
{
    return conf.gpio_irq.port;
}

void platform_st25r_gloabl_error(const char* file, long line)
{
   printk("global error handler %s:%ld\n", file, line);
   for(;;) { }
}

void platform_st25r_spi_transceive(const uint8_t* txBuf, uint8_t* rxBuf, uint16_t len)
{
   printk("platform_st25r_spi_transceive %d bytes\n", len);
}


static int st25r_init(const struct device *dev)
{
   printk("st25r_init\n");
   rfalNfcInitialize();
   return 0;
}

#if DT_NODE_HAS_STATUS(DT_DRV_INST(0), okay)

#define INIT_PRIO 32

DEVICE_DT_INST_DEFINE(0, &st25r_init, NULL,
            NULL, NULL, POST_KERNEL,
            INIT_PRIO, NULL);

#endif /* DT_NODE_HAS_STATUS(DT_DRV_INST(0), okay) */
