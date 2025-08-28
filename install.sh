#!/bin/bash

# Script de instalação do Mohavim

echo "=== Instalando Mohavim ==="

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
echo "Compilando Mohavim..."
make

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"

    read -p "Instalar no sistema (/usr/local/bin)? [y/N]: " install_system
    if [[ $install_system =~ ^[Yy]$ ]]; then
        if [ ! -w /usr/local/bin ]; then
            echo "Precisando de permissões de root para instalar em /usr/local/bin..."
            sudo cp mohavim /usr/local/bin/
            sudo chmod +x /usr/local/bin/mohavim
        else
            cp mohavim /usr/local/bin/
            chmod +x /usr/local/bin/mohavim
        fi
        echo "✅ Mohavim instalado em /usr/local/bin/mohavim"
        echo "Execute 'mohavim' de qualquer lugar no terminal."
    else
        echo "Execute './mohavim' na pasta $BASE_DIR/src para usar o editor."
    fi

    echo ""
    echo "=== Instalação concluída ==="
else
    echo "❌ Erro na compilação!"
    exit 1
fi