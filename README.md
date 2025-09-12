
                 ⚡ Mohavim ⚡
         criado por: Gabriel A. Matos
         Email: gabriellramone@gmail.com

> Editor de texto com interface moderna, com sistema completo de internacionalização e temas

> Navegação intuitiva e recursos avançados

> Arquitetura modular para melhor manutenibilidade, com sistema de plugins e logs

> Interface multilíngue e customizável para máxima produtividade

✨ Features

🌐 **Sistema de Internacionalização (i18n)**
  - Suporte completo a múltiplos idiomas
  - Português e Inglês incluídos
  - Fácil adição de novos idiomas

📂 **Seleção de arquivos no diretório atual**

   - Os arquivos de texto recém-criados serão armazenados no diretório atual ao executar o Mohavim.
   - A leitura e edição dos arquivos de texto funcionam da mesma maneira.
  
🎨 **Sistema de Temas**
  - Temas escuro, claro e cyberpunk
  - Esquemas de cores personalizáveis
  - Mudança dinâmica de tema

📋 **Sistema de Logs Interno**
  - Logs detalhados de operações
  - Buffer circular para mensagens
  - Visualização dentro do editor (tecla L)

🔧 **Modo Verbose**
  - Logs detalhados opcionais
  - Ativação via --verbose
  - Debug avançado do sistema

🔌 **Sistema de Plugins**
  - Carregamento dinâmico de plugins (.so)
  - API para extensões de terceiros
  - Gerenciamento integrado de plugins

🎨 Interface moderna com cores e elementos visuais limpos
🧭 Navegação intuitiva com setas direcionais
📂 Seleção de arquivos no diretório atual
🗑️ Exclusão de arquivos com confirmação de segurança
📝 Editor com numeração de linhas e cursor visual
🔍 Sistema de busca em arquivos
⚡ Performance otimizada em C
🗂️ Arquitetura modular para melhor organização

🚀 Como usar

### Argumentos de Linha de Comando

```bash
# Executar com idioma específico
  mohavim --lang pt          # Português
  mohavim --lang en          # English

# Executar com tema específico
  mohavim --theme dark       # Tema escuro
  mohavim --theme light      # Tema claro
  mohavim --theme cyberpunk  # Tema cyberpunk

# Modo verbose para debug
  mohavim --verbose

# Instalar plugin
  mohavim --install-plugin nome_do_plugin

# Combinar argumentos
  mohavim --lang en --theme cyberpunk --verbose
```

### Execução CLI

❯ pwd → /home/usuario/Documents/

❯ mohavim

❯ Criar Novo Arquivo → teste.txt

❯ Digitar conteúdo

❯ Ctrl+S para salvar

❯ Ctrl+Q para sair

Assim, o arquivo teste.txt será salvo em: /home/usuario/Documents/teste.txt.

🚀 Instalação

### Instalação Automática (Recomendada)
```bash
# Clone o repositório
git clone https://github.com/Anolinx/Mohavim.git
cd Mohavim/
# Torne o script executável
chmod +x install.sh
# Execute a instalação
./install.sh
```

### Compilação Manual
```bash
# Entre na pasta do código fonte
cd src/
# Limpe compilação anterior
make clean
# Compile o projeto
make
# Execute o editor
./mohavim
```

📦 Dependências

### Para execução:
- GCC (compilador C)
- Make (sistema de build)
- Bibliotecas padrão (termios, dirent)
- libdl (para sistema de plugins)

### Para desenvolvimento:
- GCC com suporte a C99+
- GNU Make 3.81+
- Sistema Unix/Linux (testado em Ubuntu, Arch, etc.)

🎮 Controles

### Navegador de Arquivos:
- ↑↓ : navegar
- Enter : abrir arquivo
- D : excluir (confirmação SIM)
- ESC : voltar

### Editor de Texto:
- ↑↓←→ : navegação
- Ctrl+S : salvar
- Ctrl+Q : sair
- ESC : voltar
- Enter : nova linha
- Backspace : apagar
- L : visualizar logs

📂 Estrutura

```
src/
├── main.c           → programa principal com argumentos CLI
├── editor.c/.h      → funcionalidades do editor
├── ui.c/.h          → interface e controles
├── files.c/.h       → gerenciamento de arquivos
├── i18n.c/.h        → sistema de internacionalização
├── theme.c/.h       → sistema de temas
├── log.c/.h         → sistema de logs
├── plugin_manager.c/.h → gerenciamento de plugins
├── Makefile         → sistema de compilação
└── mohavim          → executável compilado

plugins/            → plugins de terceiros
└── exemplo.so      → plugin exemplo

README.md           → documentação
CODIGO_FONTE.md     → documentação técnica
LICENSE             → licença
install.sh          → script de instalação
```

🌐 Internacionalização

O Mohavim suporta múltiplos idiomas através do sistema i18n:

- **Português (pt_br)**: Idioma padrão
- **English (en)**: Tradução completa
- **Extensível**: Fácil adição de novos idiomas

### Configurando o idioma:

Você pode configurar o idioma de várias maneiras:

1. **Variável de ambiente**:
   ```bash
   export LANGUAGE=en
   mohavim
   ```

2. **Arquivo de configuração**:
   Crie um arquivo `.mohavimrc` no seu diretório home:
   ```bash
   echo "language=en" > ~/.mohavimrc
   ```

3. **Linha de comando**:
   ```bash
   mohavim --lang en
   ```

Ordem de prioridade: linha de comando > variável de ambiente > arquivo de configuração > padrão (português)

### Adicionando novos idiomas:
1. Edite `src/i18n.c`
2. Adicione uma nova seção de idioma na função `init_i18n()`
3. Use um código de idioma único
4. Traduza todas as strings
5. Recompile o Mohavim

🎨 Sistema de Temas

Três temas incluídos:

- **Dark**: Tema escuro padrão
- **Light**: Tema claro
- **Cyberpunk**: Tema futurista com cores vibrantes
- **Vaporwave**: Tema com cores vibrantes

A interface web também suporta a seleção de temas através de um seletor na página principal.

### Criando novos temas:
1. Edite `themes.json`
2. Adicione nova configuração de cores
3. Use `mohavim --theme nome_tema` ou selecione no seletor da interface web

🔌 Sistema de Plugins

O Mohavim suporta plugins dinâmicos em C:

```c
// Plugin exemplo
typedef struct {
    char name[64];
    char version[16];
    void (*init)(void);
    void (*cleanup)(void);
} plugin_info_t;

plugin_info_t* plugin_init() {
    // Implementação do plugin
    return &plugin_info;
}
```

### Instalando plugins:
```bash
  mohavim --install-plugin nome_plugin
```

📋 Sistema de Logs

Logs detalhados de todas as operações:

- **Buffer circular**: Armazena últimas mensagens
- **Visualização interna**: Tecla L no editor
- **Modo verbose**: Logs detalhados com --verbose
- **Categorias**: Info, Warning, Error, Debug

🔧 Modo Verbose

Ative logs detalhados para desenvolvimento:

```bash
  mohavim --verbose
```

Mostra:
- Carregamento de módulos
- Operações de arquivo
- Estados do editor
- Erros detalhados

Uso ⌨️

Execute `mohavim`

Use argumentos para personalizar: `mohavim --lang en --theme cyberpunk`

Use as setas ↑/↓ para navegar no menu

Pressione Enter para selecionar opções

No seletor de arquivos: Pressione D para excluir arquivo (requer confirmação digitando SIM)

No editor: 
- Ctrl+S para salvar
- Ctrl+Q para sair  
- ESC para voltar ao menu
- L para visualizar logs
- T para menu de temas
- I para menu de idiomas

Contribuição

Quer contribuir com o Mohavim?  

Me contate em: gabriellramone@gmail.com

O projeto aceita:
- Traduções para novos idiomas
- Novos temas de cores
- Plugins de terceiros
- Melhorias no código

Mohavim é distribuído sob a licença GNU GPLv3.  

Consulte o arquivo LICENSE para mais detalhes.

🛰️ Versão:

**Mohavim 7.3** 🔥

Changelog v7.3:
- ✨ Sistema de internacionalização (i18n)
- 🎨 Sistema de temas customizáveis
- 📋 Sistema de logs interno
- 🔧 Modo verbose para debug
- 🔌 Sistema de plugins dinâmicos
- 🌐 Interface multilíngue
- ⚡ Arquitetura modular expandida
