#include "editor.h"
#include "ui.h"
#include "files.h"
#include "i18n.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

extern struct termios old_termios;

// Structure to hold terminal dimensions
struct winsize terminal_size;

// Function to get terminal dimensions
void get_terminal_size() {
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size) == -1) {
        // Default fallback if ioctl fails
        terminal_size.ws_row = 25;
        terminal_size.ws_col = 80;
    }
}

void editor_simples(char* nome_arquivo) {
    char buffer[10000] = "";
    int cursor = 0;
    int modificado = 0;
    
    // Variables removed as functionality was removed per user request
    
    // Carregar arquivo se existir - COM CORREÇÃO DE SEGURANÇA
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, arquivo);
        buffer[bytes_read] = '\0';
        fclose(arquivo);
    } else {
    }
    
    int buffer_len = strlen(buffer);
    
    // Get terminal dimensions
    get_terminal_size();
    
    configurar_terminal();
    
    // Hide cursor to prevent blinking
    printf("\033[?25l");
    fflush(stdout);
    
    while (1) {
        // Get updated terminal dimensions each cycle
        get_terminal_size();
        
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
        
        printf("%s%s", get_color("accent"), get_string("editor_title"));theme_reset();printf(" - \033[1m%s", nome_arquivo);theme_reset();printf(" %s\n", 
               modificado ? get_string("modified") : get_string("saved"));
        printf("%s\n\n", get_string("controls"));
        
        // Mostrar conteúdo ocupando toda a altura disponível do terminal
        int linha_exibida = 1;
        int linhas_mostradas = 0;
        int inicio_linha = cursor;
        
        // Encontrar início da linha atual
        while (inicio_linha > 0 && buffer[inicio_linha - 1] != '\n') {
            inicio_linha--;
        }
        
        // Voltar algumas linhas para contexto (ajustado para usar espaço disponível)
        int linhas_contexto = terminal_size.ws_row / 3; // Aproximadamente 1/3 do espaço para contexto
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
        
        // Calcular quantas linhas podemos mostrar (deixando espaço para título e status)
        int max_linhas_mostrar = terminal_size.ws_row - 6; // Reservar espaço para título, controles e status
        
        for (int i = inicio_linha; i < buffer_len && linhas_mostradas < max_linhas_mostrar; i++) {
            if (i == inicio_linha || (i > 0 && buffer[i-1] == '\n')) {
                printf("\033[2m%3d", linha_exibida);theme_reset();printf(" \033[2m│");theme_reset();printf(" ");
                // Reapply background and foreground colors after escape sequences
                printf("%s%s", get_current_background(), get_current_foreground());
            }
            
            if (i == cursor) {
                // Cursor personalizado baseado na seleção do usuário
                int cursor_type = get_cursor_type();
                const char* cursor_glyph = get_cursor_glyph();
                
                if (buffer[i] == '\n' || cursor >= buffer_len) {
                    // Para newline ou fim do buffer, mostrar cursor selecionado
                    printf("%s%s", get_color("accent"), cursor_glyph);theme_reset();
                } else {
                    // Para caracteres regulares
                    if (cursor_type == 2) { // Block cursor - substitui o caractere por bloco
                        printf("%s█", get_color("accent"));theme_reset();
                    } else {
                        // Underscore ou pipe - mostra o caractere normal e o cursor
                        printf("%c", buffer[i]);
                        // Move cursor back and show cursor symbol
                        printf("\b%s%s", get_color("accent"), cursor_glyph);theme_reset();
                    }
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
            printf("%s%s", get_color("accent"), get_cursor_glyph());theme_reset();
            // Reapply background and foreground colors after escape sequences
            printf("%s%s", get_current_background(), get_current_foreground());
        }
        
        // Ensure background color is applied to the rest of the screen
        printf("\033[0m%s%s\033[K", get_current_background(), get_current_foreground());
        // Fill the rest of the screen with background color to prevent transparency
        for (int i = linhas_mostradas; i < max_linhas_mostrar; i++) {
            printf("\n%s%s\033[K", get_current_background(), get_current_foreground());
        }
        
        // Position cursor at the bottom for status line
        printf("\033[%d;1H", terminal_size.ws_row - 2);
        
        // Apply background color consistently to all lines
        printf("%s%s", get_current_background(), get_current_foreground());
        
        printf("\n");
        printf("%s%s", get_color("accent"), get_string("line"));theme_reset();printf(" \033[1m%d", linha_atual);theme_reset();printf(" \033[2m|");theme_reset();printf(" %s%s", get_color("accent"), get_string("column"));theme_reset();printf(" \033[1m%d", coluna_atual);theme_reset();printf(" \033[2m|");theme_reset();printf(" %s%s", get_color("accent"), get_string("total"));theme_reset();printf(" \033[1m%d", buffer_len);theme_reset();printf(" %s \033[2m|", get_string("chars"));theme_reset();printf(" %s\n", 
               modificado ? get_string("modified") : get_string("saved"));
        
        // Ensure status line has consistent background
        printf("%s%s", get_current_background(), get_current_foreground());
        
        fflush(stdout); // Ensure all output is displayed
        
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
                } else {
                }
                break;
                
            case 27: // ESC - Return to main menu
                // Show cursor before exiting
                printf("\033[?25h");
                restaurar_terminal();
                return;
                
            case 127: // Backspace
                if (cursor > 0) {
                    memmove(&buffer[cursor - 1], &buffer[cursor], buffer_len - cursor + 1);
                    cursor--;
                    buffer_len--;
                    modificado = 1;
                }
                break;
                
            case 10: // Enter
                if (buffer_len < (int)sizeof(buffer) - 1) {
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
    printf("\033[1;36m%s", get_string("create_new_file_title"));theme_reset();printf("\n\n");
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