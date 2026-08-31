#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float P1, P2, T1, T2, V = 0;
	printf ("\n\nEste programa determina a velocidade de um móvel em km/h! :)");
	printf ("\n\nInforme a posição inicial do móvel (km): ");
	scanf ("%f", &P1);
	printf ("\n\nInforme a posição final do móvel (km): ");
	scanf ("%f", &P2);
	printf ("\n\nInforme o tempo inicial de deslocamento do móvel (h): ");
	scanf ("%f", &T1);
	printf ("\n\nInforme o tempo final de deslocamento do móvel (h): ");
	scanf ("%f", &T2);
	V = (P2 - P1) / (T2 - T1);
	printf ("\n\nA velocidade média do móvel é de %.2f km/h! :)", V);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
