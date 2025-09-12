# Implementação de Temas no Mohavim

## Estrutura de Temas

O Mohavim possui um sistema de temas consistente entre a interface de terminal e a interface web.

### Terminal (C)

Os temas para o terminal são definidos em `src/theme.c` e `src/theme.h`. Cada tema é representado por uma estrutura `theme_t` com as seguintes propriedades:

- `name`: Nome do tema
- `background`: Cor de fundo
- `foreground`: Cor do texto principal
- `accent`: Cor de destaque
- `highlight`: Cor para elementos selecionados
- `error`: Cor para mensagens de erro
- `success`: Cor para mensagens de sucesso

As cores são definidas usando códigos ANSI.

### Web (HTML/CSS/JS)

Os temas para a interface web são definidos em `themes.json`. Cada tema é um objeto com as seguintes propriedades:

- `background`: Cor de fundo
- `foreground`: Cor do texto principal
- `accent`: Cor de destaque
- `highlight`: Cor para elementos selecionados
- `error`: Cor para mensagens de erro
- `success`: Cor para mensagens de sucesso

As cores são definidas usando códigos hexadecimais.

O JavaScript carrega esses temas e aplica as cores definindo variáveis CSS no elemento `:root`. Estilos CSS usam essas variáveis para aplicar as cores.

## Como Adicionar um Novo Tema

### 1. Terminal

1. Edite `src/theme.c`.
2. Adicione uma nova entrada na função `init_themes()`.
3. Defina as cores usando códigos ANSI.
4. Recompile o Mohavim.

Exemplo:
```c
// Tema Vaporwave
strcpy(themes[3].name, "vaporwave");
strcpy(themes[3].background, "\033[48;2;25;20;40m");      // Fundo roxo escuro
strcpy(themes[3].foreground, "\033[38;2;255;105;180m");   // Rosa vibrante
strcpy(themes[3].accent, "\033[38;2;0;255;255m");         // Ciano elétrico
strcpy(themes[3].highlight, "\033[48;2;128;0;128m\033[38;2;255;255;0m"); // Fundo roxo com texto amarelo
strcpy(themes[3].error, "\033[38;2;255;20;147m");         // Rosa choque
strcpy(themes[3].success, "\033[38;2;0;255;127m");        // Verde menta
```

### 2. Web

1. Edite `themes.json`.
2. Adicione um novo objeto com as propriedades de cor.
3. Use códigos hexadecimais para as cores.
4. O tema estará automaticamente disponível na interface web.

Exemplo:
```json
"vaporwave": {
  "background": "#191428",
  "foreground": "#ff69b4",
  "accent": "#00ffff",
  "highlight": "#800080",
  "error": "#ff1493",
  "success": "#00ff7f"
}
```

5. Adicione o tema ao seletor em `index.html`:
```html
<option value="vaporwave">Vaporwave</option>
```

## Consistência de Temas

Para garantir consistência entre as interfaces:

1. Use nomes idênticos para os temas em ambos os arquivos (`theme.c` e `themes.json`).
2. Mantenha uma paleta de cores coerente entre as duas implementações.
3. Teste ambos os ambientes ao adicionar ou modificar temas.