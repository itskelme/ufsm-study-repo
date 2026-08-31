#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	double taxa, caIn, caFi;
	caIn = 0.0;
	caFi = 0.0;
	taxa = 0.0;
	printf("\n\nEste programa, em uma poupança, com base na leitura do capital inicial e da taxa de juros, determina o capital final ao término de um ano!");
	printf("\n\nDigite o capital inicial (R$): "),
	scanf("%lf", &caIn);
	printf("\nDigite a taxa de juros simples ao ano (%): ");
	scanf("%lf", &taxa);
	
	caFi = (caIn + ((caIn * (taxa/100))));
	printf("\nO capital final dessa poupança, em um ano, é de %.2f reais!", caFi);
	printf("\n\nObrigado por ultilizar este programa!\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
	
