#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	double Mercury, MercuryW, Venus, VenusW, Mars, MarsW, Jupiter, JupiterW, Saturn, SaturnW, Uranus, UranusW, Neptune, NeptuneW, YourWeight;
	Mercury = 0.37;
	Venus = 0.88;
	Mars = 0.38;
	Jupiter = 2.64;
	Saturn = 1.15;
	Uranus = 1.17;
	Neptune = 1.18;
	YourWeight = 0.0;
	printf("\n> Este programa basicamente listar� o seu peso, em quilogramas na Terra, para todos os outros planetas dentro do Sistema Solar!");
	do{
	printf("\n\n>> Por favor usu�rio, voc� dever� digitar um valor (n�o negativo, n�o nulo e em quilogramas) do seu peso atual: ");
	scanf("%lf", &YourWeight);
	system("clear||cls");
}   while (YourWeight <= 0);
	printf("\n> Caro usu�rio, estes s�o seus pesos correspondentes ()arredondados nos outros planetas dentro do Sitema Solar, listados abaixo!");
	MercuryW = YourWeight * Mercury;
	VenusW = YourWeight * Venus;
	MarsW = YourWeight * Mars;
	JupiterW = YourWeight * Jupiter;
	SaturnW = YourWeight * Saturn;
	UranusW = YourWeight * Uranus;
	NeptuneW = YourWeight * Neptune;
	printf("\n\n\n>> Merc�rio ---> %.2f kg", MercuryW);
	printf("\n\n>> V�nus ---> %.2f kg", VenusW);
	printf("\n\n>> Marte ---> %.2f kg", MarsW);
	printf("\n\n>> J�piter ---> %.2f kg", JupiterW);
	printf("\n\n>> Saturno ---> %.2f kg", SaturnW);
	printf("\n\n>> Urano ---> %.2f kg", UranusW);
	printf("\n\n>> Netuno ---> %.2f kg", NeptuneW);
	printf("\n\n> Agrade�o a sua pref�rencia em utilizar este programa, disponha! ;-)\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
	
	
