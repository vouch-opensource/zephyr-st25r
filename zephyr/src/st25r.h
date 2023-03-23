#ifndef ST25R_H_
#define ST25R_H_

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include "st25r_reg.h"

/*
 * Return ODR reg value based on data rate set
 */
#define IIS2DH_ODR_TO_REG_HR(_lp, _odr) \
	((_odr == 0) ? IIS2DH_POWER_DOWN : \
	((_odr < 10) ? IIS2DH_ODR_1Hz : \
	((_odr < 25) ? IIS2DH_ODR_10Hz : \
	((_lp == IIS2DH_LP_8bit) && (_odr >= 5376) ? IIS2DH_ODR_5kHz376_LP_1kHz344_NM_HP : \
	((_lp != IIS2DH_LP_8bit) && (_odr >= 1344) ? IIS2DH_ODR_5kHz376_LP_1kHz344_NM_HP : \
	((_lp == IIS2DH_LP_8bit) && (_odr >= 1600) ? IIS2DH_ODR_1kHz620_LP : \
	((_lp != IIS2DH_LP_8bit) && (_odr >= 800) ? IIS2DH_ODR_400Hz : \
	((31 - __builtin_clz(_odr / 25))) + 3)))))))

/* FS reg value from Full Scale */
#define IIS2DH_FS_TO_REG(_fs)	(30 - __builtin_clz(_fs))

/**
 * struct st25r_device_config - st25r hw configuration
 * @spi: SPI bus spec.
 * @i2c: I2C bus spec.
 * @pm: Power mode (lis2dh_powermode).
 * @int_gpio: GPIO spec for sensor pin interrupt.
 */
struct st25r_device_config {
#if DT_ANY_INST_ON_BUS_STATUS_OKAY(spi)
	struct spi_dt_spec spi;
#endif
#if DT_ANY_INST_ON_BUS_STATUS_OKAY(i2c)
	struct i2c_dt_spec i2c;
#endif
	uint8_t pm;
#ifdef CONFIG_IIS2DH_TRIGGER
	struct gpio_dt_spec int_gpio;
#endif /* CONFIG_IIS2DH_TRIGGER */
};

/* sensor data */
struct st25r_data {
	int16_t acc[3];
	uint32_t gain;

	stmdev_ctx_t *ctx;
#ifdef CONFIG_IIS2DH_TRIGGER
	const struct device *dev;
	struct gpio_callback gpio_cb;
	sensor_trigger_handler_t drdy_handler;
#if defined(CONFIG_IIS2DH_TRIGGER_OWN_THREAD)
	K_KERNEL_STACK_MEMBER(thread_stack, CONFIG_IIS2DH_THREAD_STACK_SIZE);
	struct k_thread thread;
	struct k_sem gpio_sem;
#elif defined(CONFIG_IIS2DH_TRIGGER_GLOBAL_THREAD)
	struct k_work work;
#endif /* CONFIG_IIS2DH_TRIGGER_GLOBAL_THREAD */
#endif /* CONFIG_IIS2DH_TRIGGER */
};

int st25r_i2c_init(const struct device *dev);
int st25r_spi_init(const struct device *dev);

#ifdef CONFIG_IIS2DH_TRIGGER
int st25r_init_interrupt(const struct device *dev);
int st25r_trigger_set(const struct device *dev,
			  const struct sensor_trigger *trig,
			  sensor_trigger_handler_t handler);
#endif /* CONFIG_IIS2DH_TRIGGER */

#endif /* ST25R_H_ */
