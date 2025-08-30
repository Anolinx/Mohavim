
#include "plugin_manager.h"
#include "ui.h"
#include "log.h"
#include "i18n.h"
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
    
    // Adicionar à lista
    strcpy(plugins[plugin_count].name, name);
    strcpy(plugins[plugin_count].path, path);
    plugins[plugin_count].handle = handle;
    plugins[plugin_count].loaded = true;
    plugins[plugin_count].init_func = init_func;
    plugins[plugin_count].cleanup_func = cleanup_func;
    
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
            
            log_message(LOG_INFO, "Plugin '%s' descarregado", name);
            return;
        }
    }
    log_message(LOG_WARNING, "Plugin '%s' não encontrado", name);
}

void list_plugins() {
    printf("\nPlugins carregados:\n");
    for (int i = 0; i < plugin_count; i++) {
        printf("  %s %s (%s)\n", 
               plugins[i].loaded ? "✅" : "❌",
               plugins[i].name,
               plugins[i].path);
    }
}

void manage_plugins_menu() {
    limpar_tela();
    printf("\033[1;36m🔌 %s\033[0m\n\n", get_string("plugins"));
    
    list_plugins();
    
    printf("\nPressione qualquer tecla para voltar...");
    configurar_terminal();
    ler_tecla();
    restaurar_terminal();
}
