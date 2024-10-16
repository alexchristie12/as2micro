#pragma once

#include "stdint.h"
#include "hardware/i2c.h"

void i2c_full_init();


int i2c_write_register(i2c_inst_t *i2c_instance_to_read, uint8_t reg, uint16_t data, uint8_t device_addr);

int i2c_read_multiple_registers(i2c_inst_t *i2c_instance_to_read, uint8_t reg, uint8_t *data, int length, uint8_t device_addr);

