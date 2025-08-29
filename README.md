                 ⚡ Mohavim  ⚡
         criado por: Gabriel A. Matos
         Email: gabriellramone@gmail.com

> Editor de texto com interface moderna e arquitetura modular

> Navegação intuitiva e recursos avançados

> Código organizado em módulos para melhor manutenibilidade

✨ Features

🎨 Interface moderna com cores e elementos visuais limpos
🧭 Navegação intuitiva com setas direcionais
📂 Seleção de arquivos no diretório atual
🗑️ Exclusão de arquivos com confirmação de segurança
📝 Editor com numeração de linhas e cursor visual
🔍 Sistema de busca em arquivos
🌐 Interface 100% em português brasileiro
⚡ Performance otimizada em C
🗂️ Arquitetura modular para melhor organização

🚀 Roadmap de Futuras Funcionalidades

O Mohavim ainda está em estágio inicial, mas estas são as próximas melhorias planejadas:

Idiomas (i18n)

Suporte a múltiplos idiomas (ex: Português, Inglês).

Arquivos de tradução externos para facilitar colaboração.

Temas

Suporte a temas claros/escuros.

Configuração customizada de cores via arquivo.

Logs internos

Visualizar logs diretamente no editor.

Nível de detalhe configurável (verbose ON/OFF).

Verbose mode

Parâmetro --verbose para ativar logs detalhados.

Útil para debug e contribuições.

Plugins

Suporte a módulos externos criados pelo usuário.

Carregamento dinâmico de plugins (.so).

API simples para estender o editor.

Configurações persistentes

Arquivo ~/.mohavimrc para salvar idioma, tema e preferências.

Qualidade de vida

Melhorar interface de menus.

Otimizar atalhos e navegação.

Reduzir bugs conhecidos.

- [x] Abrir arquivos
- [x] Salvar arquivos
- [ ] Idiomas (i18n)
- [ ] Temas
- [ ] Logs internos
- [ ] Verbose mode
- [ ] Plugins
- [ ] Configurações persistentes

🚀 Como usar

Os arquivos de texto recém-criados serão armazenados no diretório atual ao executar o Mohavim.

Por exemplo:

❯ pwd → /home/usuario/Documents/

❯ mohavim

❯ Criar Novo Arquivo → teste.txt

❯ Digitar conteúdo

❯ Ctrl+S para salvar

❯ Ctrl+Q para sair

Assim, o arquivo teste.txt será salvo em: /home/usuario/Documents/teste.txt.

A leitura e edição dos arquivos de texto funcionam da mesma maneira.


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

### Para desenvolvimento:
- GCC com suporte a C99+
- GNU Make 3.81+
- Sistema Unix/Linux (testado em Ubuntu, Arch, etc.)


🎮 Controles
Navegador de Arquivos:
- ↑↓ : navegar
- Enter : abrir arquivo
- D : excluir (confirmação SIM)
- ESC : voltar


Editor de Texto:
- ↑↓←→ : navegação
- Ctrl+S : salvar
- Ctrl+Q : sair
- ESC : voltar
- Enter : nova linha
- Backspace : apagar


📂 Estrutura

src/
├── main.c      → programa principal
├── editor.c/.h → funcionalidades do editor
├── ui.c/.h     → interface e controles
├── files.c/.h  → gerenciamento de arquivos
├── Makefile    → sistema de compilação
└── mohavim     → executável compilado

README.md       → documentação
CODIGO_FONTE.md → documentação técnica
LICENSE         → licença
install.sh      → script de instalação

Contribuição

Quer contribuir com o Mohavim?  

Me contate em: gabriellramone@gmail.com


Mohavim é distribuído sob a licença GNU GPLv3.  

Consulte o arquivo LICENSE para mais detalhes.


🛰️ Versão:

Mohavim 7.2 🔥
