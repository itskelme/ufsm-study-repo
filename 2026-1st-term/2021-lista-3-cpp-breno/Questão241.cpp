#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int vazio;
	double tempo, distancia, velocidade = 343, eco, passos;
	printf("\n\nEste programa imprime a dist√¢ncia entre duas pessoas que est√£o nas extremidades de uma");
	printf("caverna de acordo com o tempo que o eco demora para ir de uma extremidade at√© outra.");
	printf("\n\nDigite o tempo que o eco demora para voltar: ");
	scanf("%lf", &eco);
	tempo = (eco / 2);
	distancia = (velocidade * tempo);
	passos = (distancia / 0.82);

	printf("\n\nA dist‚ncia entre as duas pessoa È de %.2f metros.", distancia);
	printf("\n\nA quantidade de passos entre as duas pessoas È de %.1f passos.", passos);


	printf("Obrigado por utilizar este programa!");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}

