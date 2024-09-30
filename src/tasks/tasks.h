#pragma once
#include "config.h"
#include "stdint.h"
#include <pico/types.h>

typedef enum {
    TASK_IDLE,
    TASK_LED,
    TASK_ACCEL,
    TASK_BLUETOOTH,
} tasks;


void tasks_init();

void get_next_task();

void task_button_callback(uint gpio, uint32_t event);

void log_new_task(tasks new_task);

tasks get_current_task();