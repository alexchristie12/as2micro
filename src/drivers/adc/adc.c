#include <stdint.h>
#include "hardware/adc.h"

uint8_t adc_pin = 26;
uint16_t adc_raw_input = 0;
float adc_mapped_output = 0;

void adc_sensor_init(int adc_connection) {
    // Sent the ADC pin number based on the board connection
    switch (adc_connection)
    {
    case 0:
        adc_pin = 26;
        break;
    case 1:
        adc_pin = 27;
        break;
    case 2:
        adc_pin = 28;
        break;
    case 3:
        adc_pin = 29;
        break;
    default:
        break;
    }
    adc_gpio_init(adc_pin);
    adc_select_input(adc_connection);
}

float read_adc_sensor(int adc_min, int adc_max, int adc_map_min, int adc_map_max) {
    // Determine the gradiant for the mapping function
    float gradient = (float)(adc_map_max - adc_map_min) / (float)(adc_max - adc_min);

    adc_raw_input = adc_read();

    // Map the raw value to the required range
    adc_mapped_output = (gradient * adc_raw_input + adc_min);

    // Map the values to the mapped max and min if out of range
    if (adc_mapped_output > adc_map_max)
    {
        adc_mapped_output = adc_map_max;
    }
    else if (adc_mapped_output < adc_map_min)
    {
        adc_mapped_output = adc_map_min;
    }
    return adc_mapped_output;
}
