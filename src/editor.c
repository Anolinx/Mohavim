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
            }
            
            if (i == cursor) {
                printf("\033[4m \033[0m"); // Cursor com underline
            }
            
            if (buffer[i] == '\n') {
                printf("\n");
                linha_exibida++;
                linhas_mostradas++;
            } else {
                printf("%c", buffer[i]);
            }
        }
        
        // Se cursor está no final, mostrar ele
        if (cursor >= buffer_len) {
            printf("\033[4m \033[0m");
        }
        
        printf("\n\n");
        printf("%s%s:\033[0m \033[1m%d\033[0m \033[2m|\033[0m %s%s:\033[0m \033[1m%d\033[0m \033[2m|\033[0m %s%s:\033[0m \033[1m%d\033[0m %s \033[2m|\033[0m %s\n", 
               get_color("accent"), get_string("line"), linha_atual,
               get_color("accent"), get_string("column"), coluna_atual,
               get_color("accent"), get_string("total"), buffer_len, get_string("chars"),
               modificado ? get_string("modified") : get_string("saved"));
        
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
                    printf("\033[2J\033[H");
                    printf("⚠️  Arquivo modificado! Salvar antes de sair? (s/n): ");
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
                
            case 27: // ESC
                restaurar_terminal();
                return;
                
            case 'l':
            case 'L': // Mostrar logs
                restaurar_terminal();
                show_logs();
                configurar_terminal();
                break;
                
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
    printf("\033[2J\033[H");
    printf("\033[1;36m📝 CRIAR NOVO ARQUIVO\033[0m\n\n");
    printf("Nome do arquivo: ");
    
    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;
        if (strlen(nome) > 0) {
            editor_simples(nome);
        }
    }
}
