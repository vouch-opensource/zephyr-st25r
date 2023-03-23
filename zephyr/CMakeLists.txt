# SPDX-License-Identifier: Apache-2.0

if(CONFIG_ST25R)
    zephyr_library()
    zephyr_library_sources(
        src/platform.c
        src/st25r.c
        src/st25r_i2c.c
        src/st25r_spi.c
    )
    zephyr_library_sources_ifdef(CONFIG_ST25R_TRIGGER src/st25r_trigger.c)
    zephyr_include_directories(src inc)
endif()

add_subdirectory_ifdef(CONFIG_ST25R Middlewares/ST/rfal)
add_subdirectory_ifdef(CONFIG_ST25R3916 st25r/st25r3916)
add_subdirectory_ifdef(CONFIG_ST25R3916B st25r/st25r3916)
