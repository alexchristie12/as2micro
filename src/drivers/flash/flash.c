#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "drivers/config_loader/config_loader.h"
#include "drivers/flash/flash.h"
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

void unload_config(rmu_config* config) {
    // Pull it out of memory
    // Set the special number
    config->special_number = 69;
    uint32_t ints = save_and_disable_interrupts();
    // Write the shit
    rmu_config buf[FLASH_PAGE_SIZE/sizeof(rmu_config)]; // One page worth of rmu_config

    buf[0] = *config;

    // Erase the last sector of the flash
    flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);

    flash_range_program((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), (uint8_t*)buf, FLASH_PAGE_SIZE);

    restore_interrupts(ints);
}

rmu_config* load_config() {
    // Put the config back into memory
    uint32_t ints = save_and_disable_interrupts();
    int addr = XIP_BASE + (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE);
    rmu_config* ptr = (rmu_config*)addr;
    restore_interrupts(ints);
    return ptr;
}

int check_if_config_exists() {
    const uint8_t* config_exists = (const uint8_t*)(XIP_BASE + CONFIG_EXISTS_REGISTER);
    // Deference it and check if it is the special number
    if (*config_exists == SPECIAL_NUMBER) {
        return 0;
    }
    return 1;
}

void set_config_exists_number() {
    uint8_t* config_exists = (uint8_t*)(XIP_BASE + CONFIG_EXISTS_REGISTER);
    *config_exists = SPECIAL_NUMBER;
}