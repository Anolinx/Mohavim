# Mohavim 7.3 - Text Editor with Modern Interface

## Project Overview
Mohavim is a modular text editor written in C with a comprehensive web interface. It features internationalization, customizable themes, internal logging system, and dynamic plugin support.

## Recent Changes
- **2025-09-25**: Successfully imported and configured for Replit environment
  - Compiled C-based text editor from source
  - Set up Node.js web server to serve frontend on port 5000
  - Configured deployment for production autoscaling
  - Both terminal and web interfaces are fully functional

## Project Architecture

### C-based Terminal Editor
- **Location**: `src/` directory
- **Binary**: `src/mohavim` (compiled executable)
- **Features**:
  - Multi-language support (Portuguese, English)
  - Multiple themes (dark, light, vaporwave, vintage)
  - Plugin system with dynamic loading
  - Internal logging system
  - Command-line arguments support

### Web Frontend
- **Main file**: `index.html`
- **Server**: `server.js` (Node.js static file server)
- **Port**: 5000 (configured for Replit)
- **Features**:
  - Theme selector with live preview
  - Web-based text editor simulation
  - Source code browser
  - Installation guide
  - Multi-language interface

### Key Components
- `src/main.c` - Main program with CLI argument parsing
- `src/editor.c` - Core editor functionality
- `src/ui.c` - User interface and terminal handling
- `src/files.c` - File management system
- `src/i18n.c` - Internationalization system
- `src/theme.c` - Theme management
- `src/log.c` - Internal logging system
- `src/plugin_manager.c` - Dynamic plugin loading
- `app.js` - Web interface JavaScript
- `style.css` - Web interface styling
- `themes.json` - Theme configurations for web

## Build System
- **Makefile**: Located in `src/Makefile`
- **Dependencies**: GCC, Make, libdl
- **Build command**: `cd src && make`
- **Clean command**: `cd src && make clean`

## Deployment Configuration
- **Type**: Autoscale (stateless web application)
- **Build**: Compiles C editor from source
- **Runtime**: Node.js server serving static files and compiled editor

## Usage
### Terminal Editor
```bash
cd src
./mohavim --lang en --theme cyberpunk
```

### Web Interface
Access via Replit's web preview on port 5000

## User Preferences
- Project successfully configured for Replit environment
- Web interface properly configured with host binding for proxy access
- Both C compilation and web serving working correctly