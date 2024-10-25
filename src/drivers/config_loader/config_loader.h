/*
CONFIG FEATURE IMPLEMENTATION


is configured, this is the config loading feature in and out of
flash memory
*/
#pragma once
#include <stdint.h>

/*
NO MORE OFFSETS THIS IS THE GROUND TRUTH
WE HAVE A SEVEN BYTE BUFFER

0 - Hardware ID
1 - I2C Sensor 01 Type
2 - I2C Sensor 02 Type
3 - ADC Sensor 01 Type
4 - ADC Sensor 02 Type
5 - ADC Sensor 03 Type
6 - ADC Sensor 04 Type
*/

// The different sensor types that are possible
typedef enum {
    TEMPERATURE_HUMIDITY_SENSOR,
    SOIL_MOISTURE_SENSOR,
    FLOW_RATE_SENSOR,
    DISABLED_SENSOR,
    INVALID_SENSOR,
} sensor_type;

// General configuration
typedef struct {
   uint8_t hardware_id;
} general_config;

// I2C specific configuration
typedef struct {
    sensor_type type; 
} i2c_config;

// ADC specific configuration
typedef struct {
    sensor_type type;
} adc_config;


// Total remote unit configuration
typedef struct {
    uint8_t special_number;
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


/// @brief Load the entire config from the memory block
/// @param registers The base address of the entire config
/// @param config A pointer to the RMU config
void load_config_from_memory(uint8_t* registers, rmu_config* config);


/// @brief Matches an integer to a sensor type
/// @param num The number
/// @return The sensor type
static inline sensor_type match_sensor_type(uint8_t num);

/// @brief Load the entire config into the config memory buffer 
/// @param registers The base address of the config
/// @param config A pointer to the pointer struct
void load_config_to_registers(uint8_t* registers, rmu_config* config);

/// @brief Match the sensor type enum to its corresponding integer in the config buffer
/// @param st The sensor type
/// @return The corresponding uint8_t value
static inline uint8_t match_sensor_type_to_int(sensor_type st);


/// @brief Load the configuration sent from the user
/// @param buffer Characters sent by the config tool
/// @param config The config to load
void load_config_from_user(char* buffer, rmu_config* config);