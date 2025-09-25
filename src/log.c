#include "log.h"
#include "ui.h"
#include "i18n.h"
#include "theme.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

log_entry_t log_buffer[MAX_LOG_ENTRIES];
int log_count = 0;
int log_start = 0;

void init_logging() {
    log_count = 0;
    log_start = 0;
    log_message(INFO, "%s", get_string("system_started"));
}

void log_message(log_level_t level, const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    // Adicionar entrada no buffer circular
    int index = (log_start + log_count) % MAX_LOG_ENTRIES;
    
    // Formatar mensagem
    vsnprintf(log_buffer[index].message, MAX_LOG_MESSAGE, format, args);
    log_buffer[index].level = level;
    
    // Adicionar timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(log_buffer[index].timestamp, sizeof(log_buffer[index].timestamp), 
             "%H:%M:%S", t);
    
    // Gerenciar buffer circular
    if (log_count < MAX_LOG_ENTRIES) {
        log_count++;
    } else {
        log_start = (log_start + 1) % MAX_LOG_ENTRIES;
    }
    
    va_end(args);
}

void display_logs() {
    if (log_count == 0) {
        print_with_color(get_color("foreground"), get_string("no_logs"));
        printf("\n");
        return;
    }
    
    for (int i = 0; i < log_count; i++) {
        int index = (log_start + i) % MAX_LOG_ENTRIES;
        
        // Map log levels to theme colors
        const char* level_color = get_color("foreground"); // Default
        switch (log_buffer[index].level) {
            case INFO:
                level_color = get_color("success"); // Verde
                break;
            case WARN:
                level_color = get_color("accent"); // Amarelo
                break;
            case ERROR:
                level_color = get_color("error"); // Vermelho
                break;
            case DEBUG:
                level_color = get_color("foreground"); // Azul (using foreground for now)
                break;
        }
        
        printf("[%s] ", log_buffer[index].timestamp);
        const char* level_str = 
            log_buffer[index].level == INFO ? get_string("info") :
            log_buffer[index].level == WARN ? get_string("warn") :
            log_buffer[index].level == ERROR ? get_string("error") : 
            get_string("debug");
        print_with_color(level_color, level_str);
        printf(": %s", log_buffer[index].message);
        theme_reset();
        printf("\n");
    }
}