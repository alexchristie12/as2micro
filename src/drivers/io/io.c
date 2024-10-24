#include "drivers/io/io.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "drivers/logging/logging.h"
#include "config.h"
#include "stdint.h"
#include "string.h"

volatile char         input_buffer[500];
volatile unsigned int buffer_i    = 0;
volatile bool         input_ready = false;

#define HEADLESS

void on_uart_rx(void) {
    while (uart_is_readable(UART_1_ID)) {
        if (uart_is_writable(UART_1_ID)) {
            uint8_t ch = uart_getc(UART_1_ID);
            // Check for if we are trying to enter config mode
            if (ch == '$') {
                // We want to ignore these characters
                return;
            }
            printf("%c", ch);
            if ((ch == '\r') || (ch == '\n')) {
                input_buffer[buffer_i] = 0; // Adds trailing NULL
                // The control system requires no feedback on input
#ifndef HEADLESS
                uart_putc(UART_1_ID, '\r');
                uart_putc(UART_1_ID, '\n');
#endif
                input_ready = true;
            } else {
                input_buffer[buffer_i] = ch;
                buffer_i++;
#ifndef HEADLESS
                uart_putc(UART_1_ID, ch);
#endif
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
    stdio_uart_init_full(uart0, 115200, 16, 17);
    memset((char *)input_buffer, '\000', sizeof(input_buffer));

    // Initialise the UART
    uart_init(UART_1_ID, UART_1_BAUD_RATE);
    gpio_set_function(UART_1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_1_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_1_ID, false, false);
    uart_set_format(UART_1_ID, UART_1_DATA_BITS, UART_1_STOP_BITS, UART_1_PARITY);
    uart_set_fifo_enabled(UART_1_ID, false);
    int UART_1_IRQ = UART_1_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    irq_set_exclusive_handler(UART_1_IRQ, on_uart_rx);
    irq_set_enabled(UART_1_IRQ, true);
    uart_set_irq_enables(UART_1_ID, true, false);
}

int io_poll(char *buffer, int buffer_len) {
    memset((char *)input_buffer, '\000', sizeof(input_buffer));
    buffer_i    = 0;
    input_ready = false;

    while (!input_ready) {
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