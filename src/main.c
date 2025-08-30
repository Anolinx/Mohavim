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
    printf("Mohavim - Editor de texto modular\n\n");
    printf("Uso: mohavim [opções]\n\n");
    printf("Opções:\n");
    printf("  --help, -h          Mostrar esta ajuda\n");
    printf("  --lang <código>     Definir idioma (pt_br, en)\n");
    printf("  --theme <nome>      Definir tema (dark, light, cyberpunk)\n");
    printf("  --verbose, -v       Ativar modo verbose\n");
    printf("  --install-plugin <nome>  Instalar plugin\n");
    printf("  --list-plugins      Listar plugins\n");
    printf("\nExemplos:\n");
    printf("  mohavim --lang en --theme cyberpunk\n");
    printf("  mohavim --verbose\n");
    printf("  mohavim --install-plugin myplugin\n");
}

int main(int argc, char* argv[]) {
    // Inicializar sistemas
    init_i18n();
    init_themes();
    init_logging();
    init_plugin_manager();

    // Processar argumentos de linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        }
        else if (strcmp(argv[i], "--lang") == 0 && i + 1 < argc) {
            if (!load_language(argv[++i])) {
                printf("Idioma '%s' não encontrado\n", argv[i]);
                return 1;
            }
            log_message(LOG_INFO, "Idioma alterado para: %s", argv[i]);
        }
        else if (strcmp(argv[i], "--theme") == 0 && i + 1 < argc) {
            if (!load_theme(argv[++i])) {
                printf("Tema '%s' não encontrado\n", argv[i]);
                return 1;
            }
            log_message(LOG_INFO, "Tema alterado para: %s", argv[i]);
        }
        else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            toggle_verbose_mode();
        }
        else if (strcmp(argv[i], "--install-plugin") == 0 && i + 1 < argc) {
            if (load_plugin(argv[++i])) {
                printf("Plugin '%s' instalado com sucesso\n", argv[i]);
            }
            return 0;
        }
        else if (strcmp(argv[i], "--list-plugins") == 0) {
            list_plugins();
            return 0;
        }
        else {
            printf("Argumento desconhecido: %s\n", argv[i]);
            printf("Use --help para ver as opções disponíveis\n");
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
            case 65: opcao_atual=(opcao_atual-1+9)%9; break;
            case 66: opcao_atual=(opcao_atual+1)%9; break;
            case 10: restaurar_terminal();
                switch(opcao_atual){
                    case 0: abrir_arquivo(); break;
                    case 1: novo_arquivo(); break;
                    case 2: buscar_arquivo(); break;
                    case 3: show_logs(); break;
                    case 4: mostrar_menu_temas(); break;
                    case 5: mostrar_menu_idiomas(); break;
                    case 6: manage_plugins_menu(); break;
                    case 7: sobre(); break;
                    case 8: printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
                }
                configurar_terminal();
                break;
            case 27: restaurar_terminal(); printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
        }
    }
    return 0;
}
