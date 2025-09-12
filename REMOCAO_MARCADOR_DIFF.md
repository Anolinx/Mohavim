# Remoção do Marcador "► 📝 [Digite o nome do arquivo] ◄"

## Resumo das Alterações

O marcador "► 📝 [Digite o nome do arquivo] ◄" já havia sido removido da interface do editor em versões anteriores, mas ainda existiam referências a essa funcionalidade no código. Este trabalho teve como objetivo garantir que todas as referências fossem completamente removidas.

## Arquivos Modificados

### 1. `/src/i18n.c`

- **Removidas as seguintes strings de tradução para Português:**
  - `type_filename` ("📝 [Novo Arquivo]")
  - `type_filename_title` ("📝 DIGITE O NOME DO ARQUIVO")

- **Removidas as seguintes strings de tradução para Inglês:**
  - `type_filename` ("📝 [New File]")
  - `type_filename_title` ("📝 TYPE FILENAME")

## Verificação

- O projeto compila corretamente sem erros.
- Não há mais nenhuma referência às strings `type_filename` ou `type_filename_title` no código.
- A funcionalidade de seleção de arquivos continua funcionando normalmente, apenas sem a opção de digitar manualmente o nome de um novo arquivo.

## Diferenças no Código

As únicas mudanças realizadas foram a remoção das strings de internacionalização não utilizadas no arquivo `i18n.c`. O restante do código permanece inalterado e funcional.