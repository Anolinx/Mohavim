// Variáveis globais
let currentFile = 'sem_titulo.txt';
let isModified = false;

// Inicialização quando a página carrega
document.addEventListener('DOMContentLoaded', function() {
    initTabs();
    initEditor();
    initFileManager();
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

        currentLineSpan.textContent = currentLine;
        currentColumnSpan.textContent = currentColumn;
        totalCharsSpan.textContent = totalChars;

        updateLineNumbers();
    }

    // Marcar como modificado
    function markAsModified() {
        if (!isModified) {
            isModified = true;
            fileStatusSpan.textContent = 'Modificado';
            fileStatusSpan.className = 'status-modified';
        }
    }

    // Marcar como salvo
    function markAsSaved() {
        isModified = false;
        fileStatusSpan.textContent = 'Salvo';
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
            if (confirm('Arquivo modificado. Deseja salvar antes de criar um novo?')) {
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
        'CODIGO_FONTE.md': 'Documentação técnica detalhada da arquitetura modular...'
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
