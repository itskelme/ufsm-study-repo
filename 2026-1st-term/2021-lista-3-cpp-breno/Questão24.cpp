#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	double temp, fusao, ebulicao;
	temp = 0;
	fusao = -94.6;
	ebulicao = 56.1;
	printf("\n\n> Este programa lhe dirá com base no valor da temperatura se, ao nível do mar,");
	printf("a acetona está em temperatura inferior ao ponto de fusão ou em temperatura superior ao ponto de ebulição! :)");
	printf("\n\n Por favor usuário, digite a temperatura em graus Celsius: ");
	scanf("%lf", &temp);
	if(temp < fusao)
		printf("\n> A acetona está em temperatura inferior ao ponto de fusão! :)");
	if(temp > ebulicao)
		printf("\n> A acetona está em temperatura superior ao ponto de ebulição! :)");
	if(temp > fusao && temp < ebulicao)
		printf("\n> A acetona não está nem no ponto de ebulição e nem no ponto de fusão! :)");
	if(temp == fusao)
		printf("\n> A acetona está exatamente no ponto de fusão! :)");
	if(temp == ebulicao)
		printf("\n> A acetona está exatamente no ponto de ebulição! :)");

	printf("\n\n> Agradeço a sua prefêrencia em utilizar este programa, disponha! ;-)\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}

