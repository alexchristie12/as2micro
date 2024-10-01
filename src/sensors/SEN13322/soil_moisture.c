#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"

float read_soil_moisture(ij) {
    adc_init();
    adc_gpio_init(ADC0_PIN);
    adc_select_input(0);

    float soil_moisture_percentage;

    float soil_moisture;
    int soil_moisture_max = 1000;
    int soil_moisture_min = 500;

    float gradient = (100 - 0) / (soil_moisture_max - soil_moisture_min);

    soil_moisture = adc_read();

    soil_moisture_percentage = (gradient * soil_moisture + soil_moisture_min);

    return soil_moisture_percentage;
}