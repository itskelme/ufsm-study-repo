#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tabuleiro.h"
#include "solver.h"

#ifdef _WIN32
  #include <windows.h>
  #define DORMIR(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define DORMIR(ms) usleep((ms) * 1000)
#endif


static void LimpaBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}


static void ModoJogador(Tabuleiro *t) {
    int fim = 0;
    int venceu = 0;

    printf("\nComandos:\n");
    printf("  r L C -> Revelar celula (ex: r 2 3)\n");
    printf("  b L C -> Colocar/remover bandeira (ex: b 2 3)\n");
    printf("  s     -> Dica da proxima jogada\n");
    printf("  q     -> Sair para o menu\n");

    while (!fim) {
        printf("\nBombas no campo: %d", t->nrBombas);
        Imprimir(t, 0);

        printf("Comando: ");
        char comando;
        if (scanf(" %c", &comando) != 1) {
            LimpaBuffer();
            continue;
        }

        if (comando == 'q' || comando == 'Q') {
            printf("Jogo cancelado.\n");
            break;
        }

        if (comando == 's' || comando == 'S') {
            int l, col, acao;
            if (ProximoPasso(t, &l, &col, &acao)) {
                printf("\n[DICA] Posicao (%d, %d): %s.\n",
                       l, col, (acao == 1 ? "ABRIR" : "MARCAR BANDEIRA"));
            } else {
                printf("\n[DICA] Sem jogadas logicas imediatas.\n");
            }
            continue;
        }

        int l, col;
        if (scanf("%d %d", &l, &col) != 2) {
            LimpaBuffer();
            printf("Coordenadas invalidas.\n");
            continue;
        }

        if (!PosicaoValida(t, l, col)) {
            printf("Posicao fora do tabuleiro (%dx%d)!\n", t->linhas, t->colunas);
            continue;
        }

        if (comando == 'b' || comando == 'B') {
            MudaBandeira(t, l, col);
        } else if (comando == 'r' || comando == 'R') {
            if (t->estado[l][col] == 2) {
                printf("Tire a bandeira antes de abrir.\n");
                continue;
            }

            if (!Abrir(t, l, col)) {
                fim = 1;
                venceu = 0;
            } else if (Ganhou(t)) {
                fim = 1;
                venceu = 1;
            }
        }
    }

    if (fim) {
        if (venceu) {
            printf("\nParabens! Voce venceu o Campo Minado!\n");
        } else {
            printf("\nBomba! Fim de jogo.\n");
        }
        Imprimir(t, 1);
    }
}


static void ModoComputador(Tabuleiro *t) {
    int passos = 0;

    printf("\nO computador esta resolvendo o tabuleiro...\n");

    if (t->abertas == 0) {
        passos++;
        int mid_l = t->linhas / 2;
        int mid_c = t->colunas / 2;
        Abrir(t, mid_l, mid_c);
        printf("\n[Passo %d] Abre em (%d, %d)", passos, mid_l, mid_c);
        Imprimir(t, 0);
        DORMIR(120);
    }

    while (!Ganhou(t)) {
        int l = 0, col = 0, acao = 0;
        if (!ProximoPasso(t, &l, &col, &acao)) {
            printf("Sem jogadas deterministas restantes.\n");
            break;
        }

        passos++;
        if (acao == 2) {
            MudaBandeira(t, l, col);
            printf("\n[Passo %d] Bandeira em (%d, %d)", passos, l, col);
        } else {
            if (!Abrir(t, l, col)) {
                printf("\n[Passo %d] O computador atingiu uma bomba em (%d, %d).\n", passos, l, col);
                Imprimir(t, 1);
                return;
            }
            printf("\n[Passo %d] Abre em (%d, %d)", passos, l, col);
        }

        Imprimir(t, 0);
        DORMIR(120);
    }

    if (Ganhou(t)) {
        printf("\nCampo Minado resolvido com sucesso em %d passos!\n", passos);
        Imprimir(t, 1);
    }
}


int main(void) {
    srand((unsigned int)time(NULL));

    while (1) {
        printf("\n--- CAMPO MINADO (MODO NO-GUESS) ---\n");
        printf("1. Jogar manualmente\n");
        printf("2. Computador resolver\n");
        printf("3. Sair\n");
        printf("Opcao: ");

        int opcao;
        if (scanf("%d", &opcao) != 1) {
            LimpaBuffer();
            continue;
        }

        if (opcao == 3) {
            printf("Ate a proxima!\n");
            break;
        }

        if (opcao != 1 && opcao != 2) {
            printf("Opcao invalida.\n");
            continue;
        }

        int nrLinhas, nrColunas, nrBombas;
        printf("\nNumero de linhas: ");
        if (scanf("%d", &nrLinhas) != 1 || nrLinhas <= 0) {
            LimpaBuffer();
            continue;
        }

        printf("Numero de colunas: ");
        if (scanf("%d", &nrColunas) != 1 || nrColunas <= 0) {
            LimpaBuffer();
            continue;
        }

        printf("Numero de bombas (1 a %d): ", (nrLinhas * nrColunas) - 1);
        if (scanf("%d", &nrBombas) != 1 || nrBombas <= 0 || nrBombas >= nrLinhas * nrColunas) {
            LimpaBuffer();
            printf("Quantidade de bombas invalida.\n");
            continue;
        }

        Tabuleiro* t = Alocar(nrLinhas, nrColunas, nrBombas);
        if (!t) {
            printf("Erro ao alocar memoria para o tabuleiro.\n");
            continue;
        }

        if (opcao == 1) {
            ModoJogador(t);
        } else {
            ModoComputador(t);
        }

        Liberar(t);
    }

    return 0;
}
