#include <stdio.h>
#include <stdint.h>
#include <process.h>
#include <test_commands.h>
#include <com.h>

uint8_t GPIO_command[]  =    "AT+TESTGPIOS";
uint8_t UART_command[]  =    "AT+TESTUART";
uint8_t I2C_command[]   =     "AT+TESTI2C";
uint8_t SPI_command[]   =     "AT+TESTSPI";
uint8_t USB_command[]   =     "AT+TESTUSB";
uint8_t ADC_command[]   =     "AT+TESTADC";
uint8_t PWM_command[]   =     "AT+TESTPWM";
uint8_t NFC_command[]   =     "AT+TESTNFC";
uint8_t QSPI_command[]  =    "AT+TESTQSPI";
uint8_t SLEEP_command[] =   "AT+TESTSLEEP";
uint8_t BLE_command[]   =     "AT+TESTBLE";


void gpio_work_handler(struct k_work *work) 
{
    perform_gpio_test();
}

void uart_work_handler(struct k_work *work)
{
    perform_uart_test();   
}

// void i2c_work_handler(struct k_work *work)
// {
//     perform_i2c_test();
// }

// void spi_work_handler(struct k_work *work)
// {
//     perform_spi_test();
// }

// void usb_work_handler(struct k_work *work)
// {
//     perform_usb_test();
// }

// void adc_work_handler(struct k_work *work)
// {
//     perform_adc_test();
// }

// void pwm_work_handler(struct k_work *work)
// {
//     perform_pwm_test();
// }

// void nfc_work_handler(struct k_work *work)
// {
//     perform_nfc_test();
// }

// void qspi_work_handler(struct k_work *work)
// {
//     perform_qspi_test();
// }

// void sleep_work_handler(struct k_work *work)
// {
//     perform_sleep_test();
// }

// void ble_work_handler(struct k_work *work)
// {
//     perform_ble_test();
// }
K_WORK_DEFINE(gpio_work, gpio_work_handler);
K_WORK_DEFINE(uart_work, uart_work_handler);
// K_WORK_DEFINE(i2c_work, i2c_work_handler);
// K_WORK_DEFINE(spi_work, spi_work_handler);
// K_WORK_DEFINE(usb_work, usb_work_handler);
// K_WORK_DEFINE(adc_work, adc_work_handler);
// K_WORK_DEFINE(pwm_work, pwm_work_handler);
// K_WORK_DEFINE(nfc_work, nfc_work_handler);
// K_WORK_DEFINE(qspi_work, qspi_work_handler);
// K_WORK_DEFINE(sleep_work, sleep_work_handler);
// K_WORK_DEFINE(ble_work, ble_work_handler);

void select_test_to_perform()
{
    printk("%s\n", AT_command);

    if(0 == memcmp(AT_command, GPIO_command, sizeof(GPIO_command))) {
            k_work_submit(&gpio_work);
    }
    else if(0 == memcmp(AT_command, UART_command, sizeof(UART_command))) {
            k_work_submit(&uart_work);
    }
    // else if(0 == memcmp(AT_command, I2C_command, sizeof(I2C_command))) {
    //         k_work_submit(&i2c_work);
    // }
    // else if(0 == memcmp(AT_command, SPI_command, sizeof(SPI_command))) {
    //         k_work_submit(&spi_work);
    // }
    // else if(0 == memcmp(AT_command, USB_command, sizeof(USB_command))) {
    //         k_work_submit(&usb_work);
    // }
    // else if(0 == memcmp(AT_command, ADC_command, sizeof(ADC_command))) {
    //         k_work_submit(&adc_work);                        
    // }
    // else if(0 == memcmp(AT_command, PWM_command, sizeof(PWM_command))) {
    //         k_work_submit(&pwm_work);                     
    // }
    // else if(0 == memcmp(AT_command, NFC_command, sizeof(NFC_command))) {
    //         k_work_submit(&nfc_work);                    
    // }
    // else if(0 == memcmp(AT_command, QSPI_command, sizeof(QSPI_command))) {
    //         k_work_submit(&qspi_work);                      
    // }
    // else if(0 == memcmp(AT_command, SLEEP_command, sizeof(SLEEP_command))) {
    //         k_work_submit(&sleep_work);                     
    // }
    // else if(0 == memcmp(AT_command, BLE_command, sizeof(BLE_command))) {
    //         k_work_submit(&ble_work);                        
    // }
    else {
        printk("not selected\n");
    }
     
    memset(AT_command, 0, sizeof(AT_command));
 }