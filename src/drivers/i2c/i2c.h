#pragma once

#include "stdint.h"
#include "hardware/i2c.h"

/// @brief Select an I2C connection
/// @param i2c_connection The I2C connection to select
/// @return I2C instance type
i2c_inst_t* i2c_select_connection(int i2c_connection);

/// @brief Initialise the I2C connection
/// @param i2c_connection The I2C connection
void i2c_full_init(int i2c_connection);

/// @brief Write to an I2C connection
/// @param i2c_connection The I2C connection
/// @param reg The register to write do
/// @param data The data to write
/// @param device_addr The device I2C address
/// @return Success code
int i2c_write_register(int i2c_connection, uint8_t reg, uint16_t data, uint8_t device_addr);

/// @brief Read multiple I2C registers
/// @param i2c_connection The I2C connection
/// @param reg The register to read from
/// @param data The data buffer
/// @param length Length of data buffer
/// @param device_addr Device I2C address
/// @return Success code
int i2c_read_multiple_registers(int i2c_connection, uint8_t reg, uint8_t *data, int length, uint8_t device_addr);

