#include "drivers/config_loader/config_loader.h"
#include <stdint.h>


static inline void load_registers_from_16_bit(uint16_t number, uint8_t* addr) {
    addr[0] = number & 0xFF;
    addr[1] = (number >> 8) & 0xFF; 
}

static inline void load_registers_from_32_bit(uint16_t number, uint8_t* addr) {
    addr[0] = number & 0xFF;
    addr[1] = (number >> 8) & 0xFF; 
    addr[2] = (number >> 16) & 0xFF; 
    addr[3] = (number >> 24) & 0xFF; 
}