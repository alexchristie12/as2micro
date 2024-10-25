#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <string.h>
#include "drivers/io/io.h"
#include "drivers/i2c/i2c.h"
#include "drivers/adc/adc.h"
#include "config.h"
#include "drivers/config_loader/config_loader.h"
#include "drivers/flash/flash.h"
#include "drivers/WS2812/led.h"
#include "sensors/CHT8305C/temp_and_humidity.h"


void parse_i2c_configs_and_initialise(i2c_config *configs);


void parse_adc_configs_and_initialise(adc_config *configs);


void read_all_sensors();


void formats_data_output();


void process_input_commands();

void system_setup();