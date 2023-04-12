// Copyright © 2023 Vouch.io LLC

#ifndef ST25R_H_
#define ST25R_H_

#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>

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
	struct gpio_dt_spec int_gpio;
};

/* sensor data */
struct st25r_data {
	const struct device *dev;
	struct gpio_callback gpio_cb;
#if defined(CONFIG_ST25R_TRIGGER_OWN_THREAD)
	K_KERNEL_STACK_MEMBER(thread_stack, CONFIG_ST25R_THREAD_STACK_SIZE);
	struct k_thread thread;
	struct k_sem gpio_sem;
#elif defined(CONFIG_ST25R_TRIGGER_GLOBAL_THREAD)
	struct k_work work;
#endif /* CONFIG_ST25R_TRIGGER_GLOBAL_THREAD */
};

int st25r_i2c_init(const struct device *dev);
int st25r_spi_init(const struct device *dev);

int st25r_init_interrupt(const struct device *dev);

#endif /* ST25R_H_ */
