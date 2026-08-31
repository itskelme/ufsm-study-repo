#include <stdio.h>

void papum(int n) {
    for (int i = 1; i <= n; i++) {
        //  verificamos se é múltiplo de 3 (regra do "pa")
        if (i % 3 == 0) {
            // múltiplo de 3, checamos se o último dígito é 5
            if (i % 10 == 5) {
                printf("papum\n");
            } else {
                printf("pa\n");
            }
        } 
        // Se NÃO for múltiplo de 3, checamos apenas o último dígito
        else {
            if (i % 10 == 5) {
                printf("pum\n");
            } else {
                printf("%d\n", i);
            }
        }
    }
}

int main() {
  	// testar primeiros 20
    papum(20);
    return 0;
}
