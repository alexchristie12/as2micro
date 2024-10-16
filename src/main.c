#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "config.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/WS2812/led.h"
#include "drivers/io/io.h"

int main() {
    io_init();
    // 5 zones
    int offset = 0;
    float temperature[] = {23, 23.2, 23.1, 23}; // 4 readings from 4 different temp sensors
    float humidity[] = {76, 75.4, 75.8};
    float soil_moisture[] = {40, 40.3};
    float flow_rate = 46.3;
    bool water_on; // tell the pi if the water is running (1) or not (0)
    int zone_number = 1; // alex config to do

    void sensor_data (int zone_number){
        // Rotate through for each zone:
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "zone=%i,", zone_number);
        // Temperature Sensors:
        for (int i = 0; i < sizeof(temp); i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "temperature%i=%.2f,", i, temperature[i]);
        }
        // Humidity Sensors:
        for (int i = 0; i < sizeof(humidity); i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "humidity%i=%.2f,", i, humidity[i]);
        }
        // Soil Moisture Sensors:
        for (int i = 0; i < sizeof(soil_moisture); i++) {
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "soil_moisture%i=%.2f,", i, soil_moisture[i]);
        }
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "flow_rate=%.2f,water_on=%d", flow_rate, water_on);
    }
   
    for (;;) {
        char buffer[1024];
        sensor_data(zone_number);
        uart_puts(UART_ID, buffer); 
    }
    return 0;
}