#include "drivers/i2c/i2c.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "drivers/WS2812/led.h"
#include "config.h"

void i2c_full_init() {
    i2c_init(I2C_1_INSTANCE, 100 * 1000);
    gpio_set_function(I2C_1_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_1_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_1_SDA_PIN);
    gpio_pull_up(I2C_1_SCL_PIN);
}

int i2c_write_register(uint8_t reg, uint16_t data, uint8_t device_addr) {
    uint8_t buf[3]        = {reg, (data >> 8), (data & 0xFF)};
    int     bytes_written = i2c_write_blocking(I2C_1_INSTANCE, device_addr, buf, 3, false);
    return (bytes_written == 3) ? 0 : 1;
}

int i2c_read_multiple_registers(uint8_t reg, uint8_t *data, int length, uint8_t device_addr) {
    // Tell the device that we are reading multiple registers
    // if (length > 1)
    // {
    //     reg |= 0x80; // Set MSB to 1
    // }

    if (1 != i2c_write_blocking(I2C_1_INSTANCE, device_addr, &reg, 1, true)) {
        return 1;
    }

    sleep_ms(20); 

    int bytes_read = i2c_read_blocking(I2C_1_INSTANCE, device_addr, data, length, false);
    return (bytes_read == length) ? 0 : 1;
}