#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define DORMIR(ms)     Sleep(ms)
  #define TECLA_DISP()   _kbhit()
  #define TECLA()        _getch()
  #define LIMPAR()       system("cls")
#else
  #include <unistd.h>
  #define DORMIR(ms)     usleep((ms)*1000)
  #define TECLA_DISP()   0
  #define TECLA()        getchar()
  #define LIMPAR()       system("clear")
#endif

/* ------------------------------------------------------------------
 * Variaveis globais � espelho direto do enunciado
 * mat[lin][col] em C == mat.Cells[col, lin] em Delphi (row-major)
 * ------------------------------------------------------------------ */
char mat[30][60];
int tamY = 30;   /* total de linhas  */
int tamX = 60;   /* total de colunas */

typedef struct { int lin; int col; } Carro;
Carro carro;

int  pontuacao = 0;
bool rodando   = true;

/* offsets dos 7 pixels do carro relativos ao centro (col, lin) */
static const int FORMA[7][2] = {
    { 0,-1},
    {-1, 0}, {0, 0}, {1, 0},
    { 0, 1},
    {-1, 2}, {1, 2}
};

/* ------------------------------------------------------------------
 * Prototipos das funcoes exigidas pelo enunciado
 * ------------------------------------------------------------------ */
void inicializaObjetos(void);
void timer(void);
void keyPress(char key);
bool noBorder(char direcao);
static void gameOver(void);

/* ------------------------------------------------------------------
 * noBorder
 * Retorna verdadeiro se o carro esta na borda ('l' ou 'r').
 * O carro tem 3 pixels de largura: col-1, col, col+1.
 * ------------------------------------------------------------------ */
bool noBorder(char direcao) {
    if (direcao == 'l') return carro.col - 1 <= 1;
    if (direcao == 'r') return carro.col + 1 >= tamX - 2;
    return false;
}

/* ------------------------------------------------------------------
 * InicializaObjetos  //chamada automaticamente
 * Coloca o carro no centro inferior da tela.
 * ------------------------------------------------------------------ */
void inicializaObjetos(void) {
    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < tamY; i++) for (j = 0; j < tamX; j++) mat[i][j] = ' ';
    for (i = 0; i < tamY; i++) { mat[i][0] = '|'; mat[i][tamX-1] = '|'; }
    for (j = 0; j < tamX; j++) { mat[0][j] = '-'; mat[tamY-1][j] = '-'; }
    carro.lin = tamY - 4;
    carro.col = tamX / 2;
    for (i = 0; i < 7; i++) mat[carro.lin + FORMA[i][1]][carro.col + FORMA[i][0]] = 'C';
}

/* ------------------------------------------------------------------
 * Timer  //chamada automaticamente
 * 1) Varre a tela procurando obstaculos e os faz descer.
 * 2) Verifica se o carro esta integro (todos os 7 pixels presentes).
 * 3) Chama a funcao para gerar outros veiculos.
 * ------------------------------------------------------------------ */
void timer(void) {
    int i, j, count;
    count = 0;

    /* 1) Varre de baixo para cima � [0,0] no canto superior esquerdo */
    for (i = tamY - 2; i >= 1; i--) {
        for (j = 1; j < tamX - 1; j++) {
            if (mat[i][j] == 'X') {
                mat[i][j] = ' ';
                if (i + 1 < tamY - 1)
                    mat[i + 1][j] = 'X';
                else
                    pontuacao++;
                count++;
            }
        }
    }

    /* 2) Verifica se o carro esta integro (todos os 7 pixels presentes).
          Caso nao esteja, houve colisao. */
    bool integro =
        mat[carro.lin - 1][carro.col    ] == 'C' &&
        mat[carro.lin    ][carro.col - 1] == 'C' &&
        mat[carro.lin    ][carro.col    ] == 'C' &&
        mat[carro.lin    ][carro.col + 1] == 'C' &&
        mat[carro.lin + 1][carro.col    ] == 'C' &&
        mat[carro.lin + 2][carro.col - 1] == 'C' &&
        mat[carro.lin + 2][carro.col + 1] == 'C';

    if (!integro) {
        gameOver();
        return;
    }

    /* 3) Gera outros veiculos (centro na linha 2 para a forma caber inteira) */
    if (rand() % 4 == 0) {
        j = 2 + rand() % (tamX - 4);
        if (mat[1][j    ] == ' ' &&
            mat[2][j - 1] == ' ' && mat[2][j] == ' ' && mat[2][j + 1] == ' ' &&
            mat[3][j    ] == ' ' &&
            mat[4][j - 1] == ' ' && mat[4][j + 1] == ' ')
        {
            mat[1][j    ] = 'X';
            mat[2][j - 1] = 'X';
            mat[2][j    ] = 'X';
            mat[2][j + 1] = 'X';
            mat[3][j    ] = 'X';
            mat[4][j - 1] = 'X';
            mat[4][j + 1] = 'X';
        }
    }

    (void)count;

    /* renderiza */
    LIMPAR();
    printf("Pontuacao: %d\n", pontuacao);
    for (i = 0; i < tamY; i++) {
        for (j = 0; j < tamX; j++)
            printf("%c", mat[i][j]);
        printf("\n");
    }
}

/* ------------------------------------------------------------------
 * keyPress  //chamada automaticamente
 * Move o carro para a esquerda (A) ou direita (D).
 * ------------------------------------------------------------------ */
void keyPress(char key) {
    int i = carro.lin;
    int j = carro.col;

    /* apaga carro */
    mat[i - 1][j    ] = ' ';
    mat[i    ][j - 1] = ' ';
    mat[i    ][j    ] = ' ';
    mat[i    ][j + 1] = ' ';
    mat[i + 1][j    ] = ' ';
    mat[i + 2][j - 1] = ' ';
    mat[i + 2][j + 1] = ' ';

    if      ((key == 'a' || key == 'A') && !noBorder('l')) carro.col--;
    else if ((key == 'd' || key == 'D') && !noBorder('r')) carro.col++;

    /* redesenha carro */
    i = carro.lin;
    j = carro.col;
    mat[i - 1][j    ] = 'C';
    mat[i    ][j - 1] = 'C';
    mat[i    ][j    ] = 'C';
    mat[i    ][j + 1] = 'C';
    mat[i + 1][j    ] = 'C';
    mat[i + 2][j - 1] = 'C';
    mat[i + 2][j + 1] = 'C';
}

/* ------------------------------------------------------------------
 * gameOver
 * Espelho do bloco Delphi:
 *   timer.Enabled        := false;
 *   timer_score.Enabled  := false;
 *   application.MessageBox('Game Over', ':/');
 *   application.terminate;
 * ------------------------------------------------------------------ */
static void gameOver(void) {
    rodando = false;
    printf("\n*** GAME OVER ***  Score: %d\n", pontuacao);
}

/* ------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------ */
int main(void) {
    inicializaObjetos();
    timer();

    while (rodando) {
        if (TECLA_DISP()) {
            char key = (char)TECLA();
            keyPress(key);
        }
        DORMIR(200);
        timer();
    }
    return 0;
}
