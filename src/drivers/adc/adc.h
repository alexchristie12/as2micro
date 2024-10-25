#pragma once
#include <stdint.h>
#include "hardware/adc.h"

/// @brief Initialise and ADC sensor
/// @param adc_connection The ADC connection to use
void adc_sensor_init(int adc_connection);

/// @brief Read the data from an ADC sensor 
/// @param adc_min The minimum value from our ADC
/// @param adc_max The maximum value from our ADC
/// @param adc_map_min The value to map the minimum ADC value
/// @param adc_map_max The value to map the maximum ADC value
/// @return The value that was read
float read_adc_sensor(int adc_min, int adc_max, int adc_map_min, int adc_map_max);