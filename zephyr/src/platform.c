#include "platform.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "rfal_nfc.h"

LOG_MODULE_DECLARE(st25r, CONFIG_SENSOR_LOG_LEVEL);

K_MUTEX_DEFINE(platform_st25r_comm_mutex);

void platform_st25r_protect_comm()
{
    k_mutex_lock(&platform_st25r_comm_mutex, K_FOREVER);
}

void platform_st25r_unprotect_comm()
{
    k_mutex_unlock(&platform_st25r_comm_mutex);
}

gpio_pin_t platform_st25r_int_pin()
{
    return 0;
}

const struct device *platform_st25r_int_port()
{
    return NULL;
}

void platform_st25r_gloabl_error(const char *file, long line)
{
    printk("ST25R error at %s:%ld\n", file, line);
}

#define DT_DRV_COMPAT st_st25r

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)

static struct device *s_dev = NULL;

struct platform_st25r_device_config {
    struct spi_dt_spec spi;
};

void platform_st25r_spi_transceive(const uint8_t *txBuf, uint8_t *rxBuf, uint16_t len)
{
    printk("platform_st25r_spi_transceive %d bytes\n", len);

    const struct platform_st25r_device_config *config = s_dev->config;

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

    int result = spi_transceive_dt(&config->spi, txBuf ? &tx : NULL, rxBuf ? &rx : NULL);
    //if (result != 0) {
    printk("spi_transceive result %d\n", result);
    //}
}

static int st25r_spi_init(const struct device *dev)
{
    printk("st25r_spi_init\n");

   const struct platform_st25r_device_config *cfg = dev->config;

   if (!spi_is_ready(&cfg->spi)) {
       printk("spi not ready\n");
       return -ENODEV;
   }

   s_dev = dev;

   rfalNfcInitialize();
   return 0;
}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */