#ifndef SOLVER_H
#define SOLVER_H

#include "tabuleiro.h"

int ProximoPasso(Tabuleiro *t, int *l_out, int *c_out, int *acao_out);
int TestaSolucao(Tabuleiro *t, int safe_l, int safe_c);

#endif
