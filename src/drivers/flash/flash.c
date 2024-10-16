#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "drivers/config_loader/config_loader.h"
#include <string.h>

void read_config_from_flash(uint8_t* config_buffer) {
    const uint8_t* flash_target_config = (const uint8_t)(XIP_BASE); 
    memcpy(config_buffer, flash_target_config, 198);
}

void write_config_to_flash(uint8_t* config_buffer) {
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(0, FLASH_SECTOR_SIZE);
    flash_range_program(0, (const uint8_t*)config_buffer, 198);
    restore_interrupts(ints);
}

void load_config(rmu_config* config) {
    uint8_t config_buffer[198];
    read_config_from_flash(config_buffer);
    load_config_from_memory(config_buffer, config);
}

void unload_config(rmu_config* config) {
    // Put the config back into memory
    uint8_t config_buffer[198];
    load_config_to_registers(config_buffer, config);
    write_config_to_flash(config_buffer);
}