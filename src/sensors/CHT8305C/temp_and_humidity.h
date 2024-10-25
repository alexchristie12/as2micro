#pragma once

#include "drivers/i2c/i2c.h"

#define TEMP_AND_HUMIDITY_SLAVE_ADDRESS 0x40

/// @brief Initialise the temperature and humidity sensors
/// @param i2c_connection The i2c connection that we want to use
void temp_and_humidity_init(int i2c_connection);

/// @brief Read temperature and humidity from the I2C bus
/// @param i2c_connection The I2C connection to read from
/// @return Returns a float[2] to each value
float* read_temp_and_humidity(int i2c_connection);