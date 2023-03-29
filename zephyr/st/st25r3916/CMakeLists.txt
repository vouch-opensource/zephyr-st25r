# SPDX-License-Identifier: Apache-2.0

if(CONFIG_ST25R3916) 
    zephyr_compile_definitions("-DST25R3916")
endif()

if(CONFIG_ST25R3916B) 
    zephyr_compile_definitions("-DST25R3916B")
endif()

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
