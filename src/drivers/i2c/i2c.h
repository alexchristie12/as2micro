#pragma once

#include "stdint.h"
#include "hardware/i2c.h"

i2c_inst_t* i2c_select_connection(int i2c_connection);

void i2c_full_init(int i2c_connection);

int i2c_write_register(int i2c_connection, uint8_t reg, uint16_t data, uint8_t device_addr);

int i2c_read_multiple_registers(int i2c_connection, uint8_t reg, uint8_t *data, int length, uint8_t device_addr);

