#include "debug_log.h"
#include <string.h>

static FILE *log_file = NULL;
static bool log_initialized = false;

bool init_debug_log(void)
{
    if (log_initialized)
    {
        return true;
    }

    // For now, use stderr for logging
    // In the future, this could open a log file
    log_file = stderr;
    log_initialized = true;

    return true;
}

void cleanup_debug_log(void)
{
    if (!log_initialized)
    {
        return;
    }

    // If we opened a file, we would close it here
    // For now, we're using stderr, so no cleanup needed
    log_initialized = false;
}

void log_message(LogLevel level, const char *module, const char *format, ...)
{
    if (!log_initialized || !log_file)
    {
        return;
    }

    // Get current timestamp
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    // Remove newline from ctime output
    if (timestamp && strlen(timestamp) > 0)
    {
        timestamp[strlen(timestamp) - 1] = '\0';
    }

    // Print log header
    fprintf(log_file, "[%s][%s][%s] ",
            timestamp ? timestamp : "NO_TIME",
            log_level_str(level),
            module);

    // Print formatted message
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
}

const char *log_level_str(LogLevel level)
{
    switch (level)
    {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARNING:
        return "WARN";
    case LOG_ERROR:
        return "ERROR";
    case LOG_FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}
