#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include "st25r.h"
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)

LOG_MODULE_DECLARE(ST25R);

static struct device *s_dev;
static struct device *s_cs_dev;
static gpio_pin_t s_cs_pin;

int st25r_spi_init(const struct device *dev)
{
	struct st25r_data *data = dev->data;
	struct st25r_device_config *config = dev->config;

	if (!spi_is_ready(&config->spi)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	const struct spi_cs_control* cs = config->spi.config.cs;

	LOG_INF("cs: %p", cs);

	struct device ** pport = &config->spi.config.cs->gpio.port;
	s_cs_dev = *pport;
	s_cs_pin = config->spi.config.cs->gpio.pin;
	*pport = NULL;

	//data->ctx = &st25r_spi_ctx;
	//data->ctx->handle = (void *)dev;

	s_dev = dev;

	return 0;
}

void platform_st25r_spi_select()
{
    LOG_INF("SPI select");
    if (s_cs_dev && device_is_ready(s_cs_dev)) {
        gpio_pin_set(s_cs_dev, s_cs_pin, 1);
    } else {
        LOG_ERR("Unable to access CS");
    }
}

void platform_st25r_spi_deselect()
{
    LOG_INF("SPI deselect");
    if (s_cs_dev && device_is_ready(s_cs_dev)) {
        gpio_pin_set(s_cs_dev, s_cs_pin, 0);
    } else {
        LOG_ERR("Unable to access CS");
    }
}

void platform_st25r_spi_transceive(const uint8_t *txBuf, uint8_t *rxBuf, uint16_t len)
{
    LOG_INF("platform_st25r_spi_transceive %d bytes, txBuf: %p rxBuf: %p", len, txBuf, rxBuf);

    const struct st25r_device_config *config = s_dev->config;
    const struct spi_buf tx_buf[1] = {
            {
                    .buf = txBuf,
                    .len = len,
            },
    };
    const struct spi_buf_set tx = {
            .buffers = tx_buf,
            .count = ARRAY_SIZE(tx_buf),
    };
    const struct spi_buf rx_buf[1] = {
            {
                .buf = rxBuf,
                .len = len,
                },
    };
    const struct spi_buf_set rx = {
            .buffers = rx_buf,
            .count = ARRAY_SIZE(rx_buf),
    };

    if (spi_transceive_dt(&config->spi, &tx, &rx)) {
        LOG_ERR("error with spi write");
    }

}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */
