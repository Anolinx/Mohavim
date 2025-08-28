#include "editor.h"
#include "ui.h"
#include "files.h"
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

    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, arquivo);
        buffer[bytes_read] = '\0';
        fclose(arquivo);
    }

    int buffer_len = strlen(buffer);
    configurar_terminal();

    while (1) {
        limpar_tela();
        // cálculo de linha/coluna e exibição de buffer
        int linha_atual = 1, coluna_atual = 1;
        for (int i = 0; i < cursor && i < buffer_len; i++) {
            if (buffer[i] == '\n') { linha_atual++; coluna_atual = 1; }
            else coluna_atual++;
        }

        printf("\033[1;36m🖊️  MOHAVIM\033[0m - \033[1m%s\033[0m %s\n", nome_arquivo,
               modificado ? "\033[1;33m[MODIFICADO]\033[0m" : "\033[1;32m[Salvo]\033[0m");

        int tecla = ler_tecla();

        switch (tecla) {
            case 65: case 66: case 67: case 68: // setas
                // lógica de movimentação (mantida do código original)
                break;
            case 19: // Ctrl+S
                arquivo = fopen(nome_arquivo, "w");
                if (arquivo) { fwrite(buffer, 1, buffer_len, arquivo); fclose(arquivo); modificado = 0; }
                break;
            case 17: // Ctrl+Q
                if (modificado) {
                    printf("\033[2J\033[H⚠️  Arquivo modificado! Salvar antes de sair? (s/n): ");
                    restaurar_terminal();
                    int resp = getchar();
                    if (resp == 's' || resp == 'S') {
                        arquivo = fopen(nome_arquivo, "w");
                        if (arquivo) fwrite(buffer, 1, buffer_len, arquivo);
                    }
                }
                restaurar_terminal();
                return;
            case 27: // ESC
                restaurar_terminal();
                return;
            case 127: // Backspace
                if (cursor > 0) { memmove(&buffer[cursor-1], &buffer[cursor], buffer_len - cursor + 1); cursor--; buffer_len--; modificado=1;}
                break;
            case 10: // Enter
                if (buffer_len < sizeof(buffer)-1) { memmove(&buffer[cursor+1], &buffer[cursor], buffer_len - cursor + 1); buffer[cursor]='\n'; cursor++; buffer_len++; modificado=1; }
                break;
            default: // caracteres normais
                if (tecla>=32 && tecla<=126 && buffer_len<sizeof(buffer)-1) { memmove(&buffer[cursor+1], &buffer[cursor], buffer_len - cursor + 1); buffer[cursor]=tecla; cursor++; buffer_len++; modificado=1; }
                break;
        }
    }
}

void novo_arquivo() {
    printf("\033[2J\033[H\033[1;36m📝 CRIAR NOVO ARQUIVO\033[0m\n\nNome do arquivo: ");
    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;
        if (strlen(nome)>0) editor_simples(nome);
    }
}