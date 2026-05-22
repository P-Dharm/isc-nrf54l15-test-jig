#ifndef __TEST_COMMANDS_H__
#define __TEST_COMMANDS_H_

#include<process.h>

typedef enum select_test_t
{
    SELECT_GPIO_TEST,
    SELECT_UART_TEST,
    SELECT_I2C_TEST,
    SELECT_SPI_TEST,
    SELECT_USB_TEST,
    SELECT_ADC_TEST,
    SELECT_PWM_TEST,
    SELECT_NFC_TEST,
    SELECT_QSPI_TEST,
    SELECT_SLEEP_TEST,
    SELECT_BLE_TEST,
}select_test;

void perform_gpio_test(void);
void perform_uart_test(void);
// void perform_i2c_test(void);
// void perform_spi_test(void);
// void perform_usb_test(void);
// void perform_adc_test(void);
// void perform_pwm_test(void);
// void perform_nfc_test(void);
// void perform_qspi_test(void);
// void perform_sleep_test(void);
// void perform_ble_test(void);

test_return gpio(void);
// test_return i2c(void);
// test_return spi(void);
// test_return usb(void);
// test_return adc(void);
// test_return pwm(void);
// test_return nfc(void);
// test_return qspi(void);
// test_return sleep(void);
// test_return ble(void);


#endif