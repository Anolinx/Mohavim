
#ifndef I18N_H
#define I18N_H

#define MAX_LANGUAGES 10
#define MAX_STRING_LENGTH 256
#define MAX_STRINGS 50

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
int load_language(const char *code);
const char* get_string(const char *key);
void add_language_string(int lang_index, const char *key, const char *value);

#endif
