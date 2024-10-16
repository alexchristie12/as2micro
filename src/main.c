#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <string.h>
#include "drivers/io/io.h"
#include "drivers/i2c/i2c.h"
#include "drivers/adc/adc.h"
#include "drivers/config_loader/config_loader.h"
#include "drivers/WS2812/led.h"
#include "sensors/CHT8305C/temp_and_humidity.h"

// 5 zones
int   offset = 0;
float temperature[2]; // 4 readings from 4 different temp sensors
float humidity[2];
float soil_moisture[4];
float flow_rate[4];
bool  water_on;        // tell the pi if the water is running (1) or not (0)

char poll_command[50];
char water_on_command[50];
char water_off_command[50];
int  zone_id = 1;
char received_buffer[200];
char output_buffer[1024];

sensor_type i2c_sensor_type[2] = {DISABLED_SENSOR};
sensor_type adc_sensor_type[4] = {DISABLED_SENSOR};

/// @param configs - This is a pointer to the array of i2c configs, access it like an array
void parse_i2c_configs_and_initialise(i2c_config *configs) {
    for (size_t i = 0; i < 2; i++) {
        i2c_sensor_type[i] = configs[i].type;
        if (i2c_sensor_type[i] == TEMPERATURE_HUMIDITY_SENSOR) {
            i2c_full_init(i);
            temp_and_humidity_init(i);
        }
    }
}

void parse_adc_configs_and_initialise(adc_config *configs) {
    adc_init();
    for (size_t i = 0; i < 4; i++) {
        adc_sensor_type[i] = configs[i].type;
        if (i2c_sensor_type[i] == SOIL_MOISTURE_SENSOR) {
            adc_sensor_init(i);
        }
        else if (i2c_sensor_type[i] == FLOW_RATE_SENSOR)
        {
            adc_sensor_init(i);
        }
    }
}

void reads_all_sensors() {
    // Loop through i2c sensors and read them
    for (size_t i = 0; i < 2; i++) {
        if (i2c_sensor_type[i] == TEMPERATURE_HUMIDITY_SENSOR) {
            float *temperature_and_humidity = read_temp_and_humidity(i);
            temperature[i] = temperature_and_humidity[0];
            humidity[i] = temperature_and_humidity[1];
        }
    }
    // loop through adc sensors and read them
    for (size_t i = 0; i < 4; i++) {
        adc_select_input(i);
        if (adc_sensor_type[i] == SOIL_MOISTURE_SENSOR) {
            soil_moisture[i] = read_adc_sensor(0, 545, 0, 900);
        } else if (adc_sensor_type[i] == FLOW_RATE_SENSOR) {
            flow_rate[i] = read_adc_sensor(0, 450, 0, 20);
        }
    }
}

void formats_data_output() {
    // Rotate through for each zone:
    memset((char *)output_buffer, '\000', sizeof(output_buffer));
    offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "hardware_id=%i,", zone_id);
    // Temperature Data:
    for (int i = 0; i < sizeof(temperature); i++) {
        offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "temperature_%i=%.2f,", i, temperature[i]);
    }
    // Humidity Data:
    for (int i = 0; i < sizeof(humidity); i++) {
        offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "humidity_%i=%.2f,", i, humidity[i]);
    }
    // Soil Moisture Data:
    for (int i = 0; i < sizeof(soil_moisture); i++) {
        offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "soil_moisture_%i=%.2f,", i, soil_moisture[i]);
    }
    // Flow Rate Data
    for (int i = 0; i < sizeof(flow_rate); i++) {
        offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "flow_rate_%i=%.2f,", i, flow_rate[i]);
    }
    // Water Valve On/Off - 0 = off, 1 = on
    offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "water_on=%d", water_on);
}

void decode_input_commands() {
    memset((char *)received_buffer, '\000', sizeof(received_buffer));
    io_poll(received_buffer, 200);
    if (strcmp(received_buffer, poll_command) == 0) {
        reads_all_sensors();
        formats_data_output();
    }
    else if (strcmp(received_buffer, water_on_command) == 0) {
        set_led_color(0, 0, 255);
    }
    else if (strcmp(received_buffer, water_off_command) == 0) {
        turn_off_led();
    }
    
}

int main() {
    io_init();
    parse_i2c_configs_and_initialise();
    parse_adc_configs_and_initialise();
    led_init();

    snprintf(poll_command, sizeof(poll_command), "poll=%d\n", zone_id);
    snprintf(water_on_command, sizeof(water_on_command), "water_on=%d\n", zone_id);
    snprintf(water_off_command, sizeof(water_off_command), "water_off=%d\n", zone_id);

    for (;;) {
        decode_input_commands();
    }
    return 0;
}
