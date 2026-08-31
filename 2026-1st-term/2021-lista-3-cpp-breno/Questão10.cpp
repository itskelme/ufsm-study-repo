#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
        double raiz, a1, an, q, n, potencia;
		an = 0.0;
		
			printf("Este programa, lendo o primeiro termo e a razão, calcula o termo aₙ  de um progressão geométrica(P.G.);!");
			printf("\n\nDigite o primeiro termo: ");
				scanf ("%lf", &a1);
			printf("\n\nDigite a razão: ");
				scanf ("%lf", &q);
			printf("\n\nDigite qual o termo deseja encontrar na progressão: ");
				scanf ("%lf", &n);
			
			potencia = pow(q, n)*pow(q, -1);
			an = a1*potencia;
			
			printf ("\n\nO termo aₙ da expressão é %.2f!", an);

			printf ("\n\n> Agradeço a sua prefêrencia em utilizar este programa, disponha! ;-)\n"); 

	#ifdef WIN32
	system ("pause");
	#endif
	return 0;	
}