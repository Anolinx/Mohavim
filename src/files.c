#define _GNU_SOURCE
#include "files.h"
#include "editor.h"
#include "ui.h"
#include "log.h"
#include "i18n.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

char arquivos[100][256];
int total_arquivos = 0;

void carregar_lista_arquivos() {
    total_arquivos=0;
    DIR* dir = opendir(".");
    if(dir){
        struct dirent* entry;
        while((entry=readdir(dir)) && total_arquivos<99){
            if(entry->d_type==DT_REG){
                strcpy(arquivos[total_arquivos], entry->d_name);
                total_arquivos++;
            }
        }
        closedir(dir);
        log_message(DEBUG, "%s", get_string("files_loaded_in_dir"));
    } else {
        log_message(ERROR, "%s", get_string("error_opening_dir"));
    }
}

void mostrar_lista_arquivos(int selecionado) {
    limpar_tela();
    printf("\033[1;36m\xF0\x9F\x93\x82 SELECIONAR ARQUIVO PARA ABRIR");theme_reset();printf("\n");
    printf("Use \033[33m\xE2\x86\x91\xE2\x86\x93");theme_reset();printf(" para navegar, \033[33mEnter");theme_reset();printf(" para abrir, \033[33mD");theme_reset();printf(" para deletar, \033[33mESC");theme_reset();printf(" para voltar\n\n");

    // Mostrar apenas alguns arquivos por vez para não sobrecarregar a tela
    int inicio = (selecionado > 10) ? selecionado - 10 : 0;
    int fim = (inicio + 20 < total_arquivos) ? inicio + 20 : total_arquivos;

    if (inicio > 0) {
        printf("    \033[2m... (%d arquivos anteriores)", inicio);theme_reset();printf("\n");
    }

    for (int i = inicio; i < fim; i++) {
        if (i == selecionado) {
            printf("  \033[1;33m►");theme_reset();printf(" \033[1m📄 %s", arquivos[i]);theme_reset();printf("  \033[1;33m◄");theme_reset();printf("\n");
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }

    if (fim < total_arquivos) {
        printf("    \033[2m... (%d arquivos restantes)", total_arquivos - fim);theme_reset();printf("\n");
    }

    char file_of_text[128];
    snprintf(file_of_text, sizeof(file_of_text), get_string("file_of"), selecionado + 1, total_arquivos);
    printf("\n\033[36m%s", file_of_text);theme_reset();printf("\033[K\n");
}

void abrir_arquivo() {
    carregar_lista_arquivos();

    // Verificar se há arquivos no diretório
    if (total_arquivos == 0) {
        limpar_tela();
        printf("%s\n\n", get_string("open_file_title"));
        printf("%s\n", get_string("no_files_in_directory"));
        printf("%s\n", get_string("press_enter_back"));
        restaurar_terminal();
        getchar();
        return;
    }

    int opcao_atual = 0;
    int max_opcoes = total_arquivos;

    configurar_terminal();

    while (1) {
        mostrar_lista_arquivos(opcao_atual);

        int tecla = ler_tecla();

        switch (tecla) {
            case 65: // Seta cima
                if (max_opcoes > 0) {
                    opcao_atual = (opcao_atual - 1 + max_opcoes) % max_opcoes;
                }
                break;

            case 66: // Seta baixo
                if (max_opcoes > 0) {
                    opcao_atual = (opcao_atual + 1) % max_opcoes;
                }
                break;

            case 10: // Enter
                restaurar_terminal();
                // Arquivo selecionado da lista (não há mais opção de digitar nome)
                editor_simples(arquivos[opcao_atual]);
                return;

            case 27: // ESC
                restaurar_terminal();
                return;

            case 'd':
            case 'D': // Deletar arquivo
                // Não há mais verificação para a opção "Digite nome do arquivo"
                char* nome_arquivo = arquivos[opcao_atual];

                // Mostrar confirmação
                printf("\033[2J\033[H");
                printf("\033[1;31m🗑️ DELETAR ARQUIVO");theme_reset();printf("\n\n");
                printf("Tem certeza que deseja deletar o arquivo:\n");
                printf("\033[1;33m📄 %s", nome_arquivo);theme_reset();printf("\n\n");
                printf("Esta ação \033[1;31mNÃO PODE SER DESFEITA!");theme_reset();printf("\n\n");
                printf("Digite \033[1;32m'SIM'");theme_reset();printf(" para confirmar ou qualquer tecla para cancelar: ");

                restaurar_terminal();
                char confirmacao[10];
                if (fgets(confirmacao, sizeof(confirmacao), stdin)) {
                    confirmacao[strcspn(confirmacao, "\n")] = 0;

                    if (strcmp(confirmacao, "SIM") == 0) {
                        if (remove(nome_arquivo) == 0) {
                            printf("\n%s\n", get_string("file_deleted"));
                            log_message(INFO, "Arquivo '%s' deletado com sucesso.", nome_arquivo);

                            // Recarregar lista de arquivos
                            carregar_lista_arquivos();
                            max_opcoes = total_arquivos;

                            // Ajustar seleção se necessário
                            if (opcao_atual >= max_opcoes) {
                                opcao_atual = max_opcoes - 1;
                            }

                        } else {
                            printf("\n%s\n", get_string("error_deleting_file"));
                            log_message(ERROR, "Erro ao deletar arquivo '%s'.", nome_arquivo);
                        }
                    } else {
                        printf("\n%s\n", get_string("operation_cancelled"));
                        log_message(INFO, "Operação de deleção de arquivo cancelada.");
                    }

                    printf("Pressione Enter para continuar...");
                    getchar();
                    configurar_terminal();
                }
                break;
        }
    }
}

void buscar_arquivo() {
    log_message(DEBUG, "%s", get_string("starting_file_search"));
    limpar_tela();
    printf("\033[1;36m🔍 BUSCAR EM ARQUIVO");theme_reset();printf("\n\n");
    printf("Nome do arquivo: ");

    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;

        if (strlen(nome) == 0) {
            log_message(WARN, "%s", get_string("empty_filename_for_search"));
            printf("\n%s\n", get_string("filename_empty"));
            printf("Pressione Enter para continuar...");
            getchar();
            return;
        }

        printf("Texto para buscar: ");
        char busca[256];
        if (fgets(busca, sizeof(busca), stdin)) {
            busca[strcspn(busca, "\n")] = 0;

            if (strlen(busca) == 0) {
                log_message(WARN, "%s", get_string("empty_search_text"));
                printf("\n%s\n", get_string("search_text_empty"));
                printf("Pressione Enter para continuar...");
                getchar();
                return;
            }

            FILE* arquivo = fopen(nome, "r");
            if (arquivo) {
                log_message(INFO, get_string("searching_for_in_file"), busca, nome);
                char linha[1024];
                int num_linha = 1;
                int encontradas = 0;

                limpar_tela();
                printf("\033[1;36m🔍 Resultados em '%s':", nome);theme_reset();printf("\n\n");

                while (fgets(linha, sizeof(linha), arquivo)) {
                    if (strstr(linha, busca)) {
                        printf("Linha %d: %s", num_linha, linha);
                        encontradas++;
                    }
                    num_linha++;
                }

                if (encontradas == 0) {
                    printf("%s\n", get_string("text_not_found"));
                    log_message(INFO, get_string("text_not_found_in_file"), busca, nome);
                } else {
                    printf("\n%s\n", get_string("occurrences_found"));
                    log_message(INFO, get_string("occurrences_found_in_file"), encontradas, busca, nome);
                }

                fclose(arquivo);
            } else {
                printf("%s\n", get_string("error_opening_file"));
                log_message(ERROR, get_string("error_opening_file_for_search"), nome);
            }
        }
    }

    printf("\nPressione Enter para continuar...");
    getchar();
    configurar_terminal(); // Restaurar terminal after operations
}
