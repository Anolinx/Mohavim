
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
    strcpy(themes[0].background, "\033[48;2;40;44;52m");     // Fundo cinza escuro
    strcpy(themes[0].foreground, "\033[38;2;171;178;191m");  // Texto cinza claro
    strcpy(themes[0].accent, "\033[38;2;97;175;239m");       // Azul claro
    strcpy(themes[0].highlight, "\033[48;2;97;175;239m\033[38;2;40;44;52m"); // Fundo azul com texto escuro
    strcpy(themes[0].error, "\033[38;2;224;108;117m");       // Vermelho suave
    strcpy(themes[0].success, "\033[38;2;152;195;121m");     // Verde suave
    
    // Tema Claro
    strcpy(themes[1].name, "light");
    strcpy(themes[1].background, "\033[48;2;250;250;250m");  // Fundo branco
    strcpy(themes[1].foreground, "\033[38;2;56;58;66m");     // Texto cinza escuro
    strcpy(themes[1].accent, "\033[38;2;41;98;255m");        // Azul médio
    strcpy(themes[1].highlight, "\033[48;2;41;98;255m\033[38;2;255;255;255m"); // Fundo azul com texto branco
    strcpy(themes[1].error, "\033[38;2;202;18;67m");         // Vermelho intenso
    strcpy(themes[1].success, "\033[38;2;34;134;58m");       // Verde intenso
    
    // Tema Cyberpunk
    strcpy(themes[2].name, "cyberpunk");
    strcpy(themes[2].background, "\033[48;2;13;17;23m");     // Fundo preto azulado
    strcpy(themes[2].foreground, "\033[38;2;0;255;146m");    // Verde neon
    strcpy(themes[2].accent, "\033[38;2;0;255;255m");        // Ciano neon
    strcpy(themes[2].highlight, "\033[48;2;255;20;147m\033[38;2;0;0;0m"); // Fundo magenta neon com texto preto
    strcpy(themes[2].error, "\033[38;2;255;20;147m");        // Magenta neon
    strcpy(themes[2].success, "\033[38;2;0;255;146m");       // Verde neon
    
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
    if (current_theme < 0 || current_theme >= theme_count) {
        return "\033[0m";
    }
    
    if (strcmp(type, "background") == 0) return themes[current_theme].background;
    if (strcmp(type, "foreground") == 0) return themes[current_theme].foreground;
    if (strcmp(type, "accent") == 0) return themes[current_theme].accent;
    if (strcmp(type, "highlight") == 0) return themes[current_theme].highlight;
    if (strcmp(type, "error") == 0) return themes[current_theme].error;
    if (strcmp(type, "success") == 0) return themes[current_theme].success;
    return "\033[0m";
}

void apply_theme_colors() {
    // Reset completo antes de aplicar novas cores
    printf("\033[0m");
    // Aplicar cor de fundo e texto padrão
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    fflush(stdout);
}

void reset_colors() {
    printf("\033[0m");
    fflush(stdout);
}

void print_with_color(const char* color, const char* text) {
    printf("%s%s\033[0m", color, text);
    printf("%s%s", themes[current_theme].background, themes[current_theme].foreground);
    fflush(stdout);
}
