/*
FLASH MEMORY DRIVER OVERVIEW

This driver is reponsible for handling access to the flash memory,
it should be able to:

- Read data in and out of memory, either single registers or
  multiple registers
- Write data to registers, this may be a single register or
  multiple registers, depending on the word length.
*/

#pragma once
#include <stdint.h>
#include "drivers/config_loader/config_loader.h"

/// @brief Reads the device config from flash memory
/// @param config_buffer The config buffer, total required length is 198
void read_config_from_flash(uint8_t* config_buffer);

/// @brief Write the config to the device flash memory
/// @param config_buffer The config buffer, total required length is 198
void write_config_to_flash(uint8_t* config_buffer);

/// @brief Load the config (from flash memory)
/// @param config The config struct to load the config into
void load_config(rmu_config* config);

/// @brief Unload the config (back into flash memory)
/// @param config The config struct to load the flash memory from
void unload_config(rmu_config* config);