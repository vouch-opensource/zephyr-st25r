// Copyright © 2018 Vouch.io LLC

#define DT_DRV_COMPAT st_st25r

#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "st25r.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

LOG_MODULE_DECLARE(ST25R);

static struct device *s_i2c_dev;

int st25r_i2c_init(const struct device *dev)
{
	struct st25r_data *data = dev->data;
	const struct st25r_device_config *config = dev->config;

	if (!device_is_ready(config->i2c.bus)) {
		LOG_ERR("Bus device is not ready");
		return -ENODEV;
	}

	s_i2c_dev = dev;

	return 0;
}

void platform_st25r_i2c_send(uint16_t cmd, uint8_t* txBuf, uint16_t len, bool last, bool txOnly) {
    uint8_t buf[2 + len];
    buf[0] = cmd >> 8;
    buf[1] = cmd & 0xFF;
    /* TODO eliminate memcpy and use two msgs */
    memcpy(&buf[2], txBuf, len);
    struct i2c_msg msgs[1] = {
        {
            .buf = buf,
            .len = 2 + len,
            .flags = I2C_MSG_WRITE | ((last && txOnly) ? I2C_MSG_STOP : 0),
        },
    };
    const struct st25r_device_config *config = s_i2c_dev->config;
    int res = i2c_transfer(config->i2c.bus, msgs, 1, config->i2c.addr);
    if (res < 0) {
        LOG_ERR("I2C write failed: %d", res);
    }
}

void platform_st25r_i2c_recv(uint16_t cmd, uint8_t* rxBuf, uint16_t len) {
    uint8_t buf[2];
    buf[0] = cmd >> 8;
    buf[1] = cmd & 0xFF;
    struct i2c_msg msgs[2] = {
        {
            .buf = buf,
            .len = 2,
            .flags = I2C_MSG_WRITE,
        },
        {
            .buf = rxBuf,
            .len = len,
            .flags = I2C_MSG_READ | I2C_MSG_STOP,
        },
    };
    const struct st25r_device_config *config = s_i2c_dev->config;
    int res = i2c_transfer(config->i2c.bus, msgs, 2, config->i2c.addr);
    if (res < 0) {
        LOG_ERR("I2C read failed: %d", res);
    }
}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */
