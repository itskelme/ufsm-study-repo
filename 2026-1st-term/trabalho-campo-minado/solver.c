#include "solver.h"

typedef struct {
    int l;
    int c;
} Posicao;


static int PegaVizinhosFechados(Tabuleiro *t, int l, int c, Posicao out[]) {
    int n = 0;
    for (int dl = -1; dl <= 1; dl++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dl == 0 && dc == 0) continue;
            int nl = l + dl;
            int nc = c + dc;
            if (PosicaoValida(t, nl, nc) && t->estado[nl][nc] == 0) {
                out[n].l = nl;
                out[n].c = nc;
                n++;
            }
        }
    }
    return n;
}


static int TemPosicao(const Posicao vet[], int n, int l, int c) {
    for (int i = 0; i < n; i++) {
        if (vet[i].l == l && vet[i].c == c) return 1;
    }
    return 0;
}


static int Subconjunto(const Posicao sub[], int n1, const Posicao sup[], int n2) {
    for (int i = 0; i < n1; i++) {
        if (!TemPosicao(sup, n2, sub[i].l, sub[i].c)) return 0;
    }
    return 1;
}


int ProximoPasso(Tabuleiro *t, int *l_out, int *c_out, int *acao_out) {
    if (!t) return 0;

    // Regra 1: Analise de 1 quadrado
    for (int l = 0; l < t->linhas; l++) {
        for (int c = 0; c < t->colunas; c++) {
            if (t->estado[l][c] != 1 || t->celulas[l][c] <= 0) continue;

            int ocultos = 0, flags = 0;
            ContaVizinhos(t, l, c, &ocultos, &flags);
            if (ocultos == 0) continue;

            int n = t->celulas[l][c];
            Posicao viz[8];
            int n_viz = PegaVizinhosFechados(t, l, c, viz);

            // todas as casas fechadas sao bombas
            if (n == flags + ocultos && n_viz > 0) {
                *l_out = viz[0].l;
                *c_out = viz[0].c;
                *acao_out = 2; // bandeira
                return 1;
            }

            // todas as bombas ja foram achadas
            if (n == flags && n_viz > 0) {
                *l_out = viz[0].l;
                *c_out = viz[0].c;
                *acao_out = 1; // abrir
                return 1;
            }
        }
    }

    // Regra 2: Analise de 2 quadrados (subconjuntos)
    for (int l1 = 0; l1 < t->linhas; l1++) {
        for (int c1 = 0; c1 < t->colunas; c1++) {
            if (t->estado[l1][c1] != 1 || t->celulas[l1][c1] <= 0) continue;

            Posicao viz1[8];
            int n1 = PegaVizinhosFechados(t, l1, c1, viz1);
            if (n1 == 0) continue;

            int o1, f1;
            ContaVizinhos(t, l1, c1, &o1, &f1);
            int m1 = t->celulas[l1][c1] - f1;

            for (int dl = -2; dl <= 2; dl++) {
                for (int dc = -2; dc <= 2; dc++) {
                    int l2 = l1 + dl;
                    int c2 = c1 + dc;
                    if ((l1 == l2 && c1 == c2) || !PosicaoValida(t, l2, c2)) continue;
                    if (t->estado[l2][c2] != 1 || t->celulas[l2][c2] <= 0) continue;

                    Posicao viz2[8];
                    int n2 = PegaVizinhosFechados(t, l2, c2, viz2);
                    if (n2 == 0) continue;

                    int o2, f2;
                    ContaVizinhos(t, l2, c2, &o2, &f2);
                    int m2 = t->celulas[l2][c2] - f2;

                    if (Subconjunto(viz1, n1, viz2, n2)) {
                        int diff_m = m2 - m1;
                        int diff_tam = n2 - n1;

                        if (diff_tam > 0) {
                            if (diff_m == 0) {
                                for (int k = 0; k < n2; k++) {
                                    if (!TemPosicao(viz1, n1, viz2[k].l, viz2[k].c)) {
                                        *l_out = viz2[k].l;
                                        *c_out = viz2[k].c;
                                        *acao_out = 1;
                                        return 1;
                                    }
                                }
                            } else if (diff_m == diff_tam) {
                                for (int k = 0; k < n2; k++) {
                                    if (!TemPosicao(viz1, n1, viz2[k].l, viz2[k].c)) {
                                        *l_out = viz2[k].l;
                                        *c_out = viz2[k].c;
                                        *acao_out = 2;
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}


int TestaSolucao(Tabuleiro *orig, int safe_l, int safe_c) {
    Tabuleiro *t = Alocar(orig->linhas, orig->colunas, orig->nrBombas);
    if (!t) return 0;

    for (int i = 0; i < orig->linhas; i++) {
        for (int j = 0; j < orig->colunas; j++) {
            t->celulas[i][j] = orig->celulas[i][j];
            t->estado[i][j] = 0;
        }
    }
    t->abertas = 0;
    t->gerado = 1;

    Abrir(t, safe_l, safe_c);

    while (!Ganhou(t)) {
        int l, c, acao;
        if (!ProximoPasso(t, &l, &c, &acao)) {
            Liberar(t);
            return 0; // Travou: precisaria de sorte
        }

        if (acao == 2) {
            MudaBandeira(t, l, c);
        } else {
            if (!Abrir(t, l, c)) {
                Liberar(t);
                return 0;
            }
        }
    }

    Liberar(t);
    return 1;
}
