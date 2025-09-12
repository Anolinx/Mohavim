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
        (char*)get_string("open_file"),
        (char*)get_string("new_file"),
        (char*)get_string("search_file"),
        (char*)get_string("logs"),
        (char*)get_string("themes"),
        (char*)get_string("languages"),
        (char*)get_string("plugins"),
        (char*)get_string("about"),
        (char*)get_string("exit")
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

    printf("\n%s%s\033[0m", get_color("accent"), get_string("navigate"));
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    printf("  %s%s\033[0m", get_color("accent"), get_string("select_enter"));
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    printf("  %s%s\033[0m", get_color("accent"), get_string("exit_esc"));
    printf("%s%s\n", themes[current_theme].background, themes[current_theme].foreground);
}

void sobre() {
    limpar_tela();
    apply_theme_colors();
    print_with_color(get_color("accent"), (char*)get_string("about_title"));
    printf("\n\n");

    printf("%s%s%s%s\n", get_color("foreground"), get_string("version"), get_color("success"), "Mohavim 7.3");
    printf("%s%s%s%s\n", get_color("foreground"), get_string("language"), get_color("accent"), get_string("native_language"));
    printf("%s%s%s%s\n", get_color("foreground"), get_string("navigation"), get_color("accent"), get_string("setas_atalhos"));

    printf("\n%s%s\n", get_color("accent"), get_string("available_shortcuts"));
    printf("%s%s\n", get_color("foreground"), get_string("navigation_keys"));
    printf("%s%s\n", get_color("foreground"), get_string("save_shortcut"));  
    printf("%s%s\n", get_color("foreground"), get_string("exit_shortcut"));
    printf("%s%s\n", get_color("foreground"), get_string("menu_shortcut"));
    printf("%s%s\n", get_color("foreground"), get_string("newline_shortcut"));
    printf("%s%s\n", get_color("foreground"), get_string("delete_shortcut"));

    printf("\n%s%s", get_color("highlight"), get_string("press_enter_back"));
    restaurar_terminal();
    getchar();
}

void show_logs() {
    limpar_tela();
    apply_theme_colors();
    print_with_color(get_color("accent"), (char*)get_string("logs"));
    printf("\n\n");

    display_logs();

    printf("\n%s%s", get_color("highlight"), get_string("press_any_key_back"));
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
        print_with_color(get_color("accent"), (char*)get_string("themes"));
        printf("\n\n");

        print_with_color(get_color("foreground"), (char*)get_string("available_themes"));
        printf("\n\n");

        // Dark theme
        if (opcao_tema == 0) {
            printf("  %s► %s ◄%s\n", get_color("highlight"), get_string("dark_theme"), "\033[0m");
        } else {
            printf("   %s%s%s\n", get_color("foreground"), get_string("dark_theme"), "\033[0m");
        }

        // Light theme
        if (opcao_tema == 1) {
            printf("  %s► %s ◄%s\n", get_color("highlight"), get_string("light_theme"), "\033[0m");
        } else {
            printf("   %s%s%s\n", get_color("foreground"), get_string("light_theme"), "\033[0m");
        }

        // Vaporwave theme
        if (opcao_tema == 2) {
            printf("  %s\u25ba %s \u25c4%s\n", get_color("highlight"), get_string("vaporwave_theme"), "\033[0m");
        } else {
            printf("   %s%s%s\n", get_color("foreground"), get_string("vaporwave_theme"), "\033[0m");
        }

        // Vintage theme
        if (opcao_tema == 3) {
            printf("  %s\u25ba %s \u25c4%s\n", get_color("highlight"), get_string("vintage_theme"), "\033[0m");
        } else {
            printf("   %s%s%s\n", get_color("foreground"), get_string("vintage_theme"), "\033[0m");
        }

        printf("\n");
        print_with_color(get_color("accent"), get_string("navigate"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("select_enter"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("exit_esc"));
        printf("\n");

        int tecla = ler_tecla();
        switch(tecla) {
            case 65: // Seta para cima
                opcao_tema = (opcao_tema - 1 + 4) % 4;
                break;
            case 66: // Seta para baixo
                opcao_tema = (opcao_tema + 1) % 4;
                break;
            case 10: // Enter
                if (opcao_tema == 0) {
                    load_theme("dark");
                    save_theme_to_config("dark");
                } else if (opcao_tema == 1) {
                    load_theme("light");
                    save_theme_to_config("light");
                } else if (opcao_tema == 2) {
                    load_theme("vaporwave");
                    save_theme_to_config("vaporwave");
                } else if (opcao_tema == 3) {
                    load_theme("vintage");
                    save_theme_to_config("vintage");
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
        print_with_color(get_color("accent"), (char*)get_string("languages"));
        printf("\n\n");

        print_with_color(get_color("foreground"), (char*)get_string("available_languages"));
        printf("\n\n");

        // Português
        if (opcao_idioma == 0) {
            printf("  ");
            print_with_color(get_color("highlight"), (char*)get_string("portuguese"));
            printf("\n");
        } else {
            printf("   ");
            print_with_color(get_color("foreground"), (char*)get_string("portuguese"));
            printf("\n");
        }

        // English
        if (opcao_idioma == 1) {
            printf("  ");
            print_with_color(get_color("highlight"), (char*)get_string("english"));
            printf("\n");
        } else {
            printf("   ");
            print_with_color(get_color("foreground"), (char*)get_string("english"));
            printf("\n");
        }

        printf("\n");
        print_with_color(get_color("accent"), get_string("navigate"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("select_enter"));
        printf("  ");
        print_with_color(get_color("accent"), get_string("exit_esc"));
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
                    save_language_to_config("pt_br");
                } else if (opcao_idioma == 1) {
                    load_language("en");
                    save_language_to_config("en");
                }
                // Verify language consistency after change
                verify_language_consistency();
                restaurar_terminal();
                return;
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}
