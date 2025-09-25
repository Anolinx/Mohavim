
#ifndef THEME_H
#define THEME_H

#define MAX_THEMES 10

typedef struct {
    char name[32];
    char background[48];
    char foreground[48];
    char accent[48];
    char highlight[48];
    char error[48];
    char success[48];
} theme_t;

extern theme_t themes[MAX_THEMES];
extern int theme_count;
extern int current_theme;

void init_themes();
int load_theme(const char *name);
const char* get_color(const char *type);
const char* get_background_color();
const char* get_current_background();
const char* get_current_foreground();
void apply_theme_colors();
void reset_colors();
void print_with_color(const char* color, const char* text);

// Funções para gerenciar cursor
void set_cursor_type(int tipo);
int get_cursor_type();
char get_cursor_char();
const char* get_cursor_glyph();

// Função helper para reset e reaplicação de tema
void theme_reset();

#endif
