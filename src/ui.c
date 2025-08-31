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
    
    char*opcoes[] = {
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
    int opcao_tema = current_theme;
    configurar_terminal();

    while(1) {
        limpar_tela();
        printf("\033[1;36m🎨 %s\033[0m\n\n", get_string("themes"));

        printf("Temas disponíveis:\n");

        // Dark theme
        if (opcao_tema == 0) {
            printf("  %s► \033[1mdark (Escuro)\033[0m %s◄\033[0m\n", get_color("highlight"), get_color("highlight"));
        } else {
            printf("   dark (Escuro)\n");
        }

        // Light theme
        if (opcao_tema == 1) {
            printf("  %s► \033[1mlight (Claro)\033[0m %s◄\033[0m\n", get_color("highlight"), get_color("highlight"));
        } else {
            printf("   light (Claro)\n");
        }

        // Cyberpunk theme
        if (opcao_tema == 2) {
            printf("  %s► \033[1mcyberpunk (Cyberpunk)\033[0m %s◄\033[0m\n", get_color("highlight"), get_color("highlight"));
        } else {
            printf("   cyberpunk (Cyberpunk)\n");
        }

        printf("\n%s  %s  %s\n",
               get_string("Navegar ↑↓"),
               get_string("Selecionar: Enter"),
               get_string("Sair: ESC"));

        int tecla = ler_tecla();
        switch(tecla) {
            case 65: // Seta para cima
                opcao_tema = (opcao_tema - 1 + 3) % 3;
                break;
            case 66: // Seta para baixo
                opcao_tema = (opcao_tema + 1) % 3;
                break;
            case 10: // Enter
                if (opcao_tema == 0) {
                    load_theme("dark");
                } else if (opcao_tema == 1) {
                    load_theme("light");
                } else if (opcao_tema == 2) {
                    load_theme("cyberpunk");
                }
                restaurar_terminal();
                return;
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}

void mostrar_menu_idiomas() {
    int opcao_idioma = current_language;
    configurar_terminal();

    while(1) {
        limpar_tela();
        printf("\033[1;36m🌐 Idiomas\033[0m\n\n");

        printf("Idiomas disponíveis:\n");

        // Português
        if (opcao_idioma == 0) {
            printf("  %s► \033[1mpt (Português)\033[0m %s◄\033[0m\n", get_color("highlight"), get_color("highlight"));
        } else {
            printf("   pt (Português)\n");
        }

        // English
        if (opcao_idioma == 1) {
            printf("  %s► \033[1men (English)\033[0m %s◄\033[0m\n", get_color("highlight"), get_color("highlight"));
        } else {
            printf("   en (English)\n");
        }

        printf("\n%s  %s  %s\n",
               get_string("Navegar ↑↓"),
               get_string("Selecionar: Enter"),
               get_string("Sair: ESC"));

        int tecla = ler_tecla();
        switch(tecla) {
            case 65: // Seta para cima
                opcao_idioma = (opcao_idioma - 1 + 2) % 2;
                break;
            case 66: // Seta para baixo
                opcao_idioma = (opcao_idioma + 1) % 2;
                break;
            case 10: // Enter
                if (opcao_idioma == 0) {
                    load_language("pt_br");
                } else if (opcao_idioma == 1) {
                    load_language("en");
                }
                restaurar_terminal();
                return;
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}
