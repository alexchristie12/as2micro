#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

// #include "config.h"
// #include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
// #include "drivers/logging/logging.h"
// #include "drivers/WS2812/led.h"

#include "sensors/CHT8305C/temp_and_humidity.h"
// #include "drivers/io/io.h"

#include "drivers/i2c/i2c.h"

// #include "tasks/tasks.h"

// static led_data global_led_data = {
//     .reds   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     .greens = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     .blues  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
// };

int main() {
    // io_init();
    // Do nothing for now
    // General initiations
    // tasks_init();
    // (void)led_init();

    // // Initially switch off the LEDs
    // switch_off_leds(&global_led_data);
    // update_leds(&global_led_data);

    // char led_buffer[100];
    // bool blocking = true;
    stdio_init_all();
    stdio_uart_init_full(uart0, 115200, 16, 17);
    i2c_full_init();

    for (;;) {
        read_temp_and_humidity();
        sleep_ms(1000);
        // if (get_current_task() == TASK_IDLE) {
        //     switch_off_leds(&global_led_data);
        //     update_leds(&global_led_data);
        //     sleep_ms(1);
        //     continue;
        // }
    }
    return 0;
}
