#pragma once

/// @brief Initialise the LED
void led_init();

/// @brief Set the color of the LED
/// @param red red value
/// @param green green value
/// @param blue blue value
void set_led_color(uint8_t red, uint8_t green, uint8_t blue);

/// @brief Turn off the LED
void turn_off_led();