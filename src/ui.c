#include "ui.h"
#include "i18n.h"
#include "theme.h"
#include "log.h"
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
    printf("=============================================\n");
    printf("   ▗▖  ▗▖ ▗▄▖ ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖\n");
    printf("   ▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌  █  ▐▛▚▞▜▌\n");
    printf("   ▐▌  ▐▌▐▌ ▐▌▐▛▀▜▌▐▛▀▜▌▐▌  ▐▌  █  ▐▌  ▐▌\n");
    printf("   ▐▌  ▐▌▝▚▄▞▘▐▌ ▐▌▐▌ ▐▌ ▝▚▞▘ ▗▄█▄▖▐▌  ▐▌\n");
    printf("\n");
    printf("     Desenvolvido por: Gabriel A. Matos\n");
    printf("=============================================\n");
}
void mostrar_menu(int opcao) {
    limpar_tela();
    mostrar_logo();
    printf("\n               \033[1;36m🏠 MENU PRINCIPAL\033[0m\n\n");
    
    char* opcoes[] = {
        (char*)get_string("📂 Abrir e Editar Arquivo"),
        (char*)get_string("📝 Criar Novo Arquivo"),
        (char*)get_string("🔍 Buscar em Arquivo"),
        (char*)get_string("Logs"),
        (char*)get_string("Temas"),
        "🌐 Idiomas",
        (char*)get_string(" Plugins"),
        (char*)get_string("ℹ️ Sobre"),
        (char*)get_string("🚪 Sair do Mohavim")
    };

       for (int i = 0; i < 9; i++) {
        if (i == opcao) {
            printf("  %s►\033[0m \033[1m%s\033[0m  %s◄\033[0m\n", get_color("highlight"), opcoes[i], get_color("highlight"));
        } else {
            printf("    %s\n", opcoes[i]);
        }
    }

       printf("\n%s  %s  %s\n",
       get_string("Navegar ↑↓"),
       get_string("Selecionar: Enter"),
       get_string("Sair: ESC"));
}

void sobre() {
    printf("\033[2J\033[H");
    printf("\033[1;36mℹ️  SOBRE O MOHAVIM\033[0m\n\n");
    printf("🔧 Versão: Mohavim 7.3\n");
    printf("🏠 Linguagem: C (nativo)\n");
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

void show_logs() {
    limpar_tela();
    printf("\033[1;36m📋 %s\033[0m\n\n", get_string("logs"));

    display_logs();

    printf("\nPressione qualquer tecla para voltar...");
    configurar_terminal();
    ler_tecla();
    restaurar_terminal();
}

void mostrar_menu_temas() {
    limpar_tela();
    printf("\033[1;36m🎨 %s\033[0m\n\n", get_string("themes"));

    printf("Temas disponíveis:\n");
    printf("  %s dark (Escuro)\n", current_theme == 0 ? "►" : " ");
    printf("  %s light (Claro)\n", current_theme == 1 ? "►" : " ");
    printf("  %s cyberpunk (Cyberpunk)\n", current_theme == 2 ? "►" : " ");

    printf("\nUse os argumentos da linha de comando:\n");
    printf("  mohavim --theme dark\n");
    printf("  mohavim --theme light\n");
    printf("  mohavim --theme cyberpunk\n");

    printf("\nPressione qualquer tecla para voltar...");
    configurar_terminal();
    ler_tecla();
    restaurar_terminal();
}

void mostrar_menu_idiomas() {
    limpar_tela();
    printf("\033[1;36m🌐 Idiomas\033[0m\n\n");

    printf("Idiomas disponíveis:\n");
    printf("  %s pt (Português)\n", current_language == 0 ? "►" : " ");
    printf("  %s en (English)\n", current_language == 1 ? "►" : " ");

    printf("\nUse os argumentos da linha de comando:\n");
    printf("  mohavim --lang pt\n");
    printf("  mohavim --lang en\n");

    printf("\nPressione qualquer tecla para voltar...");
    configurar_terminal();
    ler_tecla();
    restaurar_terminal();
}
