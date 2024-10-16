#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "led.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "tasks/tasks.h"
#include "config.h"
#include "stdint.h"
#include <string.h>

uint32_t led_data;

void led_init(){
    // Initialize PIO0 to control the LED chain
    uint32_t pio_program_offset = pio_add_program(LED_PIO, &ws2812_program);
    ws2812_program_init(LED_PIO, 0, pio_program_offset, OUTPUT_LED_PIN, 800000, false);
    ws2812_program_init(LED_PIO, 0, pio_program_offset, DEBUG_LED_PIN, 800000, false);
}

void set_led_color(uint8_t red, uint8_t green, uint8_t blue) {
    led_data = (red << 24) | (green << 16) | (blue << 8);
    pio_sm_put_blocking(pio0, 0, led_data);
}

void turn_off_led() {
    led_data = 0;
    pio_sm_put_blocking(pio0, 0, led_data);
}



// int led_init() {
//     uint32_t pio_program_offset = pio_add_program(LED_PIO, &ws2812_program);
//     ws2812_program_init(LED_PIO, 0, pio_program_offset, LED_PIN, 800000, false);
//     return 0;
// }

// int set_led(led_data* ld, uint8_t led_num, uint8_t red, uint8_t green, uint8_t blue) {
//     // Check if the requested LED number is within range
//     if (led_num != 1) {
//         return 1;
//     }
//     // Set the new value
//     ld->reds[led_num - 1]   = red;
//     ld->greens[led_num - 1] = green;
//     ld->blues[led_num - 1]  = blue;

// // #ifdef DBG_LED
// //     // Create the Log Message
// //     char led_msg[80];
// //     sprintf(led_msg, "set LED %d to [%d %d, %d]", led_num, red, green, blue);
// //     log_dbg(LOG_INFORMATION, led_msg);
// // #endif

//     return 0;
// }

// void update_leds(led_data* ld) {
//     for (int i = 0; i <= NUMBER_OF_LEDS - 1; i++) {
//         uint32_t led_bit_mask = ((ld->reds[i] << 24) | (ld->greens[i] << 16) | (ld->blues[i]) << 8);
//         pio_sm_put_blocking(LED_PIO, 0, led_bit_mask);
//     }

// #ifdef DBG_LED
//     log_dbg(LOG_INFORMATION, "updated LEDs");
// #endif
// }

// void switch_off_leds(led_data* ld) {
//     for (int i = 0; i <= NUMBER_OF_LEDS - 1; i++) {
//         ld->reds[i]   = 0;
//         ld->greens[i] = 0;
//         ld->blues[i]  = 0;
//     }
// #ifdef DBG_LED
//     log_dbg(LOG_INFORMATION, "turned off LEDs");
// #endif
// }

// int handle_led_command(char* cmd_buffer, int cmd_buffer_len, led_data* ld) {
//     // Check current task is correct
//     if (get_current_task() != TASK_LED) {
//         switch_off_leds(ld);
//         update_leds(ld);
//     }

//     // Check for off and update
//     if (0 == strcmp(cmd_buffer, "off")) {
//         switch_off_leds(ld);
//         return 0;
//     }

//     if (0 == strcmp(cmd_buffer, "update")) {
//         update_leds(ld);
//         return 0;
//     }

//     // Now handle the levels
//     int pin = 0;
//     if (sscanf(cmd_buffer, "red %d", &pin) == 1) {
// #ifdef DBG_LED
//         log_dbg(LOG_INFORMATION, "got red");
// #endif
//         return set_led(ld, pin, 255, 0, 0);
//     }

//     if (sscanf(cmd_buffer, "green %d", &pin) == 1) {
// #ifdef DBG_LED
//         log_dbg(LOG_INFORMATION, "got green");
// #endif
//         return set_led(ld, pin, 0, 255, 0);
//     }

//     if (sscanf(cmd_buffer, "blue %d", &pin) == 1) {
// #ifdef DBG_LED
//         log_dbg(LOG_INFORMATION, "got blue");
// #endif
//         return set_led(ld, pin, 0, 0, 255);
//     }

//     // This is the failure condition
//     return 1;
// }

// int set_multiple_leds(led_data* ld, uint8_t low_led_num, uint8_t up_led_num, uint8_t red, uint8_t green, uint8_t blue) {
//     // Check the values
//     if (low_led_num < 1 || low_led_num > NUMBER_OF_LEDS || up_led_num < 1 || up_led_num > NUMBER_OF_LEDS) {
// #ifdef DBG_LED
//         log_dbg(LOG_ERROR, "Invalid led number inputs");
// #endif
//         return 1;
//     }

//     // Check that they are in the right order
//     if (low_led_num > up_led_num) {
// #ifdef DBG_LED
//         log_dbg(LOG_ERROR, "LED selection is backwards");
// #endif
//         return 1;
//     }

//     for (int i = low_led_num; i <= up_led_num; i++) {
//         if (0 != set_led(ld, i, red, green, blue)) {
// #ifdef DBG_LED
//             log_dbg(LOG_ERROR, "Could not update LED");
// #endif
//             return 1;
//         }
//     }

//     return 0;
// }