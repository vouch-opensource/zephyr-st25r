#include "platform.h"

#include <zephyr/kernel.h>

K_MUTEX_DEFINE(platform_st25r_comm_mutex);

void platform_st25r_protect_comm()
{
    k_mutex_lock(&platform_st25r_comm_mutex, K_FOREVER);
}

void platform_st25r_unprotect_comm()
{
    k_mutex_unlock(&platform_st25r_comm_mutex);
}

gpio_pin_t platform_st25r_int_pin()
{
    return 0;
}

const struct device *platform_st25r_int_port()
{
    return NULL;
}

void platform_st25r_gloabl_error(const char *file, long line)
{
    printk("ST25R error at %s:%ld\n", file, line);
}

void platform_st25r_spi_transceive(const uint8_t *txBuf, uint8_t *rxBuf, uint16_t len)
{
    printk("platform_st25r_spi_transceive %d bytes\n", len);
}