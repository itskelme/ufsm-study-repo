#include "tabuleiro.h"
#include "solver.h"

int PosicaoValida(Tabuleiro* t, int l, int c) {
    if (!t) return 0;
    return (l >= 0 && l < t->linhas && c >= 0 && c < t->colunas);
}

Tabuleiro* Alocar(int linhas, int colunas, int bombas) {
    if (linhas <= 0 || colunas <= 0 || bombas <= 0 || bombas >= linhas * colunas) {
        return NULL;
    }

    Tabuleiro* t = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (!t) return NULL;

    t->linhas = linhas;
    t->colunas = colunas;
    t->nrBombas = bombas;
    t->abertas = 0;
    t->gerado = 0;

    t->celulas = (int**)malloc(t->linhas * sizeof(int*));
    t->estado = (int**)malloc(t->linhas * sizeof(int*));

    for (int i = 0; i < t->linhas; i++) {
        t->celulas[i] = (int*)calloc(t->colunas, sizeof(int));
        t->estado[i] = (int*)calloc(t->colunas, sizeof(int));
    }

    return t;
}

void Liberar(Tabuleiro* t) {
    if (!t) return;
    if (t->celulas) {
        for (int i = 0; i < t->linhas; i++) {
            free(t->celulas[i]);
        }
        free(t->celulas);
    }
    if (t->estado) {
        for (int i = 0; i < t->linhas; i++) {
            free(t->estado[i]);
        }
        free(t->estado);
    }
    free(t);
}

static void SorteiaBombas(Tabuleiro* t, int safe_l, int safe_c) {
    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            t->celulas[i][j] = 0;
        }
    }

    int colocadas = 0;
    while (colocadas < t->nrBombas) {
        int l = rand() % t->linhas;
        int c = rand() % t->colunas;

        if (abs(l - safe_l) <= 1 && abs(c - safe_c) <= 1 && (t->linhas * t->colunas - 9 >= t->nrBombas)) {
            continue;
        }
        if (l == safe_l && c == safe_c) {
            continue;
        }

        if (t->celulas[l][c] != -1) {
            t->celulas[l][c] = -1;
            colocadas++;
        }
    }

    for (int i = 0; i < t->linhas; i++) {
        for (int j = 0; j < t->colunas; j++) {
            if (t->celulas[i][j] == -1) continue;

            int count = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di;
                    int nj = j + dj;
                    if (PosicaoValida(t, ni, nj) && t->celulas[ni][nj] == -1) {
                        count++;
                    }
                }
            }
            t->celulas[i][j] = count;
        }
    }
}

void Preencher(Tabuleiro* t, int safe_l, int safe_c) {
    int tentativas = 0;
    const int limite = 150;

    do {
        SorteiaBombas(t, safe_l, safe_c);
        tentativas++;
        if (TestaSolucao(t, safe_l, safe_c)) {
            break;
        }
    } while (tentativas < limite);

    t->gerado = 1;
}

int Abrir(Tabuleiro* t, int l, int c) {
    if (!PosicaoValida(t, l, c)) return 1;
    if (t->estado[l][c] == 1 || t->estado[l][c] == 2) return 1;

    if (!t->gerado) {
        Preencher(t, l, c);
    }

    if (t->celulas[l][c] == -1) {
        t->estado[l][c] = 1;
        return 0; // pisou na bomba
    }

    t->estado[l][c] = 1;
    t->abertas++;

    if (t->celulas[l][c] == 0) {
        for (int dl = -1; dl <= 1; dl++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nl = l + dl;
                int nc = c + dc;
                if (PosicaoValida(t, nl, nc) && t->estado[nl][nc] == 0) {
                    Abrir(t, nl, nc);
                }
            }
        }
    }

    return 1;
}

void MudaBandeira(Tabuleiro* t, int l, int c) {
    if (!PosicaoValida(t, l, c) || t->estado[l][c] == 1) return;
    t->estado[l][c] = (t->estado[l][c] == 2 ? 0 : 2);
}

int Ganhou(Tabuleiro* t) {
    if (!t) return 0;
    int total = t->linhas * t->colunas;
    return (t->abertas == (total - t->nrBombas));
}

void ContaVizinhos(Tabuleiro* t, int l, int c, int *ocultos, int *bandeiras) {
    *ocultos = 0;
    *bandeiras = 0;

    for (int dl = -1; dl <= 1; dl++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dl == 0 && dc == 0) continue;
            int nl = l + dl;
            int nc = c + dc;
            if (PosicaoValida(t, nl, nc)) {
                if (t->estado[nl][nc] == 2) (*bandeiras)++;
                else if (t->estado[nl][nc] == 0) (*ocultos)++;
            }
        }
    }
}

void Imprimir(Tabuleiro* t, int revelado) {
    if (!t) return;

    printf("\n    ");
    for (int j = 0; j < t->colunas; j++) printf(" %2d ", j);
    printf("\n    ┌");
    for (int j = 0; j < t->colunas; j++) printf("───%s", (j < t->colunas - 1 ? "┬" : "┐\n"));

    for (int i = 0; i < t->linhas; i++) {
        printf(" %2d │", i);
        for (int j = 0; j < t->colunas; j++) {
            if (revelado) {
                if (t->celulas[i][j] == -1) printf(" * │");
                else if (t->celulas[i][j] > 0) printf(" %d │", t->celulas[i][j]);
                else printf("   │");
            } else {
                if (t->estado[i][j] == 2) {
                    printf(" P │");
                } else if (t->estado[i][j] == 0) {
                    printf(" . │");
                } else if (t->celulas[i][j] == -1) {
                    printf(" * │");
                } else if (t->celulas[i][j] == 0) {
                    printf("   │");
                } else {
                    printf(" %d │", t->celulas[i][j]);
                }
            }
        }
        printf("\n");
        if (i < t->linhas - 1) {
            printf("    ├");
            for (int j = 0; j < t->colunas; j++) printf("───%s", (j < t->colunas - 1 ? "┼" : "┤\n"));
        }
    }
    printf("    └");
    for (int j = 0; j < t->colunas; j++) printf("───%s", (j < t->colunas - 1 ? "┴" : "┘\n\n"));
}
