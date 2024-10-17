#include "drivers/config_loader/config_loader.h"
#include <string.h>
#include <stdint.h>

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

static void load_general_config(uint8_t* registers, general_config* config) {
    config->hardware_id = load_32_bit_from_registers(registers);
    config->dob_unix    = load_32_bit_from_registers(registers + GENERAL_CONFIG_DOB_OFFSET);
    memcpy(config->name, registers + GENERAL_NAME_OFFSET, 20);
}

static void load_i2c_config(uint8_t* registers, i2c_config* config) {
    memcmp(config->name, registers + I2C_NAME_OFFSET, 20);
    config->type = match_sensor_type(*(registers + I2C_TYPE_OFFSET));
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

static void load_i2c_configs(uint8_t* registers, i2c_config* configs) {
    // Load each I2C config
    load_i2c_config(registers + I2C_1_CONFIG, &configs[0]);
    load_i2c_config(registers + I2C_2_CONFIG, &configs[1]);
}

static void load_adc_config(uint8_t* registers, adc_config* config) {
    memcmp(config->name, registers + ADC_NAME_OFFSET, 20);
    config->max_val     = load_float_from_registers(registers + ADC_MAX_VAL_OFFSET);
    config->min_val     = load_float_from_registers(registers + ADC_MAX_VAL_OFFSET);
    config->adc_min_map = load_16_bit_from_registers(registers + ADC_MIN_MAP_OFFSET);
    config->adc_max_map = load_16_bit_from_registers(registers + ADC_MAX_MAP_OFFSET);
}

static void load_adc_configs(uint8_t* registers, adc_config* configs) {
    // Load each ADC configs
    load_adc_config(registers + ADC_1_CONFIG, &configs[0]);
    load_adc_config(registers + ADC_2_CONFIG, &configs[1]);
    load_adc_config(registers + ADC_3_CONFIG, &configs[2]);
    load_adc_config(registers + ADC_4_CONFIG, &configs[3]);
}

void load_config_from_memory(uint8_t* registers, rmu_config* config) {
    // Load the general config
    load_general_config(registers, &(config->general_config));
    // Load the I2C configs
    load_i2c_configs(registers, config->i2c_configs);
    // Load the ADC configs
    load_adc_config(registers, config->adc_configs);
}

static void load_general_config_to_registers(uint8_t* registers, general_config* config) {
    // Essentially the same as before, but backwards
    uint8_t general_config_buffer[28];
    load_registers_from_32_bit(config->hardware_id, general_config_buffer + GENERAL_HARDWARE_ID_OFFSET);
    memcpy(general_config_buffer + GENERAL_NAME_OFFSET, config->name, 20);
    load_registers_from_32_bit(config->dob_unix, general_config_buffer + GENERAL_CONFIG_DOB_OFFSET);

    // Now load all this into the config buffer
    memcpy(registers + GENERAL_CONFIG_OFFSET, general_config_buffer, 28);
}

static void load_i2c_config_to_registers(uint8_t* registers, i2c_config* config) {
    // Remember that this starts at the base i2c address for the particular one!
    uint8_t i2c_config_buffer[21];
    uint8_t sensor_int = match_sensor_type_to_int(config->type);
    memcpy(i2c_config_buffer + I2C_NAME_OFFSET, config->name, 20);
    memcpy(i2c_config_buffer + I2C_TYPE_OFFSET, &sensor_int, 1);
    // Now copy into the address
    memcpy(registers, i2c_config_buffer, 21);
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

static void load_i2c_configs_to_registers(uint8_t* registers, i2c_config* configs) {
    // Load each config
    load_i2c_config_to_registers(registers + I2C_1_CONFIG, &(configs[0]));
    load_i2c_config_to_registers(registers + I2C_2_CONFIG, &(configs[1]));
}

static void load_adc_config_to_registers(uint8_t* registers, adc_config* config) {
    // Note that this is the address that the particular i2c address starts
    uint8_t adc_config_buffer[32];
    memcpy(adc_config_buffer + ADC_NAME_OFFSET, config->name, 20);

    // We are now loading in a sensor type instead of the other stuff
    uint8_t sensor_int = match_sensor_type_to_int(config->type);
    memcpy(adc_config_buffer + ADC_MIN_VAL_OFFSET, &sensor_int, 1);

    // The Min and Max floats
    // load_registers_from_float(config->min_val, adc_config_buffer + ADC_MIN_VAL_OFFSET);
    // load_registers_from_float(config->max_val, adc_config_buffer + ADC_MAX_VAL_OFFSET);

    // // The Min and Max mapping values
    // load_registers_from_16_bit(config->adc_min_map, adc_config_buffer + ADC_MIN_MAP_OFFSET);
    // load_registers_from_16_bit(config->adc_max_map, adc_config_buffer + ADC_MAX_MAP_OFFSET);
}

static void load_adc_configs_to_registers(uint8_t* registers, adc_config* configs) {
    // Note that this is the BASE address
    load_adc_config_to_registers(registers + ADC_1_CONFIG, &(configs[0]));
    load_adc_config_to_registers(registers + ADC_2_CONFIG, &(configs[1]));
    load_adc_config_to_registers(registers + ADC_3_CONFIG, &(configs[3]));
    load_adc_config_to_registers(registers + ADC_4_CONFIG, &(configs[3]));
}

void load_config_to_registers(uint8_t* registers, rmu_config* config) {
    // Remember that I only give it all the base addresses
    // Remember that this will be put in in the right spot
    load_general_config_to_registers(registers, &(config->general_config));
    load_i2c_configs_to_registers(registers, config->i2c_configs);
    load_adc_configs_to_registers(registers, config->adc_configs);
}