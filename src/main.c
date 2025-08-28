#include "ui.h"
#include "editor.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int opcao_atual=0;
    mostrar_logo();
    printf("\nPressione Enter para continuar..."); getchar();
    configurar_terminal();

    while(1){
        mostrar_menu(opcao_atual);
        int tecla=ler_tecla();
        switch(tecla){
            case 65: opcao_atual=(opcao_atual-1+5)%5; break;
            case 66: opcao_atual=(opcao_atual+1)%5; break;
            case 10: restaurar_terminal();
                switch(opcao_atual){
                    case 0: abrir_arquivo(); break;
                    case 1: novo_arquivo(); break;
                    case 2: buscar_arquivo(); break;
                    case 3: sobre(); break;
                    case 4: printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
                }
                configurar_terminal();
                break;
            case 27: restaurar_terminal(); printf("\033[2J\033[H🚪 Saindo...\n💀 Obrigado por usar o Mohavim!\n"); exit(0);
        }
    }
    return 0;
}