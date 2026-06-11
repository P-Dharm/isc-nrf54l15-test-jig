#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include <process.h>
#include <mac.h>

LOG_MODULE_REGISTER(gpio_test, LOG_LEVEL_DBG);

#define SLEEP_TIME_MS 5
volatile bool gpios_flag = true;

static int test_gpio_pair(const struct device *port, uint8_t out_pin, uint8_t in_pin)
{
    if (!device_is_ready(port)) {
        LOG_ERR("GPIO port not ready: %s", port->name);
        return -ENODEV;
    }

    struct gpio_dt_spec output = {
        .port = port,
        .pin = out_pin,
        .dt_flags = GPIO_ACTIVE_HIGH
    };  

    struct gpio_dt_spec input = {
        .port = port,
        .pin = in_pin,
        .dt_flags = GPIO_ACTIVE_HIGH
    };

    int err;

    err = gpio_pin_configure_dt(&input, GPIO_INPUT | GPIO_PULL_DOWN); // 1 DETECT LOGIC
    // err = gpio_pin_configure_dt(&input, GPIO_INPUT | GPIO_PULL_UP); // 0 DETECT LOGIC
    if (err) {
        LOG_ERR("Failed to configure input pin %d", input.pin);
        return err;
    }

    err = gpio_pin_configure_dt(&output, GPIO_OUTPUT_ACTIVE); // 1 DETECT LOGIC
    // err = gpio_pin_configure_dt(&output, GPIO_OUTPUT_INACTIVE); // 0 DETECT LOGIC
    if (err) {
        LOG_ERR("Failed to configure output pin %d", output.pin);
        return err;
    }

    k_msleep(SLEEP_TIME_MS);

    int val = gpio_pin_get_dt(&input);
    if (val < 0) {
        LOG_ERR("Failed to read from input pin %d", input.pin);
        return val;
    }

    if (val == 0) {     // 1 DETECT LOGIC
    // if (val == 1) {         // 0 DETECT LOGIC
        // LOG_WRN("No signal received at input pin %d from output pin %d", input.pin, output.pin);
        gpios_flag = false;
    } else {
        // LOG_INF("Signal received OK: Output pin %d -> Input pin %d", output.pin, input.pin);
    }

    return val;
}


// static void scan_gpio2_pairs(const struct device *port)
// {
//     LOG_INF("=== Scanning GPIO2 all pairs ===");
//     for (uint8_t out = 0; out <= 9; out++) {
//         for (uint8_t in = 0; in <= 9; in++) {
//             if (out == in) continue;
//             int val = test_gpio_pair(port, out, in);
//             if (val > 0) {
//                 LOG_INF(">>> WORKING PAIR: OUT %d -> IN %d", out, in);
//             }
//             k_msleep(5);
//         }
//     }
// }

// static void scan_gpio2_raw(const struct device *port)
// {
//     LOG_INF("=== Raw GPIO2 test ===");
    
//     // directly configure without dt_spec
//     gpio_pin_configure(port, 5, GPIO_INPUT | GPIO_PULL_DOWN);
//     gpio_pin_configure(port, 4, GPIO_OUTPUT_ACTIVE);
//     k_msleep(10);
//     int val = gpio_pin_get(port, 5);
//     LOG_INF("Raw P2.04 -> P2.05: %d", val);

//     gpio_pin_configure(port, 7, GPIO_INPUT | GPIO_PULL_DOWN);
//     gpio_pin_configure(port, 6, GPIO_OUTPUT_ACTIVE);
//     k_msleep(10);
//     val = gpio_pin_get(port, 7);
//     LOG_INF("Raw P2.06 -> P2.07: %d", val);
// }

test_return gpio(void)
{
    const struct device *gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    const struct device *gpio1 = DEVICE_DT_GET(DT_NODELABEL(gpio1));
    const struct device *gpio2 = DEVICE_DT_GET(DT_NODELABEL(gpio2));

    const uint8_t first_half_port_0[] =  { 1 , 0};
    const uint8_t second_half_port_0[] = { 2, 3 };

    const uint8_t first_half_port_1[] =  { 6, 12 };
    const uint8_t second_half_port_1[] = { 9, 11};

    // const uint8_t first_half_port_2[] =  { 0, 1 ,2,3,4, 5};
    // const uint8_t second_half_port_2[] = { 5, 6, 7,8,9,10};
    // const uint8_t first_half_port_2[] =  { 0, 4, 6 };
    // const uint8_t second_half_port_2[] = { 1, 5, 7 };
    // const uint8_t first_half_port_2[] =  { 0, 2, 4, 6, 8 };
    // const uint8_t second_half_port_2[] = { 1, 3, 5, 7, 9 };
    const uint8_t first_half_port_2[] =  { 6, 8 };
    const uint8_t second_half_port_2[] = { 7, 9 };

    // const uint8_t first_half_port_2[]  = { 0, 1, 2, 3, 4, 5 };
    // const uint8_t second_half_port_2[] = { 6, 7, 8, 9, 4, 10 };

    int received_vals_0[ARRAY_SIZE(first_half_port_0)];
    int received_vals_1[ARRAY_SIZE(first_half_port_1)];
    int received_vals_2[ARRAY_SIZE(first_half_port_2)];

    if (gpio0 == NULL || gpio1 == NULL|| gpio2 == NULL)
    // if (gpio0 == NULL ||gpio1 == NULL  )
    {
        LOG_ERR("GPIO device(s) not found");
        return TEST_ERROR;
    }

    for (size_t i = 0; i < ARRAY_SIZE(first_half_port_0); i++) {
        received_vals_0[i] = test_gpio_pair(gpio0, first_half_port_0[i], second_half_port_0[i]);
        k_msleep(SLEEP_TIME_MS);
        // printk("\nREVIVED VALUES FOR GPIO0:%d\n", received_vals_0[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(first_half_port_1); i++) {
        received_vals_1[i] = test_gpio_pair(gpio1, first_half_port_1[i], second_half_port_1[i]);
        k_msleep(SLEEP_TIME_MS);
        // printk("\nREVIVED VALUES FOR GPIO1:%d\n", received_vals_1[i]);
    }


    for (size_t i = 0; i < ARRAY_SIZE(first_half_port_2); i++) {
        received_vals_2[i] = test_gpio_pair(gpio2, first_half_port_2[i], second_half_port_2[i]);
        k_msleep(SLEEP_TIME_MS);
        // printk("\nREVIVED VALUES FOR GPIO2:%d\n", received_vals_2[i]);
    }

    printk("\n=== GPIO0 Test Results ===\n");
    for (size_t i = 0; i < ARRAY_SIZE(received_vals_0); i++) {
        printk("OUT %d -> IN %d: %s\n",
            first_half_port_0[i], second_half_port_0[i],
            received_vals_0[i] > 0 ? "PASS" : "FAIL");  // 1 DETECT LOGIC
            // received_vals_0[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
    }

    printk("\n=== GPIO1 Test Results ===\n");
    for (size_t i = 0; i < ARRAY_SIZE(received_vals_1); i++) {
        printk("OUT %d -> IN %d: %s\n",
            first_half_port_1[i], second_half_port_1[i],
            received_vals_1[i] > 0 ? "PASS" : "FAIL"); // 1 DETECT LOGIC
            // received_vals_1[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
    }

    printk("\n=== GPIO2 Test Results ===\n");
    for (size_t i = 0; i < ARRAY_SIZE(received_vals_2); i++) {
        printk("OUT %d -> IN %d: %s\n",
            first_half_port_2[i], second_half_port_2[i],
            received_vals_2[i] > 0 ? "PASS" : "FAIL");   // 1 DETECT LOGIC
            // received_vals_2[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
    }
    
    if(gpios_flag) {
        return TEST_OK;
    }
    else {
        return TEST_ERROR;
    }
}
