#pragma once

typedef struct {
    /* data */
    uint8_t reds[12];
    uint8_t greens[12];
    uint8_t blues[12];
} led_data;

typedef enum {
    LED_CMD_SET,
    LED_CMD_UPDATE,
    LED_CMD_OFF,
} LED_COMMAND;

/// @brief Initialise the board leds.
/// @return Return `0` if successful, `1` if failure.
int led_init();

/// @brief Set a particular LED to a new colour
/// @param ld The Led Data struct instance
/// @param led_num LED number to set `[0,NUMBER_OF_LEDS]`. See `config.h`
/// @param red Value of red to set
/// @param green Value of green to set
/// @param blue Value of blue of set
/// @return `0` on success, `1` on failure
int set_led(led_data* ld, uint8_t led_num, uint8_t red, uint8_t green, uint8_t blue);

/// @brief Update the LEDS
/// @param ld The Led Data struct instance
void update_leds(led_data* ld);

/// @brief Swith off all the LEDs, does not update
/// @param ld The Led Data struct instance
void switch_off_leds(led_data* ld);

/// @brief Handle the command from serial input
/// @param cmd_buffer Buffer containing command
/// @param cmd_buffer_len Length of buffer containing command
/// @param ld The Led Data struct instance
/// @return `0` on success, `1` on failure
int handle_led_command(char* cmd_buffer, int cmd_buffer_len, led_data* ld);

/// @brief Set multiple LEDs to the same colour, does not update
/// @param ld The Led Data struct instance
/// @param low_led_num Lower LED number, inclusive
/// @param up_led_num Upper LED number, exclusive
/// @param red The red level
/// @param green The green level
/// @param blue The blue level
/// @return `0` on success, `1` on failure
int set_multiple_leds(led_data *ld, uint8_t low_led_num, uint8_t up_led_num, uint8_t red, uint8_t green, uint8_t blue);
