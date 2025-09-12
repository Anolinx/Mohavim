
#ifndef I18N_H
#define I18N_H

#define MAX_LANGUAGES 10
#define MAX_STRING_LENGTH 256
#define MAX_STRINGS 100

#include "theme.h"

typedef struct {
    char key[64];
    char value[MAX_STRING_LENGTH];
} language_string_t;

typedef struct {
    char code[8];
    char name[32];
    language_string_t strings[MAX_STRINGS];
    int string_count;
} language_t;

extern language_t languages[MAX_LANGUAGES];
extern int language_count;
extern int current_language;

void init_i18n();
void auto_detect_language();
int load_language(const char *code);
const char* get_string(const char *key);
void add_language_string(int lang_index, const char *key, const char *value);
const char* get_language_from_env();
const char* get_language_from_config();
void save_language_to_config(const char* language);
const char* get_theme_from_config();
void save_theme_to_config(const char* theme);
void verify_language_consistency();

#endif
