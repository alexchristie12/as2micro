
#include "logging.h"

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "stdint.h"
#include "config.h"

// --- Device driver internal state:

/// Drop messages whose level is below this threshold.
static LogLevel max_log_level = LOG_INFORMATION;

void set_log_level(LogLevel new_log_level) {
    max_log_level = new_log_level;
}

void log_dbg(LogLevel level, const char *msg) {

    // Disable logging
#ifndef DBG_LOGGING
    return;
#endif
    // Don't log below this level
    if (level < max_log_level) {
        return;
    }

    // Get the time since boot
    uint32_t time         = to_ms_since_boot(get_absolute_time());
    uint32_t time_sec     = time / 1000;
    uint32_t time_decimal = (time % 1000);

    // Convert the level to a string
    const char *level_str;
    switch (level) {
        case LOG_INFORMATION:
            level_str = "Information";
            break;
        case LOG_WARNING:
            level_str = "Warning";
            break;
        case LOG_ERROR:
            level_str = "Error";
            break;
    };
    printf("[%u.%03u %s]: %s\n", time_sec, time_decimal, level_str, msg);
}