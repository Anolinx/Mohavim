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


Uso ⌨️

Execute cd src/ && ./mohavim

Use as setas ↑/↓ para navegar no menu

Pressione Enter para selecionar opções

No seletor de arquivos: Pressione D para excluir arquivo (requer confirmação digitando SIM)

No editor: Ctrl+S para salvar, Ctrl+Q para sair, ESC para voltar ao menu


Contribuição

Quer contribuir com o Mohavim?  

Me contate em: gabriellramone@gmail.com


Mohavim é distribuído sob a licença GNU GPLv3.  

Consulte o arquivo LICENSE para mais detalhes.


🛰️ Versão:

Mohavim 7.2 🔥
