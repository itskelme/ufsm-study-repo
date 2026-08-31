#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void) 
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float km, m, cm, mm;
	m = 0;
	cm = 0;
	mm = 0;
	printf ("\n\nEste programa converte uma valor em km para m, cm e mm!");
	printf ("\n\nDigite o valor em km: ");
	scanf ("%f", &km);
	m = 1000 * km;
	cm = 100 * m;
	mm = 1000 * cm;
	printf ("\n\nConversão de valores: ");
	printf ("\n\n%.2f km = %.2f m = %.2f cm = %.2f mm!", km, m, cm, mm);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
