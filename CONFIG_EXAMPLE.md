# Mohavim Configuration Example

Mohavim supports multiple ways to set your preferred language:

## 1. Environment Variables

You can set the language using either `LANGUAGE` or `LANG` environment variables:

```bash
# Set language to English
export LANGUAGE=en
mohavim

# Or use LANG
export LANG=en_US.UTF-8
mohavim
```

## 2. Configuration File

Create a configuration file named `.mohavimrc` in your home directory:

```bash
# Create the config file
echo "language=en" > ~/.mohavimrc

# Or edit it manually
nano ~/.mohavimrc
```

Add the following line to set your preferred language:
```
language=en
```

## 3. Command Line

You can also specify the language when running Mohavim:

```bash
# Run with English interface
mohavim --lang en

# Run with Portuguese interface
mohavim --lang pt_br
```

## Language Priority

Mohavim will use the language in this priority order:
1. Command line argument (`--lang`)
2. Environment variable (`LANGUAGE` or `LANG`)
3. Configuration file (`.mohavimrc`)
4. Default (Portuguese)

## Supported Languages

- `pt_br` - Portuguese (Brazil) - Default
- `en` - English

## Adding New Languages

To add a new language:
1. Edit `src/i18n.c`
2. Add a new language section in `init_i18n()` function
3. Use a unique language code
4. Translate all strings
5. Recompile Mohavim