#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int linhas;
    int colunas;
    int nrBombas;
    int abertas;
    int gerado;
    int **celulas; 
    int **estado;  
} Tabuleiro;


Tabuleiro* Alocar(int linhas, int colunas, int bombas);
void Liberar(Tabuleiro* t);

void Preencher(Tabuleiro* t, int safe_l, int safe_c);
int Abrir(Tabuleiro* t, int l, int c);
void MudaBandeira(Tabuleiro* t, int l, int c);
int Ganhou(Tabuleiro* t);
void Imprimir(Tabuleiro* t, int revelado);

int PosicaoValida(Tabuleiro* t, int l, int c);
void ContaVizinhos(Tabuleiro* t, int l, int c, int *ocultos, int *bandeiras);

#endif
