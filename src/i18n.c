#include "i18n.h"
#include "theme.h"
#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

language_t languages[MAX_LANGUAGES];
int language_count = 0;
int current_language = 0;

// Function to get language from environment variable
const char* get_language_from_env() {
    char* lang = getenv("LANGUAGE");
    if (lang == NULL) {
        lang = getenv("LANG");
    }
    return lang;
}

// Function to get language from config file
const char* get_language_from_config() {
    FILE* config_file = fopen(".mohavimrc", "r");
    if (config_file == NULL) {
        // Try home directory config
        char home_config[256];
        char* home = getenv("HOME");
        if (home != NULL) {
            snprintf(home_config, sizeof(home_config), "%s/.mohavimrc", home);
            config_file = fopen(home_config, "r");
        }
    }
    
    if (config_file != NULL) {
        static char lang[32];
        char line[256];
        while (fgets(line, sizeof(line), config_file)) {
            if (strncmp(line, "language=", 9) == 0) {
                sscanf(line, "language=%31s", lang);
                fclose(config_file);
                return lang;
            }
        }
        fclose(config_file);
    }
    return NULL;
}

// Function to get theme from config file
const char* get_theme_from_config() {
    FILE* config_file = fopen(".mohavimrc", "r");
    if (config_file == NULL) {
        // Try home directory config
        char home_config[256];
        char* home = getenv("HOME");
        if (home != NULL) {
            snprintf(home_config, sizeof(home_config), "%s/.mohavimrc", home);
            config_file = fopen(home_config, "r");
        }
    }
    
    if (config_file != NULL) {
        static char theme[32];
        char line[256];
        while (fgets(line, sizeof(line), config_file)) {
            if (strncmp(line, "theme=", 6) == 0) {
                sscanf(line, "theme=%31s", theme);
                fclose(config_file);
                return theme;
            }
        }
        fclose(config_file);
    }
    return NULL;
}

// Function to save configuration to config file (JSON-like format)
void save_config_to_file(const char* language, const char* theme) {
    // Try home directory config
    char home_config[256];
    char* home = getenv("HOME");
    if (home != NULL) {
        snprintf(home_config, sizeof(home_config), "%s/.mohavimrc", home);
        
        // Write config file
        FILE* config_file = fopen(home_config, "w");
        if (config_file != NULL) {
            // Write in a simple key=value format (similar to JSON but simpler for C)
            if (language != NULL) {
                fprintf(config_file, "language=%s\n", language);
            }
            if (theme != NULL) {
                fprintf(config_file, "theme=%s\n", theme);
            }
            fclose(config_file);
        }
    }
}

// Function to save language to config file
void save_language_to_config(const char* language) {
    // Get current theme to preserve it
    extern int current_theme;
    extern theme_t themes[MAX_THEMES];
    const char* current_theme_name = themes[current_theme].name;
    save_config_to_file(language, current_theme_name);
}

// Function to save theme to config file
void save_theme_to_config(const char* theme) {
    // Get current language to preserve it
    const char* current_lang = languages[current_language].code;
    save_config_to_file(current_lang, theme);
}

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
    add_language_string(0, "controls", "Ctrl+S:Salvar  Ctrl+Q:Sair  ESC:Menu  Ctrl+C:Copy  Ctrl+V:Paste  Ctrl+Z:Undo  ↑↓←→:Navegar");
    add_language_string(0, "line", "Linha:");
    add_language_string(0, "column", "Coluna:");
    add_language_string(0, "total", "Total:");
    add_language_string(0, "chars", "chars");
    add_language_string(0, "logs", "📋 Logs");
    add_language_string(0, "themes", "🎨 Temas");
    add_language_string(0, "plugins", "🔌 Plugins");
    add_language_string(0, "settings", "⚙️ Configurações");
    add_language_string(0, "languages", "🌐 Idiomas");
    add_language_string(0, "version", "🔧 Versão: ");
    add_language_string(0, "language", "🏠 Linguagem: ");
    add_language_string(0, "navigation", "⌨️  Navegação: ");
    add_language_string(0, "available_shortcuts", "Atalhos disponíveis:");
    add_language_string(0, "navigation_keys", "- ↑↓←→: Navegação");
    add_language_string(0, "save_shortcut", "- Ctrl+S: Salvar arquivo");
    add_language_string(0, "exit_shortcut", "- Ctrl+Q: Sair do editor");
    add_language_string(0, "menu_shortcut", "- ESC: Voltar ao menu");
    add_language_string(0, "newline_shortcut", "- Enter: Nova linha");
    add_language_string(0, "delete_shortcut", "- Backspace: Deletar");
    add_language_string(0, "available_themes", "Temas disponíveis:");
    add_language_string(0, "dark_theme", "Escuro (Dark)");
    add_language_string(0, "light_theme", "Claro (Light)");
    add_language_string(0, "vaporwave_theme", "Vaporwave (Retro)");
    add_language_string(0, "vintage_theme", "Vintage (Clássico)");
    add_language_string(0, "available_languages", "Idiomas disponíveis:");
    add_language_string(0, "portuguese", "pt (Português)");
    add_language_string(0, "english", "en (English)");
    add_language_string(0, "press_enter_back", "Pressione Enter para voltar...");
    add_language_string(0, "press_any_key_back", "Pressione qualquer tecla para voltar...");
    add_language_string(0, "press_enter_continue", "Pressione Enter para continuar...");
    add_language_string(0, "about_title", "ℹ️  SOBRE O MOHAVIM");
    add_language_string(0, "developed_by", "     Desenvolvido por: Gabriel A. Matos");
    add_language_string(0, "native_language", "C (nativo)");
    add_language_string(0, "setas_atalhos", "Setas + Atalhos");
    add_language_string(0, "open_file_title", "📂 SELECIONAR ARQUIVO PARA ABRIR");
    add_language_string(0, "file_nav_instructions", "Use ↑↓ para navegar, Enter para abrir, D para deletar, ESC para voltar");
    add_language_string(0, "previous_files", "... (%d arquivos anteriores)");
    add_language_string(0, "remaining_files", "... (%d arquivos restantes)");
    add_language_string(0, "file_of", "Arquivo %d de %d");
    add_language_string(0, "filename_prompt", "Nome: ");
    add_language_string(0, "delete_file_title", "🗑️ DELETAR ARQUIVO");
    add_language_string(0, "confirm_delete", "Tem certeza que deseja deletar o arquivo:");
    add_language_string(0, "cannot_undo", "Esta ação NÃO PODE SER DESFEITA!");
    add_language_string(0, "confirm_prompt", "Digite 'SIM' para confirmar ou qualquer tecla para cancelar: ");
    add_language_string(0, "file_deleted", "✅ Arquivo '%s' deletado com sucesso!");
    add_language_string(0, "error_deleting_file", "❌ Erro ao deletar arquivo '%s'!");
    add_language_string(0, "operation_cancelled", "⚠️ Operação cancelada.");
    add_language_string(0, "search_file_title", "🔍 Buscar arquivo");
    add_language_string(0, "search_text_prompt", "Insira o texto: ");
    add_language_string(0, "search_results", "🔍 Resultados em '%s':");
    add_language_string(0, "text_not_found", "❌ não encontrado");
    add_language_string(0, "occurrences_found", "✅ %d ocorrência(s) encontrada(s).");
    add_language_string(0, "error_opening_file", "❌ Erro ao abrir arquivo '%s'");
    add_language_string(0, "filename_empty", "⚠️ Nome de arquivo não pode ser vazio.");
    add_language_string(0, "search_text_empty", "⚠️ Não encontrado.");
    add_language_string(0, "create_new_file_title", "📝 Criar novo arquivo");
    add_language_string(0, "filename_prompt_new", "Nome do arquivo: ");
    add_language_string(0, "file_modified", "⚠️  Arquivo modificado! Salvar antes de sair? (s/n): ");
    add_language_string(0, "save_before_exit", "⚠️  Arquivo modificado! Salvar antes de sair? (s/n): ");
    add_language_string(0, "exiting", "🚪 Saindo...");
    add_language_string(0, "thanks", "💀 Obrigado por usar o Mohavim!");
    add_language_string(0, "unknown_argument", "Argumento desconhecido: '%s'");
    add_language_string(0, "use_help", "Use --help para ver as opções disponíveis");
    add_language_string(0, "language_not_found", "Idioma '%s' não encontrado");
    add_language_string(0, "theme_not_found", "Tema '%s' não encontrado");
    add_language_string(0, "plugin_installed", "Plugin '%s' instalado com sucesso");
    add_language_string(0, "mohavim_modular_editor", "Mohavim - Editor de texto modular");
    add_language_string(0, "usage", "Uso: mohavim [opções]");
    add_language_string(0, "options", "Opções:");
    add_language_string(0, "help_option", "  --help, -h          Mostrar esta ajuda");
    add_language_string(0, "lang_option", "  --lang <código>     Definir idioma (pt_br, en)");
    add_language_string(0, "theme_option", "  --theme <nome>      Definir tema (dark, light, cyberpunk)");
    add_language_string(0, "verbose_option", "  --verbose, -v       Ativar modo verbose");
    add_language_string(0, "install_plugin_option", "  --install-plugin <nome>  Instalar plugin");
    add_language_string(0, "list_plugins_option", "  --list-plugins      Listar plugins");
    add_language_string(0, "examples", "Exemplos:");
    add_language_string(0, "language_config", "Configuração de idioma:");
    add_language_string(0, "language_config_desc", "  O idioma pode ser definido através de:");
    add_language_string(0, "env_var", "  1. Variável de ambiente: LANGUAGE=en ou LANG=en_US.UTF-8");
    add_language_string(0, "config_file", "  2. Arquivo de configuração: ~/.mohavimrc com 'language=en'");
    add_language_string(0, "cmd_line", "  3. Argumento de linha de comando: --lang en");
    
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
    add_language_string(1, "controls", "Ctrl+S:Save  Ctrl+Q:Quit  ESC:Menu  Ctrl+C:Copy  Ctrl+V:Paste  Ctrl+Z:Undo  ↑↓←→:Navigate");
    add_language_string(1, "line", "Line:");
    add_language_string(1, "column", "Column:");
    add_language_string(1, "total", "Total:");
    add_language_string(1, "chars", "chars");
    add_language_string(1, "logs", "📋 Logs");
    add_language_string(1, "themes", "🎨 Themes");
    add_language_string(1, "plugins", "🔌 Plugins");
    add_language_string(1, "settings", "⚙️ Settings");
    add_language_string(1, "languages", "🌐 Languages");
    add_language_string(1, "version", "🔧 Version: ");
    add_language_string(1, "language", "🏠 Language: ");
    add_language_string(1, "navigation", "⌨️  Navigation: ");
    add_language_string(1, "available_shortcuts", "Available shortcuts:");
    add_language_string(1, "navigation_keys", "- ↑↓←→: Navigation");
    add_language_string(1, "save_shortcut", "- Ctrl+S: Save file");
    add_language_string(1, "exit_shortcut", "- Ctrl+Q: Exit editor");
    add_language_string(1, "menu_shortcut", "- ESC: Back to menu");
    add_language_string(1, "newline_shortcut", "- Enter: New line");
    add_language_string(1, "delete_shortcut", "- Backspace: Delete");
    add_language_string(1, "available_themes", "Available themes:");
    add_language_string(1, "dark_theme", "Dark");
    add_language_string(1, "light_theme", "Light");
    add_language_string(1, "vaporwave_theme", "Vaporwave (Retro)");
    add_language_string(1, "vintage_theme", "Vintage (Classic)");
    add_language_string(1, "available_languages", "Available languages:");
    add_language_string(1, "portuguese", "pt (Portuguese)");
    add_language_string(1, "english", "en (English)");
    add_language_string(1, "press_enter_back", "Press Enter to go back...");
    add_language_string(1, "press_any_key_back", "Press any key to go back...");
    add_language_string(1, "press_enter_continue", "Press Enter to continue...");
    add_language_string(1, "about_title", "ℹ️  ABOUT MOHAVIM");
    add_language_string(1, "developed_by", "     Developed by: Gabriel A. Matos");
    add_language_string(1, "native_language", "C (native)");
    add_language_string(1, "setas_atalhos", "Arrows + Shortcuts");
    add_language_string(1, "open_file_title", "📂 SELECT FILE TO OPEN");
    add_language_string(1, "file_nav_instructions", "Use ↑↓ to navigate, Enter to open, D to delete, ESC to go back");
    add_language_string(1, "previous_files", "... (%d previous files)");
    add_language_string(1, "remaining_files", "... (%d remaining files)");
    add_language_string(1, "file_of", "Archive %d of %d");
    add_language_string(1, "filename_prompt", "Name: ");
    add_language_string(1, "delete_file_title", "🗑️ DELETE FILE");
    add_language_string(1, "confirm_delete", "Are you sure you want to delete the file:");
    add_language_string(1, "cannot_undo", "This action CANNOT BE UNDONE!");
    add_language_string(1, "confirm_prompt", "Type 'YES' to confirm or any key to cancel: ");
    add_language_string(1, "file_deleted", "✅ File '%s' deleted successfully!");
    add_language_string(1, "error_deleting_file", "❌ Error deleting file '%s'!");
    add_language_string(1, "operation_cancelled", "⚠️ Operation cancelled.");
    add_language_string(1, "search_file_title", "🔍 Search file");
    add_language_string(1, "search_text_prompt", "Enter text: ");
    add_language_string(1, "search_results", "🔍 Results in '%s':");
    add_language_string(1, "text_not_found", "❌ Not found");
    add_language_string(1, "occurrences_found", "✅ %d occurrence(s) found.");
    add_language_string(1, "error_opening_file", "❌ Error opening file '%s'");
    add_language_string(1, "filename_empty", "⚠️ Filename cannot be empty.");
    add_language_string(1, "search_text_empty", "⚠️ Not found.");
    add_language_string(1, "create_new_file_title", "📝 Create new file");
    add_language_string(1, "filename_prompt_new", "Archive name: ");
    add_language_string(1, "file_modified", "⚠️  File modified! Save before exit? (y/n): ");
    add_language_string(1, "save_before_exit", "⚠️  File modified! Save before exit? (y/n): ");
    add_language_string(1, "exiting", "🚪 Exiting...");
    add_language_string(1, "thanks", "💀 Thanks for using Mohavim!");
    add_language_string(1, "unknown_argument", "Unknown argument: %s");
    add_language_string(1, "use_help", "Use --help to see available options");
    add_language_string(1, "language_not_found", "Language '%s' not found");
    add_language_string(1, "theme_not_found", "Theme '%s' not found");
    add_language_string(1, "plugin_installed", "Plugin '%s' installed successfully");
    add_language_string(1, "mohavim_modular_editor", "Mohavim - Modular text editor");
    add_language_string(1, "usage", "Usage: mohavim [options]");
    add_language_string(1, "options", "Options:");
    add_language_string(1, "help_option", "  --help, -h          Show this help");
    add_language_string(1, "lang_option", "  --lang <code>       Set language (pt_br, en)");
    add_language_string(1, "theme_option", "  --theme <name>      Set theme (dark, light, cyberpunk)");
    add_language_string(1, "verbose_option", "  --verbose, -v       Enable verbose mode");
    add_language_string(1, "install_plugin_option", "  --install-plugin <name>  Install plugin");
    add_language_string(1, "list_plugins_option", "  --list-plugins      List plugins");
    add_language_string(1, "examples", "Examples:");
    add_language_string(1, "language_config", "Language configuration:");
    add_language_string(1, "language_config_desc", "  Language can be set through:");
    add_language_string(1, "env_var", "  1. Environment variable: LANGUAGE=en or LANG=en_US.UTF-8");
    add_language_string(1, "config_file", "  2. Configuration file: ~/.mohavimrc with 'language=en'");
    add_language_string(1, "cmd_line", "  3. Command line argument: --lang en");
    add_language_string(1, "plugin_system_started", "Plugin system started");
    add_language_string(0, "plugin_system_started", "Sistema de plugins iniciado");
    add_language_string(1, "max_plugins_reached", "Maximum plugin limit reached");
    add_language_string(0, "max_plugins_reached", "Limite máximo de plugins atingido");
    add_language_string(1, "plugin_already_loaded", "Plugin '%s' is already loaded");
    add_language_string(0, "plugin_already_loaded", "Plugin '%s' já está carregado");
    add_language_string(1, "error_loading_plugin", "Error loading plugin '%s': %s");
    add_language_string(0, "error_loading_plugin", "Erro ao carregar plugin '%s': %s");
    add_language_string(1, "plugin_no_init_func", "Plugin '%s' has no plugin_init function");
    add_language_string(0, "plugin_no_init_func", "Plugin '%s' sem função plugin_init");
    add_language_string(1, "plugin_loaded_success", "Plugin '%s' loaded successfully");
    add_language_string(0, "plugin_loaded_success", "Plugin '%s' carregado com sucesso");
    add_language_string(1, "plugin_unloaded", "Plugin '%s' unloaded");
    add_language_string(0, "plugin_unloaded", "Plugin '%s' descarregado");
    add_language_string(1, "plugin_not_found", "Plugin '%s' not found");
    add_language_string(0, "plugin_not_found", "Plugin '%s' não encontrado");
    add_language_string(1, "loaded_plugins", "Loaded plugins");
    add_language_string(0, "loaded_plugins", "Plugins carregados");
    add_language_string(1, "available_plugins", "Available plugins:");
    add_language_string(0, "available_plugins", "Plugins disponíveis:");
    add_language_string(1, "load_plugin_option", "Load Plugin");
    add_language_string(0, "load_plugin_option", "Carregar Plugin");
    add_language_string(1, "list_plugins_option", "List Plugins");
    add_language_string(0, "list_plugins_option", "Listar Plugins");
    add_language_string(1, "unload_plugin_option", "Unload Plugin");
    add_language_string(0, "unload_plugin_option", "Descarregar Plugin");
    add_language_string(1, "run_plugin_option", "Run Plugin");
    add_language_string(0, "run_plugin_option", "Executar Plugin");
    
    // Log translations
    add_language_string(1, "log_system_started", "Log system started");
    add_language_string(0, "log_system_started", "Sistema de logs iniciado");
    add_language_string(1, "verbose_mode_status", "Verbose mode %s");
    add_language_string(0, "verbose_mode_status", "Modo verbose %s");
    add_language_string(1, "no_logs_available", "No logs available.");
    add_language_string(0, "no_logs_available", "Nenhum log disponível.");
    add_language_string(1, "log_info", "INFO");
    add_language_string(0, "log_info", "INFO");
    add_language_string(1, "log_warn", "WARN");
    add_language_string(0, "log_warn", "WARN");
    add_language_string(1, "log_error", "ERROR");
    add_language_string(0, "log_error", "ERROR");
    add_language_string(1, "log_debug", "DEBUG");
    add_language_string(0, "log_debug", "DEBUG");
    
    // File and editor translations
    add_language_string(1, "files_loaded_in_dir", "Files loaded in directory");
    add_language_string(0, "files_loaded_in_dir", "%d arquivos carregados no diretório");
    add_language_string(1, "error_opening_dir", "Error opening current directory");
    add_language_string(0, "error_opening_dir", "Erro ao abrir diretório atual");
    add_language_string(1, "starting_file_search", "Starting file search");
    add_language_string(0, "starting_file_search", "Iniciando busca de arquivo");
    add_language_string(1, "empty_filename_for_search", "Empty filename provided for search");
    add_language_string(0, "empty_filename_for_search", "Nome de arquivo vazio fornecido para busca");
    add_language_string(1, "empty_search_text", "Empty search text provided");
    add_language_string(0, "empty_search_text", "Texto de busca vazio fornecido");
    add_language_string(1, "searching_for_in_file", "Searching for '%s' in '%s'");
    add_language_string(0, "searching_for_in_file", "Buscando por '%s' em '%s'");
    add_language_string(1, "text_not_found_in_file", "Text '%s' not found in '%s'");
    add_language_string(0, "text_not_found_in_file", "Texto '%s' não encontrado em '%s'");
    add_language_string(1, "occurrences_found_in_file", "%d occurrence(s) of '%s' found in '%s'");
    add_language_string(0, "occurrences_found_in_file", "%d ocorrência(s) de '%s' encontradas em '%s'");
    add_language_string(1, "error_opening_file_for_search", "Error opening file '%s' for search");
    add_language_string(0, "error_opening_file_for_search", "Erro ao abrir arquivo '%s' para busca");
    add_language_string(1, "file_loaded", "File '%s' loaded (%zu bytes)");
    add_language_string(0, "file_loaded", "Arquivo '%s' carregado (%zu bytes)");
    add_language_string(1, "new_file_created", "New file '%s' created");
    add_language_string(0, "new_file_created", "Novo arquivo '%s' criado");
    add_language_string(1, "file_saved_success", "File '%s' saved successfully");
    add_language_string(0, "file_saved_success", "Arquivo '%s' salvo com sucesso");
    add_language_string(1, "error_saving_file", "Error saving file '%s'");
    add_language_string(0, "error_saving_file", "Erro ao salvar arquivo '%s'");
    add_language_string(1, "file_deleted_success", "File '%s' deleted successfully");
    add_language_string(0, "file_deleted_success", "Arquivo '%s' deletado com sucesso");
    add_language_string(1, "file_delete_cancelled", "File deletion cancelled");
    add_language_string(0, "file_delete_cancelled", "Operação de deleção de arquivo cancelada");
    add_language_string(1, "error_deleting_file_log", "Error deleting file '%s'");
    add_language_string(0, "error_deleting_file_log", "Erro ao deletar arquivo '%s'");
    
    language_count = 2;
}

// Function to automatically detect and set language
void auto_detect_language() {
    // 1. Check command line (this is handled in main.c)
    // 2. Check environment variable
    const char* env_lang = get_language_from_env();
    if (env_lang != NULL && load_language(env_lang)) {
        return;
    }
    
    // 3. Check config file
    const char* config_lang = get_language_from_config();
    if (config_lang != NULL && load_language(config_lang)) {
        return;
    }
    
    // 4. Default to Portuguese (already set as default)
}

void add_language_string(int lang_index, const char *key, const char *value) {
    if (lang_index >= 0 && lang_index < MAX_LANGUAGES && 
        languages[lang_index].string_count < MAX_STRINGS) {
        
        strcpy(languages[lang_index].strings[languages[lang_index].string_count].key, key);
        strcpy(languages[lang_index].strings[languages[lang_index].string_count].value, value);
        languages[lang_index].string_count++;
    }
}

int load_language(const char *code) {
    // Handle NULL input
    if (code == NULL) {
        return 0;
    }
    
    // Try exact match first
    for (int i = 0; i < language_count; i++) {
        if (strcmp(languages[i].code, code) == 0) {
            current_language = i;
            return 1;
        }
    }
    
    // Try partial match (e.g., "en_US" should match "en", "pt_BR.UTF-8" should match "pt_br")
    for (int i = 0; i < language_count; i++) {
        // Compare only the first part of the code
        if (strlen(code) >= 2 && strlen(languages[i].code) >= 2) {
            // Convert code to lowercase for comparison
            char code_lower[32];
            strncpy(code_lower, code, sizeof(code_lower) - 1);
            code_lower[sizeof(code_lower) - 1] = '\0';
            
            for (int j = 0; code_lower[j]; j++) {
                code_lower[j] = tolower(code_lower[j]);
            }
            
            // Remove any charset info after dot (e.g., "pt_BR.UTF-8" -> "pt_br")
            char *dot = strchr(code_lower, '.');
            if (dot) {
                *dot = '\0';
            }
            
            // Replace underscores with hyphens to normalize (e.g., "pt_br" <-> "pt-br")
            for (int j = 0; code_lower[j]; j++) {
                if (code_lower[j] == '_') {
                    code_lower[j] = '-';
                }
            }
            
            // Also normalize the language code for comparison
            char lang_code_lower[32];
            strncpy(lang_code_lower, languages[i].code, sizeof(lang_code_lower) - 1);
            lang_code_lower[sizeof(lang_code_lower) - 1] = '\0';
            
            for (int j = 0; lang_code_lower[j]; j++) {
                lang_code_lower[j] = tolower(lang_code_lower[j]);
            }
            
            for (int j = 0; lang_code_lower[j]; j++) {
                if (lang_code_lower[j] == '_') {
                    lang_code_lower[j] = '-';
                }
            }
            
            // Now compare the first 2 characters or the full normalized strings
            if (strncmp(lang_code_lower, code_lower, 2) == 0 || 
                strcmp(lang_code_lower, code_lower) == 0) {
                current_language = i;
                return 1;
            }
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

// Function to verify language consistency and report any issues
void verify_language_consistency() {
    // Ensure current_language is within valid bounds
    if (current_language < 0 || current_language >= language_count) {
        current_language = 0; // Default to first language (Portuguese)
        log_message(LOG_WARNING, "Idioma inválido, revertendo para padrão: %s", languages[0].code);
    }
    
    // Verify that all required strings are present
    const char* required_strings[] = {
        "open_file", "new_file", "search_file", "about", "exit",
        "navigate", "editor_title", "modified", "saved", "controls",
        "line", "column", "total", "chars", "logs", "themes", "plugins",
        "languages", "version", "language", "navigation", "available_shortcuts",
        "navigation_keys", "save_shortcut", "exit_shortcut", "menu_shortcut",
        "newline_shortcut", "delete_shortcut", "available_themes", "dark_theme",
        "light_theme", "cyberpunk_theme", "vaporwave_theme", "vintage_theme",
        "available_languages", "portuguese", "english", "press_enter_back",
        "press_any_key_back", "press_enter_continue", "about_title", "developed_by",
        "native_language", "setas_atalhos"
    };
    
    int missing_strings = 0;
    for (size_t i = 0; i < sizeof(required_strings) / sizeof(required_strings[0]); i++) {
        const char* value = get_string(required_strings[i]);
        if (value == required_strings[i]) { // If key is returned, string is missing
            log_message(LOG_WARNING, "String ausente para o idioma %s: %s", 
                       languages[current_language].code, required_strings[i]);
            missing_strings++;
        }
    }
    
    if (missing_strings > 0) {
        log_message(LOG_WARNING, "Total de strings ausentes: %d", missing_strings);
    } else {
        log_message(LOG_INFO, "Verificação de consistência de idioma concluída com sucesso");
    }
}