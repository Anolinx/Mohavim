# Mohavim Internationalization (i18n) System Implementation

## Summary of Changes

This implementation adds a complete internationalization system to the Mohavim text editor with the following features:

### 1. Multiple Language Configuration Methods
- **Command line argument**: `mohavim --lang en`
- **Environment variables**: `LANGUAGE=en` or `LANG=en_US.UTF-8`
- **Configuration file**: `.mohavimrc` with `language=en`
- **Default fallback**: Portuguese (Brazil)

### 2. Priority Order
1. Command line argument (`--lang`)
2. Environment variable (`LANGUAGE` or `LANG`)
3. Configuration file (`.mohavimrc`)
4. Default (Portuguese)

### 3. Supported Languages
- Portuguese (Brazil) - Default
- English

### 4. Implementation Details

#### Files Modified:
- `src/i18n.c` - Enhanced i18n system with config support
- `src/i18n.h` - Updated function declarations
- `src/main.c` - Integrated auto-detection
- `README.md` - Updated documentation

#### New Functions Added:
- `get_language_from_env()` - Reads LANGUAGE/LANG environment variables
- `get_language_from_config()` - Reads language from .mohavimrc file
- `auto_detect_language()` - Implements the priority order logic

#### Enhanced Functions:
- `load_language()` - Improved matching with partial matches and normalization
- `add_language_string()` - Fixed string addition logic
- `init_i18n()` - Added all strings for both languages

### 5. Configuration File Format

Create a `.mohavimrc` file in your home directory or in the current directory:

```
language=en
```

### 6. Environment Variables

Set the language using environment variables:

```bash
export LANGUAGE=en
mohavim

# Or
export LANG=en_US.UTF-8
mohavim
```

### 7. Command Line Usage

Specify the language when running Mohavim:

```bash
mohavim --lang en
```

### 8. Adding New Languages

To add a new language:
1. Edit `src/i18n.c`
2. Add a new language section in `init_i18n()` function
3. Use a unique language code
4. Translate all strings
5. Recompile Mohavim

### 9. Fallback Behavior

If an unsupported language is specified:
- Environment variable: Falls back to default (Portuguese)
- Config file: Falls back to default (Portuguese)
- Command line: Shows error and exits

The system handles various language code formats:
- `en` matches `en`
- `en_US` matches `en`
- `pt_BR.UTF-8` matches `pt_br`