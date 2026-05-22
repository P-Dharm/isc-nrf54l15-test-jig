#ifndef __COM_H__
#define __COM_H_

#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>

#define RECEIVE_TIMEOUT 1000
#define SLEEP_TIME_MS 10 * 60 * 1000

// static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

// static void uart_comm_callback(const struct device *dev, struct uart_event *evt, void *user_data);

#define AT_CMD_MAX_LEN 64
extern uint8_t AT_command[AT_CMD_MAX_LEN];


void begin();
int uart_tx_send(uint8_t *data);
void serial_cb(const struct device *dev, void *user_data);

#endif