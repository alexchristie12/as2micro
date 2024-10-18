#include "drivers/i2c/i2c.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "drivers/WS2812/led.h"
#include "config.h"

i2c_inst_t* i2c_select_connection(int i2c_connection){
    i2c_inst_t *i2c_instance = I2C_0_INSTANCE;
    int sda_pin = I2C_0_SDA_PIN;
    int scl_pin = I2C_0_SCL_PIN;
    if (i2c_connection == 0) {
        i2c_inst_t *i2c_instance = I2C_0_INSTANCE;
        int sda_pin = I2C_0_SDA_PIN;
        int scl_pin = I2C_0_SCL_PIN;
    } else if (i2c_connection == 1) {
        i2c_inst_t *i2c_instance = I2C_1_INSTANCE;
        int sda_pin = I2C_1_SDA_PIN;
        int scl_pin = I2C_1_SCL_PIN;
    }
    return i2c_instance;
}

void i2c_full_init(int i2c_connection) {
    i2c_inst_t *i2c_instance = I2C_0_INSTANCE;
    int sda_pin = I2C_0_SDA_PIN;
    int scl_pin = I2C_0_SCL_PIN;
    if (i2c_connection == 0) {
        i2c_inst_t *i2c_instance = I2C_0_INSTANCE;
        int sda_pin = I2C_0_SDA_PIN;
        int scl_pin = I2C_0_SCL_PIN;
    } else if (i2c_connection == 1) {
        i2c_inst_t *i2c_instance = I2C_1_INSTANCE;
        int sda_pin = I2C_1_SDA_PIN;
        int scl_pin = I2C_1_SCL_PIN;
    }
    i2c_init(i2c_instance, 100 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

int i2c_write_register(int i2c_connection, uint8_t reg, uint16_t data, uint8_t device_addr) {
    i2c_inst_t *i2c_instance = i2c_select_connection(i2c_connection);
    uint8_t buf[3]        = {reg, (data >> 8), (data & 0xFF)};
    int     bytes_written = i2c_write_blocking(i2c_instance, device_addr, buf, 3, false);
    return (bytes_written == 3) ? 0 : 1;
}

int i2c_read_multiple_registers(int i2c_connection, uint8_t reg, uint8_t *data, int length, uint8_t device_addr) {
    i2c_inst_t *i2c_instance = i2c_select_connection(i2c_connection);
    if (1 != i2c_write_blocking(i2c_instance, device_addr, &reg, 1, true)) {
        return 1;
    }
    sleep_ms(20); 
    int bytes_read = i2c_read_blocking(i2c_instance, device_addr, data, length, false);
    return (bytes_read == length) ? 0 : 1;
}
