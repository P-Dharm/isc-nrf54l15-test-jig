cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Test_Jig_demo)

set(COMMON_PATH ${CMAKE_CURRENT_LIST_DIR}/common)
include(${COMMON_PATH}/common.cmake)

target_sources(app PRIVATE src/main.c
                            app/com.c
                            app/test_commands.c
                            app/process.c
                            app/mac.c
                            # test_samples/adc/adc.c
                            # test_samples/pwm/pwm.c
                            # test_samples/spi/spi.c
                            # test_samples/i2c/i2c.c
                            # test_samples/usb/usb.c
                            # test_samples/nfc/nfc.c
                            # test_samples/nfc/ndef_file_m.c
                            # test_samples/ble/ble.c
                            test_samples/gpio/gpio.c
                            # test_samples/sleep/sleep.c
                            )

target_include_directories(app PRIVATE app
                            # app PRIVATE test_samples/nfc
                            app PRIVATE common)