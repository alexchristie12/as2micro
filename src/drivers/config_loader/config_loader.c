#include "drivers/config_loader/config_loader.h"
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
    union
    {
        float f;
        uint32_t u;
    } float_union;

    float_union.f = number;
    load_registers_from_32_bit(float_union.u, addr);
}

static inline float load_float_from_registers(uint8_t* registers) {
    union {
        float f;
        uint32_t u;
    } float_union;
    float_union.u = load_32_bit_from_registers(registers);
    return float_union.f;
}