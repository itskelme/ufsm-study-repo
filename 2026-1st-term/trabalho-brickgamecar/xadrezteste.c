#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define fill '-'

char board[5][6];
int sr, sc, er, ec;

void inic_matriz(){
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (j == 0) {
                board[i][j] = j;
            }
            board[i][j] = fill;
        }
    }

    board[2][1] = 'x'; board[2][2] = 'y'; board[0][2] = 'Z';
    board[0][4] = 'Y'; board[2][4] = 'z'; board[2][5] = 'X';
}

void imprimir() {
    printf("X  0  1  2  3  4  5\n");
    for(int i = 0; i < 5; i++) {
        printf("%d", i);
        for(int j = 0; j < 6; j++) {
            printf("  %c", board[i][j]);
        }
        printf("\n");
    }
}

void limpar_tela(int qntLinhas){
    for(int i = 0; i < qntLinhas; i++){
        printf("\x1b[1F");
        printf("\x1b[2K");
    }
}

bool is_upper() {
    char peca = board[er][ec];
    if (peca == 'X' || peca == 'Y' || peca == 'Z')
        return true;
    return false;
}

bool is_path_blocked() {
    int p = 1, i, j;
    switch (board[sr][sc]) {
    case 'x':
        if ((er < sr) && (ec < sc)) {
            for (i = sr - 1; i > er; i--) {
                for (j = sc - 1; j > ec; j--) {
                    if ((i + p == sr) && (j + p == sc)) {
                        if (board[i][j] != fill) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
                p++;
            }

        }
        if ((er > sr) && (ec > sc)) {
            for (i = sr + 1; i < er; i++) {
                for (j = sc + 1; j < ec; j++) {
                    if ((i - p == sr) && (j - p == sc)) {
                        if (board[i][j] != fill) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
                p++;
            }
        }
        if ((er > sr) && (ec < sc)) {
            for (i = sr + 1; i < er; i++) {
                for (j = sc - 1; j > ec; j--) {
                    if ((i - p == sr) && (j + p == sc)) {
                        if (board[i][j] != fill) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
                p++;
            }
        }
        if ((er < sr) && (ec > sc)) {
            for (i = sr - 1; i > er; i--) {
                for (j = sc + 1; j < ec; j++) {
                    if ((i + p == sr) && (j - p == sc)) {
                        if (board[i][j] != fill) {
                            return true;
                        }
                    }
                }
                p++;
            }
        }
        break;
    case 'z':
        if ((er < sr) && (ec == sc)) {
            for (i = sr - 1; i > er; i--) {
                if (board[i][sc] != fill) {
                    return true;
                }
            }
        }
        if ((er > sr) && (ec == sc)) {
            for (i = sr + 1; i < er; i++) {
                if (board[i][sc] != fill) {
                    return true;
                }
            }
        }
        if ((er == sr) && (ec < sc)) {
            for (j = sc - 1; j > ec; j--) {
                if (board[sr][j] != fill) {
                    return true;
                }
            }
        }
        if ((er == sr) && (ec > sc)) {
            for (j = sc + 1; j < ec; j++) {
                if (board[sr][j] != fill) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_valid_x_move() {
    if (abs(er - sr) == abs(ec - sc) && abs(er - sr) <= 2) {
        if (!is_path_blocked()) {
            if (board[er][ec] == fill || is_upper()) {
                return true;
            }
        }
    }
    return false;
}


bool is_valid_y_move() {
    if(is_upper() == true || board[er][ec] == fill){
        if ((ec == sc + 2 || ec == sc - 2) && er == sr)
            return true;
        if ((er == sr + 2 || er == sr - 2) && ec == sc)
            return true;
    }        
    return false;
}

bool is_valid_z_move() {
    if (er == sr && (ec >= sc - 2 && ec <= sc + 2) ||
        ec == sc && (er >= sr - 2 && er <= sr + 2)) {
        if (is_path_blocked() == false) {
            if (board[er][ec] == fill || is_upper() && board[er][ec] != 'Y') {
                return true;
            }
        }
    }
    return false;
}

void make_move() {
    if ((er < 5 || ec < 6) && (er > 0 || ec > 0)) {
        switch (board[sr][sc]) {
        case 'x':
            if (is_valid_x_move() == true) {
                board[er][ec] = board[sr][sc];
                board[sr][sc] = fill;
            }
            else
                printf("movimento do x inválido\n");
            break;
        case 'y':
            if (is_valid_y_move() == true) {
                board[er][ec] = board[sr][sc];
                board[sr][sc] = fill;
            }
            else
                printf("movimento do y inválido\n");
            break;
        case 'z':
            if (is_valid_z_move() == true) {
                board[er][ec] = board[sr][sc];
                board[sr][sc] = fill;
            }
            else
                printf("movimento do z inválido");
            break;
        default: printf("movimento inválido");
            break;
        }
    }
}

bool venceu() {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            if ((board[i][j] == 'X') || (board[i][j] == 'Y') || (board[i][j] == 'Z')) {
                return false;
            }
        }
    }
    return true;
}

void loc_coordenada(char resp) {
    switch (resp) {
    case 'x':
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (board[i][j] == 'x') {
                    sr = i;
                    sc = j;
                }
            }
        }
        break;
    case 'y':
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (board[i][j] == 'y') {
                    sr = i;
                    sc = j;
                }
            }
        }
        break;
    case 'z':
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (board[i][j] == 'z') {
                    sr = i;
                    sc = j;
                }
            }
        }
        break;
    default:
        printf("peça inválida\n");
        break;
    }
}
int main() {
    inic_matriz();

    printf("BEM-VINDO AO MINI CHESS DO JOAQUIM\n");
    int partida = 1;
    char resposta;
    do {
        printf("\n\nmovimento %i\n", partida);
        imprimir();
        printf("\n");
        printf("qual a peça que você quer:\n");
        scanf(" %c", &resposta);
        loc_coordenada(resposta);
        printf("agora qual a coordenada que você quer que ela vá (linha, coluna)\nlembrando que:"
            "\nx se move na diagonal\ny se move pulando 1 quadrado diretamente pro lado\nz se move em linha reta\n");
        scanf("%i %i", &er, &ec);
        limpar_tela(21);
        make_move();
        venceu();
        partida++;
    } while (venceu() == false);
    printf("PARABÉNS, VOCÊ VENCEU!!\n");
}