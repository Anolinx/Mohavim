#!/bin/bash

# Script de instalação do Mohavim

echo "=== Instalando Mohavim ==="

# Verificar se o make está disponível
if ! command -v make &> /dev/null; then
    echo "Erro: 'make' não encontrado. Instale build-essential ou equivalent."
    exit 1
fi

# Verificar se o gcc está disponível
if ! command -v gcc &> /dev/null; then
    echo "Erro: 'gcc' não encontrado. Instale o compilador GCC."
    exit 1
fi

# Compilar o projeto
echo "Compilando Mohavim..."
cd MohavimEditor/
make clean
make

if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"
    
    # Perguntar se quer instalar no sistema
    read -p "Instalar no sistema (/usr/local/bin)? [y/N]: " install_system
    
    if [[ $install_system =~ ^[Yy]$ ]]; then
        echo "Instalando no sistema..."
        sudo cp mohavim /usr/local/bin/
        sudo chmod +x /usr/local/bin/mohavim
        echo "Mohavim instalado em /usr/local/bin/mohavim"
        echo "Execute 'mohavim' de qualquer lugar no terminal."
    else
        echo "Execute './mohavim' na pasta MohavimEditor para usar o editor."
    fi
    
    echo ""
    echo "=== Instalação concluída ==="
    echo "Para usar: ./mohavim (ou 'mohavim' se instalado no sistema)"
else
    echo "Erro na compilação!"
    exit 1
fi