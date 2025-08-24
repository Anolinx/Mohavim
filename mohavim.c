/**************************************************************************
 *   mohavim.c  --  Um fork modernizado do nano                           *
*    Criado por: Gabriel A. Matos                                         *
 *   Copyright (C) 2025 Projeto Mohavim                                   *
 *   Baseado no GNU nano 7.2                                             *
 *                                                                        *
 *   Este programa é software livre; você pode redistribuí-lo e/ou       *
 *   modificá-lo sob os termos da Licença Pública Geral GNU conforme     *
 *   publicada pela Free Software Foundation.                            *
 *                                                                        *
 *   Mohavim é um fork modernizado do nano com interface melhorada       *
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
    // Método mais compatível para limpar tela
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
    printf("\n               🏠 MENU PRINCIPAL\n");
    printf("==================================================\n\n");
    
    char* opcoes[] = {
        "📂 Abrir e Editar Arquivo",
        "📝 Criar Novo Arquivo",
        "🔍 Buscar em Arquivo",
        "⚙️ Configurações",
        "🚪 Sair do Mohavim"
    };
    
    for (int i = 0; i < 5; i++) {
        if (i == opcao) {
            printf("  ► %s  ◄\n", opcoes[i]);
        } else {
            printf("    %s\n", opcoes[i]);
        }
    }
    
    printf("\n==================================================\n");
    printf("Navegue: ↑/↓  Selecionar: Enter  Sair: ESC\n");
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
        
        printf("🖊️  MOHAVIM - Editando: %s %s\n", nome_arquivo, 
               modificado ? "[MODIFICADO]" : "");
        printf("================================================================\n");
        printf("Ctrl+S:Salvar  Ctrl+Q:Sair  ESC:Menu  ↑↓←→:Navegar\n");
        printf("================================================================\n");
        
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
                printf("%3d │ ", linha_exibida);
            }
            
            if (i == cursor) {
                printf("\033[7m \033[0m"); // Cursor em destaque
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
            printf("\033[7m \033[0m");
        }
        
        printf("\n\n================================================================\n");
        printf("Linha: %d | Coluna: %d | Total: %d chars | %s\n", 
               linha_atual, coluna_atual, buffer_len,
               modificado ? "MODIFICADO" : "Salvo");
        
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
    printf("📝 CRIAR NOVO ARQUIVO\n");
    printf("=====================\n");
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
    printf("📂 SELECIONAR ARQUIVO PARA ABRIR\n");
    printf("=================================\n");
    printf("Use ↑↓ para navegar, Enter para abrir, ESC para voltar\n\n");
    
    // Mostrar opção para digitar nome manualmente
    if (selecionado == 0) {
        printf("  ► 📝 [Digite nome do arquivo]  ◄\n\n");
    } else {
        printf("    📝 [Digite nome do arquivo]\n\n");
    }
    
    // Mostrar apenas alguns arquivos por vez para não sobrecarregar a tela
    int inicio = (selecionado > 10) ? selecionado - 10 : 0;
    int fim = (inicio + 20 < total_arquivos) ? inicio + 20 : total_arquivos;
    
    if (inicio > 0) {
        printf("    ... (%d arquivos anteriores)\n", inicio);
    }
    
    for (int i = inicio; i < fim; i++) {
        if (i + 1 == selecionado) {
            printf("  ► 📄 %s  ◄\n", arquivos[i]);
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }
    
    if (fim < total_arquivos) {
        printf("    ... (%d arquivos restantes)\n", total_arquivos - fim);
    }
    
    printf("\n=================================\n");
    printf("Arquivo %d de %d\n", selecionado, total_arquivos + 1);
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
                    printf("📝 DIGITE O NOME DO ARQUIVO\n");
                    printf("===========================\n");
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
        }
    }
}

void buscar_arquivo() {
    printf("\033[2J\033[H");
    printf("🔍 BUSCAR EM ARQUIVO\n");
    printf("====================\n");
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
                
                printf("\n🔍 Resultados em '%s':\n", nome);
                printf("========================\n");
                
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

void configuracoes() {
    printf("\033[2J\033[H");
    printf("⚙️ CONFIGURAÇÕES MOHAVIM\n");
    printf("=========================\n");
    printf("🔧 Versão: Mohavim 7.2-fork\n");
    printf("📅 Baseado em: GNU nano 7.2\n");
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
                    case 3: configuracoes(); break;
                    case 4:
                        printf("\033[2J\033[H");
                        printf("🚪 Saindo do Mohavim...\n");
                        printf("💀 Obrigado por usar o Mohavim!\n");
                        exit(0);
                }
                
                configurar_terminal();
                break;
                
            case 27: // ESC
                restaurar_terminal();
                printf("\033[2J\033[H");
                printf("🚪 Saindo do Mohavim...\n");
                printf("💀 Obrigado por usar o Mohavim!\n");
                exit(0);
        }
    }
    
    return 0;
}
