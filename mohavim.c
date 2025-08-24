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
    
    // Mostrar lista de arquivos
    for (int i = 0; i < total_arquivos; i++) {
        if (i + 1 == selecionado) {
            printf("  ► 📄 %s  ◄\n", arquivos[i]);
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }
    
    if (total_arquivos == 0) {
        printf("    (Nenhum arquivo encontrado no diretório atual)\n");
    }
}

void carregar_lista_arquivos() {
    total_arquivos = 0;
    DIR *dir = opendir(".");
    if (dir == NULL) return;
    
    struct dirent *entrada;
    while ((entrada = readdir(dir)) != NULL && total_arquivos < 100) {
        // Filtrar apenas arquivos regulares (não diretórios)
        if (entrada->d_type == DT_REG) {
            strcpy(arquivos[total_arquivos], entrada->d_name);
            total_arquivos++;
        }
    }
    closedir(dir);
}

void abrir_arquivo() {
    carregar_lista_arquivos();
    
    int selecionado = 0;
    int max_opcoes = total_arquivos + 1; // +1 para opção de digitar nome
    
    configurar_terminal();
    
    while (1) {
        mostrar_lista_arquivos(selecionado);
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                selecionado = (selecionado - 1 + max_opcoes) % max_opcoes;
                break;
                
            case 66: // Seta baixo
                selecionado = (selecionado + 1) % max_opcoes;
                break;
                
            case 10: // Enter
                if (selecionado == 0) {
                    // Opção de digitar nome manualmente
                    printf("\033[2J\033[H");
                    printf("📂 ABRIR ARQUIVO\n");
                    printf("================\n");
                    printf("Nome do arquivo: ");
                    
                    char nome[256];
                    restaurar_terminal();
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            editor_simples(nome);
                        }
                    }
                    return;
                } else {
                    // Arquivo selecionado da lista
                    restaurar_terminal();
                    editor_simples(arquivos[selecionado - 1]);
                    return;
                }
                break;
                
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}

void buscar_em_arquivo() {
    printf("\033[2J\033[H");
    printf("🔍 BUSCAR EM ARQUIVO\n");
    printf("====================\n");
    printf("Nome do arquivo: ");
    
    char nome_arquivo[256];
    char termo_busca[256];
    
    restaurar_terminal();
    if (!fgets(nome_arquivo, sizeof(nome_arquivo), stdin)) return;
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    printf("Termo a buscar: ");
    if (!fgets(termo_busca, sizeof(termo_busca), stdin)) return;
    termo_busca[strcspn(termo_busca, "\n")] = 0;
    
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("❌ Erro: Não foi possível abrir o arquivo '%s'\n", nome_arquivo);
        printf("Pressione Enter para continuar...");
        getchar();
        return;
    }
    
    printf("\n🔍 Resultados da busca por '%s' em '%s':\n", termo_busca, nome_arquivo);
    printf("================================================\n");
    
    char linha[1000];
    int numero_linha = 1;
    int encontrados = 0;
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strstr(linha, termo_busca)) {
            printf("%3d: %s", numero_linha, linha);
            encontrados++;
        }
        numero_linha++;
    }
    
    if (encontrados == 0) {
        printf("❌ Nenhuma ocorrência encontrada.\n");
    } else {
        printf("\n✅ Total de %d linha(s) encontrada(s).\n", encontrados);
    }
    
    fclose(arquivo);
    printf("\nPressione Enter para continuar...");
    getchar();
}

void configuracoes() {
    printf("\033[2J\033[H");
    printf("⚙️  CONFIGURAÇÕES\n");
    printf("=================\n");
    printf("💀 Mohavim - Fork Modernizado do Nano\n");
    printf("Versão: 1.0\n");
    printf("Autor: Gabriel A. Matos\n\n");
    printf("🔧 Funcionalidades:\n");
    printf("• Editor de texto com sintaxe de navegação melhorada\n");
    printf("• Numeração de linhas\n");
    printf("• Indicador de posição do cursor\n");
    printf("• Sistema de busca integrado\n");
    printf("• Interface moderna com emojis\n");
    printf("• Cursor underline para melhor visibilidade\n\n");
    printf("⌨️  Atalhos principais:\n");
    printf("• Ctrl+S: Salvar arquivo\n");
    printf("• Ctrl+Q: Sair do editor\n");
    printf("• Setas: Navegar pelo texto\n");
    printf("• ESC: Voltar ao menu principal\n\n");
    
    restaurar_terminal();
    printf("Pressione Enter para voltar ao menu...");
    getchar();
}

int main() {
    int opcao = 0;
    
    configurar_terminal();
    
    while (1) {
        mostrar_menu(opcao);
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                opcao = (opcao - 1 + 5) % 5;
                break;
                
            case 66: // Seta baixo
                opcao = (opcao + 1) % 5;
                break;
                
            case 10: // Enter
                switch (opcao) {
                    case 0:
                        abrir_arquivo();
                        configurar_terminal();
                        break;
                    case 1:
                        novo_arquivo();
                        configurar_terminal();
                        break;
                    case 2:
                        buscar_em_arquivo();
                        configurar_terminal();
                        break;
                    case 3:
                        configuracoes();
                        configurar_terminal();
                        break;
                    case 4:
                        restaurar_terminal();
                        printf("👋 Obrigado por usar o Mohavim!\n");
                        exit(0);
                }
                break;
                
            case 27: // ESC
                restaurar_terminal();
                printf("👋 Obrigado por usar o Mohavim!\n");
                exit(0);
        }
    }
    
    return 0;
}

