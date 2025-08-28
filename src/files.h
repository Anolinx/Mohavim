#ifndef FILES_H
#define FILES_H

void carregar_lista_arquivos();
void mostrar_lista_arquivos(int selecionado);
void abrir_arquivo();
void buscar_arquivo();

extern char arquivos[100][256];
extern int total_arquivos;

#endif