/**************************************************************************
 *   mohavim.c  --  Um editor de texto modernizado o                      *
 *                                                                        *
 *   Copyright (C) 2024 Projeto Mohavim                                   *
 *                                                                        *
 *   Este programa é software livre; você pode redistribuí-lo e/ou        *
 *   modificá-lo sob os termos da Licença Pública Geral GNU conforme      *
 *   publicada pela Free Software Foundation.                             *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>

// Variáveis globais para lista de arquivos
char arquivos[100][256];
int total_arquivos = 0;

struct termios old_termios;

void limpar_tela() {
    // Limpar tela
    system("clear");
}

void configurar_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void restaurar_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

int ler_tecla() {
    int ch = getchar();
    if (ch == 27) { // ESC sequence
        int next = getchar();
        if (next == 91) { // '['
            return getchar(); // Return the actual arrow key code
        }
        // Se não for '[', é ESC puro
        ungetc(next, stdin);
        return 27;
    }
    return ch;
}

void mostrar_logo() {
    printf("============================================================\n");
    printf("▗▖  ▗▖ ▗▄▖ ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖\n");
    printf("▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌  █  ▐▛▚▞▜▌\n");
    printf("▐▌  ▐▌▐▌ ▐▌▐▛▀▜▌▐▛▀▜▌▐▌  ▐▌  █  ▐▌  ▐▌\n");
    printf("▐▌  ▐▌▝▚▄▞▘▐▌ ▐▌▐▌ ▐▌ ▝▚▞▘ ▗▄█▄▖▐▌  ▐▌\n");
    printf("\n");
    printf("              💀 MOHAVIM - Fork Modernizado do Nano 💀\n");
    printf("          Um fork modernizado com interface melhorada\n");
    printf("============================================================\n");
}

void mostrar_menu(int opcao) {
    limpar_tela();
    mostrar_logo();
    printf("\n               \033[1;36m🏠 MENU PRINCIPAL\033[0m\n\n");
    
    char* opcoes[] = {
        "📂 Abrir e Editar Arquivo",
        "📝 Criar Novo Arquivo",
        "🔍 Buscar em Arquivo",
        "ℹ️ Sobre",
        "🚪 Sair do Mohavim"
    };
    
    for (int i = 0; i < 5; i++) {
        if (i == opcao) {
            printf("  \033[1;33m►\033[0m \033[1m%s\033[0m  \033[1;33m◄\033[0m\n", opcoes[i]);
        } else {
            printf("    %s\n", opcoes[i]);
        }
    }
    
    printf("\nNavegue: \033[33m↑/↓\033[0m  Selecionar: \033[33mEnter\033[0m  Sair: \033[33mESC\033[0m\n");
}

void editor_simples(char* nome_arquivo) {
    char buffer[10000] = "";
    int cursor = 0;
    int modificado = 0;
    
    // Carregar arquivo se existir
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        fread(buffer, 1, sizeof(buffer) - 1, arquivo);
        fclose(arquivo);
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
        
        printf("\033[1;36m🖊️  MOHAVIM\033[0m - \033[1m%s\033[0m %s\n", nome_arquivo, 
               modificado ? "\033[1;33m[MODIFICADO]\033[0m" : "\033[1;32m[Salvo]\033[0m");
        printf("\033[33mCtrl+S\033[0m:Salvar  \033[33mCtrl+Q\033[0m:Sair  \033[33mESC\033[0m:Menu  \033[33m↑↓←→\033[0m:Navegar\n\n");
        
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
        printf("\033[36mLinha:\033[0m \033[1m%d\033[0m \033[2m|\033[0m \033[36mColuna:\033[0m \033[1m%d\033[0m \033[2m|\033[0m \033[36mTotal:\033[0m \033[1m%d\033[0m chars \033[2m|\033[0m %s\n", 
               linha_atual, coluna_atual, buffer_len,
               modificado ? "\033[1;33mMODIFICADO\033[0m" : "\033[1;32mSalvo\033[0m");
        
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
                
            case 127: // Backspace
                if (cursor > 0) {
                    memmove(&buffer[cursor - 1], &buffer[cursor], buffer_len - cursor + 1);
                    cursor--;
                    buffer_len--;
                    modificado = 1;
                }
                break;
                
            case 10: // Enter
                if (buffer_len < sizeof(buffer) - 1) {
                    memmove(&buffer[cursor + 1], &buffer[cursor], buffer_len - cursor + 1);
                    buffer[cursor] = '\n';
                    cursor++;
                    buffer_len++;
                    modificado = 1;
                }
                break;
                
            default:
                // Inserir caractere normal
                if (tecla >= 32 && tecla <= 126 && buffer_len < sizeof(buffer) - 1) {
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

void mostrar_lista_arquivos(int selecionado) {
    limpar_tela();
    printf("\033[1;36m📂 SELECIONAR ARQUIVO PARA ABRIR\033[0m\n");
    printf("Use \033[33m↑↓\033[0m para navegar, \033[33mEnter\033[0m para abrir, \033[33mD\033[0m para deletar, \033[33mESC\033[0m para voltar\n\n");
    
    // Mostrar opção para digitar nome manualmente
    if (selecionado == 0) {
        printf("  \033[1;33m►\033[0m \033[1m📝 [Digite nome do arquivo]\033[0m  \033[1;33m◄\033[0m\n\n");
    } else {
        printf("    📝 [Digite nome do arquivo]\n\n");
    }
    
    // Mostrar apenas alguns arquivos por vez para não sobrecarregar a tela
    int inicio = (selecionado > 10) ? selecionado - 10 : 0;
    int fim = (inicio + 20 < total_arquivos) ? inicio + 20 : total_arquivos;
    
    if (inicio > 0) {
        printf("    \033[2m... (%d arquivos anteriores)\033[0m\n", inicio);
    }
    
    for (int i = inicio; i < fim; i++) {
        if (i + 1 == selecionado) {
            printf("  \033[1;33m►\033[0m \033[1m📄 %s\033[0m  \033[1;33m◄\033[0m\n", arquivos[i]);
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }
    
    if (fim < total_arquivos) {
        printf("    \033[2m... (%d arquivos restantes)\033[0m\n", total_arquivos - fim);
    }
    
    printf("\n\033[36mArquivo\033[0m \033[1m%d\033[0m \033[36mde\033[0m \033[1m%d\033[0m\n", selecionado, total_arquivos + 1);
}

void carregar_lista_arquivos() {
    total_arquivos = 0;
    DIR *dir = opendir(".");
    
    if (dir != NULL) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL && total_arquivos < 99) {
            if (entry->d_type == DT_REG) { // Apenas arquivos regulares
                strcpy(arquivos[total_arquivos], entry->d_name);
                total_arquivos++;
            }
        }
        closedir(dir);
    }
}

void abrir_arquivo() {
    carregar_lista_arquivos();
    
    int opcao_atual = 0;
    int max_opcoes = total_arquivos + 1; // +1 para opção de digitar nome
    
    configurar_terminal();
    
    while (1) {
        mostrar_lista_arquivos(opcao_atual);
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                opcao_atual = (opcao_atual - 1 + max_opcoes) % max_opcoes;
                break;
                
            case 66: // Seta baixo
                opcao_atual = (opcao_atual + 1) % max_opcoes;
                break;
                
            case 10: // Enter
                restaurar_terminal();
                if (opcao_atual == 0) {
                    // Opção para digitar nome
                    printf("\033[2J\033[H");
                    printf("\033[1;36m📝 DIGITE O NOME DO ARQUIVO\033[0m\n\n");
                    printf("Nome: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            editor_simples(nome);
                        }
                    }
                } else {
                    // Arquivo selecionado da lista
                    editor_simples(arquivos[opcao_atual - 1]);
                }
                return;
                
            case 27: // ESC
                restaurar_terminal();
                return;
                
            case 'd':
            case 'D': // Deletar arquivo
                if (opcao_atual > 0) { // Não pode deletar a opção "Digite nome do arquivo"
                    char* nome_arquivo = arquivos[opcao_atual - 1];
                    
                    // Mostrar confirmação
                    printf("\033[2J\033[H");
                    printf("\033[1;31m🗑️ DELETAR ARQUIVO\033[0m\n\n");
                    printf("Tem certeza que deseja deletar o arquivo:\n");
                    printf("\033[1;33m📄 %s\033[0m\n\n", nome_arquivo);
                    printf("Esta ação \033[1;31mNÃO PODE SER DESFEITA!\033[0m\n\n");
                    printf("Digite \033[1;32m'SIM'\033[0m para confirmar ou qualquer tecla para cancelar: ");
                    
                    restaurar_terminal();
                    char confirmacao[10];
                    if (fgets(confirmacao, sizeof(confirmacao), stdin)) {
                        confirmacao[strcspn(confirmacao, "\n")] = 0;
                        
                        if (strcmp(confirmacao, "SIM") == 0) {
                            if (remove(nome_arquivo) == 0) {
                                printf("\n\033[1;32m✅ Arquivo '%s' deletado com sucesso!\033[0m\n", nome_arquivo);
                                
                                // Recarregar lista de arquivos
                                carregar_lista_arquivos();
                                max_opcoes = total_arquivos + 1;
                                
                                // Ajustar seleção se necessário
                                if (opcao_atual > max_opcoes - 1) {
                                    opcao_atual = max_opcoes - 1;
                                }
                                
                            } else {
                                printf("\n\033[1;31m❌ Erro ao deletar arquivo '%s'!\033[0m\n", nome_arquivo);
                            }
                        } else {
                            printf("\n\033[1;33m⚠️ Operação cancelada.\033[0m\n");
                        }
                        
                        printf("Pressione Enter para continuar...");
                        getchar();
                        configurar_terminal();
                    }
                }
                break;
        }
    }
}

void buscar_arquivo() {
    printf("\033[2J\033[H");
    printf("\033[1;36m🔍 BUSCAR EM ARQUIVO\033[0m\n\n");
    printf("Nome do arquivo: ");
    
    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;
        
        printf("Texto para buscar: ");
        char busca[256];
        if (fgets(busca, sizeof(busca), stdin)) {
            busca[strcspn(busca, "\n")] = 0;
            
            FILE* arquivo = fopen(nome, "r");
            if (arquivo) {
                char linha[1024];
                int num_linha = 1;
                int encontradas = 0;
                
                printf("\n\033[1;36m🔍 Resultados em '%s':\033[0m\n\n", nome);
                
                while (fgets(linha, sizeof(linha), arquivo)) {
                    if (strstr(linha, busca)) {
                        printf("Linha %d: %s", num_linha, linha);
                        encontradas++;
                    }
                    num_linha++;
                }
                
                if (encontradas == 0) {
                    printf("❌ Texto '%s' não encontrado.\n", busca);
                } else {
                    printf("\n✅ %d ocorrência(s) encontrada(s).\n", encontradas);
                }
                
                fclose(arquivo);
            } else {
                printf("❌ Erro ao abrir arquivo '%s'\n", nome);
            }
        }
    }
    
    printf("\nPressione Enter para continuar...");
    getchar();
}

void sobre() {
    printf("\033[2J\033[H");
    printf("\033[1;36mℹ️  SOBRE O MOHAVIM\033[0m\n\n");
    printf("🔧 Versão: Mohavim 7.2\n");
    printf("🏠 Linguagem: C (nativo)\n");
    printf("🌐 Interface: Português\n");
    printf("⌨️  Navegação: Setas + Atalhos\n");
    
    printf("Atalhos disponíveis:\n");
    printf("- ↑↓←→: Navegação\n");
    printf("- Ctrl+S: Salvar arquivo\n");  
    printf("- Ctrl+Q: Sair do editor\n");
    printf("- ESC: Voltar ao menu\n");
    printf("- Enter: Nova linha\n");
    printf("- Backspace: Deletar\n");
    
    printf("\nPressione Enter para voltar...");
    restaurar_terminal();
    getchar();
}

int main() {
    int opcao_atual = 0;
    
    mostrar_logo();
    printf("\nPressione Enter para continuar...");
    getchar();
    
    configurar_terminal();
    
    while (1) {
        mostrar_menu(opcao_atual);
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                opcao_atual = (opcao_atual - 1 + 5) % 5;
                break;
                
            case 66: // Seta baixo
                opcao_atual = (opcao_atual + 1) % 5;
                break;
                
            case 10: // Enter
                restaurar_terminal();
                
                switch (opcao_atual) {
                    case 0: abrir_arquivo(); break;
                    case 1: novo_arquivo(); break;
                    case 2: buscar_arquivo(); break;
                    case 3: sobre(); break;
                    case 4:
                        printf("\033[2J\033[H");
                        printf("🚪 Saindo...\n");
                        printf("💀 Obrigado por usar o Mohavim!\n");
                        exit(0);
                }
                
                configurar_terminal();
                break;
                
            case 27: // ESC
                restaurar_terminal();
                printf("\033[2J\033[H");
                printf("🚪 Saindo...\n");
                printf("💀 Obrigado por usar o Mohavim!\n");
                exit(0);
        }
    }
    
    return 0;
}
