#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
        

	
	    double logaritmo1, logaritmo2, soma, base1, base2;
		int numero1, numero2;
	
		soma = 0.0;
		logaritmo1 = 0.0;
		logaritmo2 = 0.0;
		
		printf("Este programa determina a soma dos resultados dos logaritmos  de dois números inteiros não negativos!");
			
		    do {
			printf ("\n\nDigite o valor do primeiro logaritmado (inteiro e não negativo): ");
				scanf ("%d", &numero1);
			} while (numero1 <= 0);


			do {
			printf ("\n\nDigite o valor da base do primeiro logaritmo (não nulo e diferente de 1): ");
				scanf("%lf", &base1);
			} while (base1 < 2);

	
		    do {
			printf ("\n\nDigite o valor do segundo logaritmado (inteiro e não negativo): ");
				scanf("%d", &numero2);
			} while (numero2 <= 0);

		    do {
			printf ("\n\nDigite o valor da base do segundo logaritmo (não nulo e diferente de 1): ");
				scanf("%lf", &base2);
			} while (base2 < 2);

	

		logaritmo1 = log(numero1)/log(base1);
		logaritmo2 = log(numero2)/log(base2);
		soma = logaritmo1 + logaritmo2;
		
	

		printf("\n\nA soma dos resultados dos logaritmos de dois números inteiros e não negativos, é de %lf!\n", soma);

		printf("\n\nObrigado por ultilizar este programa!\n");

	#ifdef WIN32
	system ("pause");
	#endif
	return 0;	
}