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
    // Limpar tela completamente e resetar cores
    printf("\033[2J\033[H\033[0m");
    apply_theme_colors();
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
    printf("\033[2J\033[H\033[0m"); // Reset completo da tela
    apply_theme_colors();
    
    printf("=============================================\n");
    print_with_color(get_color("accent"), "   ▗▖  ▗▖ ▗▄▖ ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖\n");
    print_with_color(get_color("accent"), "   ▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌  █  ▐▛▚▞▜▌\n");
    print_with_color(get_color("accent"), "   ▐▌  ▐▌▐▌ ▐▌▐▛▀▜▌▐▛▀▜▌▐▌  ▐▌  █  ▐▌  ▐▌\n");
    print_with_color(get_color("accent"), "   ▐▌  ▐▌▝▚▄▞▘▐▌ ▐▌▐▌ ▐▌ ▝▚▞▘ ▗▄█▄▖▐▌  ▐▌\n");
    printf("\n");
    printf("%s     Desenvolvido por: Gabriel A. Matos\n", get_color("foreground"));
    printf("=============================================\n");
}
void mostrar_menu(int opcao) {
    limpar_tela();
    apply_theme_colors();
    mostrar_logo();
    
    printf("\n               %s🏠 MENU PRINCIPAL\033[0m", get_color("accent"));
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    printf("\n\n");

    char*opcoes[] = {
        (char*)get_string("📂 Abrir e Editar Arquivo"),
        (char*)get_string("📝 Criar Novo Arquivo"),
        (char*)get_string("🔍 Buscar em Arquivo"),
        (char*)get_string("📋 Logs"),
        (char*)get_string("🎨 Temas"),
        "🌐 Idiomas",
        (char*)get_string("🔌 Plugins"),
        (char*)get_string("ℹ️ Sobre"),
        (char*)get_string("🚪 Sair do Mohavim")
    };

    for (int i = 0; i < 9; i++) {
        if (i == opcao) {
            printf("  %s► %s ◄\033[0m", get_color("highlight"), opcoes[i]);
            printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
            printf("\n");
        } else {
            printf("    %s\n", opcoes[i]);
        }
    }

    printf("\n%sNavegar ↑↓\033[0m", get_color("accent"));
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    printf("  %sSelecionar: Enter\033[0m", get_color("accent"));
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    printf("  %sSair: ESC\033[0m", get_color("accent"));
    printf("%s%s\n", themes[current_theme].background, themes[current_theme].foreground);
}

void sobre() {
    limpar_tela();
    apply_theme_colors();
    print_with_color(get_color("accent"), "ℹ️  SOBRE O MOHAVIM");
    printf("\n\n");

    printf("%s🔧 Versão: %sMohavim 7.3%s\n", get_color("foreground"), get_color("success"), "\033[0m");
    printf("%s🏠 Linguagem: %sC (nativo)%s\n", get_color("foreground"), get_color("accent"), "\033[0m");
    printf("%s⌨️  Navegação: %sSetas + Atalhos%s\n", get_color("foreground"), get_color("accent"), "\033[0m");

    printf("\n%sAtalhos disponíveis:%s\n", get_color("accent"), "\033[0m");
    printf("%s- ↑↓←→: Navegação%s\n", get_color("foreground"), "\033[0m");
    printf("%s- Ctrl+S: Salvar arquivo%s\n", get_color("foreground"), "\033[0m");  
    printf("%s- Ctrl+Q: Sair do editor%s\n", get_color("foreground"), "\033[0m");
    printf("%s- ESC: Voltar ao menu%s\n", get_color("foreground"), "\033[0m");
    printf("%s- Enter: Nova linha%s\n", get_color("foreground"), "\033[0m");
    printf("%s- Backspace: Deletar%s\n", get_color("foreground"), "\033[0m");

    printf("\n%sPressione Enter para voltar...%s", get_color("highlight"), "\033[0m");
    restaurar_terminal();
    getchar();
}

void show_logs() {
    limpar_tela();
    apply_theme_colors();
    print_with_color(get_color("accent"), "📋 Logs");
    printf("\n\n");

    display_logs();

    printf("\n%sPressione qualquer tecla para voltar...%s", get_color("highlight"), "\033[0m");
    configurar_terminal();
    ler_tecla();
    restaurar_terminal();
}

void mostrar_menu_temas() {
    int opcao_tema = current_theme;
    configurar_terminal();

    while(1) {
        limpar_tela();
        apply_theme_colors();
        print_with_color(get_color("accent"), "🎨 Temas");
        printf("\n\n");

        print_with_color(get_color("foreground"), "Temas disponíveis:");
        printf("\n\n");

        // Dark theme
        if (opcao_tema == 0) {
            printf("  %s► Escuro (Dark) ◄%s\n", get_color("highlight"), "\033[0m");
        } else {
            printf("   %sEscuro (Dark)%s\n", get_color("foreground"), "\033[0m");
        }

        // Light theme
        if (opcao_tema == 1) {
            printf("  %s► Claro (Light) ◄%s\n", get_color("highlight"), "\033[0m");
        } else {
            printf("   %sClaro (Light)%s\n", get_color("foreground"), "\033[0m");
        }

        // Cyberpunk theme
        if (opcao_tema == 2) {
            printf("  %s► Cyberpunk (Neon) ◄%s\n", get_color("highlight"), "\033[0m");
        } else {
            printf("   %sCyberpunk (Neon)%s\n", get_color("foreground"), "\033[0m");
        }

        printf("\n");
        print_with_color(get_color("accent"), get_string("Navegar ↑↓"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("Selecionar: Enter"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("Sair: ESC"));
        printf("\n");

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
                current_theme = opcao_tema;
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
        apply_theme_colors();
        print_with_color(get_color("accent"), "🌐 Idiomas");
        printf("\n\n");

        print_with_color(get_color("foreground"), "Idiomas disponíveis:");
        printf("\n\n");

        // Português
        if (opcao_idioma == 0) {
            printf("  ");
            print_with_color(get_color("highlight"), "► pt (Português) ◄");
            printf("\n");
        } else {
            printf("   ");
            print_with_color(get_color("foreground"), "pt (Português)");
            printf("\n");
        }

        // English
        if (opcao_idioma == 1) {
            printf("  ");
            print_with_color(get_color("highlight"), "► en (English) ◄");
            printf("\n");
        } else {
            printf("   ");
            print_with_color(get_color("foreground"), "en (English)");
            printf("\n");
        }

        printf("\n");
        print_with_color(get_color("accent"), get_string("Navegar ↑↓"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("Selecionar: Enter"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("Sair: ESC"));
        printf("\n");

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
