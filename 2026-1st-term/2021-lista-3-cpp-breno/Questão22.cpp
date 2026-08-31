#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <math.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float num, produto, mediaGeometrica = 0;
	int i = 1;
	produto = 1;
	printf("\n\nEste programa calcula a m�dia geom�trica de 04 n�meros reais! :)");
	printf("\n\nInforme os 04 n�meros! :)");
	do
	{
		printf("\n\nDigite o %d� n�mero: ", i);
		scanf("%f", &num);
		produto = produto * num;
		i++;
	} while (i <= 4);
	mediaGeometrica = pow(produto, 0.25);
	printf("\n\nA m�dia geom�trica dos 04 n�meros informados � %.2f! :)", mediaGeometrica);
	printf("\n\nObrigado por utilizar nosso programa! :)\n\n");
#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
