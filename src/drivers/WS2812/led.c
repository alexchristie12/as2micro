#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "led.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "config.h"
#include "stdint.h"
#include <string.h>

uint32_t led_data;

void led_init(){
    // Initialize PIO0 to control the LED chain
    uint32_t pio_program_offset = pio_add_program(LED_PIO, &ws2812_program);
    ws2812_program_init(LED_PIO, 0, pio_program_offset, OUTPUT_LED_PIN, 800000, false);
}

void set_led_color(uint8_t red, uint8_t green, uint8_t blue) {
    // Set the colour of the LED
    led_data = (red << 24) | (green << 16) | (blue << 8);
    pio_sm_put_blocking(pio0, 0, led_data);
}

void turn_off_led() {
    // Turn off the LED
    led_data = 0;
    pio_sm_put_blocking(pio0, 0, led_data);
}