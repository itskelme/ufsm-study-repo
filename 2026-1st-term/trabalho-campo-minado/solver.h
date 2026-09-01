#ifndef SOLVER_H
#define SOLVER_H

#include "tabuleiro.h"

// Encontra a proxima jogada por deducao logica (1: abrir, 2: bandeira)
int ProximoPasso(Tabuleiro *t, int *l_out, int *c_out, int *acao_out);

// Testa se o tabuleiro pode ser resolvido puramente por logica
int TestaSolucao(Tabuleiro *t, int safe_l, int safe_c);

#endif
