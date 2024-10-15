#pragma once

#include "drivers/i2c/i2c.h"

#define TEMP_AND_HUMIDITY_SLAVE_ADDRESS 0x40

void temp_and_humidity_init();

void read_temp_and_humidity();