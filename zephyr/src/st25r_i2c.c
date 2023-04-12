// Copyright © 2023 Vouch.io LLC

#define DT_DRV_COMPAT st_st25r

#include <assert.h>
#include <string.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "platform.h"

#include "st25r.h"

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)

LOG_MODULE_DECLARE(ST25R);

static const struct device *s_i2c_dev;

int st25r_i2c_init(const struct device *dev)
{
    const struct st25r_device_config *config = dev->config;

    if (!device_is_ready(config->i2c.bus)) {
        LOG_ERR("Bus device is not ready");
        return -ENODEV;
    }

    s_i2c_dev = dev;

    return 0;
}

bool i2c_address_mismatch(uint16_t addr, const struct st25r_device_config *config)
{
    if ((addr >> 1) != config->i2c.addr) {
        LOG_ERR("I2C address mismatch: %#04x != %#04x", addr >> 1, config->i2c.addr);
        return true;
    }
    return false;
}

static bool s_pending_reg_read = false;
static uint8_t s_reg_read_addr;
static bool s_pending_reg_write = false;
static uint8_t s_reg_write_addr;

void platform_st25r_i2c_send(uint16_t addr, uint8_t *txBuf, uint16_t len, bool last, bool txOnly)
{
    LOG_DBG("platform_st25r_i2c_send addr=%#04x, len=%d, last=%d, txOnly=%d", addr >> 1, len, last, txOnly);
    const struct st25r_device_config *config = s_i2c_dev->config;

    if (i2c_address_mismatch(addr, config)) {
        return;
    }

    if (last && !txOnly) {
        assert(len == 1);
        s_pending_reg_read = true;
        s_reg_read_addr = *txBuf;
    } else if (!last) {
        assert(len == 1);
        s_pending_reg_write = true;
        s_reg_write_addr = *txBuf;
    } else {
        struct i2c_msg msgs[2];
        int msg_count = 1;

        if (s_pending_reg_write) {
            msgs[0].buf = &s_reg_write_addr;
            msgs[0].len = 1;
            msgs[0].flags = I2C_MSG_WRITE;
            msg_count = 2;
            s_pending_reg_write = false;
        }

        msgs[msg_count - 1].buf = txBuf;
        msgs[msg_count - 1].len = len;
        msgs[msg_count - 1].flags = I2C_MSG_WRITE | (last && txOnly ? I2C_MSG_STOP : 0);

        if (s_pending_reg_read) {
            LOG_WRN("Clearing a previously pending register read");
            s_pending_reg_read = false;
        }

        int res = i2c_transfer(config->i2c.bus, msgs, msg_count, config->i2c.addr);
        if (res < 0) {
            LOG_ERR("I2C write failed: %d", res);
        }
    }
}

void platform_st25r_i2c_recv(uint16_t addr, uint8_t *rxBuf, uint16_t len)
{
    LOG_DBG("platform_st25r_i2c_recv addr=%#04x, len=%d", addr >> 1, len);
    const struct st25r_device_config *config = s_i2c_dev->config;

    if (i2c_address_mismatch(addr, config)) {
        return;
    }

    if (s_pending_reg_read) {
        s_pending_reg_read = false;
        struct i2c_msg msgs[2] = {
                {
                        .buf = &s_reg_read_addr,
                        .len = 1,
                        .flags = I2C_MSG_WRITE,
                },
                {
                        .buf = rxBuf,
                        .len = len,
                        .flags = I2C_MSG_READ | I2C_MSG_RESTART | I2C_MSG_STOP,
                },
        };
        int res = i2c_transfer(config->i2c.bus, msgs, 2, config->i2c.addr);
        if (res < 0) {
            LOG_ERR("I2C read failed: %d", res);
        }
    } else {
        LOG_WRN("No pending register read");
    }
}

#endif /* DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c) */
