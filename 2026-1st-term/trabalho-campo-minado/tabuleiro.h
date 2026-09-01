#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdlib.h>

// Estrutura do tabuleiro seguindo o modelo do professor
typedef struct {
    int linhas;
    int colunas;
    int nrBombas;
    int abertas;
    int gerado;
    int **celulas; // -1 se for bomba, 0 a 8 para quantidade de bombas vizinhas
    int **estado;  // 0: fechada, 1: aberta, 2: bandeira
} Tabuleiro;

// Gerenciamento de memoria
Tabuleiro* Alocar(int linhas, int colunas, int bombas);
void Liberar(Tabuleiro* t);

// Funcoes principais do jogo
void Preencher(Tabuleiro* t, int safe_l, int safe_c);
int Abrir(Tabuleiro* t, int l, int c);
void MudaBandeira(Tabuleiro* t, int l, int c);
int Ganhou(Tabuleiro* t);
void Imprimir(Tabuleiro* t, int revelado);

// Funcoes auxiliares
int PosicaoValida(Tabuleiro* t, int l, int c);
void ContaVizinhos(Tabuleiro* t, int l, int c, int *ocultos, int *bandeiras);

#endif
