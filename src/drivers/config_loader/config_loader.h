/*
CONFIG FEATURE IMPLEMENTATION


is configured, this is the config loading feature in and out of
flash memory
*/
#pragma once
#include <stdint.h>

#define GENERAL_HARDWARE_ID_OFFSET 0x00
#define GENERAL_NAME_OFFSET 0x04
#define GENERAL_CONFIG_DOB_OFFSET 0x18

#define I2C_NAME_OFFSET 0x00
#define I2C_TYPE_OFFSET 0x14

#define ADC_NAME_OFFSET 0x00
#define ADC_MIN_VAL_OFFSET 0x14
#define ADC_MAX_VAL_OFFSET 0x18
#define ADC_MIN_MAP_OFFSET 0x1C
#define ADC_MAX_MAP_OFFSET 0x1E

#define GENERAL_CONFIG_OFFSET 0x00
#define I2C_1_CONFIG 0x1C
#define I2C_2_CONFIG 0x31
#define ADC_1_CONFIG 0x46
#define ADC_2_CONFIG 0x66
#define ADC_3_CONFIG 0x86
#define ADC_4_CONFIG 0xA6


typedef enum {
    TEMPERATURE_SENSOR,
    HUMIDITY_SENSOR,
    SOIL_MOISTURE_SENSOR,
    INVALID_SENSOR,
} sensor_type;

typedef struct {
   uint32_t hardware_id;
   uint32_t dob_unix;  
   char name[20];
} general_config;

typedef struct {
    char name[20];
    sensor_type type; 
} i2c_config;

typedef struct {
    float min_val;
    float max_val;
    char name[20];
    uint16_t adc_min_map;
    uint16_t adc_max_map;
} adc_config;


typedef struct {
    general_config general_config;
    i2c_config i2c_configs[2];
    adc_config adc_configs[4];
} rmu_config;

/// @brief Load a 16 bit unsigned integer from the config registers 
/// @param registers The registers, must be length 2
/// @return Return the loaded 16 bit unsigned integer
static inline uint16_t load_16_bit_from_registers(uint8_t *registers);

/// @brief Load a 32 bit unsigned integer from the config registers 
/// @param registers The registers, must be a length 4
/// @return Return the loaded 32 bit unsigned integer
static inline uint32_t load_32_bit_from_registers(uint8_t *registers);


/// @brief Load 2 8-bit registers from a single unsigned 16 bit integer.
/// @param number The number to be loaded
/// @param addr The address of the base register
static inline void load_registers_from_32_bit(uint32_t number, uint8_t* addr);

/// @brief Load 4 8-bit registers from a single unsigned 32 bit integer.
/// @param number The number to be loaded
/// @param addr The address of the base register
static inline void load_registers_from_16_bit(uint16_t number, uint8_t* addr);

/// @brief Load 4 8-bit registers from a float
/// @param number The number to be loaded
/// @param addr The register to be loaded
static inline void load_registers_from_float(float number, uint8_t* addr);

/// @brief Load a floating point number from 4 8-bit registers
/// @param registers The base address of the registers, length of 4
/// @return The float stored in these registers
static inline float load_float_from_registers(uint8_t* registers);

/// @brief Load the general config from the config memory block
/// @param registers The base address of the general config, length 28
/// @return The general config struct
static void load_general_config(uint8_t* registers, general_config* config);


/// @brief Load a single I2C config from the memory block
/// @param registers The base address of the I2C config
/// @param config A pointer to the individual I2C config 
/// @return MIGHT CHANGE THIS
static void load_i2c_config(uint8_t* registers, i2c_config* config);


/// @brief Load the I2C configs from the config memory block
/// @param registers The base address to load the config from
/// @param configs A pointer to both of the I2C configs
static void load_i2c_configs(uint8_t* registers, i2c_config* configs);

/// @brief Load a single ADC config from the memory block 
/// @param registers The base address to load the config from
/// @param config The config to place the register into
static void load_adc_config(uint8_t* registers, adc_config* config);

/// @brief Load all 4 of the ADC configs from the memory block
/// @param registers The base address to load the config from 
/// @param configs The 4 ADC configs
static void load_adc_configs(uint8_t* registers, adc_config* configs);

/// @brief Load the entire config from the memory block
/// @param registers The base address of the entire config
/// @param config A pointer to the RMU config
void load_config_from_memory(uint8_t* registers, rmu_config* config);


/// @brief Matches an integer to a sensor type
/// @param num The number
/// @return The sensor type
static inline sensor_type match_sensor_type(uint8_t num);

/// @brief Load the general config into the registers, starting at the config base address
/// @param registers The base config address
/// @param config A pointer to the config
static void load_general_config_to_registers(uint8_t* registers, general_config* config);

/// @brief Load a single I2C config into the registers 
/// @param registers The base address of the I2C config
/// @param config A pointer to the single I2C config
static void load_i2c_config_to_registers(uint8_t* registers, i2c_config* config);

/// @brief Load both the I2C configs into the registers, starting at the base address 
/// @param registers The base config address
/// @param configs A pointer to the I2C configs, length of 2
static void load_i2c_configs_to_registers(uint8_t* registers, i2c_config* configs);

/// @brief Load a single ADC config to the registers 
/// @param registers The base address of the ADC config
/// @param config A pointer to the ADC config
static void load_adc_config_to_registers(uint8_t* registers, adc_config* config);

/// @brief Load all 4 ADC configs into the registers, starting at the base address 
/// @param registers The base config address
/// @param configs A pointer to the ADC addresses, length of 4
static void load_adc_configs_to_registers(uint8_t* registers, adc_config* configs);

/// @brief Load the entire config into the config memory buffer 
/// @param registers The base address of the config
/// @param config A pointer to the pointer struct
void load_config_to_registers(uint8_t* registers, rmu_config* config);

/// @brief Match the sensor type enum to its corresponding integer in the config buffer
/// @param st The sensor type
/// @return The corresponding uint8_t value
static inline uint8_t match_sensor_type_to_int(sensor_type st);