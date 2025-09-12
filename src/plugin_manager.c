
#include "plugin_manager.h"
#include "ui.h"
#include "log.h"
#include "i18n.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>

plugin_t plugins[MAX_PLUGINS];
int plugin_count = 0;

void init_plugin_manager() {
    plugin_count = 0;
    log_message(LOG_INFO, "Sistema de plugins iniciado");
    
    // Criar diretório plugins se não existir
    system("mkdir -p plugins");
    
    // Carregar plugins automaticamente
    DIR* dir = opendir("plugins");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) && plugin_count < MAX_PLUGINS) {
            if (strstr(entry->d_name, ".so")) {
                char name[MAX_PLUGIN_NAME];
                strncpy(name, entry->d_name, sizeof(name) - 1);
                name[sizeof(name) - 1] = '\0';
                
                // Remover extensão .so
                char* dot = strrchr(name, '.');
                if (dot) *dot = '\0';
                
                load_plugin(name);
            }
        }
        closedir(dir);
    }
}

int load_plugin(const char* name) {
    if (plugin_count >= MAX_PLUGINS) {
        log_message(LOG_ERROR, "Limite máximo de plugins atingido");
        return 0;
    }
    
    // Verificar se já está carregado
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, name) == 0) {
            log_message(LOG_WARNING, "Plugin '%s' já está carregado", name);
            return 0;
        }
    }
    
    // Construir caminho do plugin
    char path[512];
    snprintf(path, sizeof(path), "plugins/%s.so", name);
    
    // Tentar carregar biblioteca
    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        log_message(LOG_ERROR, "Erro ao carregar plugin '%s': %s", name, dlerror());
        return 0;
    }
    
    // Buscar função de inicialização
    void (*init_func)(void) = dlsym(handle, "plugin_init");
    if (!init_func) {
        log_message(LOG_WARNING, "Plugin '%s' sem função plugin_init", name);
    }
    
    // Buscar função de limpeza
    void (*cleanup_func)(void) = dlsym(handle, "plugin_cleanup");
    
    // Buscar função de execução
    void (*run_func)(void) = dlsym(handle, "plugin_run");
    
    // Adicionar à lista
    strcpy(plugins[plugin_count].name, name);
    strcpy(plugins[plugin_count].path, path);
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].loaded = true;
    plugins[plugin_count].init_func = init_func;
    plugins[plugin_count].cleanup_func = cleanup_func;
    plugins[plugin_count].run_func = run_func;
    
    // Executar inicialização se disponível
    if (init_func) {
        init_func();
    }
    
    plugin_count++;
    log_message(LOG_INFO, "Plugin '%s' carregado com sucesso", name);
    return 1;
}

void unload_plugin(const char* name) {
    for (int i = 0; i < plugin_count; i++) {
        if (strcmp(plugins[i].name, name) == 0 && plugins[i].loaded) {
            // Executar limpeza se disponível
            if (plugins[i].cleanup_func) {
                plugins[i].cleanup_func();
            }
            
            // Descarregar biblioteca
            dlclose(plugins[i].handle);
            plugins[i].loaded = false;
            plugins[i].init_func = NULL;
            plugins[i].cleanup_func = NULL;
            plugins[i].run_func = NULL;
            
            log_message(LOG_INFO, "Plugin '%s' descarregado", name);
            return;
        }
    }
    log_message(LOG_WARNING, "Plugin '%s' não encontrado", name);
}

void list_plugins() {
    printf("\nPlugins carregados:\n");
    for (int i = 0; i < plugin_count; i++) {
        printf("  %s %s (%s)", 
               plugins[i].loaded ? "✅" : "❌",
               plugins[i].name,
               plugins[i].path);
        
        // Show if plugin has run function
        if (plugins[i].loaded && plugins[i].run_func) {
            printf(" ⚡");
        }
        printf("\n");
    }
}

// Function pointer type for plugin run function
typedef void (*plugin_run_func)(void);

void manage_plugins_menu() {
    int opcao_plugin = 0;
    configurar_terminal();

    while(1) {
        limpar_tela();
        apply_theme_colors();
        print_with_color(get_color("accent"), (char*)get_string("plugins"));
        printf("\n\n");

        // Listar plugins disponíveis
        printf("%s\n", get_string("available_plugins"));
        printf("\n");

        // Opção para carregar plugin
        if (opcao_plugin == 0) {
            printf("  %s►%s %s%s%s  %s◄%s\n", 
                   get_color("highlight"), get_color("accent"),
                   get_color("highlight"), get_string("load_plugin_option"),
                   get_color("accent"), get_color("highlight"), get_color("accent"));
        } else {
            printf("   %s%s\n", get_color("foreground"), get_string("load_plugin_option"));
        }

        // Opção para listar plugins
        if (opcao_plugin == 1) {
            printf("  %s►%s %s%s%s  %s◄%s\n", 
                   get_color("highlight"), get_color("accent"),
                   get_color("highlight"), get_string("list_plugins_option"),
                   get_color("accent"), get_color("highlight"), get_color("accent"));
        } else {
            printf("   %s%s\n", get_color("foreground"), get_string("list_plugins_option"));
        }

        // Opção para descarregar plugin
        if (opcao_plugin == 2) {
            printf("  %s►%s %s%s%s  %s◄%s\n", 
                   get_color("highlight"), get_color("accent"),
                   get_color("highlight"), get_string("unload_plugin_option"),
                   get_color("accent"), get_color("highlight"), get_color("accent"));
        } else {
            printf("   %s%s\n", get_color("foreground"), get_string("unload_plugin_option"));
        }
        
        // Opção para executar plugin
        if (opcao_plugin == 3) {
            printf("  %s►%s %s%s%s  %s◄%s\n", 
                   get_color("highlight"), get_color("accent"),
                   get_color("highlight"), get_string("run_plugin_option"),
                   get_color("accent"), get_color("highlight"), get_color("accent"));
        } else {
            printf("   %s%s\n", get_color("foreground"), get_string("run_plugin_option"));
        }

        printf("\n");
        print_with_color(get_color("accent"), (char*)get_string("navigate"));
        printf("\n");

        int tecla = ler_tecla();
        switch(tecla) {
            case 65: // Seta para cima
                opcao_plugin = (opcao_plugin - 1 + 4) % 4;
                break;
            case 66: // Seta para baixo
                opcao_plugin = (opcao_plugin + 1) % 4;
                break;
            case 10: // Enter
                restaurar_terminal();
                if (opcao_plugin == 0) {
                    // Carregar plugin
                    limpar_tela();
                    apply_theme_colors();
                    printf("%s📥 CARREGAR PLUGIN%s\n\n", get_color("accent"), get_color("foreground"));
                    printf("Nome do plugin: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            if (load_plugin(nome)) {
                                printf("\n✅ %s\n", get_string("plugin_loaded_success"));
                                log_message(LOG_INFO, "Plugin '%s' carregado com sucesso", nome);
                            } else {
                                printf("\n❌ %s\n", get_string("error_loading_plugin"));
                                log_message(LOG_ERROR, "Erro ao carregar plugin '%s'", nome);
                            }
                        }
                    }
                    printf("\n%sPressione Enter para continuar...%s", get_color("highlight"), get_color("foreground"));
                    getchar();
                    configurar_terminal();
                } else if (opcao_plugin == 1) {
                    // Listar plugins
                    limpar_tela();
                    apply_theme_colors();
                    print_with_color(get_color("accent"), (char*)get_string("plugins"));
                    printf("\n\n");
                    list_plugins();
                    printf("\n%s", get_string("press_any_key_back"));
                    configurar_terminal();
                    ler_tecla();
                    restaurar_terminal();
                } else if (opcao_plugin == 2) {
                    // Descarregar plugin
                    limpar_tela();
                    apply_theme_colors();
                    printf("%s📤 DESCARREGAR PLUGIN%s\n\n", get_color("accent"), get_color("foreground"));
                    printf("Nome do plugin: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            unload_plugin(nome);
                        }
                    }
                    printf("\n%sPressione Enter para continuar...%s", get_color("highlight"), get_color("foreground"));
                    getchar();
                    configurar_terminal();
                } else if (opcao_plugin == 3) {
                    // Executar plugin
                    limpar_tela();
                    apply_theme_colors();
                    printf("%s⚡ EXECUTAR PLUGIN%s\n\n", get_color("accent"), get_color("foreground"));
                    printf("Nome do plugin: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\n")] = 0;
                        if (strlen(nome) > 0) {
                            // Try to find and run the plugin
                            int plugin_found = 0;
                            for (int i = 0; i < plugin_count; i++) {
                                if (strcmp(plugins[i].name, nome) == 0 && plugins[i].loaded) {
                                    // Try to find the plugin_run function
                                    plugin_run_func run_func = (plugin_run_func)dlsym(plugins[i].handle, "plugin_run");
                                    if (run_func) {
                                        printf("\n%sIniciando plugin '%s'...%s\n", get_color("accent"), nome, get_color("foreground"));
                                        configurar_terminal();
                                        run_func();
                                        restaurar_terminal();
                                        printf("\n%sPlugin '%s' finalizado.%s\n", get_color("accent"), nome, get_color("foreground"));
                                        plugin_found = 1;
                                    } else {
                                        printf("\n❌ %s\n", "Plugin não possui função de execução (plugin_run)");
                                    }
                                    break;
                                }
                            }
                            
                            if (!plugin_found) {
                                printf("\n❌ %s\n", "Plugin não encontrado ou não carregado");
                            }
                        }
                    }
                    printf("\n%sPressione Enter para continuar...%s", get_color("highlight"), get_color("foreground"));
                    getchar();
                    configurar_terminal();
                }
                break;
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}
