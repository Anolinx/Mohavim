 ⚡ Mohavim — Editor de Texto Modernizado ⚡
         criado por: Gabriel A. Matos

> Um fork moderno e estilizado do GNU nano 7.2
> Interface moderna e minimalista

==========================================================================================

✨ Features
- Interface moderna com cores e elementos visuais limpos
- Navegação intuitiva com setas direcionais
- Seleção de arquivos no diretório atual
- Exclusão de arquivos com confirmação de segurança
- Editor com numeração de linhas
- Cursor visual destacado
- Sistema de busca em arquivos
- Interface 100% em português

==========================================================================================

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

==========================================================================================

⚡ Instalação automática:

Clone ou baixe os arquivos

git clone https://github.com/Anolinx/Mohavim

cd Mohavim/

Mude as permissões de execução no arquivo install.sh;
Exemplo: chmod +x install.sh

Para executar o script: ./install.sh

===========================================================================================

📦 Dependências
- GCC (compilador C)
- Bibliotecas padrão (termios, dirent)
- Make

==========================================================================================

🎮 Controles
Navegador de Arquivos:
- ↑↓ : navegar
- Enter : abrir arquivo
- D : excluir (confirmação SIM)
- ESC : voltar

==========================================================================================

Editor de Texto:
- ↑↓←→ : navegação
- Ctrl+S : salvar
- Ctrl+Q : sair
- ESC : voltar
- Enter : nova linha
- Backspace : apagar

==========================================================================================

📂 Estrutura

mohavim.c   → núcleo modificado

src/        → código fonte do nano

lib/        → libs auxiliares

syntax/     → sintaxes

doc/        → documentação

==========================================================================================

Uso ⌨️

Execute ./mohavim

Use as setas ↑/↓ para navegar no menu

Pressione Enter para selecionar opções

No seletor de arquivos: Pressione D para excluir arquivo (requer confirmação digitando SIM)

No editor: Ctrl+S para salvar, Ctrl+Q para sair, ESC para voltar ao menu

==========================================================================================

Contribuição

Quer contribuir com o Mohavim?  

Me contate em:

==========================================================================================

Mohavim é distribuído sob a licença GNU GPLv3.  

Consulte o arquivo LICENSE para mais detalhes.

===========================================================================================

🛰️ Versão:
Mohavim 7.2-fork baseado no GNU nano 7.2 🔥
