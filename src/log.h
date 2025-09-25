
#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

#define MAX_LOG_ENTRIES 100
#define MAX_LOG_MESSAGE 256

typedef enum {
    INFO,
    WARN,
    ERROR,
    DEBUG
} log_level_t;

typedef struct {
    char message[MAX_LOG_MESSAGE];
    log_level_t level;
    char timestamp[32];
} log_entry_t;

extern log_entry_t log_buffer[MAX_LOG_ENTRIES];
extern int log_count;
extern int log_start;

void init_logging();
void log_message(log_level_t level, const char *format, ...);
void display_logs();

#endif
