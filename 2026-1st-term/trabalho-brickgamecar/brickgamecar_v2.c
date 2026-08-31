 #include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define DORMIR(ms)   Sleep(ms)
  #define TECLA_DISP() _kbhit()
  #define TECLA()      _getch()
  #define LIMPAR()     system("cls")
#else
  #include <unistd.h>
  #define DORMIR(ms)   usleep((ms)*1000)
  #define TECLA_DISP() 0
  #define TECLA()      getchar()
  #define LIMPAR()     system("clear")
#endif

/* ------------------------------------------------------------------
 * Variaveis globais — espelho direto do enunciado
 * ------------------------------------------------------------------ */
char mat[30][60];

int tamY    = 30;
int tamX    = 60;

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
 * Prototipos
 * ------------------------------------------------------------------ */
void inicializaObjetos(void);
void timer(void);
void keyPress(char key);
bool noBorder(char direcao);
static void gameOver(void);

/* ------------------------------------------------------------------
 * noBorder
 * Encontra o centro do carro escaneando a linha com tres 'C' seguidos.
 * ------------------------------------------------------------------ */
bool noBorder(char direcao) {

    int i, j;
    int carroCol = 0;

    for (i = 1; i < tamY - 1; i++)
        for (j = 2; j < tamX - 2; j++)
            if (mat[i][j - 1] == 'C' && mat[i][j] == 'C' && mat[i][j + 1] == 'C')
                carroCol = j;

    if (direcao == 'l') return carroCol - 1 <= 1;
    if (direcao == 'r') return carroCol + 1 >= tamX - 2;
    return false;
}

/* ------------------------------------------------------------------
 * inicializaObjetos  //chamada automaticamente
 * ------------------------------------------------------------------ */
void inicializaObjetos(void) {

    int i, j;
    srand((unsigned)time(NULL));
    for (i = 0; i < tamY; i++) for (j = 0; j < tamX; j++) mat[i][j] = ' ';
    for (i = 0; i < tamY; i++) { mat[i][0] = '|'; mat[i][tamX-1] = '|'; }
    for (j = 0; j < tamX; j++) { mat[0][j] = '-'; mat[tamY-1][j] = '-'; }
    for (i = 0; i < 7; i++) mat[(tamY-4) + FORMA[i][1]][(tamX/2) + FORMA[i][0]] = 'C';
}

/* ------------------------------------------------------------------
 * timer  //chamada automaticamente
 * 1) Move inimigos.
 * 2) Verifica integridade do carro.
 * 3) Gera inimigos.
 * ------------------------------------------------------------------ */
void timer(void) {

    int i;
    int j;
    int count = 0;
    int carroLin = 0;
    int carroCol = 0;

    /* 1) move inimigos de baixo para cima */

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

    /* localiza centro do carro (linha com tres 'C' consecutivos) */

    for (i = 1; i < tamY - 1; i++)
        for (j = 2; j < tamX - 2; j++)
            if (mat[i][j - 1] == 'C' && mat[i][j] == 'C' && mat[i][j + 1] == 'C') {
                carroLin = i;
                carroCol = j;
            }

    /* 2) verifica integridade do carro */

    bool integro =

        mat[carroLin - 1][carroCol    ] == 'C' &&

        mat[carroLin    ][carroCol - 1] == 'C' &&
        mat[carroLin    ][carroCol    ] == 'C' &&
        mat[carroLin    ][carroCol + 1] == 'C' &&

        mat[carroLin + 1][carroCol    ] == 'C' &&

        mat[carroLin + 2][carroCol - 1] == 'C' &&
        mat[carroLin + 2][carroCol + 1] == 'C';

    if (!integro) {
        gameOver();
        return;
    }

    /* 3) gera inimigos */

    if (rand() % 4 == 0) {

        j = 2 + rand() % (tamX - 4);

        if (

            mat[1][j    ] == ' ' &&

            mat[2][j - 1] == ' ' &&
            mat[2][j    ] == ' ' &&
            mat[2][j + 1] == ' ' &&

            mat[3][j    ] == ' ' &&

            mat[4][j - 1] == ' ' &&
            mat[4][j + 1] == ' '

        ) {
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
 * ------------------------------------------------------------------ */
void keyPress(char key) {

    int i;
    int j;
    int carroLin = 0;
    int carroCol = 0;

    /* localiza centro do carro (linha com tres 'C' consecutivos) */

    for (i = 1; i < tamY - 1; i++)
        for (j = 2; j < tamX - 2; j++)
            if (mat[i][j - 1] == 'C' && mat[i][j] == 'C' && mat[i][j + 1] == 'C') {
                carroLin = i;
                carroCol = j;
            }

    i = carroLin;
    j = carroCol;

    /* apaga carro */

    mat[i - 1][j    ] = ' ';

    mat[i    ][j - 1] = ' ';
    mat[i    ][j    ] = ' ';
    mat[i    ][j + 1] = ' ';

    mat[i + 1][j    ] = ' ';

    mat[i + 2][j - 1] = ' ';
    mat[i + 2][j + 1] = ' ';

    /* movimenta */

    if      ((key == 'a' || key == 'A') && !noBorder('l')) carroCol--;
    else if ((key == 'd' || key == 'D') && !noBorder('r')) carroCol++;

    /* redesenha */

    i = carroLin;
    j = carroCol;

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
