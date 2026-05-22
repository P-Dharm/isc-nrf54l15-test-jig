#include <zephyr/sys/printk.h>
#include <stdint.h>
#include <test_commands.h>
#include <com.h>
#include <process.h>

uint8_t AT_response[20] = {0};

test_return ret = TEST_ERROR;

void perform_gpio_test()
{
    printk("perform_gpio_test\n");
    ret = gpio();
    if(ret == TEST_OK) {
        // strcpy(AT_response, "GPIO+OK");
        printk("GPIO+OK\n");
    } else {
        // strcpy(AT_response, "GPIOS+ERROR");
        printk("GPIO+ERROR\n");
    }
    // uart_tx_send(AT_response);   
}

void perform_uart_test()
{
    printk("perform_uart_test\n");
    ret = TEST_OK;
    // strcpy(AT_response, "UART+OK");
    // uart_tx_send(AT_response);   
    printk("UART+OK\n");
 }

// void perform_i2c_test()
// {
//     printk("perform_i2c_test\n");
//     ret = i2c();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "I2C+OK");
//     }
//     else {
//         strcpy(AT_response, "I2C+ERROR");
//     }

//     uart_tx_send(AT_response);   
//  }

// void perform_spi_test()
// {
//     printk("perform_spi_test\n");
//     ret = spi();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "SPI+OK");
//     }
//     else {
//         strcpy(AT_response, "SPI+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }

// void perform_usb_test()
// {   
//     printk("perform_usb_test\n");
//     ret = usb();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "USB+OK");
//     }
//     else {
//         strcpy(AT_response, "USB+ERROR");
//     }

//     uart_tx_send(AT_response);   
     
// }

// void perform_adc_test()
// {
//     printk("perform_adc_test\n");
//     ret = adc();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "ADC+OK");
//     }
//     else {
//         strcpy(AT_response, "ADC+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }

// void perform_pwm_test()
// {
//     printk("perform_pwm_test\n");
//     ret = pwm();
//     if(ret == TEST_OK) {
//         snprintf(AT_response, sizeof(AT_response), "PWM+OK");
//     }
//     else {
//         snprintf(AT_response, sizeof(AT_response), "PWM+ERROR");
//     }
//     AT_response[10] = '\0';
//     uart_tx_send(AT_response);   

// }

// void perform_nfc_test()
// {
//     printk("perform_nfc_test\n");
//     ret = nfc();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "NFC+OK");
//     }
//     else {
//         strcpy(AT_response, "NFC+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }

// void perform_qspi_test()
// {
//     printk("perform_qspi_test\n");
//     // ret = qspi();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "QSPI+OK");
//     }
//     else {
//         strcpy(AT_response, "QSPI+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }

// void perform_sleep_test()
// {
//     printk("perform_sleep_test\n");
//     ret = sleep();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "SLEEP+OK");
//     }
//     else {
//         strcpy(AT_response, "SLEEP+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }

// void perform_ble_test()
// {
//     printk("perform_ble_test\n");
//     ret = ble();
//     if(ret == TEST_OK) {
//         strcpy(AT_response, "BLE+OK");
//     }
//     else {
//         strcpy(AT_response, "BLE+ERROR");
//     }
//     uart_tx_send(AT_response);   
//  }