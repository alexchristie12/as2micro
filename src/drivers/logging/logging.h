#pragma once 


/// @brief Logging level enum
typedef enum {
    LOG_INFORMATION,
    LOG_WARNING,
    LOG_ERROR,
} LogLevel;

/// 



/// @brief Set the log level. Messages with a level below this threshold will be discarded. 
/// @param new_log_level The new level to be used
void set_log_level(LogLevel new_log_level);

/// @brief Log a message with a level 
/// @param level Level to log with
/// @param msg Message to log with
void log_dbg(LogLevel level, const char *msg);
