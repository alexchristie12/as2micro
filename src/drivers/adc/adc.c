#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"

uint8_t adc_pin = 26;

float read_adc_sensor(uint8_t adc_connection, int adc_min, int adc_max, int adc_map_min, int adc_map_max) {
    adc_init();
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

    float adc_mapped_output;

    float adc_raw_input;

    float gradient = (adc_map_max - adc_map_min) / (adc_max - adc_min);

    adc_raw_input = adc_read();

    adc_mapped_output = (gradient * adc_raw_input + adc_min);

    return adc_mapped_output;
}