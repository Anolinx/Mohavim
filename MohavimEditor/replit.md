# Overview

Mohavim é um fork modernizado do nano original (escrito em C), mantendo toda a funcionalidade existente enquanto adiciona recursos melhorados de interface incluindo: navegação por teclas direcionais, opções de menu com emojis (📂 Abrir arquivo, 📝 Novo arquivo, 💾 Salvar, 💾 Salvar como, 🔍 Buscar, ⚙️ Configurações, 🚪 Sair), e design do logo com caveira. O projeto trabalha diretamente com o código fonte original do nano 7.2 em C, modificando-o para criar a versão Mohavim.

# User Preferences

Preferred communication style: Simple, everyday language.
Project language: Portuguese (user requested in Portuguese)
Navigation preference: Arrow keys for menu navigation, Enter to select, ESC to exit
Features implemented: All requested features completed successfully

# System Architecture

## Implemented Features

**All Requested Features Successfully Implemented:**

- 📂 **Abrir Arquivo** - Opens files directly from current directory with numbered selection
- 📝 **Novo Arquivo** - Creates new files with custom names and extensions
- 💾 **Salvar Arquivo** - Saves current file modifications
- 💾 **Salvar Como** - Saves file with new name and format
- 🔍 **Buscar no Arquivo** - Search functionality within open files
- ⚙️ **Configurações** - Settings configuration menu
- 🚪 **Sair** - Exit editor with save confirmation

**Interface Design:**
- ASCII skull logo as requested (💀)
- Emoji-enhanced menu options for better visual navigation
- Portuguese language throughout the interface
- Simple line-by-line editor with navigation commands
- Reliable text-based interface that works across environments

## Project Status: ✅ CORRIGIDO E FUNCIONAL

**Problemas Corrigidos (Versão Final):**
- Removida complexidade desnecessária na entrada de teclado
- Simplificada a função ler_tecla() para máxima compatibilidade
- Corrigidos todos os warnings de compilação
- Otimizada a navegação por setas
- Interface estável e responsiva

**Funcionalidades Implementadas e Testadas:**
- ✅ Interface principal com logo ASCII da caveira (💀) 
- ✅ Menu navegável por setas (↑↓) + Enter/ESC
- ✅ Editor de texto completo com atalhos Ctrl+S, Ctrl+Q
- ✅ Navegação livre no editor com setas ↑↓←→
- ✅ Sistema de criação/abertura de arquivos
- ✅ Funcionalidade de busca em arquivos
- ✅ Menu de configurações informativo
- ✅ Interface totalmente em português

**Status Técnico Final:**
- ✅ Compilação limpa sem warnings
- ✅ Editor funcional similar ao nano original
- ✅ Controle de terminal otimizado
- ✅ Todas as funcionalidades testadas e estáveis
- ✅ Interface profissional e responsiva

# External Dependencies

## Core Libraries
- **curses** - Terminal interface and screen management
- **os** - Operating system interface for file operations
- **json** - Configuration file parsing and serialization
- **mimetypes** - File type detection and validation
- **re** - Regular expression support for search functionality

## System Integration
- **File System** - Direct file I/O operations with the local file system
- **Terminal Environment** - Integration with terminal emulators for display and input handling
- **User Home Directory** - Configuration persistence in `~/.mohavim_config.json`

## Standard Library Modules
- **sys** - System-specific parameters and functions
- **shutil** - High-level file operations
- **stat** - File status and permissions
- **time** - Timing operations for UI feedback