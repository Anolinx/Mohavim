#include "editor.h"
#include "ui.h"
#include "files.h"
#include "i18n.h"
#include "theme.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

extern struct termios old_termios;

void editor_simples(char* nome_arquivo) {
    char buffer[10000] = "";
    int cursor = 0;
    int modificado = 0;
    
    // Variables for clipboard and undo functionality
    char clipboard[10000] = "";
    char undo_buffer[10000] = "";
    int undo_cursor = 0;
    int undo_buffer_len = 0;
    int has_undo = 0;
    int selection_start = -1; // -1 means no selection
    int selection_end = -1;
    
    // Carregar arquivo se existir - COM CORREÇÃO DE SEGURANÇA
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, arquivo);
        buffer[bytes_read] = '\0';
        fclose(arquivo);
        log_message(LOG_INFO, "Arquivo '%s' carregado (%zu bytes)", nome_arquivo, bytes_read);
    } else {
        log_message(LOG_INFO, "Novo arquivo '%s' criado", nome_arquivo);
    }
    
    int buffer_len = strlen(buffer);
    
    configurar_terminal();
    
    while (1) {
        limpar_tela();
        
        // Calcular linha e coluna atual
        int linha_atual = 1;
        int coluna_atual = 1;
        for (int i = 0; i < cursor && i < buffer_len; i++) {
            if (buffer[i] == '\n') {
                linha_atual++;
                coluna_atual = 1;
            } else {
                coluna_atual++;
            }
        }
        
        // Apply background color consistently
        printf("%s%s", get_current_background(), get_current_foreground());
        
        printf("%s%s\033[0m - \033[1m%s\033[0m %s\n", get_color("accent"), get_string("editor_title"), nome_arquivo, 
               modificado ? get_string("modified") : get_string("saved"));
        printf("%s\n\n", get_string("controls"));
        
        // Mostrar até 15 linhas do conteúdo com numeração
        int linha_exibida = 1;
        int linhas_mostradas = 0;
        int inicio_linha = cursor;
        
        // Encontrar início da linha atual
        while (inicio_linha > 0 && buffer[inicio_linha - 1] != '\n') {
            inicio_linha--;
        }
        
        // Voltar algumas linhas para contexto
        int linhas_contexto = 7;
        for (int i = 0; i < linhas_contexto && inicio_linha > 0; i++) {
            inicio_linha--;
            while (inicio_linha > 0 && buffer[inicio_linha - 1] != '\n') {
                inicio_linha--;
            }
        }
        
        // Contar linha inicial
        int linha_inicial = 1;
        for (int i = 0; i < inicio_linha; i++) {
            if (buffer[i] == '\n') linha_inicial++;
        }
        
        linha_exibida = linha_inicial;
        
        for (int i = inicio_linha; i < buffer_len && linhas_mostradas < 15; i++) {
            if (i == inicio_linha || (i > 0 && buffer[i-1] == '\n')) {
                printf("\033[2m%3d\033[0m \033[2m│\033[0m ", linha_exibida);
                // Reapply background and foreground colors after escape sequences
                printf("%s%s", get_current_background(), get_current_foreground());
            }
            
            if (i == cursor) {
                // Improved cursor with inverted colors
                if (buffer[i] == '\n' || cursor >= buffer_len) {
                    // For newline or end of buffer, show block cursor
                    printf("\033[7m \033[0m");
                } else {
                    // For regular characters, show inverted character
                    printf("\033[7m%c\033[0m", buffer[i]);
                }
                // Reapply background and foreground colors after escape sequences
                printf("%s%s", get_current_background(), get_current_foreground());
            }
            
            if (buffer[i] == '\n') {
                printf("\n");
                linha_exibida++;
                linhas_mostradas++;
                // Reapply background and foreground colors after new line
                printf("%s%s", get_current_background(), get_current_foreground());
            } else {
                printf("%c", buffer[i]);
            }
        }
        
        // Se cursor está no final, mostrar ele
        if (cursor >= buffer_len) {
            printf("\033[7m \033[0m");
            // Reapply background and foreground colors after escape sequences
            printf("%s%s", get_current_background(), get_current_foreground());
        }
        
        // Ensure background color is applied to the rest of the screen
        printf("\033[0m%s%s\033[K", get_current_background(), get_current_foreground());
        // Fill the rest of the screen with background color to prevent transparency
        for (int i = linhas_mostradas; i < 25; i++) {
            printf("\n%s%s\033[K", get_current_background(), get_current_foreground());
        }
        printf("\033[%d;1H", 25); // Position cursor at the bottom for status line
        
        // Apply background color consistently to all lines
        printf("%s%s", get_current_background(), get_current_foreground());
        
        printf("\n\n");
        printf("%s%s\033[0m \033[1m%d\033[0m \033[2m|\033[0m %s%s\033[0m \033[1m%d\033[0m \033[2m|\033[0m %s%s\033[0m \033[1m%d\033[0m %s \033[2m|\033[0m %s\n", 
               get_color("accent"), get_string("line"), linha_atual,
               get_color("accent"), get_string("column"), coluna_atual,
               get_color("accent"), get_string("total"), buffer_len, get_string("chars"),
               modificado ? get_string("modified") : get_string("saved"));
        
        // Ensure status line has consistent background
        printf("%s%s", get_current_background(), get_current_foreground());
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                {
                    // Encontrar início da linha atual
                    int inicio_linha_atual = cursor;
                    while (inicio_linha_atual > 0 && buffer[inicio_linha_atual - 1] != '\n') {
                        inicio_linha_atual--;
                    }
                    
                    // Se não estamos na primeira linha
                    if (inicio_linha_atual > 0) {
                        // Encontrar início da linha anterior
                        int fim_linha_anterior = inicio_linha_atual - 1;
                        int inicio_linha_anterior = fim_linha_anterior;
                        while (inicio_linha_anterior > 0 && buffer[inicio_linha_anterior - 1] != '\n') {
                            inicio_linha_anterior--;
                        }
                        
                        // Calcular coluna atual
                        int coluna = cursor - inicio_linha_atual;
                        int tamanho_linha_anterior = fim_linha_anterior - inicio_linha_anterior;
                        
                        // Posicionar na linha anterior, mesma coluna (ou final da linha se menor)
                        cursor = inicio_linha_anterior + (coluna < tamanho_linha_anterior ? coluna : tamanho_linha_anterior);
                    }
                }
                break;
                
            case 66: // Seta baixo  
                {
                    // Encontrar fim da linha atual
                    int fim_linha_atual = cursor;
                    while (fim_linha_atual < buffer_len && buffer[fim_linha_atual] != '\n') {
                        fim_linha_atual++;
                    }
                    
                    // Se não estamos na última linha
                    if (fim_linha_atual < buffer_len) {
                        // Encontrar início e fim da próxima linha
                        int inicio_proxima_linha = fim_linha_atual + 1;
                        int fim_proxima_linha = inicio_proxima_linha;
                        while (fim_proxima_linha < buffer_len && buffer[fim_proxima_linha] != '\n') {
                            fim_proxima_linha++;
                        }
                        
                        // Calcular coluna atual
                        int inicio_linha_atual = cursor;
                        while (inicio_linha_atual > 0 && buffer[inicio_linha_atual - 1] != '\n') {
                            inicio_linha_atual--;
                        }
                        int coluna = cursor - inicio_linha_atual;
                        int tamanho_proxima_linha = fim_proxima_linha - inicio_proxima_linha;
                        
                        // Posicionar na próxima linha, mesma coluna (ou final da linha se menor)
                        cursor = inicio_proxima_linha + (coluna < tamanho_proxima_linha ? coluna : tamanho_proxima_linha);
                    }
                }
                break;
                
            case 67: // Seta direita
                if (cursor < buffer_len) cursor++;
                break;
                
            case 68: // Seta esquerda
                if (cursor > 0) cursor--;
                break;
                
            case 19: // Ctrl+S
                arquivo = fopen(nome_arquivo, "w");
                if (arquivo) {
                    fwrite(buffer, 1, buffer_len, arquivo);
                    fclose(arquivo);
                    modificado = 0;
                    log_message(LOG_INFO, "Arquivo '%s' salvo com sucesso", nome_arquivo);
                } else {
                    log_message(LOG_ERROR, "Erro ao salvar arquivo '%s'", nome_arquivo);
                }
                break;
                
            case 17: // Ctrl+Q
                if (modificado) {
                    limpar_tela();
                    printf("%s", get_string("save_before_exit"));
                    restaurar_terminal();
                    int resp = getchar();
                    if (resp == 's' || resp == 'S') {
                        arquivo = fopen(nome_arquivo, "w");
                        if (arquivo) {
                            fwrite(buffer, 1, buffer_len, arquivo);
                            fclose(arquivo);
                        }
                    }
                }
                restaurar_terminal();
                return;
                
            case 27: // ESC - Return to main menu
                restaurar_terminal();
                return;
                
            case 3: // Ctrl+C - Copy
                // Save current state for undo
                strcpy(undo_buffer, buffer);
                undo_cursor = cursor;
                undo_buffer_len = buffer_len;
                has_undo = 1;
                
                // If there's a selection, copy it to clipboard
                if (selection_start != -1 && selection_end != -1) {
                    int start = (selection_start < selection_end) ? selection_start : selection_end;
                    int end = (selection_start < selection_end) ? selection_end : selection_start;
                    int copy_len = end - start;
                    if (copy_len < sizeof(clipboard) - 1) {
                        strncpy(clipboard, buffer + start, copy_len);
                        clipboard[copy_len] = '\0';
                    }
                } else {
                    // If no selection, copy the current line
                    int line_start = cursor;
                    while (line_start > 0 && buffer[line_start - 1] != '\n') {
                        line_start--;
                    }
                    int line_end = cursor;
                    while (line_end < buffer_len && buffer[line_end] != '\n') {
                        line_end++;
                    }
                    int copy_len = line_end - line_start;
                    if (copy_len < sizeof(clipboard) - 1) {
                        strncpy(clipboard, buffer + line_start, copy_len);
                        clipboard[copy_len] = '\0';
                    }
                }
                break;
                
            case 22: // Ctrl+V - Paste
                // Save current state for undo
                strcpy(undo_buffer, buffer);
                undo_cursor = cursor;
                undo_buffer_len = buffer_len;
                has_undo = 1;
                
                // Paste clipboard content at cursor position
                if (strlen(clipboard) > 0 && buffer_len + strlen(clipboard) < sizeof(buffer) - 1) {
                    memmove(&buffer[cursor + strlen(clipboard)], &buffer[cursor], buffer_len - cursor + 1);
                    memcpy(&buffer[cursor], clipboard, strlen(clipboard));
                    cursor += strlen(clipboard);
                    buffer_len += strlen(clipboard);
                    modificado = 1;
                }
                break;
                
            case 26: // Ctrl+Z - Undo
                // Restore previous state
                if (has_undo) {
                    strcpy(buffer, undo_buffer);
                    cursor = undo_cursor;
                    buffer_len = undo_buffer_len;
                    modificado = 1;
                    has_undo = 0;
                }
                break;
                
            case 'l':
            case 'L': // Mostrar logs
                restaurar_terminal();
                show_logs();
                configurar_terminal();
                break;
                
            case 127: // Backspace
                if (cursor > 0) {
                    // Save current state for undo
                    strcpy(undo_buffer, buffer);
                    undo_cursor = cursor;
                    undo_buffer_len = buffer_len;
                    has_undo = 1;
                    
                    memmove(&buffer[cursor - 1], &buffer[cursor], buffer_len - cursor + 1);
                    cursor--;
                    buffer_len--;
                    modificado = 1;
                }
                break;
                
            case 10: // Enter
                if (buffer_len < (int)sizeof(buffer) - 1) {
                    // Save current state for undo
                    strcpy(undo_buffer, buffer);
                    undo_cursor = cursor;
                    undo_buffer_len = buffer_len;
                    has_undo = 1;
                    
                    memmove(&buffer[cursor + 1], &buffer[cursor], buffer_len - cursor + 1);
                    buffer[cursor] = '\n';
                    cursor++;
                    buffer_len++;
                    modificado = 1;
                }
                break;
                
            default:
                // Inserir caractere normal
                if (tecla >= 32 && tecla <= 126 && buffer_len < (int)sizeof(buffer) - 1) {
                    // Save current state for undo
                    strcpy(undo_buffer, buffer);
                    undo_cursor = cursor;
                    undo_buffer_len = buffer_len;
                    has_undo = 1;
                    
                    memmove(&buffer[cursor + 1], &buffer[cursor], buffer_len - cursor + 1);
                    buffer[cursor] = tecla;
                    cursor++;
                    buffer_len++;
                    modificado = 1;
                }
                break;
        }
    }
}

void novo_arquivo() {
    limpar_tela();
    printf("\033[1;36m%s\033[0m\n\n", get_string("create_new_file_title"));
    printf("%s ", get_string("filename_prompt_new"));
    
    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;
        if (strlen(nome) > 0) {
            editor_simples(nome);
        }
    }
}