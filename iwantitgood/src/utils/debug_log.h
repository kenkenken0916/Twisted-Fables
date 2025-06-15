#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum
{
    LOG_DEBUG,   // Development information
    LOG_INFO,    // Normal operations
    LOG_WARNING, // Potential issues
    LOG_ERROR,   // Recoverable errors
    LOG_FATAL    // Unrecoverable errors
} LogLevel;

/**
 * Initialize the debug logging system
 * @return true if successful, false otherwise
 */
bool init_debug_log(void);

/**
 * Cleanup the debug logging system
 */
void cleanup_debug_log(void);

/**
 * Log a message with specified level
 * @param level Log severity level
 * @param module Module name
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
void log_message(LogLevel level, const char *module, const char *format, ...);

/**
 * Get string representation of log level
 * @param level Log level
 * @return String representation
 */
const char *log_level_str(LogLevel level);

// Convenience macros for different log levels
#define DEBUG_LOG(format, ...) log_message(LOG_DEBUG, "MAIN", format, ##__VA_ARGS__)
#define INFO_LOG(format, ...) log_message(LOG_INFO, "MAIN", format, ##__VA_ARGS__)
#define WARNING_LOG(format, ...) log_message(LOG_WARNING, "MAIN", format, ##__VA_ARGS__)
#define ERROR_LOG(format, ...) log_message(LOG_ERROR, "MAIN", format, ##__VA_ARGS__)
#define FATAL_LOG(format, ...) log_message(LOG_FATAL, "MAIN", format, ##__VA_ARGS__)

// Module-specific logging macros
#define CARD_LOG(level, format, ...) log_message(level, "CARD", format, ##__VA_ARGS__)
#define CHAR_LOG(level, format, ...) log_message(level, "CHAR", format, ##__VA_ARGS__)
#define GAME_LOG(level, format, ...) log_message(level, "GAME", format, ##__VA_ARGS__)

#endif // _DEBUG_LOG_H
