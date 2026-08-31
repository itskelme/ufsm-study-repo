#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>

int main (void) 
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float x1, y1, x2, y2, distancia = 0;
	printf ("\n\nEste programa determina a distância entre dois pontos (A e B) no Plano Cartesiano! :)");
	printf ("\n\nDigite a coordenada x1 do ponto A: ");
	scanf ("%f", &x1);
	printf ("\n\nDigite a coordenada y1 do ponto A: ");
	scanf ("%f", &y1);
	printf ("\n\nDigite a coordenada x2 do ponto B: ");
	scanf ("%f", &x2);
	printf ("\n\nDigite a coordenada y2 do ponto B: ");
	scanf ("%f", &y2);
	distancia = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	printf ("\n\nA distância entre os dois pontos (A e B) é de %.2f! :)", distancia);	
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
