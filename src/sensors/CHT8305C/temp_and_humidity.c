#include "temp_and_humidity.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "stdint.h"
#include "config.h"
#include "hardware/i2c.h"

uint8_t data_out[4] = {0};
uint8_t config_register = 0x02;
uint16_t config_data = 0x1000;
uint8_t temp_register = 0x00;
uint16_t temperature_data;
uint16_t humidity_data;
int bytes_read;

float temp_and_humidity[2];

void temp_and_humidity_init(int i2c_connection) {
    i2c_inst_t *i2c_instance_to_read = I2C_0_INSTANCE;
    if (i2c_connection == 0)
    {
        i2c_inst_t *i2c_instance_to_read = I2C_0_INSTANCE;
    }
    else if (i2c_connection == 1)
    {
        i2c_inst_t *i2c_instance_to_read = I2C_1_INSTANCE;
    }
    bytes_read = i2c_write_register(i2c_instance_to_read, config_register, config_data, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);
}

// void read_temp_and_humidity() {
float* read_temp_and_humidity(int i2c_connection) {
    i2c_inst_t *i2c_instance_to_read = I2C_0_INSTANCE;
    if (i2c_connection == 0)
    {
        i2c_inst_t *i2c_instance_to_read = I2C_0_INSTANCE;
    }
    else if (i2c_connection == 0)
    {
        i2c_inst_t *i2c_instance_to_read = I2C_1_INSTANCE;
    }
    bytes_read = i2c_read_multiple_registers(i2c_instance_to_read, temp_register, &data_out, 4, TEMP_AND_HUMIDITY_SLAVE_ADDRESS);
    temperature_data = ((data_out[0] << 8) | data_out[1]);
    humidity_data = ((data_out[2] << 8) | data_out[3]);

    float temperature = ((float)temperature_data * 165.0 / 65535.0) - 40.0;
    float humidity = ((float)humidity_data / 65535.0) * 100.0;
    // printf("temp: %.2f\r\n", temperature);
    printf("temp: %.2f, humidity: %.2f\r\n", temperature, humidity);

    temp_and_humidity[0] = temperature;
    temp_and_humidity[1] = humidity;
    return temp_and_humidity;
}