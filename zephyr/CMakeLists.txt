# SPDX-License-Identifier: Apache-2.0

if(CONFIG_ST25R)
    zephyr_include_directories(inc)
endif()
add_subdirectory_ifdef(CONFIG_ST25R Middlewares/ST/rfal)
add_subdirectory_ifdef(CONFIG_ST25R3916 st25r/st25r3916)
add_subdirectory_ifdef(CONFIG_ST25R3916B st25r/st25r3916)
