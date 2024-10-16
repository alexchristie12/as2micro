#pragma once
#include <stdint.h>
#include "hardware/adc.h"

void adc_sensor_init(int adc_connection);

float read_adc_sensor(int adc_min, int adc_max, int adc_map_min, int adc_map_max);