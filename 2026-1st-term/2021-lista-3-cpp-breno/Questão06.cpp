#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void) 
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int i, soma = 0;
	printf ("\n\nEste programa imprime e soma os números inteiros múltiplos de 7 entre 100 e 300!");
	printf ("\n\nNúmeros inteiros múltiplos de 7 entre 100 e 300: ");
	for (i = 100; i <= 300; i++) {
		if (i%7 == 0) {
			printf ("  %d", i);
			soma += i; //soma = soma + i
		}
	}
	printf ("\n\nO valor soma dos números inteiros múltiplos de 7 entre 100 e 300 é %d! :)", soma);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
