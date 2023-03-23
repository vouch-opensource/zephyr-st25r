#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include "st25r.h"
#include <zephyr/logging/log.h>

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)

LOG_MODULE_DECLARE(ST25R, CONFIG_SENSOR_LOG_LEVEL);

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

	return 0;
}
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(spi) */
