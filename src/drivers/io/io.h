/*
IO Driver

This driver is intended to act as the main interface with the UART
on the RP2040
*/

#pragma once
#include <stdbool.h>
#include "tasks/tasks.h"

/// @brief Callback to handle the serial connection
/// @param _ Should be used as a function pointer 
void on_uart_rx(void);

/// @brief Initialise the UART connection
void io_init();

/// @brief Poll the UART connection until message is sent, or block changes to
/// false from interupt, it will clear buffer in this case.
/// @param current_task The current task
int io_poll(char* buffer, int buffer_len);