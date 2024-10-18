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

float temperature[2]; // 2 readings from 2 possible temp sensors
float humidity[2]; // 2 readings from 2 possible humidity sensors
float soil_moisture[4]; // 4 readings from 4 possible soil moisture sensors
float flow_rate[4]; // 4 readings from 4 possible flow rates sensors
bool  water_on;        // tell the pi if the water is running (1) or not (0)

char poll_command[50];
char water_on_command[50];
char water_off_command[50];
char received_buffer[200];
static char output_buffer[1024];
int output_offset = 0;

static rmu_config default_config = {
    .general_config = {
        .dob_unix = 0,
        .hardware_id = 1,
        .name = "default_config"
    },
    .i2c_configs = {
        {
            .name = "temperature_01",
            .type = TEMPERATURE_HUMIDITY_SENSOR,
        },
        {
            .name = "temperature_02",
            .type = DISABLED_SENSOR,
        },
    },
    .adc_configs = {
        {
            .name = "soil_moisture_01",
            .type = SOIL_MOISTURE_SENSOR,
        },
        {
            .name = "soil_moisture_02",
            .type = DISABLED_SENSOR,
        },
        {
            .name = "soil_moisture_03",
            .type = SOIL_MOISTURE_SENSOR,
        },
        {
            .name = "flow_rate_01",
            .type = DISABLED_SENSOR,
        },
    },
};

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
        if (adc_sensor_type[i] == SOIL_MOISTURE_SENSOR) {
            adc_sensor_init(i);
        }
        else if (adc_sensor_type[i] == FLOW_RATE_SENSOR)
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
        if (i2c_sensor_type[i] == DISABLED_SENSOR) {
            temperature[i] = 99999;
            humidity[i] = 99999;
        }
    }
    // loop through adc sensors and read them
    for (size_t i = 0; i < 4; i++) {
        adc_select_input(i);
        if (adc_sensor_type[i] == SOIL_MOISTURE_SENSOR) {
            soil_moisture[i] = read_adc_sensor(0, 545, 0, 900);
        } else if (adc_sensor_type[i] == FLOW_RATE_SENSOR) {
            flow_rate[i] = read_adc_sensor(0, 450, 0, 20);
        } else if (adc_sensor_type[i] == DISABLED_SENSOR)
        {
            soil_moisture[i] = 99999;
            flow_rate[i] = 99999;
        }
        
    }
}

void formats_data_output() {
    // Rotate through for each zone:
    memset((char *)output_buffer, '\000', sizeof(output_buffer));
    output_offset = 0;
    output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "hardware_id=%i,", default_config.general_config.hardware_id);
    // Temperature Data:
    for (int i = 0; i < 2; i++) {
        output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "temperature_%i=%.2f,", i, temperature[i]);
    }
    // Humidity Data:
    for (int i = 0; i < 2; i++) {
        output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "humidity_%i=%.2f,", i, humidity[i]);
    }
    // Soil Moisture Data:
    for (int i = 0; i < 4; i++) {
        output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "soil_moisture_%i=%.2f,", i, soil_moisture[i]);
    }
    // Flow Rate Data
    for (int i = 0; i < 4; i++) {
        output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "flow_rate_%i=%.2f,", i, flow_rate[i]);
    }
    // Water Valve On/Off - 0 = off, 1 = on
    output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "water_on=%d", water_on);
    output_offset += snprintf(output_buffer + output_offset, sizeof(output_buffer) - output_offset, "\r\n");
    uart_puts(UART_1_ID, output_buffer);
}

void decode_input_commands() {
    memset((char *)received_buffer, '\000', sizeof(received_buffer));
    io_poll(received_buffer, 200);
    if ((strcmp(received_buffer, poll_command)) == 0) {
        reads_all_sensors();
        formats_data_output();
    }
    else if (strcmp(received_buffer, water_on_command) == 0) {
        set_led_color(0, 0, 255);
        water_on = true;
    }
    else if (strcmp(received_buffer, water_off_command) == 0) {
        turn_off_led();
        water_on = false;
    }
}

int main() {
    io_init();
    parse_i2c_configs_and_initialise(default_config.i2c_configs);
    parse_adc_configs_and_initialise(default_config.adc_configs);
    led_init();

    snprintf(poll_command, sizeof(poll_command), "poll=%d", default_config.general_config.hardware_id);
    snprintf(water_on_command, sizeof(water_on_command), "water_on=%d", default_config.general_config.hardware_id);
    snprintf(water_off_command, sizeof(water_off_command), "water_off=%d", default_config.general_config.hardware_id);

    for (;;) {
        decode_input_commands();
    }
    return 0;
}
