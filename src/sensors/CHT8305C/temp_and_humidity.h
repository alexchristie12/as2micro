#pragma once

#include "drivers/i2c/i2c.h"

#define TEMP_AND_HUMIDITY_SLAVE_ADDRESS 0x40

void temp_and_humidity_init(int i2c_connection);

float* read_temp_and_humidity(int i2c_connection);