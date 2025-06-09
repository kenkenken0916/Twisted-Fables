#ifndef _DEBUG_LOG_H
#define _DEBUG_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// 日誌級別
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// 初始化日誌系統
void init_logging(const char* logfile);

// 寫入日誌
void log_message(LogLevel level, const char* format, ...);

// 關閉日誌系統
void close_logging(void);

// 快捷日誌函數
#define DEBUG_LOG(fmt, ...) log_message(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define INFO_LOG(fmt, ...) log_message(LOG_INFO, fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) log_message(LOG_WARNING, fmt, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) log_message(LOG_ERROR, fmt, ##__VA_ARGS__)

#endif // _DEBUG_LOG_H
