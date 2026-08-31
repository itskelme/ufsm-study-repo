#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int num, valor , divisores , vazio;
	valor = 1;
	divisores = 0;
	printf("Este programa determina se um número inteiro é primo");
	printf("\n\nEscreva o número: ");
	scanf("%d", &num);
	if (num > 0){
		while(valor <= num){
			if(num % valor == 0){
				divisores++ ;
			}
			valor++ ;

		}
		if(divisores == 2){
			printf("\n\nO número %d é primo", num);
		}
		else if(divisores > 2){
			printf("\n\n O número %d não é primo", num);
		}

	}
	else if(divisores < 2){
		printf("\n\nO número é negativo ou igual a zero!");
	}
	printf("\n\nObrigado por ultilizar este programa!");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
