#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int i, num, expoente, potencia = 1;
	printf ("\n\nEste programa determina a potência de um número inteiro qualquer, considerando um exponente natural! :)");
	printf ("\n\nInforme o valor do número base na potenciação: ");
	scanf ("%d", &num);
	printf ("\n\nInforme o valor do expoente para cálculo de potenciação: ");
	scanf ("%d", &expoente);
	if (expoente >= 0){
		for (i = 1; i <= expoente; i++)
			potencia = potencia * num;
		
		printf ("\n\nO resultado da potenciação é %d! :)", potencia);
	}
	else 
		printf ("\n\nInforme um expoente natural para a realização do cálculo! :)");
	
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
