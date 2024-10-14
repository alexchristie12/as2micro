#include "temp_and_humidity.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "stdint.h"

uint16_t data;
uint16_t temp_register = 0x00;
uint16_t humidity_register = 0x01;
uint16_t temperature_data;
uint16_t humidity_data;

void read_temp_and_humidity() {
// float* read_temp_and_humidity() {
    temperature_data = i2c_read_multiple_registers(temp_register, &data, 1, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);
    humidity_data = i2c_read_multiple_registers(humidity_register, &data, 1, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);

    float temperature = (165 * (temperature_data / (2^16 - 1)) - 40);
    float humidity = (100 * (humidity_data / (2^16 - 1)));

    printf("temp: %f, humidity: %f\r\n", temperature, humidity);

    float temp_and_humidity[2];
    temp_and_humidity[0] = temperature;
    temp_and_humidity[1] = humidity;
    // return temp_and_humidity;
}