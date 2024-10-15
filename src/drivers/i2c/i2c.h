#pragma once

#include "stdint.h"

void i2c_full_init();


int i2c_write_register(uint8_t reg, uint16_t data, uint8_t device_addr);

int i2c_read_multiple_registers(uint8_t reg, uint8_t *data, int length, uint8_t device_addr);

