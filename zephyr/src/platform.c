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

#define ST25R_INST DT_INST(0, st_st25r)
#define ST25R_SPI	DT_PARENT(DT_INST(0, st_st25r))

static const struct device* spi;
static struct spi_cs_control* cs_ctrl = SPI_CS_CONTROL_PTR_DT(ST25R_INST, 0);
static struct spi_config spi_cfgs[1] = {0};
static struct spi_config* spi_cfg;

struct st25r_config {
	struct gpio_dt_spec gpio_irq;
//	struct gpio_dt_spec gpio_reset;
//	struct gpio_dt_spec gpio_wakeup;
//	struct gpio_dt_spec gpio_spi_pol;
//	struct gpio_dt_spec gpio_spi_pha;
};

static const struct st25r_config conf = {
	.gpio_irq = GPIO_DT_SPEC_GET_OR(ST25R_INST, irq_gpios, {0}),
//	.gpio_reset = GPIO_DT_SPEC_GET_OR(ST25R_INST, reset_gpios, {0}),
//	.gpio_wakeup = GPIO_DT_SPEC_GET_OR(ST25R_INST, wakeup_gpios, {0}),
//	.gpio_spi_pol = GPIO_DT_SPEC_GET_OR(ST25R_INST, spi_pol_gpios, {0}),
//	.gpio_spi_pha = GPIO_DT_SPEC_GET_OR(ST25R_INST, spi_pha_gpios, {0}),
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
   printk("ST25R error at %s:%ld\n", file, line);
}

void platform_st25r_spi_transceive(const uint8_t *txBuf, uint8_t *rxBuf, uint16_t len)
{
    printk("platform_st25r_spi_transceive %d bytes\n", len);

    const struct spi_buf tx_buf[1] = {
            {
                    .buf = (void *) txBuf,
                    .len = len,
            },
    };
    const struct spi_buf_set tx = {
            .buffers = tx_buf,
            .count = ARRAY_SIZE(tx_buf),
    };

    const struct spi_buf rx_buf[1] = {
            {
                    .buf = (void *) rxBuf,
                    .len = len,
            },
    };
    const struct spi_buf_set rx = {
            .buffers = rx_buf,
            .count = ARRAY_SIZE(rx_buf),
    };

    int result = spi_transceive(spi, spi_cfg, &tx, &rx);
    if (result != 0) {
        printk("spi_transceive result %d\n", result);
    }
}

static int st25r_init(const struct device *dev)
{
   printk("st25r_init\n");

   /* set common SPI config */
   for (int i = 0; i < ARRAY_SIZE(spi_cfgs); i++) {
       spi_cfgs[i].cs = cs_ctrl;
       spi_cfgs[i].operation = SPI_WORD_SET(8);
       spi_cfgs[i].frequency = 2000000;
   }

   spi_cfg = &spi_cfgs[0];

   spi = DEVICE_DT_GET(ST25R_SPI);
   if (!spi) {
       printk("ST25R SPI binding failed\n");
       return -1;
   } else {
       printk("ST25R on %s (max %dMHz)\n", dev->name,
               spi_cfgs[0].frequency / 1000000);
   }

   rfalNfcInitialize();
   return 0;
}

#if DT_NODE_HAS_STATUS(DT_DRV_INST(0), okay)

#define INIT_PRIO 32

DEVICE_DT_INST_DEFINE(0, &st25r_init, NULL,
            NULL, NULL, POST_KERNEL,
            INIT_PRIO, NULL);

#endif /* DT_NODE_HAS_STATUS(DT_DRV_INST(0), okay) */
