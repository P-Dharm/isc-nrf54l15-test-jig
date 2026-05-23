#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#include <com.h>
#include <process.h>
#include <test_commands.h>

#define RING_BUF_SIZE 128

// Declare a ring buffer instance
RING_BUF_DECLARE(ringbuf, RING_BUF_SIZE);

uint8_t AT_command[AT_CMD_MAX_LEN];
// static const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart20));
const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart20));
static struct k_work test_work;

static bool command_ready = false;

// UART send function
int uart_tx_send(uint8_t *data)
{
    if (!data) return -EINVAL;

    if (!uart || !device_is_ready(uart)) {
        printk("UART not ready\n");
        return -ENODEV;
    }

    for (int i = 0; i < strlen((char *)data); i++) {
        uart_poll_out(uart, data[i]);
    }

    uart_poll_out(uart, '\r');
    uart_poll_out(uart, '\n');

    return 0;
}

// Work handler to execute the command
void run_test_work(struct k_work *work)
{
    ARG_UNUSED(work);
    select_test_to_perform();  // Uses AT_command buffer
    command_ready = false;
}

// UART interrupt callback
void serial_cb(const struct device *dev, void *user_data)
{
    uint8_t c;
    static uint8_t temp_buf[AT_CMD_MAX_LEN];
    static uint32_t temp_len = 0;

    if (!uart_irq_update(uart) || !uart_irq_rx_ready(uart)) {
        return;
    }

    while (uart_fifo_read(uart, &c, 1) == 1) {
        if (!command_ready) {
            if (c == '\r' || c == '\n') {
                if (temp_len > 0) {
                    // Retrieve complete command from ring buffer
                    uint32_t actual_len = ring_buf_get(&ringbuf, temp_buf, sizeof(temp_buf));
                    if (actual_len > 0 && actual_len < AT_CMD_MAX_LEN) {
                        memcpy(AT_command, temp_buf, actual_len);
                        AT_command[actual_len] = '\0';

                        uart_tx_send((uint8_t *)"Received CMD:");
                        // uart_tx_send(AT_command);

                        command_ready = true;
                        k_work_submit(&test_work);
                    }
                }
                temp_len = 0;
            } else {
                if (ring_buf_put(&ringbuf, &c, 1) == 1) {
                    temp_len++;
                } else {
                    printk("Ring buffer overflow\n");
                    temp_len = 0;
                    ring_buf_reset(&ringbuf);
                }
            }
        }
    }
}

// Initialization function
void begin(void)
{
    printk("\n========== AT Command Test Console ==========\n");
    printk("Type a command and press Enter\n");
    printk("Enter AT CMD:\n");

    if (!device_is_ready(uart)) {
        printk("UART device not ready!\n");
        return;
    }

    ring_buf_reset(&ringbuf);  // Clear the ring buffer at start
    k_work_init(&test_work, run_test_work);

    int ret = uart_irq_callback_user_data_set(uart, serial_cb, NULL);
    if (ret < 0) {
        printk("Failed to set UART callback: %d\n", ret);
        return;
    }

    uart_irq_rx_enable(uart);
}
