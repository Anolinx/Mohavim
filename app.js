// Variáveis globais
let currentFile = 'sem_titulo.txt';
let isModified = false;
let currentTheme = 'dark'; // Tema padrão
let currentLanguage = 'pt_br'; // Idioma padrão
let themes = {}; // Objeto para armazenar os temas

// Strings de tradução
const translations = {
    'pt_br': {
        'theme': 'Tema',
        'language': 'Idioma',
        'new_file': 'Novo Arquivo',
        'save': 'Salvar',
        'load_file': 'Carregar Arquivo',
        'modified': 'Modificado',
        'saved': 'Salvo',
        'line': 'Linha',
        'column': 'Coluna',
        'total': 'Total',
        'chars': 'chars'
    },
    'en': {
        'theme': 'Theme',
        'language': 'Language',
        'new_file': 'New File',
        'save': 'Save',
        'load_file': 'Load File',
        'modified': 'Modified',
        'saved': 'Saved',
        'line': 'Line',
        'column': 'Column',
        'total': 'Total',
        'chars': 'chars'
    }
};

// Inicialização quando a página carrega
document.addEventListener('DOMContentLoaded', function() {
    loadThemes().then(() => {
        loadSavedSettings(); // Carregar tema e idioma salvos
        applyTheme(currentTheme);
        applyLanguage(); // Aplicar idioma
        initTabs();
        initEditor();
        initFileManager();
        initThemeSelector(); // Inicializar seletor de temas
        initLanguageSelector(); // Inicializar seletor de idiomas
    });
});

// Aplicar tema quando a aba do editor for selecionada
document.addEventListener('click', function(e) {
    if (e.target.classList.contains('tab-button') && e.target.getAttribute('data-tab') === 'editor') {
        // Pequeno atraso para garantir que a aba foi carregada
        setTimeout(() => {
            applyEditorTheme();
        }, 100);
    }
});

// Sistema de abas
function initTabs() {
    const tabButtons = document.querySelectorAll('.tab-button');
    const tabPanes = document.querySelectorAll('.tab-pane');

    tabButtons.forEach(button => {
        button.addEventListener('click', () => {
            const targetTab = button.getAttribute('data-tab');
            
            // Remove classe active de todas as abas
            tabButtons.forEach(btn => btn.classList.remove('active'));
            tabPanes.forEach(pane => pane.classList.remove('active'));
            
            // Adiciona classe active à aba clicada
            button.classList.add('active');
            document.getElementById(targetTab).classList.add('active');
        });
    });
}

// Sistema do editor
function initEditor() {
    const textEditor = document.getElementById('text-editor');
    const lineNumbers = document.getElementById('line-numbers');
    const currentLineSpan = document.getElementById('current-line');
    const currentColumnSpan = document.getElementById('current-column');
    const totalCharsSpan = document.getElementById('total-chars');
    const fileStatusSpan = document.getElementById('file-status');
    const currentFileSpan = document.getElementById('current-file');

    // Atualizar números de linha
    function updateLineNumbers() {
        const lines = textEditor.value.split('\n');
        const lineNumbersText = lines.map((_, index) => index + 1).join('\n');
        lineNumbers.textContent = lineNumbersText;
        return lines.length;
    }

    // Atualizar status do editor
    function updateEditorStatus() {
        const text = textEditor.value;
        const cursorPosition = textEditor.selectionStart;
        const textBeforeCursor = text.substring(0, cursorPosition);
        const lines = textBeforeCursor.split('\n');
        
        const currentLine = lines.length;
        const currentColumn = lines[lines.length - 1].length + 1;
        const totalChars = text.length;

        const t = translations[currentLanguage];
        currentLineSpan.textContent = currentLine;
        currentColumnSpan.textContent = currentColumn;
        totalCharsSpan.textContent = totalChars;

        updateLineNumbers();
    }

    // Marcar como modificado
    function markAsModified() {
        if (!isModified) {
            isModified = true;
            const t = translations[currentLanguage];
            fileStatusSpan.textContent = t.modified;
            fileStatusSpan.className = 'status-modified';
        }
    }

    // Marcar como salvo
    function markAsSaved() {
        isModified = false;
        const t = translations[currentLanguage];
        fileStatusSpan.textContent = t.saved;
        fileStatusSpan.className = 'status-saved';
    }

    // Event listeners do editor
    textEditor.addEventListener('input', () => {
        updateEditorStatus();
        markAsModified();
    });

    textEditor.addEventListener('click', updateEditorStatus);
    textEditor.addEventListener('keyup', updateEditorStatus);

    // Botões da toolbar
    document.getElementById('new-file').addEventListener('click', () => {
        if (isModified) {
            const t = translations[currentLanguage];
            const confirmMessage = currentLanguage === 'pt_br' ? 
                'Arquivo modificado. Deseja salvar antes de criar um novo?' : 
                'File modified. Do you want to save before creating a new one?';
            if (confirm(confirmMessage)) {
                saveFile();
            }
        }
        textEditor.value = '';
        currentFile = 'sem_titulo.txt';
        currentFileSpan.textContent = currentFile;
        markAsSaved();
        updateEditorStatus();
    });

    document.getElementById('save-file').addEventListener('click', saveFile);
    document.getElementById('load-file-btn').addEventListener('click', () => {
        document.getElementById('load-file').click();
    });

    // Carregar arquivo
    document.getElementById('load-file').addEventListener('change', (e) => {
        const file = e.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (e) => {
                textEditor.value = e.target.result;
                currentFile = file.name;
                currentFileSpan.textContent = currentFile;
                markAsSaved();
                updateEditorStatus();
            };
            reader.readAsText(file);
        }
    });

    // Atalhos de teclado
    textEditor.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key === 's') {
            e.preventDefault();
            saveFile();
        }
        if (e.ctrlKey && e.key === 'n') {
            e.preventDefault();
            document.getElementById('new-file').click();
        }
        if (e.ctrlKey && e.key === 'o') {
            e.preventDefault();
            document.getElementById('load-file-btn').click();
        }
        // Adicionar mais atalhos de teclado
        if (e.key === 'l' || e.key === 'L') {
            // Simular funcionalidade de logs
            alert('Visualização de logs não disponível na interface web');
        }
        if (e.key === 't' || e.key === 'T') {
            // Simular funcionalidade de temas
            document.getElementById('theme-select').focus();
        }
        if (e.key === 'i' || e.key === 'I') {
            // Simular funcionalidade de idiomas
            alert('Seleção de idiomas não disponível na interface web');
        }
        if (e.key === 'p' || e.key === 'P') {
            // Simular funcionalidade de plugins
            alert('Gerenciamento de plugins não disponível na interface web');
        }
    });

    // Função para salvar arquivo
    function saveFile() {
        const content = textEditor.value;
        const blob = new Blob([content], { type: 'text/plain' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = currentFile;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
        markAsSaved();
    }

    // Inicializar status
    updateEditorStatus();
    
    // Aplicar tema ao editor
    applyEditorTheme();
    
    // Observar mudanças no tema e aplicar ao editor
    const themeSelect = document.getElementById('theme-select');
    if (themeSelect) {
        themeSelect.addEventListener('change', function() {
            setTimeout(() => {
                applyEditorTheme();
            }, 50);
        });
    }
}

// Função para aplicar tema ao editor
function applyEditorTheme() {
    const theme = themes[currentTheme];
    if (!theme) return;

    const textEditor = document.getElementById('text-editor');
    const lineNumbers = document.getElementById('line-numbers');
    const editorWrapper = document.querySelector('.editor-wrapper');
    const editorContainer = document.querySelector('.editor-container');
    const editorToolbar = document.querySelector('.editor-toolbar');
    const editorStatus = document.querySelector('.editor-status');
    
    if (textEditor && lineNumbers && editorWrapper) {
        // Aplicar cores ao editor
        textEditor.style.color = theme.foreground;
        textEditor.style.background = theme.background;
        lineNumbers.style.background = theme.background;
        lineNumbers.style.color = theme.accent;
        editorWrapper.style.background = theme.background;
        editorWrapper.style.border = `1px solid ${theme.accent}`;
        
        // Aplicar cores aos elementos adicionais do editor
        if (editorContainer) {
            editorContainer.style.background = theme.background;
        }
        
        if (editorToolbar) {
            editorToolbar.style.background = theme.background;
        }
        
        if (editorStatus) {
            editorStatus.style.background = theme.background;
            editorStatus.style.color = theme.foreground;
            editorStatus.style.borderTop = `1px solid ${theme.accent}`;
        }
        
        // Aplicar cores aos botões da toolbar
        const buttons = document.querySelectorAll('.btn');
        buttons.forEach(button => {
            // Determinar tipo de botão para aplicar cor apropriada
            if (button.id === 'save-file') {
                button.style.background = theme.success;
                button.style.color = theme.background;
            } else if (button.id === 'load-file-btn') {
                button.style.background = theme.highlight;
                button.style.color = theme.background;
            } else {
                button.style.background = theme.accent;
                button.style.color = theme.background;
            }
        });
    }
}

// Sistema de visualização de código fonte
function initFileManager() {
    const fileItems = document.querySelectorAll('.file-item');
    const sourceContent = document.getElementById('source-content');

    // Conteúdo simulado dos arquivos
    const fileContents = {
        'src/main.c': `#include "ui.h"
#include "editor.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int opcao_atual = 0;
    mostrar_logo();
    printf("\\nPressione Enter para continuar...");
    getchar();
    configurar_terminal();

    while(1) {
        mostrar_menu(opcao_atual);
        int tecla = ler_tecla();
        switch(tecla) {
            case 65: opcao_atual = (opcao_atual - 1 + 5) % 5; break;
            case 66: opcao_atual = (opcao_atual + 1) % 5; break;
            case 10: restaurar_terminal();
                switch(opcao_atual) {
                    case 0: abrir_arquivo(); break;
                    case 1: novo_arquivo(); break;
                    case 2: buscar_arquivo(); break;
                    case 3: sobre(); break;
                    case 4: 
                        printf("\\033[2J\\033[H🚪 Saindo...\\n💀 Obrigado por usar o Mohavim!\\n");
                        exit(0);
                }
                configurar_terminal();
                break;
            case 27: restaurar_terminal();
                printf("\\033[2J\\033[H🚪 Saindo...\\n💀 Obrigado por usar o Mohavim!\\n");
                exit(0);
        }
    }
    return 0;
}`,
        'src/editor.c': `#include "editor.h"
#include "ui.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

extern struct termios old_termios;

void editor_simples(char* nome_arquivo) {
    char buffer[10000] = "";
    int cursor = 0;
    int modificado = 0;
    
    // Carregar arquivo se existir
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, arquivo);
        buffer[bytes_read] = '\\0';
        fclose(arquivo);
    }
    
    int buffer_len = strlen(buffer);
    configurar_terminal();
    
    while (1) {
        limpar_tela();
        
        // Interface do editor com numeração de linhas
        printf("\\033[1;36m🖊️  MOHAVIM\\033[0m - \\033[1m%s\\033[0m %s\\n", 
               nome_arquivo, 
               modificado ? "\\033[1;33m[MODIFICADO]\\033[0m" : "\\033[1;32m[Salvo]\\033[0m");
        printf("\\033[33mCtrl+S\\033[0m:Salvar  \\033[33mCtrl+Q\\033[0m:Sair  \\033[33mESC\\033[0m:Menu\\n\\n");
        
        // ... resto da implementação do editor
    }
}`,
        'src/ui.c': `#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios old_termios;

void limpar_tela() {
    printf("\\033[2J\\033[H");
}

void configurar_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void mostrar_logo() {
    printf("============================================================\\n");
    printf("▗▖  ▗▖ ▗▄▖ ▗▖ ▗▖ ▗▄▖ ▗▖  ▗▖▗▄▄▄▖▗▖  ▗▖\\n");
    printf("▐▛▚▞▜▌▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌  ▐▌  █  ▐▛▚▞▜▌\\n");
    printf("▐▌  ▐▌▐▌ ▐▌▐▛▀▜▌▐▛▀▜▌▐▌  ▐▌  █  ▐▌  ▐▌\\n");
    printf("▐▌  ▐▌▝▚▄▞▘▐▌ ▐▌▐▌ ▐▌ ▝▚▞▘ ▗▄█▄▖▐▌  ▐▌\\n");
    printf("\\n");
    printf("              💀 MOHAVIM\\n");
    printf("          Desenvolvido por: Gabriel A. Matos\\n");     
    printf("============================================================\\n");
}

void mostrar_menu(int opcao) {
    limpar_tela();
    mostrar_logo();
    printf("\\n               \\033[1;36m🏠 MENU PRINCIPAL\\033[0m\\n\\n");
    
    char* opcoes[] = {
        "📂 Abrir e Editar Arquivo",
        "📝 Criar Novo Arquivo", 
        "🔍 Buscar em Arquivo",
        "ℹ️ Sobre",
        "🚪 Sair do Mohavim"
    };
    
    for (int i = 0; i < 5; i++) {
        if (i == opcao) {
            printf("  \\033[1;33m►\\033[0m \\033[1m%s\\033[0m  \\033[1;33m◄\\033[0m\\n", opcoes[i]);
        } else {
            printf("    %s\\n", opcoes[i]);
        }
    }
    
    printf("\\nNavegue: \\033[33m↑/↓\\033[0m  Selecionar: \\033[33mEnter\\033[0m  Sair: \\033[33mESC\\033[0m\\n");
}`,
        'src/files.c': `#include "files.h"
#include "editor.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

char arquivos[100][256];
int total_arquivos = 0;

void carregar_lista_arquivos() {
    total_arquivos = 0;
    DIR* dir = opendir(".");
    if(dir) {
        struct dirent* entry;
        while((entry = readdir(dir)) && total_arquivos < 99) {
            if(entry->d_type == DT_REG) {
                strcpy(arquivos[total_arquivos], entry->d_name);
                total_arquivos++;
            }
        }
        closedir(dir);
    }
}

void abrir_arquivo() {
    carregar_lista_arquivos();
    
    int opcao_atual = 0;
    int max_opcoes = total_arquivos + 1;
    
    configurar_terminal();
    
    while (1) {
        mostrar_lista_arquivos(opcao_atual);
        
        int tecla = ler_tecla();
        
        switch (tecla) {
            case 65: // Seta cima
                opcao_atual = (opcao_atual - 1 + max_opcoes) % max_opcoes;
                break;
            case 66: // Seta baixo
                opcao_atual = (opcao_atual + 1) % max_opcoes;
                break;
            case 10: // Enter
                restaurar_terminal();
                if (opcao_atual == 0) {
                    // Digitar nome do arquivo
                    printf("\\033[2J\\033[H");
                    printf("\\033[1;36m📝 DIGITE O NOME DO ARQUIVO\\033[0m\\n\\n");
                    printf("Nome: ");
                    
                    char nome[256];
                    if (fgets(nome, sizeof(nome), stdin)) {
                        nome[strcspn(nome, "\\n")] = 0;
                        if (strlen(nome) > 0) {
                            editor_simples(nome);
                        }
                    }
                } else {
                    editor_simples(arquivos[opcao_atual - 1]);
                }
                return;
            case 27: // ESC
                restaurar_terminal();
                return;
        }
    }
}`,
        'src/Makefile': `CC = gcc
CFLAGS = -Wall -O2
TARGET = mohavim
SRC = main.c editor.c files.c ui.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
\t$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
\t$(CC) $(CFLAGS) -c $< -o $@

clean:
\trm -f $(OBJ) $(TARGET)

install: $(TARGET)
\tcp $(TARGET) /usr/local/bin/

.PHONY: all clean install`,
        'install.sh': `#!/bin/bash

# Script de instalação do Mohavim

echo "=== Instalando o Mohavim ==="

# Diretório base
BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$BASE_DIR/src" || exit 1

# Verificar se gcc está disponível
if ! command -v gcc &> /dev/null; then
    echo "Erro: 'gcc' não encontrado. Instale o compilador GCC."
    exit 1
fi

# Limpar compilação anterior
echo "Limpando compilação anterior..."
make clean 2>/dev/null || true

# Compilar usando Makefile
echo "Compilando Mohavim..."
make

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    
    read -p "Instalar no sistema (/usr/local/bin)? [y/N]: " install_system
    if [[ $install_system =~ ^[Yy]$ ]]; then
        if [ ! -w /usr/local/bin ]; then
            echo "Precisando de permissões de root para instalar em /usr/local/bin..."
            sudo cp mohavim /usr/local/bin/
            sudo chmod +x /usr/local/bin/mohavim
        else
            cp mohavim /usr/local/bin/
            chmod +x /usr/local/bin/mohavim
        fi
        echo "✅ Mohavim instalado em /usr/local/bin/mohavim"
        echo "Execute 'mohavim' de qualquer lugar no terminal."
    else
        echo "Execute 'mohavim' para usar o editor."
    fi
    
    echo ""
    echo "=== Instalação concluída ==="
else
    echo "❌ Erro na compilação!"
    exit 1
fi`,
        'README.md': 'Arquivo README.md com documentação completa do projeto...',
        'CODIGO_FONTE.md': 'Documentação técnica detalhada da arquitetura modular...',
        'THEMES_IMPLEMENTATION.md': `# Implementação de Temas no Mohavim

## Estrutura de Temas

O Mohavim possui um sistema de temas consistente entre a interface de terminal e a interface web.

### Terminal (C)

Os temas para o terminal são definidos em \`src/theme.c\` e \`src/theme.h\`. Cada tema é representado por uma estrutura \`theme_t\` com as seguintes propriedades:

- \`name\`: Nome do tema
- \`background\`: Cor de fundo
- \`foreground\`: Cor do texto principal
- \`accent\`: Cor de destaque
- \`highlight\`: Cor para elementos selecionados
- \`error\`: Cor para mensagens de erro
- \`success\`: Cor para mensagens de sucesso

As cores são definidas usando códigos ANSI.

### Web (HTML/CSS/JS)

Os temas para a interface web são definidos em \`themes.json\`. Cada tema é um objeto com as seguintes propriedades:

- \`background\`: Cor de fundo
- \`foreground\`: Cor do texto principal
- \`accent\`: Cor de destaque
- \`highlight\`: Cor para elementos selecionados
- \`error\`: Cor para mensagens de erro
- \`success\`: Cor para mensagens de sucesso

As cores são definidas usando códigos hexadecimais.

O JavaScript carrega esses temas e aplica as cores definindo variáveis CSS no elemento \`::root\`. Estilos CSS usam essas variáveis para aplicar as cores.

## Como Adicionar um Novo Tema

### 1. Terminal

1. Edite \`src/theme.c\`.
2. Adicione uma nova entrada na função \`init_themes()\`.
3. Defina as cores usando códigos ANSI.
4. Recompile o Mohavim.

Exemplo:
\\\`\\\`\\\`c

strcpy(themes[3].name, "vaporwave");
strcpy(themes[3].background, "\\033[48;2;25;20;40m");      // Fundo roxo escuro
strcpy(themes[3].foreground, "\\033[38;2;255;105;180m");   // Rosa vibrante
strcpy(themes[3].accent, "\\033[38;2;0;255;255m");         // Ciano elétrico
strcpy(themes[3].highlight, "\\033[48;2;128;0;128m\\033[38;2;255;255;0m"); // Fundo roxo com texto amarelo
strcpy(themes[3].error, "\\033[38;2;255;20;147m");         // Rosa choque
strcpy(themes[3].success, "\\033[38;2;0;255;127m");        // Verde menta
\\\`\\\`\\\`

### 2. Web

1. Edite \`themes.json\`.
2. Adicione um novo objeto com as propriedades de cor.
3. Use códigos hexadecimais para as cores.
4. O tema estará automaticamente disponível na interface web.

Exemplo:
\\\`\\\`\\\`json

\\\`\\\`\\\`

5. Adicione o tema ao seletor em \`index.html\`:
\\\`\\\`\\\`html
<option value="vaporwave">Vaporwave</option>
\\\`\\\`\\\`

## Consistência de Temas

Para garantir consistência entre as interfaces:

1. Use nomes idênticos para os temas em ambos os arquivos (\`theme.c\` e \`themes.json\`).
2. Mantenha uma paleta de cores coerente entre as duas implementações.
3. Teste ambos os ambientes ao adicionar ou modificar temas.
`
    };

    fileItems.forEach(item => {
        item.addEventListener('click', () => {
            const fileName = item.getAttribute('data-file');
            
            // Remove seleção anterior
            fileItems.forEach(i => i.style.background = '');
            
            // Marca como selecionado
            item.style.background = 'rgba(255, 255, 255, 0.1)';
            
            // Mostra conteúdo
            if (fileContents[fileName]) {
                sourceContent.innerHTML = `<code>${escapeHtml(fileContents[fileName])}</code>`;
            } else {
                sourceContent.innerHTML = `<code>// Arquivo: ${fileName}
// Conteúdo não disponível para visualização.
// Este é um exemplo da interface web do Mohavim.</code>`;
            }
        });
    });
}

// Função auxiliar para escapar HTML
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// Função para carregar os temas do arquivo JSON
async function loadThemes() {
    try {
        const response = await fetch('themes.json');
        themes = await response.json();
        console.log('Temas carregados:', themes);
    } catch (error) {
        console.error('Erro ao carregar temas:', error);
        // Tema padrão caso o arquivo não seja encontrado
        themes = {
            "dark": {
                "background": "#1a1a2e",
                "foreground": "#ffffff",
                "accent": "#00ff88",
                "highlight": "#ff6b6b",
                "error": "#ff6b6b",
                "success": "#00ff88"
            }
        };
    }
}

// Função para aplicar um tema
function applyTheme(themeName) {
    const theme = themes[themeName];
    if (!theme) {
        console.error(`Tema '${themeName}' não encontrado.`);
        return;
    }

    // Aplicar cores ao CSS
    const root = document.documentElement;
    root.style.setProperty('--background-color', theme.background);
    root.style.setProperty('--foreground-color', theme.foreground);
    root.style.setProperty('--accent-color', theme.accent);
    root.style.setProperty('--highlight-color', theme.highlight);
    root.style.setProperty('--error-color', theme.error);
    root.style.setProperty('--success-color', theme.success);
    
    // Atualizar variável global
    currentTheme = themeName;
    
    // Salvar configurações
    saveCurrentSettings();
    
    // Atualizar o seletor de temas
    const themeSelect = document.getElementById('theme-select');
    if (themeSelect) {
        themeSelect.value = currentTheme;
    }
    
    // Aplicar tema ao editor
    applyEditorTheme();
    
    console.log(`Tema '${themeName}' aplicado.`);
}

// Função para carregar o tema salvo
function loadSavedSettings() {
    const savedTheme = localStorage.getItem('mohavim-theme');
    const savedLanguage = localStorage.getItem('mohavim-language');
    
    if (savedTheme && themes[savedTheme]) {
        currentTheme = savedTheme;
        // Atualizar o seletor de temas
        const themeSelect = document.getElementById('theme-select');
        if (themeSelect) {
            themeSelect.value = currentTheme;
        }
    }
    
    if (savedLanguage && translations[savedLanguage]) {
        currentLanguage = savedLanguage;
        // Atualizar o seletor de idiomas
        const languageSelect = document.getElementById('language-select');
        if (languageSelect) {
            languageSelect.value = currentLanguage;
        }
    }
}

// Função para salvar as configurações atuais
function saveCurrentSettings() {
    localStorage.setItem('mohavim-theme', currentTheme);
    localStorage.setItem('mohavim-language', currentLanguage);
    
    // Atualizar os seletores na interface
    const themeSelect = document.getElementById('theme-select');
    const languageSelect = document.getElementById('language-select');
    
    if (themeSelect) {
        themeSelect.value = currentTheme;
    }
    
    if (languageSelect) {
        languageSelect.value = currentLanguage;
    }
}

// Função para aplicar o idioma
function applyLanguage() {
    // Update UI elements with translated text
    const t = translations[currentLanguage];
    
    // Update labels
    const themeLabel = document.querySelector('.theme-selector label');
    const languageLabel = document.querySelector('.language-selector label');
    
    if (themeLabel) themeLabel.textContent = t.theme + ':';
    if (languageLabel) languageLabel.textContent = t.language + ':';
    
    // Update editor toolbar
    const newFileBtn = document.getElementById('new-file');
    const saveFileBtn = document.getElementById('save-file');
    const loadFileBtn = document.getElementById('load-file-btn');
    
    if (newFileBtn) newFileBtn.innerHTML = `<i class="fas fa-file"></i> ${t.new_file}`;
    if (saveFileBtn) saveFileBtn.innerHTML = `<i class="fas fa-save"></i> ${t.save}`;
    if (loadFileBtn) loadFileBtn.innerHTML = `<i class="fas fa-folder-open"></i> ${t.load_file}`;
    
    // Update status elements
    const fileStatusSpan = document.getElementById('file-status');
    if (fileStatusSpan) {
        if (fileStatusSpan.classList.contains('status-modified')) {
            fileStatusSpan.textContent = t.modified;
        } else if (fileStatusSpan.classList.contains('status-saved')) {
            fileStatusSpan.textContent = t.saved;
        }
    }
    
    // Update editor status
    const lineLabel = document.querySelector('.editor-status span:nth-child(1)');
    const columnLabel = document.querySelector('.editor-status span:nth-child(2)');
    const totalLabel = document.querySelector('.editor-status span:nth-child(3)');
    
    if (lineLabel) lineLabel.innerHTML = `${t.line}: <span id="current-line">1</span>`;
    if (columnLabel) columnLabel.innerHTML = `${t.column}: <span id="current-column">1</span>`;
    if (totalLabel) totalLabel.innerHTML = `${t.total}: <span id="total-chars">0</span> ${t.chars}`;
    
    // Update language selector to show current language
    const languageSelect = document.getElementById('language-select');
    if (languageSelect) {
        languageSelect.value = currentLanguage;
    }
    
    // Salvar configurações quando o idioma mudar
    saveCurrentSettings();
}

// Função para inicializar o seletor de temas
function initThemeSelector() {
    const themeSelect = document.getElementById('theme-select');
    if (themeSelect) {
        // Definir o valor do seletor para o tema atual
        themeSelect.value = currentTheme;
        
        // Adicionar evento de mudança
        themeSelect.addEventListener('change', (e) => {
            const selectedTheme = e.target.value;
            applyTheme(selectedTheme);
        });
    }
}

// Função para inicializar o seletor de idiomas
function initLanguageSelector() {
    const languageSelect = document.getElementById('language-select');
    if (languageSelect) {
        // Definir o valor do seletor para o idioma atual
        languageSelect.value = currentLanguage;
        
        // Adicionar evento de mudança
        languageSelect.addEventListener('change', (e) => {
            const selectedLanguage = e.target.value;
            currentLanguage = selectedLanguage;
            saveCurrentSettings(); // Salvar configurações quando o idioma mudar
            applyLanguage(); // Aplicar o novo idioma
        });
    }
}

// Adicionar atalhos de teclado globais
document.addEventListener('keydown', function(e) {
    // Verificar se o foco não está em um campo de entrada
    if (document.activeElement.tagName !== 'INPUT' && document.activeElement.tagName !== 'TEXTAREA') {
        switch(e.key) {
            case 'l':
            case 'L':
                // Mostrar logs
                alert('Visualização de logs não disponível na interface web');
                e.preventDefault();
                break;
            case 't':
            case 'T':
                // Focar no seletor de temas
                document.getElementById('theme-select').focus();
                e.preventDefault();
                break;
            case 'i':
            case 'I':
                // Mostrar idiomas
                alert('Seleção de idiomas não disponível na interface web');
                e.preventDefault();
                break;
            case 'p':
            case 'P':
                // Mostrar plugins
                alert('Gerenciamento de plugins não disponível na interface web');
                e.preventDefault();
                break;
        }
    }
});

// Adicionar efeitos visuais
document.addEventListener('DOMContentLoaded', function() {
    // Efeito de digitação no logo
    const logoElement = document.querySelector('.ascii-logo');
    if (logoElement) {
        logoElement.style.opacity = '0';
        setTimeout(() => {
            logoElement.style.transition = 'opacity 1s ease';
            logoElement.style.opacity = '1';
        }, 500);
    }
    
    // Efeito de aparição dos cards
    const cards = document.querySelectorAll('.feature-card, .about-card');
    cards.forEach((card, index) => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        setTimeout(() => {
            card.style.transition = 'opacity 0.5s ease, transform 0.5s ease';
            card.style.opacity = '1';
            card.style.transform = 'translateY(0)';
        }, 100 * index);
    });
});
