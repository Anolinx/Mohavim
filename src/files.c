#define _GNU_SOURCE
#include "files.h"
#include "editor.h"
#include "ui.h"
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
    }
}

void mostrar_lista_arquivos(int selecionado) {
    limpar_tela();
    printf("\033[1;36m📂 SELECIONAR ARQUIVO PARA ABRIR\033[0m\n");
    printf("Use \033[33m↑↓\033[0m para navegar, \033[33mEnter\033[0m para abrir, \033[33mD\033[0m para deletar, \033[33mESC\033[0m para voltar\n\n");
    
    // Mostrar opção para digitar nome manualmente
    if (selecionado == 0) {
        printf("  \033[1;33m►\033[0m \033[1m📝 [Digite nome do arquivo]\033[0m  \033[1;33m◄\033[0m\n\n");
    } else {
        printf("    📝 [Digite nome do arquivo]\n\n");
    }
    
    // Mostrar apenas alguns arquivos por vez para não sobrecarregar a tela
    int inicio = (selecionado > 10) ? selecionado - 10 : 0;
    int fim = (inicio + 20 < total_arquivos) ? inicio + 20 : total_arquivos;
    
    if (inicio > 0) {
        printf("    \033[2m... (%d arquivos anteriores)\033[0m\n", inicio);
    }
    
    for (int i = inicio; i < fim; i++) {
        if (i + 1 == selecionado) {
            printf("  \033[1;33m►\033[0m \033[1m📄 %s\033[0m  \033[1;33m◄\033[0m\n", arquivos[i]);
        } else {
            printf("    📄 %s\n", arquivos[i]);
        }
    }
    
    if (fim < total_arquivos) {
        printf("    \033[2m... (%d arquivos restantes)\033[0m\n", total_arquivos - fim);
    }
    
    printf("\n\033[36mArquivo\033[0m \033[1m%d\033[0m \033[36mde\033[0m \033[1m%d\033[0m\n", selecionado, total_arquivos + 1);
}

void abrir_arquivo() {
    carregar_lista_arquivos();
    
    int opcao_atual = 0;
    int max_opcoes = total_arquivos + 1; // +1 para opção de digitar nome
    
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
                if (opcao_atual == 0) {
                    // Opção para digitar nome
                    printf("\033[2J\033[H");
                    printf("\033[1;36m📝 DIGITE O NOME DO ARQUIVO\033[0m\n\n");
                    printf("Nome: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            editor_simples(nome);
                        }
                    }
                } else {
                    // Arquivo selecionado da lista
                    editor_simples(arquivos[opcao_atual - 1]);
                }
                return;
                
            case 27: // ESC
                restaurar_terminal();
                return;
                
            case 'd':
            case 'D': // Deletar arquivo
                if (opcao_atual > 0) { // Não pode deletar a opção "Digite nome do arquivo"
                    char* nome_arquivo = arquivos[opcao_atual - 1];
                    
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
                                printf("\n\033[1;32m✅ Arquivo '%s' deletado com sucesso!\033[0m\n", nome_arquivo);
                                
                                // Recarregar lista de arquivos
                                carregar_lista_arquivos();
                                max_opcoes = total_arquivos + 1;
                                
                                // Ajustar seleção se necessário
                                if (opcao_atual > max_opcoes - 1) {
                                    opcao_atual = max_opcoes - 1;
                                }
                                
                            } else {
                                printf("\n\033[1;31m❌ Erro ao deletar arquivo '%s'!\033[0m\n", nome_arquivo);
                            }
                        } else {
                            printf("\n\033[1;33m⚠️ Operação cancelada.\033[0m\n");
                        }
                        
                        printf("Pressione Enter para continuar...");
                        getchar();
                        configurar_terminal();
                    }
                }
                break;
        }
    }
}

void buscar_arquivo() {
    printf("\033[2J\033[H");
    printf("\033[1;36m🔍 BUSCAR EM ARQUIVO\033[0m\n\n");
    printf("Nome do arquivo: ");
    
    char nome[256];
    restaurar_terminal();
    if (fgets(nome, sizeof(nome), stdin)) {
        nome[strcspn(nome, "\n")] = 0;
        
        printf("Texto para buscar: ");
        char busca[256];
        if (fgets(busca, sizeof(busca), stdin)) {
            busca[strcspn(busca, "\n")] = 0;
            
            FILE* arquivo = fopen(nome, "r");
            if (arquivo) {
                char linha[1024];
                int num_linha = 1;
                int encontradas = 0;
                
                printf("\n\033[1;36m🔍 Resultados em '%s':\033[0m\n\n", nome);
                
                while (fgets(linha, sizeof(linha), arquivo)) {
                    if (strstr(linha, busca)) {
                        printf("Linha %d: %s", num_linha, linha);
                        encontradas++;
                    }
                    num_linha++;
                }
                
                if (encontradas == 0) {
                    printf("❌ Texto '%s' não encontrado.\n", busca);
                } else {
                    printf("\n✅ %d ocorrência(s) encontrada(s).\n", encontradas);
                }
                
                fclose(arquivo);
            } else {
                printf("❌ Erro ao abrir arquivo '%s'\n", nome);
            }
        }
    }
    
    printf("\nPressione Enter para continuar...");
    getchar();
}
