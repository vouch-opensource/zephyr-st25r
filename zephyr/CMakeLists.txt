# SPDX-License-Identifier: Apache-2.0

if(CONFIG_ST25R)
    zephyr_library()
    zephyr_library_sources(
        src/platform.c
        src/st25r.c
        src/st25r_i2c.c
        src/st25r_spi.c
        src/st25r_trigger.c
    )
    zephyr_include_directories(src)
endif()

add_subdirectory_ifdef(CONFIG_ST25R st/rfal)
add_subdirectory_ifdef(CONFIG_ST25R3916 st/st25r3916)
add_subdirectory_ifdef(CONFIG_ST25R3916B st/st25r3916)
