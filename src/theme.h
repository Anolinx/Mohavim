
#ifndef THEME_H
#define THEME_H

#define MAX_THEMES 10

typedef struct {
    char name[32];
    char background[16];
    char foreground[16];
    char accent[16];
    char highlight[16];
    char error[16];
    char success[16];
} theme_t;

extern theme_t themes[MAX_THEMES];
extern int theme_count;
extern int current_theme;

void init_themes();
int load_theme(const char *name);
const char* get_color(const char *type);
void apply_theme_colors();
void reset_colors();
void print_with_color(const char* color, const char* text);

#endif
