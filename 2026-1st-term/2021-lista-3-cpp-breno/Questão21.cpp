#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	float C, K, F;
	K = 0;
	F= 0;
	setlocale (LC_ALL, "Portuguese");
	printf ("\n\nEste programa realiza conversões entre escalas termométricas! :)");
	printf ("\n\nInforme a temperatura em °C: ");
	scanf ("%f", &C);
	K = C + 273.15;
	F = (C * 1.8) + 32;
	printf ("\n\n");
	system ("Pause");
	system ("cls");
	printf ("\n\nConversão entre as escalas termométricas! :)");
	printf ("\n\nA temperatura informada foi %.2f°C! :)", C);
	printf ("\n\nApós conversão: %.2f°C = %.2f°F = %.2fK!", C, F, K);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
