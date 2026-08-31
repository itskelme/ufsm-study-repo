#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float v, a, t, s;
	t = 0;
	s = 0;
	printf ("\n\nEste programa determina o tempo e o espaço de encontro entre um carro e um caminhão!");
	printf ("\n\nInforme a aceleção do carro (m/s²): ");
	scanf ("%f", &a);
	printf ("\n\nInforme a velocidade do caminhão (m/s): ");
	scanf ("%f", &v);
	t = (2 * v) / a;
	s = v * t;
	printf ("\n\nO carro e o caminhão se encontram %.2f segundos após o semáforo abrir! :)", t);
	printf ("\n\nDessa forma, os dois se encontram a uma distância de %.2f m! :)", s);	
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
