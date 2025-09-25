#include "ui.h"
#include "editor.h"
#include "files.h"
#include "i18n.h"
#include "theme.h"
#include "log.h"
#include "plugin_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help() {
    printf("%s\n\n", get_string("mohavim_modular_editor"));
    printf("%s\n\n", get_string("usage"));
    printf("%s\n", get_string("options"));
    printf("%s\n", get_string("help_option"));
    printf("%s\n", get_string("lang_option"));
    printf("%s\n", get_string("theme_option"));
    printf("%s\n", get_string("install_plugin_option"));
    printf("%s\n", get_string("list_plugins_option"));
    printf("\n%s\n", get_string("examples"));
    printf("  mohavim --lang en --theme cyberpunk\n");
    printf("  mohavim --install-plugin myplugin\n");
}

int main(int argc, char* argv[]) {
    // Inicializar sistemas
    init_i18n();
    init_themes();
    init_logging();
    init_plugin_manager();

    // Carregar configurações salvas antes de processar argumentos da linha de comando
    const char* saved_language = get_language_from_config();
    if (saved_language != NULL) {
        load_language(saved_language);
        log_message(INFO, get_string("language_loaded_from_config"), saved_language);
    }
    
    const char* saved_theme = get_theme_from_config();
    if (saved_theme != NULL) {
        load_theme(saved_theme);
        log_message(INFO, get_string("theme_loaded_from_config"), saved_theme);
    }

    // Create an array to track which arguments have been processed
    int processed[argc];
    for (int i = 0; i < argc; i++) {
        processed[i] = 0;
    }

    // Processar argumentos de linha de comando primeiro para --lang
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--lang") == 0 && i + 1 < argc) {
            if (!load_language(argv[++i])) {
                printf("%s '%s'\n", get_string("language_not_found"), argv[i]);
                return 1;
            }
            log_message(INFO, get_string("language_changed_to"), argv[i]);
            processed[i-1] = 1; // Mark --lang as processed
            processed[i] = 1;   // Mark language code as processed
        }
    }

    // Processar novamente para outras opções
    for (int i = 1; i < argc; i++) {
        if (processed[i]) continue; // Skip already processed arguments

        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        }
        else if (strcmp(argv[i], "--theme") == 0 && i + 1 < argc) {
            if (!load_theme(argv[++i])) {
                printf("%s '%s'\n", get_string("theme_not_found"), argv[i]);
                return 1;
            }
            log_message(INFO, get_string("theme_changed_to"), argv[i]);
            processed[i-1] = 1; // Mark --theme as processed
            processed[i] = 1;   // Mark theme name as processed
        }
        
        else if (strcmp(argv[i], "--install-plugin") == 0 && i + 1 < argc) {
            if (load_plugin(argv[++i])) {
                printf("%s '%s'\n", get_string("plugin_installed"), argv[i]);
            }
            processed[i-1] = 1; // Mark --install-plugin as processed
            processed[i] = 1;   // Mark plugin name as processed
            return 0;
        }
        else if (strcmp(argv[i], "--list-plugins") == 0) {
            list_plugins();
            processed[i] = 1;   // Mark --list-plugins as processed
            return 0;
        }
        else {
            printf("%s '%s'\n", get_string("unknown_argument"), argv[i]);
            printf("%s\n", get_string("use_help"));
            return 1;
        }
    }

    int opcao_atual=0;
    mostrar_logo();
    printf("\nPressione Enter para continuar..."); getchar();
    configurar_terminal();

    while(1){
        mostrar_menu(opcao_atual);
        int tecla=ler_tecla();
        switch(tecla){
            case 65: opcao_atual=(opcao_atual-1+10)%10; break;
            case 66: opcao_atual=(opcao_atual+1)%10; break;
            case 10: restaurar_terminal();
                switch(opcao_atual){
                    case 0: abrir_arquivo(); break;
                    case 1: novo_arquivo(); break;
                    case 2: buscar_arquivo(); break;
                    case 3: show_logs(); break;
                    case 4: mostrar_menu_temas(); break;
                    case 5: mostrar_menu_cursores(); break;
                    case 6: mostrar_menu_idiomas(); break;
                    case 7: manage_plugins_menu(); break;
                    case 8: sobre(); break;
                    case 9: printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
                }
                configurar_terminal();
                break;
            case 27: restaurar_terminal(); printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
        }
    }
    return 0;
}