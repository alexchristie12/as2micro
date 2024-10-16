#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <string.h>
#include "drivers/io/io.h"
#include "drivers/i2c/i2c.h"
#include "drivers/adc/adc.h"
#include "sensors/CHT8305C/temp_and_humidity.h"
#include "drivers/config_loader/config_loader.h"

// 5 zones
int   offset = 0;
float temperature[2]; // 4 readings from 4 different temp sensors
float humidity[2];
float soil_moisture[4];
float flow_rate[4];
bool  water_on;        // tell the pi if the water is running (1) or not (0)
int   zone_number = 1; // alex config to do

char poll_command[50];
int  zone_id = 1;
char received_buffer[200];
char output_buffer[1024];

int i2c_sensor_count = 2;
int i2c_sensor_type = 1;
int temperature_and_humidity_sensor_counter = 0;

int adc_sensor_count = 3;
int adc_sensor_type = 1;
int soil_moisture_sensor_counter = 0;
int flow_rate_sensor_counter = 0;

rmu_config config;

void determines_and_initialises_connected_sensors(){
}

void reads_all_sensors(){
    //Loop through i2c sensors and read them
    for (size_t i = 0; i < i2c_sensor_count; i++)
    {
        if (i2c_sensor_type == 1)
        {
            float* temperature_and_humidity = read_temp_and_humidity(i);
            temperature[i] = temperature_and_humidity[0];
            humidity[i] = temperature_and_humidity[1];
        }      
    }
    // loop through adc sensors and read them
    for (size_t i = 0; i < adc_sensor_count; i++)
    {
        adc_select_input(i);
        if (adc_sensor_type == 2)
        {
            // soil moisture
            soil_moisture[soil_moisture_sensor_counter] = read_adc_sensor(0, 545, 0, 900);
            soil_moisture_sensor_counter++;
        }
        else if (adc_sensor_type == 3)
        {
            flow_rate[flow_rate_sensor_counter] = read_adc_sensor(0, 450, 0, 20);
            flow_rate_sensor_counter++;
        }
    }
    soil_moisture_sensor_counter = 0;
    flow_rate_sensor_counter++;
}

void formats_data_output() {
    // Rotate through for each zone:
    memset((char *)output_buffer, '\000', sizeof(output_buffer));
    offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "hardware_id=%i,", zone_number);
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
    for (int i = 0; i < sizeof(flow_rate); i++)
    {
        offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "flow_rate_%i=%.2f,", i, flow_rate[i]);
    }
    // Water Valve On/Off - 0 = off, 1 = on
    offset += snprintf(output_buffer + offset, sizeof(output_buffer) - offset, "water_on=%d", water_on);
}

void sends_sensor_data() {
    memset((char *)received_buffer, '\000', sizeof(received_buffer));
    io_poll(received_buffer, 200);
    if (strcmp(received_buffer, poll_command) == 0) {
        reads_all_sensors();
        formats_data_output();
    }
}

int main() {
    io_init();
    i2c_full_init();
    // temp_and_humidity_init();
    // adc_sensor_init(3);

    snprintf(poll_command, sizeof(poll_command), "poll=%d\n", zone_id);

    for (;;) {
        sends_sensor_data();
        uart_puts(UART_1_ID, output_buffer);
    }
    return 0;
}
