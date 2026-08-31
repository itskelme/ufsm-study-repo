#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float raio, areaCirculo = 0;
	printf ("\n\nEste programa determina a área de um círculo qualquer!");
	printf ("\n\nDigite o valor do raio do círculo (m): ");
	scanf ("%f", &raio);
	areaCirculo = 3.1416 * (raio * raio);
	printf ("\n\nA área do círculo é de %.2f m²! :)", areaCirculo);	
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");	
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;	
}

