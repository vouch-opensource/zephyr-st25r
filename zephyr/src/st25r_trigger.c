#define DT_DRV_COMPAT st_st25r

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "platform.h"
#include "st25r3916_irq.h"

#include "st25r.h"

LOG_MODULE_DECLARE(ST25R);

#if 0
/**
 * st25r_enable_int - enable selected int pin to generate interrupt
 */
static int st25r_enable_drdy(const struct device *dev,
			      enum sensor_trigger_type type, int enable)
{
	struct st25r_data *st25r = dev->data;
	st25r_ctrl_reg3_t reg3;

	/* set interrupt for pin INT1 */
	st25r_pin_int1_config_get(st25r->ctx, &reg3);

	reg3.i1_drdy1 = enable;

	return st25r_pin_int1_config_set(st25r->ctx, &reg3);
}

/**
 * st25r_trigger_set - link external trigger to event data ready
 */
int st25r_trigger_set(const struct device *dev,
		       const struct sensor_trigger *trig,
		       sensor_trigger_handler_t handler)
{
	struct st25r_data *st25r = dev->data;
	const struct st25r_device_config *cfg = dev->config;
	int16_t raw[3];
	int state = (handler != NULL) ? PROPERTY_ENABLE : PROPERTY_DISABLE;

	if (!cfg->int_gpio.port) {
		return -ENOTSUP;
	}

	switch (trig->type) {
	case SENSOR_TRIG_DATA_READY:
		st25r->drdy_handler = handler;
		if (state) {
			/* dummy read: re-trigger interrupt */
			st25r_acceleration_raw_get(st25r->ctx, raw);
		}
		return st25r_enable_drdy(dev, SENSOR_TRIG_DATA_READY, state);
	default:
		LOG_ERR("Unsupported sensor trigger");
		return -ENOTSUP;
	}
}

static int st25r_handle_drdy_int(const struct device *dev)
{
	struct st25r_data *data = dev->data;

	struct sensor_trigger drdy_trig = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ALL,
	};

	if (data->drdy_handler) {
		data->drdy_handler(dev, &drdy_trig);
	}

	return 0;
}

#endif

/**
 * st25r_handle_interrupt - handle the drdy event
 * read data and call handler if registered any
 */
static void st25r_handle_interrupt(const struct device *dev)
{
	const struct st25r_device_config *cfg = dev->config;

	st25r3916Isr();

	gpio_pin_interrupt_configure_dt(&cfg->int_gpio, GPIO_INT_EDGE_TO_ACTIVE);
}

static void st25r_gpio_callback(const struct device *dev,
				 struct gpio_callback *cb, uint32_t pins)
{
	struct st25r_data *st25r =
		CONTAINER_OF(cb, struct st25r_data, gpio_cb);
	const struct st25r_device_config *cfg = st25r->dev->config;

	if ((pins & BIT(cfg->int_gpio.pin)) == 0U) {
		return;
	}

	gpio_pin_interrupt_configure_dt(&cfg->int_gpio, GPIO_INT_DISABLE);

#if defined(CONFIG_ST25R_TRIGGER_OWN_THREAD)
	k_sem_give(&st25r->gpio_sem);
#elif defined(CONFIG_ST25R_TRIGGER_GLOBAL_THREAD)
	k_work_submit(&st25r->work);
#endif /* CONFIG_ST25R_TRIGGER_OWN_THREAD */
}

#ifdef CONFIG_ST25R_TRIGGER_OWN_THREAD
static void st25r_thread(struct st25r_data *st25r)
{
	while (1) {
		k_sem_take(&st25r->gpio_sem, K_FOREVER);
		st25r_handle_interrupt(st25r->dev);
	}
}
#endif /* CONFIG_ST25R_TRIGGER_OWN_THREAD */

#ifdef CONFIG_ST25R_TRIGGER_GLOBAL_THREAD
static void st25r_work_cb(struct k_work *work)
{
	struct st25r_data *st25r =
		CONTAINER_OF(work, struct st25r_data, work);

	st25r_handle_interrupt(st25r->dev);
}
#endif /* CONFIG_ST25R_TRIGGER_GLOBAL_THREAD */

static gpio_pin_t s_int_pin;
static struct device *s_int_port;

gpio_pin_t platform_st25r_int_pin()
{
    return s_int_pin;
}

struct device *platform_st25r_int_port()
{
    return s_int_port;
}

int st25r_init_interrupt(const struct device *dev)
{
	struct st25r_data *st25r = dev->data;
	const struct st25r_device_config *cfg = dev->config;
	int ret;

	if (!device_is_ready(cfg->int_gpio.port)) {
		LOG_ERR("%s: device %s is not ready", dev->name, cfg->int_gpio.port->name);
		return -ENODEV;
	}

	s_int_pin = cfg->int_gpio.pin;
	s_int_port = cfg->int_gpio.port;

	st25r->dev = dev;

#if defined(CONFIG_ST25R_TRIGGER_OWN_THREAD)
	k_sem_init(&st25r->gpio_sem, 0, K_SEM_MAX_LIMIT);

	k_thread_create(&st25r->thread, st25r->thread_stack,
                    CONFIG_ST25R_THREAD_STACK_SIZE,
		       (k_thread_entry_t)st25r_thread, st25r,
		       0, NULL, K_PRIO_COOP(CONFIG_ST25R_THREAD_PRIORITY),
		       0, K_NO_WAIT);
#elif defined(CONFIG_ST25R_TRIGGER_GLOBAL_THREAD)
	st25r->work.handler = st25r_work_cb;
#endif /* CONFIG_ST25R_TRIGGER_OWN_THREAD */

	ret = gpio_pin_configure_dt(&cfg->int_gpio, GPIO_INPUT);
	if (ret < 0) {
		LOG_DBG("Could not configure gpio");
		return ret;
	}

	gpio_init_callback(&st25r->gpio_cb,
			   st25r_gpio_callback,
			   BIT(cfg->int_gpio.pin));

	if (gpio_add_callback(cfg->int_gpio.port, &st25r->gpio_cb) < 0) {
		LOG_DBG("Could not set gpio callback");
		return -EIO;
	}

	/* enable drdy on int1 in pulse mode */
	//if (st25r_int1_pin_notification_mode_set(st25r->ctx, ST25R_INT1_PULSED)) {
	//	return -EIO;
	//}

	return gpio_pin_interrupt_configure_dt(&cfg->int_gpio, GPIO_INT_EDGE_TO_ACTIVE);
}
