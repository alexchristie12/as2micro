#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <string.h>
#include "drivers/io/io.h"
#include "drivers/i2c/i2c.h"
#include "drivers/adc/adc.h"
#include "sensors/CHT8305C/temp_and_humidity.h"

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
        formats_data_output();
    }
}

int main() {
    io_init();
    i2c_full_init();
    temp_and_humidity_init();
    adc_sensor_init(3);

    snprintf(poll_command, sizeof(poll_command), "poll=%d\n", zone_id);

    for (;;) {
        sends_sensor_data();
        uart_puts(UART_1_ID, output_buffer);
    }
    return 0;
}
