#include "drivers/i2c/i2c.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "drivers/WS2812/led.h"
#include "config.h"

void i2c_full_init() {
    i2c_init(I2C_INSTANCE, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_down(SCL_PIN);
}

int i2c_write_register(uint8_t reg, uint8_t data, uint8_t device_addr) {
    uint8_t buf[2]        = {reg, data};
    int     bytes_written = i2c_write_blocking(I2C_INSTANCE, device_addr, buf, 2, false);
    return (bytes_written == 2) ? 0 : 1;
}

int i2c_read_multiple_registers(uint8_t reg, uint8_t *data, int length, uint8_t device_addr) {
    // Tell the device that we are reading multiple registers
    if (length > 1)
    {
        reg |= 0x80; // Set MSB to 1
    }
    if (1 != i2c_write_blocking(I2C_INSTANCE, device_addr, &reg, 1, true)) {
        return 1;
    }

    int bytes_read = i2c_read_blocking(I2C_INSTANCE, device_addr, data, length, false);
    return (bytes_read == length) ? 0 : 1;
}