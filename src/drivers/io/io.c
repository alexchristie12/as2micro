#include "drivers/io/io.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "drivers/logging/logging.h"
#include "tasks/tasks.h"
#include "config.h"
#include "stdint.h"
#include "string.h"

volatile char         input_buffer[100];
volatile unsigned int buffer_i    = 0;
volatile bool         input_ready = false;

void on_uart_rx(void) {
    while (uart_is_readable(UART_ID)) {
        if (uart_is_writable(UART_ID)) {
            uint8_t ch = uart_getc(UART_ID);
            if ((ch == '\r') || (ch == '\n')) {
                input_buffer[buffer_i] = 0; // Adds trailing NULL
                uart_putc(UART_ID, '\r');
                uart_putc(UART_ID, '\n');
                input_ready = true;
            } else {
                input_buffer[buffer_i] = ch;
                buffer_i++;
                uart_putc(UART_ID, ch);
                if (ch == 0x7f) {
                    buffer_i               = buffer_i - 2;
                    input_buffer[buffer_i] = '\000';
                }
            }
        }
    }
}

void io_init() {
    stdio_init_all();
    memset((char *)input_buffer, '\000', sizeof(input_buffer));

    // Initialise the UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);
}

int io_poll(char *buffer, int buffer_len) {
    while (!input_ready) {
        if (get_current_task() != TASK_LED) {
            // Clean up and exit
            memset((char *)input_buffer, '\000', sizeof(input_buffer));
            buffer_i    = 0;
            input_ready = false;
            return 0;
        }
        __asm("wfi");
    }

    int msg_len = strlen((char *)input_buffer);
    if (msg_len > buffer_len) {
        // We cannot safely copy the buffer
        return 1;
    }
    // Copy over the contents into new buffer
    strncpy(buffer, (char *)input_buffer, buffer_len);

    memset((char *)input_buffer, '\000', sizeof(input_buffer));
    buffer_i    = 0;
    input_ready = false;
    return 0;
}
