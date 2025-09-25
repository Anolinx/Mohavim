
# 📂 Código Fonte do Mohavim 7.3

# Criado por: Gabriel A. Matos

## Nova Arquitetura Modular Expandida

### 🔥 **Estrutura Modular**
- Sistema completo de internacionalização (i18n)
- Sistema de temas customizáveis
- Sistema de logs interno
- Sistema de plugins dinâmicos
- Arquitetura limpa e manutenível
- Modo verbose para debug
- Arquitetura modular para extensibilidade
- Localização: `./src/`

## 🏗️ Estrutura do Projeto 

O projeto `src/` contém:
- `main.c` - Programa principal com argumentos CLI
- `editor.c/.h` - Motor do editor com logs integrados
- `ui.c/.h` - Interface multilíngue e sistema de temas
- `files.c/.h` - Gerenciamento e navegação de arquivos
- `i18n.c/.h` - Sistema de internacionalização
- `theme.c/.h` - Sistema de temas
- `log.c/.h` - Sistema de logs interno
- `plugin_manager.c/.h` - Gerenciamento de plugins
- `Makefile` - Sistema de compilação otimizado
  

## 🗂️ Detalhamento dos Módulos

### 📄 **main.c** 
- Programa principal com suporte a argumentos CLI
- `--lang`: Seleção de idioma (pt, en)
- `--theme`: Seleção de tema (dark, light, cyberpunk)
- `--install-plugin`: Instalação de plugins
- Inicialização de todos os sistemas (i18n, temas, logs, plugins)
- Loop de menu com interface multilíngue

### ✏️ **editor.c/.h** (Expandido)
- Motor principal do editor com sistema de logs
- Buffer otimizado com 10.000 caracteres
- Navegação avançada com cursor visual
- Integração com sistema de temas
- Logs de operações de edição
- Suporte a plugins para extensões
- Teclas especiais: L (logs), T (temas), I (idiomas)
- Sistema de callbacks para plugins

### 🖥️ **ui.c/.h** (Multilíngue)
- Interface completamente internacionalizada
- Sistema de temas com mudança dinâmica
- Menus para configuração de idiomas e temas
- Logo ASCII com efeitos visuais
- Sistema de menus navegáveis multilíngue
- Integração com sistema de logs
- Menu de gerenciamento de plugins
- Configuração de terminal otimizada

### 📁 **files.c/.h** 
- Gerenciamento completo de arquivos
- Logs detalhados de operações
- Sistema de segurança melhorado
- Integração com sistema de temas
- Mensagens internacionalizadas
- Buffer circular para logs de operações

### 🌐 **i18n.c/.h** 
- Sistema completo de internacionalização
- Carregamento dinâmico de traduções JSON
- Função `get_string(key)` para obter traduções
- Suporte a fallback para idioma padrão
- API extensível para novos idiomas
- Cache de strings para performance

### 🎨 **theme.c/.h** 
- Sistema de temas customizáveis
- Carregamento de configurações JSON
- Esquemas de cores: dark, light, cyberpunk
- API para mudança dinâmica de temas
- Integração com todos os módulos
- Sistema de fallback para tema padrão

### 📋 **log.c/.h** 
- Sistema de logs interno robusto
- Buffer circular para eficiência de memória
- Níveis de log: ✅ Info, ⚠️ Warn, ❌ Error, 🐛 Debug
- Timestamps automáticos
- Modo verbose para debug
- API completa para logging
- Visualização integrada no editor

### 🔌 **plugin_manager.c/.h** 
- Sistema de plugins dinâmicos
- Carregamento via `dlopen`
- API padronizada para plugins
- Gerenciamento de ciclo de vida
- Interface de instalação e desinstalação
- Sistema de dependências
- Sandbox de segurança para plugins

## ✅ Features Implementadas v7.3

### 🌐 Internacionalização
- [x] Sistema i18n completo
- [x] Português e Inglês incluídos
- [x] Carregamento dinâmico de traduções
- [x] API extensível para novos idiomas
- [x] Argumento `--lang` para seleção

### 🎨 Sistema de Temas
- [x] Temas: dark, light, cyberpunk
- [x] Carregamento de configurações JSON
- [x] Mudança dinâmica de cores
- [x] Argumento `--theme` para seleção
- [x] Menu integrado no editor (tecla T)

### 📋 Sistema de Logs
- [x] Buffer circular otimizado
- [x] Múltiplos níveis de log
- [x] Timestamps automáticos
- [x] Visualização no editor (tecla L)

### 🔌 Sistema de Plugins
- [x] Carregamento dinâmico (.so)
- [x] API padronizada
- [x] Gerenciamento integrado
- [x] Instalação via `--install-plugin`
- [x] Menu de gerenciamento (tecla P)

### 🔧 Melhorias Gerais
- [x] Argumentos de linha de comando
- [x] Arquitetura modular expandida
- [x] Interface moderna multilíngue
- [x] Sistema de configuração
- [x] Debug avançado
- [x] Performance otimizada

## 🔧 Compilação e Uso
### Compilação Manual:
```bash
cd src/
make clean
make
```

### Instalação Automática:
```bash
chmod +x install.sh
./install.sh
```

### Execução:
```bash
mohavim
```

### Uso com Argumentos:
```bash
# Idioma e tema específicos
mohavim --lang en --theme cyberpunk

# Instalar plugin
mohavim --install-plugin exemplo

# Combinação de argumentos
mohavim --lang pt --theme dark
```

### Arquivos Gerados:
- `*.o` - Arquivos objeto de todos os módulos
- `mohavim` - Executável final

## 🌐 Sistema de Traduções

### Estrutura JSON:
```json
{
  "menu_title": "MENU PRINCIPAL",
  "open_file": "Abrir Arquivo",
  "new_file": "Novo Arquivo",
  "search": "Buscar",
  "about": "Sobre",
  "exit": "Sair",
  "logs": "Logs do Sistema",
  "themes": "Temas",
  "plugins": "Plugins"
}
```

### Adicionando Idiomas:
1. Criar `translations/codigo.json`
2. Traduzir todas as chaves necessárias
3. Testar com `./mohavim --lang codigo`

## 🎨 Sistema de Temas

### Estrutura JSON:
```json
{
  "themes": [
    {
      "name": "dark",
      "colors": {
        "background": "\033[40m",
        "text": "\033[37m",
        "accent": "\033[1;36m",
        "error": "\033[1;31m",
        "success": "\033[1;32m"
      }
    }
  ]
}
```

## 🔌 API de Plugins

### Estrutura Mínima:
```c
#include <stdio.h>

typedef struct {
    char name[64];
    char version[16];
    void (*init)(void);
    void (*cleanup)(void);
} plugin_info_t;

static plugin_info_t plugin_info = {
    .name = "ExemploPlugin",
    .version = "1.0",
    .init = plugin_init_func,
    .cleanup = plugin_cleanup_func
};

plugin_info_t* plugin_init() {
    printf("Plugin %s v%s carregado!\n", 
           plugin_info.name, plugin_info.version);
    return &plugin_info;
}

void plugin_init_func(void) {
    // Inicialização do plugin
}

void plugin_cleanup_func(void) {
    // Limpeza do plugin
}
```

### Compilação de Plugin:
```bash
gcc -shared -fPIC -o exemplo.so plugin.c
cp exemplo.so plugins/
mohavim --install-plugin exemplo
```

## 📋 Sistema de Logs

### Níveis Disponíveis:
- `INFO`: ✅ Info - Informações gerais
- `WARN`: ⚠️ Warn - Avisos não críticos
- `ERROR`: ❌ Error - Erros críticos
- `DEBUG`: 🐛 Debug - Debug detalhado (verbose)

### API de Logging:
```c
log_message(INFO, "Arquivo salvo: %s", filename);
log_message(ERROR, "Erro ao abrir: %s", error);
```

## 🚀 Roadmap Futuro

### v8.1 Planejado:
- [ ] Configuração ~mohavimrc
- [ ] Mais temas de cores
- [ ] Plugin de syntax highlighting
- [ ] Sistema de macros
- [ ] Integração com Git

### v7.4 Visão:
- [ ] Interface gráfica opcional
- [ ] LSP (Language Server Protocol)
- [ ] Sistema de abas
- [ ] Terminal integrado
- [ ] Gerenciador de projetos

---

**Mohavim 7.3** representa uma evolução significativa do editor, mantendo a simplicidade original enquanto adiciona funcionalidades profissionais para desenvolvedores modernos.
