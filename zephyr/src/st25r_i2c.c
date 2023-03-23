#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "st25r.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

LOG_MODULE_DECLARE(ST25R);

int st25r_i2c_init(const struct device *dev)
{
	struct st25r_data *data = dev->data;
	const struct st25r_device_config *config = dev->config;

	if (!device_is_ready(config->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	//data->ctx = &st25r_i2c_ctx;
	//data->ctx->handle = (void *)dev;

	return 0;
}
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */
