#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	double volume = 0.0, raio = 0.0, altura = 0.0, volTOTAL;
	printf("Este programa determina o volume de água de um reservatório em formato de cilindro onde apenas 80% está cheio");
	printf("\n\nDigite a altura do reservatório em metros: ");
	scanf("%lf", &altura);
	printf("\n\nDigite o raio da base do reservatório em metros: ");
	scanf("%lf", &raio);
	volume = (3.14 * (raio * raio)) * altura;
	volTOTAL = ((volume/100) * 80)* 1000;
	if(volTOTAL > 0){
		printf("\n\nO volume de água no reservatório é de %.2f litros\n\n", volTOTAL);
	}
	if(volTOTAL <= 0){
		printf("\n\ndigite valores válidos!");
	}
	printf("\n\nObrigado por ultilizar este programa!\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
	
