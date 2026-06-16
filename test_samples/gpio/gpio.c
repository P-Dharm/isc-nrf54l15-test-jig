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

static int test_gpio_cross_port(const struct device *out_port, uint8_t out_pin,
                                 const struct device *in_port, uint8_t in_pin)
{
    if (!device_is_ready(out_port)) {
        LOG_ERR("Output GPIO port not ready: %s", out_port->name);
        return -ENODEV;
    }
    if (!device_is_ready(in_port)) {
        LOG_ERR("Input GPIO port not ready: %s", in_port->name);
        return -ENODEV;
    }

    struct gpio_dt_spec output = {
        .port = out_port,
        .pin = out_pin,
        .dt_flags = GPIO_ACTIVE_HIGH
    };

    struct gpio_dt_spec input = {
        .port = in_port,
        .pin = in_pin,
        .dt_flags = GPIO_ACTIVE_HIGH
    };

    int err;

    err = gpio_pin_configure_dt(&input, GPIO_INPUT | GPIO_PULL_DOWN);
    if (err) {
        LOG_ERR("Failed to configure input pin %d", input.pin);
        return err;
    }

    err = gpio_pin_configure_dt(&output, GPIO_OUTPUT_ACTIVE);
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

    if (val == 0) {
        gpios_flag = false;
    }

    return val;
}

test_return gpio(void)
{
    const struct device *gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    const struct device *gpio1 = DEVICE_DT_GET(DT_NODELABEL(gpio1));
    const struct device *gpio2 = DEVICE_DT_GET(DT_NODELABEL(gpio2));

    const uint8_t first_half_port_0[] =  { 0, 2};
    const uint8_t second_half_port_0[] = { 1, 3 };

    const uint8_t first_half_port_1[] =  { 9,  11, 13, 6, 0 };
    const uint8_t second_half_port_1[] = { 10, 12, 14, 7, 1};

    const uint8_t first_half_port_2[] =  { 3, 1, 5, 9,  8 };
    const uint8_t second_half_port_2[] = { 4, 2, 6, 10, 7 };

    // const uint8_t first_half_port_0[] =  { };
    // const uint8_t second_half_port_0[] = { };

    // const uint8_t first_half_port_1[] =  { };
    // const uint8_t second_half_port_1[] = { };

    // const uint8_t first_half_port_2[] =  {};
    // const uint8_t second_half_port_2[] = { };

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
        printk("\nport 1 have zero pins");
        // printk("\nREVIVED VALUES FOR GPIO1:%d\n", received_vals_1[i]);
    }


    for (size_t i = 0; i < ARRAY_SIZE(first_half_port_2); i++) {
        received_vals_2[i] = test_gpio_pair(gpio2, first_half_port_2[i], second_half_port_2[i]);
        k_msleep(SLEEP_TIME_MS);
        // printk("\nREVIVED VALUES FOR GPIO2:%d\n", received_vals_2[i]);
    }

    if(ARRAY_SIZE(received_vals_0)>0){
        printk("\n=== GPIO0 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(received_vals_0); i++) {
            printk("OUT %d -> IN %d: %s\n",
                first_half_port_0[i], second_half_port_0[i],
                received_vals_0[i] > 0 ? "PASS" : "FAIL");  // 1 DETECT LOGIC
                // received_vals_0[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
        }
    }

    if(ARRAY_SIZE(received_vals_1)>0) {
        printk("\n=== GPIO1 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(received_vals_1); i++) {
            printk("OUT %d -> IN %d: %s\n",
                first_half_port_1[i], second_half_port_1[i],
                received_vals_1[i] > 0 ? "PASS" : "FAIL"); // 1 DETECT LOGIC
                // received_vals_1[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
        }
    }

    if(ARRAY_SIZE(received_vals_2)>0) {
        printk("\n=== GPIO2 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(received_vals_2); i++) {
            printk("OUT %d -> IN %d: %s\n",
                first_half_port_2[i], second_half_port_2[i],
                received_vals_2[i] > 0 ? "PASS" : "FAIL");   // 1 DETECT LOGIC
                // received_vals_2[i] == 0 ? "PASS" : "FAIL");  // 0 DETECT LOGIC
        }
    }

    // --- Cross-port pin pairs ---
    // Format: out_port -> in_port, out_pin -> in_pin
    // GPIO0 -> GPIO1
    const uint8_t cross_01_out[] = {  };   // GPIO0 output pins
    const uint8_t cross_01_in[]  = {  };   // GPIO1 input pins

    // GPIO1 -> GPIO2
    const uint8_t cross_12_out[] = { 8 };   // GPIO1 output pins
    const uint8_t cross_12_in[]  = { 0};   // GPIO2 input pins

    // GPIO0 -> GPIO2
    const uint8_t cross_02_out[] = {  };      // GPIO0 output pins
    const uint8_t cross_02_in[]  = {  };      // GPIO2 input pins

    int cross_vals_01[ARRAY_SIZE(cross_01_out)];
    int cross_vals_12[ARRAY_SIZE(cross_12_out)];
    int cross_vals_02[ARRAY_SIZE(cross_02_out)];

    // Run cross-port tests
    for (size_t i = 0; i < ARRAY_SIZE(cross_01_out); i++) {
        cross_vals_01[i] = test_gpio_cross_port(gpio0, cross_01_out[i],
                                                 gpio1, cross_01_in[i]);
        k_msleep(SLEEP_TIME_MS);
    }

    for (size_t i = 0; i < ARRAY_SIZE(cross_12_out); i++) {
        cross_vals_12[i] = test_gpio_cross_port(gpio1, cross_12_out[i],
                                                 gpio2, cross_12_in[i]);
        k_msleep(SLEEP_TIME_MS);
    }

    for (size_t i = 0; i < ARRAY_SIZE(cross_02_out); i++) {
        cross_vals_02[i] = test_gpio_cross_port(gpio0, cross_02_out[i],
                                                 gpio2, cross_02_in[i]);
        k_msleep(SLEEP_TIME_MS);
    }

    // Print cross-port results
    if(ARRAY_SIZE(cross_vals_01)>0){
        printk("\n=== Cross-Port GPIO0 -> GPIO1 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(cross_vals_01); i++) {
            printk("GPIO0 OUT %d -> GPIO1 IN %d: %s\n",
                cross_01_out[i], cross_01_in[i],
                cross_vals_01[i] > 0 ? "PASS" : "FAIL");
        }
    }

    if(ARRAY_SIZE(cross_vals_12)>0) {
        printk("\n=== Cross-Port GPIO1 -> GPIO2 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(cross_vals_12); i++) {
            printk("GPIO1 OUT %d -> GPIO2 IN %d: %s\n",
                cross_12_out[i], cross_12_in[i],
                cross_vals_12[i] > 0 ? "PASS" : "FAIL");
        }
    }

    if(ARRAY_SIZE(cross_vals_02)>0) {
        printk("\n=== Cross-Port GPIO0 -> GPIO2 Test Results ===\n");
        for (size_t i = 0; i < ARRAY_SIZE(cross_vals_02); i++) {
            printk("GPIO0 OUT %d -> GPIO2 IN %d: %s\n",
                cross_02_out[i], cross_02_in[i],
                cross_vals_02[i] > 0 ? "PASS" : "FAIL");
        }
    }

    if(gpios_flag) {
        return TEST_OK;
    }
    else {
        return TEST_ERROR;
    }
}
