#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
// #include "hardware/adc.h"

// #include "config.h"
// #include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
// #include "drivers/logging/logging.h"
// #include "drivers/WS2812/led.h"

#include "sensors/CHT8305C/temp_and_humidity.h"
// #include "drivers/io/io.h"

#include "drivers/i2c/i2c.h"
#include "drivers/adc/adc.h"

// #include "tasks/tasks.h"

// static led_data global_led_data = {
//     .reds   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     .greens = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     .blues  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
// };

float soil_moisture = 0;

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
    temp_and_humidity_init();
    adc_sensor_init(3);

    for (;;) {
        read_temp_and_humidity();
        soil_moisture = read_adc_sensor(0, 545, 0, 1000);
        printf("moisture: %f\r\n", soil_moisture);
        sleep_ms(1000);
    }
    return 0;
}
