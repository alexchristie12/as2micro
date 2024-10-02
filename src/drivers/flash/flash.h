/*
FLASH MEMORY DRIVER OVERVIEW

This driver is reponsible for handling access to the flash memory,
it should be able to:

- Read data in and out of memory, either single registers or
  multiple registers
- Write data to registers, this may be a single register or
  multiple registers, depending on the word length.
*/