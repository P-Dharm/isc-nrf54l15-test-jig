#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/drivers/hwinfo.h>

#include <mac.h>
#include <com.h>
static char response[64];

static void get_mac_address(char *out, size_t out_len)
{
	uint8_t buf[6];
	ssize_t ret = hwinfo_get_device_id(buf, sizeof(buf));
	if (ret > 0)
	{
		snprintf(out, out_len,
				 "%02X:%02X:%02X:%02X:%02X:%02X",
				 buf[0], buf[1], buf[2],
				 buf[3], buf[4], buf[5]);
	}
	else
	{
		strncpy(out, "00:00:00:00:00:00", out_len);
	}
}

void send_test_result(void)
{
	char mac[18];
	
	get_mac_address(mac, sizeof(mac));

	int n = snprintf(response, sizeof(response),
					 "MAC|%s|\r\n", mac);

	// uart_tx(uart, (uint8_t *)response, (size_t)n, SYS_FOREVER_US);
	// printk("Test result sent → MAC=%s \r\n", mac);
	// printk("RESULT|%s|\r\n", mac);
	for (int i = 0; i < n; i++) {
        uart_poll_out(uart, response[i]);
    }
	
}