# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_library_sources(
    rfal_rfst25r3916.c
    st25r3916.c
    st25r3916_aat.c
    st25r3916_com.c
    st25r3916_irq.c
    st25r3916_led.c
    timer.c
)
zephyr_include_directories(.)
