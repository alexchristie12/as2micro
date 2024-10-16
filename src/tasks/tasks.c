#include "tasks/tasks.h"
#include "stddef.h"
#include "stdint.h"
#include <pico/types.h>
#include "drivers/logging/logging.h"
#include "drivers/WS2812/led.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static tasks task_list[] = {
    TASK_IDLE,
#ifdef LED_TASK
    TASK_LED,
#endif
#ifdef ACCEL_TASK
    TASK_ACCEL,
#endif
#ifdef BLUETOOTH_TASK
    TASK_BLUETOOTH,
#endif
};

volatile int tasks_i = 0;

void get_next_task() {
    tasks_i += 1;
    if (tasks_i == sizeof(task_list)) {
        tasks_i = 0;
    }
#ifdef DBG_LOGGING
    log_new_task(get_current_task());
#endif
}

tasks get_current_task() {
    return task_list[tasks_i];
}

void tasks_init() {
    // Now set up the interupt
    gpio_set_irq_enabled_with_callback(TASK_PIN, GPIO_IRQ_EDGE_RISE, true, &task_button_callback);
}

void log_new_task(tasks new_task) {
    switch (new_task) {
        case TASK_IDLE:
            log_dbg(LOG_INFORMATION, "switched to idle task");
            break;
        case TASK_LED:
            log_dbg(LOG_INFORMATION, "switched to led task");
            break;
        case TASK_ACCEL:
            log_dbg(LOG_INFORMATION, "switched to accelerometer task");
            break;
        case TASK_BLUETOOTH:
            log_dbg(LOG_INFORMATION, "switched to bluetooth task");
            break;
        default:
            log_dbg(LOG_ERROR, "should not be able to get here");
            break;
    }
}

void task_button_callback(uint gpio, uint32_t event) {
#ifdef DBG_LOGGING
    log_dbg(LOG_INFORMATION, "entered task callback");
#endif
    get_next_task();
}
