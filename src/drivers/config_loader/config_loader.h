/*
CONFIG FEATURE IMPLEMENTATION

This is not to be confused with the config.h file where the project
is configured, this is the config loading feature in and out of
flash memory
*/

#include <stdint.h>

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

typedef enum {
    TEMPERATURE,
    HUMIDITY,
    SOIL_MOISTURE,
} sensor_type;

typedef struct {
    general_config general_config;
    i2c_config i2c_configs[2];
    adc_config adc_configs[4];
} rmu_config;

/// @brief Load a 16 bit unsigned integer from the config registers 
/// @param registers The registers, must be length 2
/// @return Return the loaded 16 bit unsigned integer
static uint16_t load_16_bit_from_registers(uint8_t *registers);

/// @brief Load a 32 bit unsigned integer from the config registers 
/// @param registers The registers, must be a length 4
/// @return Return the loaded 32 bit unsigned integer
static uint32_t load_32_bit_from_registers(uint8_t *registers);


static inline void load_registers_from_32_bit(uint16_t number, uint8_t* addr);

static inline void load_registers_from_16_bit(uint16_t number, uint8_t* addr);
