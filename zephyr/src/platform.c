// Copyright © 2023 Vouch.io LLC

#include "platform.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(ST25R);

K_MUTEX_DEFINE(platform_st25r_comm_mutex);

void platform_st25r_protect_comm()
{
    k_mutex_lock(&platform_st25r_comm_mutex, K_FOREVER);
}

void platform_st25r_unprotect_comm()
{
    k_mutex_unlock(&platform_st25r_comm_mutex);
}

void platform_st25r_global_error(const char *file, long line)
{
    LOG_ERR("Error at %s:%ld", file, line);
}
