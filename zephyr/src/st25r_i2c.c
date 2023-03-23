#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "st25r.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

LOG_MODULE_DECLARE(IIS2DH, CONFIG_SENSOR_LOG_LEVEL);

static int st25r_i2c_read(const struct device *dev, uint8_t reg_addr, uint8_t *value, uint16_t len)
{
	const struct st25r_device_config *config = dev->config;

	return i2c_burst_read_dt(&config->i2c, reg_addr | 0x80, value, len);
}

static int st25r_i2c_write(const struct device *dev, uint8_t reg_addr, uint8_t *value,
			    uint16_t len)
{
	const struct st25r_device_config *config = dev->config;

	return i2c_burst_write_dt(&config->i2c, reg_addr | 0x80, value, len);
}

stmdev_ctx_t st25r_i2c_ctx = {
	.read_reg = (stmdev_read_ptr) st25r_i2c_read,
	.write_reg = (stmdev_write_ptr) st25r_i2c_write,
};

int st25r_i2c_init(const struct device *dev)
{
	struct st25r_data *data = dev->data;
	const struct st25r_device_config *config = dev->config;

	if (!device_is_ready(config->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	data->ctx = &st25r_i2c_ctx;
	data->ctx->handle = (void *)dev;

	return 0;
}
#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */
