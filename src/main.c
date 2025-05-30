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
#include "tasks/tasks.h"


int main() {
    io_init();
    system_setup();


    for (;;) {
        process_input_commands();
    }
    return 0;
}
