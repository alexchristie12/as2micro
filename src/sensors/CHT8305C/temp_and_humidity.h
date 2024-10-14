#pragma once

#include "drivers/i2c/i2c.h"

#define TEMP_AND_HUMIDITY_SLAVE_ADDRESS 0x80

void read_temp_and_humidity();