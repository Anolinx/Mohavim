# 📂 Código Fonte do Mohavim

# Criado por: Gabriel A. Matos

## Arquivos principais:

### 🔥 **Estrutura Modular**
- Código organizado em módulos separados
- Arquitetura limpa e manutenível
- Interface moderna com menu visual e navegação intuitiva
- Localização: `./src/`

## 🏗️ Estrutura do Projeto
O projeto `src/` contém:
- `main.c` - Programa principal e loop de menu
- `editor.c/.h` - Funcionalidades do editor de texto
- `ui.c/.h` - Interface de usuário e controles de terminal
- `files.c/.h` - Gerenciamento e navegação de arquivos
- `Makefile` - Sistema de compilação otimizado
- `mohavim` - Executável final compilado

## 🗂️ Detalhamento dos Módulos

### 📄 **main.c**
- Programa principal com loop de menu
- Controle de navegação entre opções
- Integração entre todos os módulos
- Gerenciamento de estado da aplicação

### ✏️ **editor.c/.h**
- Motor principal do editor de texto
- Sistema de buffer com 10.000 caracteres
- Navegação por cursor com setas direcionais
- Funcionalidades: salvar (Ctrl+S), sair (Ctrl+Q)
- Numeração de linhas e indicador de posição
- Detecção de modificações no arquivo

### 🖥️ **ui.c/.h**
- Interface de usuário e controles de terminal
- Logo ASCII do Mohavim
- Sistema de menus navegáveis
- Configuração de terminal para modo raw
- Captura de teclas especiais (setas, ESC, Ctrl)
- Tela "Sobre" com informações do programa

### 📁 **files.c/.h**
- Gerenciamento completo de arquivos
- Listagem de arquivos do diretório atual
- Seleção visual de arquivos
- Criação de novos arquivos
- Exclusão segura com confirmação ("SIM")
- Sistema de busca em arquivos existentes

## ✅ Features Implementadas
- Interface moderna com logo ASCII
- Menu principal navegável
- Editor com numeração de linhas
- Navegação por setas
- Navegador de arquivos integrado
- Exclusão segura de arquivos
- Interface 100% em português
- Arquitetura modular e manutenível

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
cd src/
./mohavim
```

### Arquivos Gerados:
- `main.o, editor.o, files.o, ui.o` - Arquivos objeto
- `mohavim` - Executável final
