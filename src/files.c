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
        log_message(LOG_DEBUG, "%d arquivos carregados no diretório", total_arquivos);
    } else {
        log_message(LOG_ERROR, "Erro ao abrir diretório atual");
    }
}

void mostrar_lista_arquivos(int selecionado) {
    limpar_tela();
    printf("\033[1;36m\xF0\x9F\x93\x82 SELECIONAR ARQUIVO PARA ABRIR\033[0m\n");
    printf("Use \033[33m\xE2\x86\x91\xE2\x86\x93\033[0m para navegar, \033[33mEnter\033[0m para abrir, \033[33mD\033[0m para deletar, \033[33mESC\033[0m para voltar\n\n");

    // Mostrar apenas alguns arquivos por vez para não sobrecarregar a tela
    int inicio = (selecionado > 10) ? selecionado - 10 : 0;
    int fim = (inicio + 20 < total_arquivos) ? inicio + 20 : total_arquivos;

    if (inicio > 0) {
        printf("    \033[2m... (%d arquivos anteriores)\033[0m\n", inicio);
    }

    // Ajustar seleção para compensar a remoção da opção de digitar nome
    int ajuste_selecao = (selecionado > 0) ? selecionado - 1 : 0;

    for (int i = inicio; i < fim; i++) {
        if (i == ajuste_selecao) {
            printf("  \033[1;33m►\033[0m \033[1m📄 %s\033[0m  \033[1;33m◄\033[0m\n", arquivos[i]);
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }

    if (fim < total_arquivos) {
        printf("    \033[2m... (%d arquivos restantes)\033[0m\n", total_arquivos - fim);
    }

    char file_of_text[128];
    snprintf(file_of_text, sizeof(file_of_text), get_string("file_of"), selecionado, total_arquivos);
    printf("\n\033[36m%s\033[0m%s\033[K\n", file_of_text, get_background_color());
}

void abrir_arquivo() {
    carregar_lista_arquivos();

    int opcao_atual = 0;
    int max_opcoes = total_arquivos; // Removida a opção de digitar nome

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
                printf("\033[1;31m🗑️ DELETAR ARQUIVO\033[0m\n\n");
                printf("Tem certeza que deseja deletar o arquivo:\n");
                printf("\033[1;33m📄 %s\033[0m\n\n", nome_arquivo);
                printf("Esta ação \033[1;31mNÃO PODE SER DESFEITA!\033[0m\n\n");
                printf("Digite \033[1;32m'SIM'\033[0m para confirmar ou qualquer tecla para cancelar: ");

                restaurar_terminal();
                char confirmacao[10];
                if (fgets(confirmacao, sizeof(confirmacao), stdin)) {
                    confirmacao[strcspn(confirmacao, "\n")] = 0;

                    if (strcmp(confirmacao, "SIM") == 0) {
                        if (remove(nome_arquivo) == 0) {
                            printf("\n%s\n", get_string("file_deleted"));
                            log_message(LOG_INFO, "Arquivo '%s' deletado com sucesso.", nome_arquivo);

                            // Recarregar lista de arquivos
                            carregar_lista_arquivos();
                            max_opcoes = total_arquivos;

                            // Ajustar seleção se necessário
                            if (opcao_atual >= max_opcoes) {
                                opcao_atual = max_opcoes - 1;
                            }

                        } else {
                            printf("\n%s\n", get_string("error_deleting_file"));
                            log_message(LOG_ERROR, "Erro ao deletar arquivo '%s'.", nome_arquivo);
                        }
                    } else {
                        printf("\n%s\n", get_string("operation_cancelled"));
                        log_message(LOG_INFO, "Operação de deleção de arquivo cancelada.");
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
    log_message(LOG_DEBUG, "Iniciando busca de arquivo.");
    limpar_tela();
    printf("\033[1;36m🔍 BUSCAR EM ARQUIVO\033[0m\n\n");
    printf("Nome do arquivo: ");

    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;

        if (strlen(nome) == 0) {
            log_message(LOG_WARNING, "Nome de arquivo vazio fornecido para busca.");
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
                log_message(LOG_WARNING, "Texto de busca vazio fornecido.");
                printf("\n%s\n", get_string("search_text_empty"));
                printf("Pressione Enter para continuar...");
                getchar();
                return;
            }

            FILE* arquivo = fopen(nome, "r");
            if (arquivo) {
                log_message(LOG_INFO, "Buscando por '%s' em '%s'.", busca, nome);
                char linha[1024];
                int num_linha = 1;
                int encontradas = 0;

                limpar_tela();
                printf("\033[1;36m🔍 Resultados em '%s':\033[0m\n\n", nome);

                while (fgets(linha, sizeof(linha), arquivo)) {
                    if (strstr(linha, busca)) {
                        printf("Linha %d: %s", num_linha, linha);
                        encontradas++;
                    }
                    num_linha++;
                }

                if (encontradas == 0) {
                    printf("%s\n", get_string("text_not_found"));
                    log_message(LOG_INFO, "Texto '%s' não encontrado em '%s'.", busca, nome);
                } else {
                    printf("\n%s\n", get_string("occurrences_found"));
                    log_message(LOG_INFO, "%d ocorrência(s) de '%s' encontradas em '%s'.", encontradas, busca, nome);
                }

                fclose(arquivo);
            } else {
                printf("%s\n", get_string("error_opening_file"));
                log_message(LOG_ERROR, "Erro ao abrir arquivo '%s' para busca.", nome);
            }
        }
    }

    printf("\nPressione Enter para continuar...");
    getchar();
    configurar_terminal(); // Restaurar terminal after operations
}
