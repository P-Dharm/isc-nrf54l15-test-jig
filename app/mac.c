#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdio.h>
#include <zephyr/drivers/hwinfo.h>
#include <hal/nrf_ficr.h>

#include <mac.h>
#include <com.h>
static char response[64];

// static void get_mac_address(char *out, size_t out_len)
// {
// 	uint8_t buf[6];
// 	ssize_t ret = hwinfo_get_device_id(buf, sizeof(buf));
// 	if (ret > 0)
// 	{
// 		snprintf(out, out_len,
// 				 "%02X:%02X:%02X:%02X:%02X:%02X",
// 				 buf[0], buf[1], buf[2],
// 				 buf[3], buf[4], buf[5]);
// 	}
// 	else
// 	{
// 		strncpy(out, "00:00:00:00:00:00", out_len);
// 	}
// }


// static void get_ficr_mac(char *out, size_t out_len)
// {
//     uint32_t addr0 = nrf_ficr_deviceaddr_get(NRF_FICR, 0); /* DEVICEADDR[0] least significant 32 bits */
//     uint32_t addr1 = nrf_ficr_deviceaddr_get(NRF_FICR, 1); /* DEVICEADDR[1] only bits [15:0] used    */

//     if (addr0 != 0xFFFFFFFF && (addr1 & 0xFFFF) != 0xFFFF)
//     {
//         uint8_t mac[6];

//         mac[0] = (uint8_t)(addr0 >> 0);
//         mac[1] = (uint8_t)(addr0 >> 8);
//         mac[2] = (uint8_t)(addr0 >> 16);
//         mac[3] = (uint8_t)(addr0 >> 24);
//         mac[4] = (uint8_t)(addr1 >> 0);
//         mac[5] = (uint8_t)(addr1 >> 8);

//         snprintf(out, out_len,
//                  "%02X:%02X:%02X:%02X:%02X:%02X",
//                  mac[5], mac[4], mac[3],
//                  mac[2], mac[1], mac[0]);
//     }
//     else
//     {
//         strncpy(out, "00:00:00:00:00:00", out_len);
//     }
// }


static void get_ficr_mac(char *out, size_t out_len)
{
    uint32_t addr0 = nrf_ficr_deviceaddr_get(NRF_FICR, 0); /* DEVICEADDR[0] least significant 32 bits */
    uint32_t addr1 = nrf_ficr_deviceaddr_get(NRF_FICR, 1); /* DEVICEADDR[1] only bits [15:0] used    */

    if (addr0 != 0xFFFFFFFF && (addr1 & 0xFFFF) != 0xFFFF)
    {
        uint8_t mac[6];

        mac[0] = (uint8_t)(addr0 >> 0);
        mac[1] = (uint8_t)(addr0 >> 8);
        mac[2] = (uint8_t)(addr0 >> 16);
        mac[3] = (uint8_t)(addr0 >> 24);
        mac[4] = (uint8_t)(addr1 >> 0);
        mac[5] = (uint8_t)(addr1 >> 8);

        /* Apply Random Static Address bits only if address type is Random.
         * Sets two MSBs of mac[5] to 1 (0xC0) per Bluetooth spec,
         * so the address matches the BLE identity address exactly.
         */
        if (NRF_FICR->DEVICEADDRTYPE == 1) {
            mac[5] |= 0xC0;
        }

        snprintf(out, out_len,
                 "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac[5], mac[4], mac[3],
                 mac[2], mac[1], mac[0]);
    }
    else
    {
        strncpy(out, "00:00:00:00:00:00", out_len);
    }
}


void send_test_result(void)
{
	char mac[18];
	
	// get_mac_address(mac, sizeof(mac));
	get_ficr_mac(mac, sizeof(mac));

	int n = snprintf(response, sizeof(response),
					 "MAC|%s|\r\n", mac);

	// uart_tx(uart, (uint8_t *)response, (size_t)n, SYS_FOREVER_US);
	// printk("Test result sent → MAC=%s \r\n", mac);
	// printk("RESULT|%s|\r\n", mac);
	for (int i = 0; i < n; i++) {
        uart_poll_out(uart, response[i]);
    }
	
}