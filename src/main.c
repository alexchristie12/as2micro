#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "config.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/WS2812/led.h"
#include "drivers/io/io.h"


#include "drivers/i2c/i2c.h"
#include "drivers/accel/accel.h"

#include "tasks/tasks.h"

static led_data global_led_data = {
    .reds   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .greens = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .blues  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

#define TEMP_AND_HUMIDITY_SLAVE_ADDRESS 0x80

float read_temp_and_humidity() {
    uint16_t data;
    uint16_t temp_register = 0x00;
    uint16_t humidity_register = 0x01;
    uint16_t temperature_data;
    uint16_t humidity_data;

    temperature_data = i2c_read_multiple_registers(temp_register, data, 1, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);
    humidity_data = i2c_read_multiple_registers(humidity_register, data, 1, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);

    float temperature = (165 * (temperature_data / (2^16 - 1)) - 40);
    float humidity = (100 * (humidity_data / (2^16 - 1)));

    float temp_and_humidity[2];
    temp_and_humidity[0] = temperature;
    temp_and_humidity[1] = humidity;
    return temp_and_humidity[];
}

float read_soil_moisture() {
    adc_init();
    adc_gpio_init(ADC0_PIN);
    adc_select_input(0);

    float soil_moisture_percentage;

    float soil_moisture;
    int soil_moisture_max = 1000;
    int soil_moisture_min = 500;

    float gradient = (100 - 0) / (soil_moisture_max - soil_moisture_min)

    soil_moisture = adc_read();

    soil_moisture_percentage = (gradient * soil_moisture + soil_moisture_min);

    return soil_moisture_percentage;
}


int main() {
    io_init();
    // Do nothing for now
    // General initiations
    tasks_init();
    (void)led_init();

    // Initially switch off the LEDs
    switch_off_leds(&global_led_data);
    update_leds(&global_led_data);

#ifdef ACCEL_TASK
    i2c_full_init();
    (void)accel_init();
#endif

    char led_buffer[100];
    bool blocking = true;

    for (;;) {
        if (get_current_task() == TASK_IDLE) {
            switch_off_leds(&global_led_data);
            update_leds(&global_led_data);
            sleep_ms(1);
            continue;
        }

#ifdef LED_TASK
        if (get_current_task() == TASK_LED) {
            io_poll(led_buffer, sizeof(led_buffer));
            // Handle the command
            handle_led_command(led_buffer, sizeof(led_buffer), &global_led_data);
        }
#endif

#ifdef ACCEL_TASK
        if (get_current_task() == TASK_ACCEL) {
        // Implement Accelerometer task here
        vector_3d new_readings = accel_get_values();
        set_leds_from_accel(new_readings, &global_led_data);
        update_leds(&global_led_data);
        sleep_ms(200);
        }
#endif

#ifdef MICROPHONE_TASK
        // Implement Microphone task here
        log_dbg(LOG_WARNING, "Not yet implemented");
        sleep_ms(5000);
#endif

#ifdef BLUETOOTH_TASK
        // Implement bluetooth task here
        log_dbg(LOG_WARNING, "Not yet implemented");
        sleep_ms(5000);
#endif
    }
    return 0;
}
