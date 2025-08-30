#!/bin/bash

# Script de instalação do Mohavim

echo "=== Instalando o Mohavim 7.3 ==="

# Diretório base
BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$BASE_DIR/src" || exit 1

# Verificar se gcc está disponível
if ! command -v gcc &> /dev/null; then
    echo "Erro: 'gcc' não encontrado. Instale o compilador GCC."
    exit 1
fi

# Limpar compilação anterior
echo "Limpando compilação anterior..."
make clean 2>/dev/null || true

# Compilar usando Makefile
echo "Compilando o Mohavim..."
make

if [ $? -eq 0 ]; then
    echo "✅ Compilação bem-sucedida!"
    
    read -p "Instalar no sistema (/usr/local/bin)? [y/N]: " install_system
    if [[ $install_system =~ ^[Yy]$ ]]; then    


        if ! cp mohavim /usr/local/bin/ 2>/dev/null; then
            echo "Precisando de permissões de root para instalar em /usr/local/bin..."
            sudo cp mohavim /usr/local/bin/
            sudo chmod +x /usr/local/bin/mohavim
        else
            # Se a cópia direta funcionar, só é preciso dar permissão de execução.
            chmod +x /usr/local/bin/mohavim
        fi
        
        echo "✅ Mohavim instalado em /usr/local/bin/mohavim"
        echo "🚀 Execute 'mohavim' de qualquer lugar no terminal!"
        
        # Testar a instalação
        echo ""
        echo "🔍 Testando instalação..."
        if command -v mohavim &> /dev/null; then
            echo "✅ Instalação verificada com sucesso!"
            echo ""
            echo "📋 Exemplos de uso:"
            echo "  mohavim                                 # Executar normalmente"
            echo "  mohavim --lang pt --theme dark        # Com argumentos"
            echo "  mohavim --verbose                     # Modo debug"
        else
            echo "⚠️ Aviso: Pode ser necessário reiniciar o terminal"
            echo "    ou adicionar /usr/local/bin ao seu PATH"
        fi
        
        echo ""
        echo "=== Instalação concluída ==="
    fi
else
    echo "❌ Erro na compilação!"
    exit 1
fi
