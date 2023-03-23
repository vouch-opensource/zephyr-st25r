#define DT_DRV_COMPAT st_st25r

#include <zephyr/init.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)
#include <zephyr/drivers/spi.h>
#elif DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)
#include <zephyr/drivers/i2c.h>
#endif

#include "st25r.h"

#include "rfal_nfc.h"

LOG_MODULE_REGISTER(ST25R, CONFIG_SENSOR_LOG_LEVEL);

static int st25r_init_interface(const struct device *dev)
{
	int res;

#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)
	res = st25r_spi_init(dev);
	if (res) {
		return res;
	}
#elif DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)
	res = st25r_i2c_init(dev);
	if (res) {
		return res;
	}
#else
#error "BUS MACRO NOT DEFINED IN DTS"
#endif

	return 0;
}

static int st25r_init(const struct device *dev)
{
	struct st25r_data *st25r = dev->data;
	const struct st25r_device_config *cfg = dev->config;

	if (st25r_init_interface(dev)) {
	    return -EINVAL;
	}

	LOG_INF("rfalNfcInitialize starting");
	rfalNfcInitialize();
	LOG_INF("rfalNfcInitialize complete");

#ifdef CONFIG_ST25R_TRIGGER
	if (cfg->int_gpio.port) {
		if (st25r_init_interrupt(dev) < 0) {
			LOG_ERR("Failed to initialize interrupts");
			return -EIO;
		}
	}
#endif /* CONFIG_ST25R_TRIGGER */

	return 0;
}

#define ST25R_SPI(inst)                                                                           \
	(.spi = SPI_DT_SPEC_INST_GET(                                                              \
		 0, SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_MODE_CPHA | SPI_WORD_SET(8), 0),)

#define ST25R_I2C(inst) (.i2c = I2C_DT_SPEC_INST_GET(inst),)

#define ST25R_DEFINE(inst)									\
	static struct st25r_data st25r_data_##inst;						\
												\
	static const struct st25r_device_config st25r_device_config_##inst = {		\
		COND_CODE_1(DT_INST_ON_BUS(inst, i2c), ST25R_I2C(inst), ())			\
		COND_CODE_1(DT_INST_ON_BUS(inst, spi), ST25R_SPI(inst), ())			\
		IF_ENABLED(CONFIG_ST25R_TRIGGER,						\
			   (.int_gpio = GPIO_DT_SPEC_INST_GET_OR(inst, drdy_gpios, { 0 }),))	\
	};											\
												\
	DEVICE_DT_INST_DEFINE(inst, st25r_init, NULL,						\
			      &st25r_data_##inst, &st25r_device_config_##inst, POST_KERNEL,	\
			      99, NULL);			\

DT_INST_FOREACH_STATUS_OKAY(ST25R_DEFINE)
