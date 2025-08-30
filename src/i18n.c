
#include "i18n.h"
#include <string.h>
#include <stdio.h>

language_t languages[MAX_LANGUAGES];
int language_count = 0;
int current_language = 0;

void init_i18n() {
    language_count = 0;
    current_language = 0;
    
    // Português (padrão)
    strcpy(languages[0].code, "pt_br");
    strcpy(languages[0].name, "Português BR");
    languages[0].string_count = 0;
    
    // Adicionar strings em português Brasileiro
    add_language_string(0, "menu_title", "🏠 MENU PRINCIPAL");
    add_language_string(0, "open_file", "📂 Abrir e Editar Arquivo");
    add_language_string(0, "new_file", "📝 Criar Novo Arquivo");
    add_language_string(0, "search_file", "🔍 Buscar em Arquivo");
    add_language_string(0, "about", "ℹ️ Sobre");
    add_language_string(0, "exit", "🚪 Sair do Mohavim");
    add_language_string(0, "navigate", "Navegue: ↑/↓  Selecionar: Enter  Sair: ESC");
    add_language_string(0, "editor_title", "🖊️  MOHAVIM");
    add_language_string(0, "modified", "[MODIFICADO]");
    add_language_string(0, "saved", "[Salvo]");
    add_language_string(0, "controls", "Ctrl+S:Salvar  Ctrl+Q:Sair  ESC:Menu  ↑↓←→:Navegar");
    add_language_string(0, "line", "Linha:");
    add_language_string(0, "column", "Coluna:");
    add_language_string(0, "total", "Total:");
    add_language_string(0, "chars", "chars");
    add_language_string(0, "logs", "📋 Logs");
    add_language_string(0, "themes", "🎨 Temas");
    add_language_string(0, "plugins", "🔌 Plugins");
    add_language_string(0, "settings", "⚙️ Configurações");
    
    language_count = 1;
    
    // Inglês
    strcpy(languages[1].code, "en");
    strcpy(languages[1].name, "English");
    languages[1].string_count = 0;
    
    add_language_string(1, "menu_title", "🏠 MAIN MENU");
    add_language_string(1, "open_file", "📂 Open and Edit File");
    add_language_string(1, "new_file", "📝 Create New File");
    add_language_string(1, "search_file", "🔍 Search in File");
    add_language_string(1, "about", "ℹ️ About");
    add_language_string(1, "exit", "🚪 Exit Mohavim");
    add_language_string(1, "navigate", "Navigate: ↑/↓  Select: Enter  Exit: ESC");
    add_language_string(1, "editor_title", "🖊️  MOHAVIM");
    add_language_string(1, "modified", "[MODIFIED]");
    add_language_string(1, "saved", "[Saved]");
    add_language_string(1, "controls", "Ctrl+S:Save  Ctrl+Q:Exit  ESC:Menu  ↑↓←→:Navigate");
    add_language_string(1, "line", "Line:");
    add_language_string(1, "column", "Column:");
    add_language_string(1, "total", "Total:");
    add_language_string(1, "chars", "chars");
    add_language_string(1, "logs", "📋 Logs");
    add_language_string(1, "themes", "🎨 Themes");
    add_language_string(1, "plugins", "🔌 Plugins");
    add_language_string(1, "settings", "⚙️ Settings");
    
    language_count = 2;
}

void add_language_string(int lang_index, const char *key, const char *value) {
    if (lang_index >= 0 && lang_index < language_count && 
        languages[lang_index].string_count < MAX_STRINGS) {
        
        strcpy(languages[lang_index].strings[languages[lang_index].string_count].key, key);
        strcpy(languages[lang_index].strings[languages[lang_index].string_count].value, value);
        languages[lang_index].string_count++;
    }
}

int load_language(const char *code) {
    for (int i = 0; i < language_count; i++) {
        if (strcmp(languages[i].code, code) == 0) {
            current_language = i;
            return 1;
        }
    }
    return 0;
}

const char* get_string(const char *key) {
    for (int i = 0; i < languages[current_language].string_count; i++) {
        if (strcmp(languages[current_language].strings[i].key, key) == 0) {
            return languages[current_language].strings[i].value;
        }
    }
    return key; // Retorna a chave se não encontrar
}
