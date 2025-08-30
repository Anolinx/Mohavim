
#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <stdbool.h>

#define MAX_PLUGINS 20
#define MAX_PLUGIN_NAME 64

typedef struct {
    char name[MAX_PLUGIN_NAME];
    char path[256];
    void* handle;
    bool loaded;
    void (*init_func)(void);
    void (*cleanup_func)(void);
} plugin_t;

extern plugin_t plugins[MAX_PLUGINS];
extern int plugin_count;

void init_plugin_manager();
int load_plugin(const char* name);
void unload_plugin(const char* name);
void list_plugins();
void manage_plugins_menu();

#endif
