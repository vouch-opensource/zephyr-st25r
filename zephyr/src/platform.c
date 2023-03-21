#include "platform.h"

#include <zephyr/device.h>

#define DW_INST DT_INST(0, st_st25r)

struct st25r_config {
	struct gpio_dt_spec gpio_irq;
//	struct gpio_dt_spec gpio_reset;
//	struct gpio_dt_spec gpio_wakeup;
//	struct gpio_dt_spec gpio_spi_pol;
//	struct gpio_dt_spec gpio_spi_pha;
};

static const struct st25r_config conf = {
	.gpio_irq = GPIO_DT_SPEC_GET_OR(DW_INST, irq_gpios, {0}),
//	.gpio_reset = GPIO_DT_SPEC_GET_OR(DW_INST, reset_gpios, {0}),
//	.gpio_wakeup = GPIO_DT_SPEC_GET_OR(DW_INST, wakeup_gpios, {0}),
//	.gpio_spi_pol = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pol_gpios, {0}),
//	.gpio_spi_pha = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pha_gpios, {0}),
};

gpio_pin_t platform_st25r_int_pin()
{
    return conf.gpio_irq.pin;
}

const struct device *platform_st25r_int_port()
{
    return conf.gpio_irq.port;
}