#include "drivers/config_loader/config_loader.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static rmu_config default_config = {
    .general_config =
        {
            .hardware_id = 0,
        },
    .i2c_configs =
        {
            {
                .type = TEMPERATURE_HUMIDITY_SENSOR,
            },
            {
                .type = TEMPERATURE_HUMIDITY_SENSOR,
            },
        },
    .adc_configs =
        {
            {
                .type = SOIL_MOISTURE_SENSOR,
            },
            {
                .type = SOIL_MOISTURE_SENSOR,
            },
            {
                .type = SOIL_MOISTURE_SENSOR,
            },
            {
                .type = FLOW_RATE_SENSOR,
            },
        },
};

static inline void load_registers_from_16_bit(uint16_t number, uint8_t* addr) {
    addr[0] = number & 0xFF;
    addr[1] = (number >> 8) & 0xFF;
}

static inline void load_registers_from_32_bit(uint32_t number, uint8_t* addr) {
    addr[0] = number & 0xFF;
    addr[1] = (number >> 8) & 0xFF;
    addr[2] = (number >> 16) & 0xFF;
    addr[3] = (number >> 24) & 0xFF;
}

static inline uint32_t load_32_bit_from_registers(uint8_t* registers) {
    return (uint32_t)registers[0] | ((uint32_t)registers[1] << 8) | ((uint32_t)registers[2] << 16) | ((uint32_t)registers[3] << 24);
}

static inline uint16_t load_16_bit_from_registers(uint8_t* registers) {
    return (uint16_t)registers[0] | ((uint16_t)registers[1] << 8);
}

static inline void load_registers_from_float(float number, uint8_t* addr) {
    // This is brilliant
    union {
        float    f;
        uint32_t u;
    } float_union;

    float_union.f = number;
    load_registers_from_32_bit(float_union.u, addr);
}

static inline float load_float_from_registers(uint8_t* registers) {
    union {
        float    f;
        uint32_t u;
    } float_union;
    float_union.u = load_32_bit_from_registers(registers);
    return float_union.f;
}

static inline sensor_type match_sensor_type(uint8_t num) {
    sensor_type st;
    switch (num) {
        case 1:
            st = TEMPERATURE_HUMIDITY_SENSOR;
            break;
        case 2:
            st = SOIL_MOISTURE_SENSOR;
            break;
        case 3:
            st = FLOW_RATE_SENSOR;
            break;
        case 4:
            st = DISABLED_SENSOR;
            break;
        default:
            st = INVALID_SENSOR;
            break;
    }
    return st;
}

void load_config_from_memory(uint8_t* registers, rmu_config* config) {
    // This is much better
    config->general_config.hardware_id = registers[0];
    config->i2c_configs[0].type        = registers[1];
    config->i2c_configs[1].type        = registers[2];
    config->adc_configs[0].type        = registers[3];
    config->adc_configs[1].type        = registers[4];
    config->adc_configs[2].type        = registers[5];
    config->adc_configs[3].type        = registers[6];
}

static inline uint8_t match_sensor_type_to_int(sensor_type st) {
    uint8_t corresponding_int;
    switch (st) {
        case TEMPERATURE_HUMIDITY_SENSOR:
            corresponding_int = 1;
            break;
        case SOIL_MOISTURE_SENSOR:
            corresponding_int = 2;
            break;
        case FLOW_RATE_SENSOR:
            corresponding_int = 3;
            break;
        case DISABLED_SENSOR:
            corresponding_int = 4;
            break;
        default:
            corresponding_int = 0;
            break;
    }
    return corresponding_int;
}

void load_config_to_registers(uint8_t* registers, rmu_config* config) {
    uint8_t config_memory[7];
    config_memory[0] = config->general_config.hardware_id;
    config_memory[1] = config->i2c_configs[0].type;
    config_memory[2] = config->i2c_configs[1].type;
    config_memory[3] = config->adc_configs[0].type;
    config_memory[4] = config->adc_configs[1].type;
    config_memory[5] = config->adc_configs[2].type;
    config_memory[6] = config->adc_configs[3].type;
}

void load_config_from_user(char* buffer, rmu_config* config) {
    uint8_t hardware_id;
    uint8_t i2c_type_0;
    uint8_t i2c_type_1;
    uint8_t adc_type_0;
    uint8_t adc_type_1;
    uint8_t adc_type_2;
    uint8_t adc_type_3;
    printf("%s\n\r", buffer);
    int res = sscanf((const char*)buffer, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &hardware_id, &i2c_type_0, &i2c_type_1, &adc_type_0, &adc_type_1, &adc_type_2, &adc_type_3);
    printf("We managed to get %d matches\r\n", res);
    if (res != 7) {
        return;
    }
    config->general_config.hardware_id = hardware_id;
    config->i2c_configs[0].type        = match_sensor_type(i2c_type_0);
    config->i2c_configs[1].type        = match_sensor_type(i2c_type_1);
    config->adc_configs[0].type        = match_sensor_type(adc_type_0);
    config->adc_configs[1].type        = match_sensor_type(adc_type_1);
    config->adc_configs[2].type        = match_sensor_type(adc_type_2);
    config->adc_configs[3].type        = match_sensor_type(adc_type_3);
}