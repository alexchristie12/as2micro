#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
#define UART1_ID uart1
#define BAUD_RATE 115200

// UART pins
#define UART1_TX_PIN 24
#define UART1_RX_PIN 25

void rn4871_send_command(const char *cmd) {
    uart_puts(UART1_ID, cmd);
    uart_puts(UART1_ID, "\r\n");  // Send carriage return + new line
    sleep_ms(100);               // Delay for processing
}

void init_bluetooth() {
    uart_init(UART1_ID, BAUD_RATE);
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);

    // Enter command mode
    rn4871_send_command("$$$");
    sleep_ms(500); // Wait for response

    // Reset RN4871 to factory defaults
    rn4871_send_command("SF,1");
    sleep_ms(1000); // Wait for the module to reset

    // Set the name of the Bluetooth module
    rn4871_send_command("SN,Sensor_System_1");
    sleep_ms(500);

    // Start advertising
    rn4871_send_command("A,0");
}

int main() {
    // Initialize UART
    stdio_init_all();
    init_bluetooth();
   
    while (1) {
        // Loop here. You can add more logic to handle connection and data exchange.
        // For example, monitor for incoming UART data from the RN4871 and handle it.
        if (uart_is_readable(UART1_ID)) {
            char received_char = uart_getc(UART1_ID);
            printf("%c", received_char);  // Print received data to stdout
        }
    }

    return 0;
}
