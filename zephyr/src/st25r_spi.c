#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include "st25r.h"
#include <zephyr/logging/log.h>

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)

LOG_MODULE_DECLARE(ST25R, CONFIG_SENSOR_LOG_LEVEL);

static struct device *s_dev;

int st25r_spi_init(const struct device *dev)
{
	struct st25r_data *data = dev->data;
	const struct st25r_device_config *config = dev->config;

	if (!spi_is_ready(&config->spi)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	//data->ctx = &st25r_spi_ctx;
	//data->ctx->handle = (void *)dev;

	s_dev = dev;

	return 0;
}

void platform_st25r_spi_transceive(const uint8_t *txBuf, uint8_t *rxBuf, uint16_t len)
{
    printk("platform_st25r_spi_transceive %d bytes\n", len);

    const struct st25r_device_config *config = s_dev->config;
    const struct spi_buf tx_buf[2] = {
            {
                .buf = txBuf,
                .len = len,
                },
    };
    const struct spi_buf_set tx = {
            .buffers = tx_buf,
            .count = sizeof(tx_buf),
    };

    /* TODO deail with non-NULL rxBuf */
    if (spi_write_dt(&config->spi, &tx)) {
        printk("error with spi write %d\n");
    }

}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */
