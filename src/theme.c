
#include "theme.h"
#include <string.h>
#include <stdio.h>

theme_t themes[MAX_THEMES];
int theme_count = 0;
int current_theme = 0;

void init_themes() {
    theme_count = 0;
    
    // Tema Escuro (padrão)
    strcpy(themes[0].name, "dark");
    strcpy(themes[0].background, "\033[40m");
    strcpy(themes[0].foreground, "\033[37m");
    strcpy(themes[0].accent, "\033[1;36m");
    strcpy(themes[0].highlight, "\033[1;33m");
    strcpy(themes[0].error, "\033[1;31m");
    strcpy(themes[0].success, "\033[1;32m");
    
    // Tema Claro
    strcpy(themes[1].name, "light");
    strcpy(themes[1].background, "\033[47m");
    strcpy(themes[1].foreground, "\033[30m");
    strcpy(themes[1].accent, "\033[1;34m");
    strcpy(themes[1].highlight, "\033[1;35m");
    strcpy(themes[1].error, "\033[1;31m");
    strcpy(themes[1].success, "\033[1;32m");
    
    // Tema Cyberpunk
    strcpy(themes[2].name, "cyberpunk");
    strcpy(themes[2].background, "\033[40m");
    strcpy(themes[2].foreground, "\033[1;92m");
    strcpy(themes[2].accent, "\033[1;96m");
    strcpy(themes[2].highlight, "\033[1;93m");
    strcpy(themes[2].error, "\033[1;91m");
    strcpy(themes[2].success, "\033[1;92m");
    
    theme_count = 3;
    current_theme = 0;
}

int load_theme(const char *name) {
    for (int i = 0; i < theme_count; i++) {
        if (strcmp(themes[i].name, name) == 0) {
            current_theme = i;
            apply_theme_colors();
            return 1;
        }
    }
    return 0;
}

const char* get_color(const char *type) {
    if (strcmp(type, "background") == 0) return themes[current_theme].background;
    if (strcmp(type, "foreground") == 0) return themes[current_theme].foreground;
    if (strcmp(type, "accent") == 0) return themes[current_theme].accent;
    if (strcmp(type, "highlight") == 0) return themes[current_theme].highlight;
    if (strcmp(type, "error") == 0) return themes[current_theme].error;
    if (strcmp(type, "success") == 0) return themes[current_theme].success;
    return "\033[0m";
}

void apply_theme_colors() {
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
}
