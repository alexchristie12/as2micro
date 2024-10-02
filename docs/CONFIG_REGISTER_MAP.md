# Config Register Map

This is the register map for the config feature, detailing where the different
configs will live in the flash memory, we will detail mappings for the different
sensors, we will go from the base address, and then the offsets for each sensor
type will be provided.

## Base Addresses

The following are the base addresses of each of the sensor configs.

| Sensor  | Base Address |
| ------- | ------------ |
| General | `0x10000000` |
| I2C 1   | `0x1000001C` |
| I2C 2   | `0x10000031` |
| ADC 1   | `0x10000046` |
| ADC 2   | `0x10000066` |
| ADC 3   | `0x10000086` |
| ADC 4   | `0x700000A6` |

## Sensor Offsets

### General Config Offsets

| Value       | Offset | Length |
| ----------- | ------ | ------ |
| Base        | `0x00` | -      |
| Hardware ID | `0x00` | 4      |
| Name        | `0x04` | 20     |
| Config DOB  | `0x18` | 4      |

### I2C Config Offsets

| Value | Offset | Length |
| ----- | ------ | ------ |
| Base  | `0x00` | -      |
| Name  | `0x00` | 20     |
| Type  | `0x14` | 1      |

### ADC Config Offsets

| Value        | Offset | Length |
| ------------ | ------ | ------ |
| Base         | `0x00` | -      |
| Name         | `0x00` | 20     |
| Min Val      | `0x14` | 4      |
| Max Val      | `0x18` | 4      |
| ADC Min Map  | `0x1C` | 2      |
| ADC Max Map  | `0x1E` | 2      |