
#include "log.h"
#include "ui.h"
#include "i18n.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

log_entry_t log_buffer[MAX_LOG_ENTRIES];
int log_count = 0;
int log_start = 0;
bool verbose_mode = false;

void init_logging() {
    log_count = 0;
    log_start = 0;
    verbose_mode = false;
    log_message(LOG_INFO, "Sistema de logs iniciado");
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
    
    // Mostrar no console se verbose mode estiver ativo
    if (verbose_mode) {
        const char* level_str[] = {"INFO", "WARN", "ERROR", "DEBUG"};
        printf("[%s] %s: %s\n", log_buffer[index].timestamp, 
               level_str[level], log_buffer[index].message);
    }
    
    va_end(args);
}



void toggle_verbose_mode() {
    verbose_mode = !verbose_mode;
    log_message(LOG_INFO, "Modo verbose %s", verbose_mode ? "ativado" : "desativado");
}

void display_logs() {
    if (log_count == 0) {
        printf("Nenhum log disponível.\n");
        return;
    }
    
    const char* level_colors[] = {
        "\033[1;32m", // INFO - Verde
        "\033[1;33m", // WARNING - Amarelo
        "\033[1;31m", // ERROR - Vermelho
        "\033[1;34m"  // DEBUG - Azul
    };
    const char* level_str[] = {"INFO", "WARN", "ERROR", "DEBUG"};
    
    for (int i = 0; i < log_count; i++) {
        int index = (log_start + i) % MAX_LOG_ENTRIES;
        printf("[%s] %s%s\033[0m: %s\n", 
               log_buffer[index].timestamp,
               level_colors[log_buffer[index].level],
               level_str[log_buffer[index].level],
               log_buffer[index].message);
    }
}
