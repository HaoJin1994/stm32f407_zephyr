# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.28.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE 
    src/main.c
)
# LED模块
if(CONFIG_MODULE_LED)
    target_sources(app PRIVATE src/shell_led_cmd.c)
endif()
# cdc acm模块
if(CONFIG_MODULE_CDC_ACM)
    target_sources(app PRIVATE src/shell_uart_cmd.c)
endif()
# PWM LED模块
if(CONFIG_MODULE_PWM)
    target_sources(app PRIVATE src/shell_pwm_led_cmd.c)
endif()
# LITTLEFS模块
if(CONFIG_MODULE_SPI_FLASH)
    target_sources(app PRIVATE src/shell_littlefs_cmd.c)
endif()


target_include_directories(app PRIVATE
    include
)

