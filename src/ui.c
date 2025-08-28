#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios old_termios;

void limpar_tela() {
    // Limpar tela
    printf("\033[2J\033[H");
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
    printf("         Desenvolvido por: Gabriel A. Matos\n");
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

