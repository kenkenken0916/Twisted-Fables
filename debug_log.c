#include "debug_log.h"
#include <string.h>
#include <stdarg.h>
#include <time.h>

static FILE* log_file = NULL;

void init_logging(const char* logfile) {
    log_file = fopen(logfile, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Cannot open log file!\n");
        return;
    }
}

void log_message(LogLevel level, const char* format, ...) {
    if (log_file == NULL) return;

    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0'; // Remove newline

    const char* level_str = "UNKNOWN";
    switch (level) {
        case LOG_DEBUG:   level_str = "DEBUG"; break;
        case LOG_INFO:    level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:   level_str = "ERROR"; break;
    }

    fprintf(log_file, "[%s] [%s] ", date, level_str);

    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);

    fprintf(log_file, "\n");
    fflush(log_file);
}

void close_logging(void) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}
