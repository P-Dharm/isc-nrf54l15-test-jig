/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <bluetooth/services/nus.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <process.h>

LOG_MODULE_REGISTER(simple_ble, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

// #define RUN_STATUS_LED DK_LED1
// #define CON_STATUS_LED DK_LED2

static struct bt_conn *current_conn;

volatile bool connection = false;
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("Connection failed (err %u)", err);
    } else {
        current_conn = bt_conn_ref(conn);
        LOG_INF("Connected");
        // dk_set_led_on(CON_STATUS_LED);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected (reason 0x%02x)", reason);
    if (current_conn) {
        bt_conn_unref(current_conn);
        current_conn = NULL;
    }
    // dk_set_led_off(CON_STATUS_LED);
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

test_return ble(void)
{
    int err;

    // dk_leds_init();
    // dk_set_led_on(RUN_STATUS_LED);

    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return TEST_ERROR;
    }

    LOG_INF("Bluetooth initialized");

    bt_conn_cb_register(&conn_callbacks);

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err);
        return TEST_ERROR;
    }

    LOG_INF("Advertising successfully started");

    k_msleep(35000);

    // if(current_conn) {
    //     err = bt_conn_disconnect(current_conn, 0x15);
    //     printk("Bluetooth conn disconnect returned with %d\n", err);
    // }

    err = bt_le_adv_stop();
    // printk("Bluetooth adv stop returned with %d\n", err);

    err = bt_disable();
    // printk("Bluetooth disable returned with %d\n", err);

    // return TEST_OK;
}