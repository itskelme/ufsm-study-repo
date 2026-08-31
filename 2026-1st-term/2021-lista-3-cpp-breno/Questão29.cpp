#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int i, n;
	float dolar, euro, libras, valorRealPessoa, valorRealTotal;
	valorRealPessoa = 0;
	valorRealTotal = 0;
	printf ("\n\nEste programa realiza a conversão de moeda de n pessoas! :)");
	printf ("\n\nA conversão será realizada para R$! :)");
	printf ("\n\nInforme a quantidade de pessoas para conversão de moeda: ");
	scanf ("%d", &n);
	for (i = 1; i <= n; i++) {
		system ("cls");
		printf ("\n\nDigite o valor em DÓLARES que a pessoa %d possui: ", i);
		scanf ("%f", &dolar);
		printf ("\n\nDigite o valor em EUROS que a pessoa %d possui: ", i);
		scanf ("%f", &euro);
		printf ("\n\nDigite o valor em LIBRAS que a pessoa %d possui: ", i);
		scanf ("%f", &libras);
		
		if (dolar >= 0 && euro >= 0 && libras >= 0) {
			valorRealPessoa = (dolar * 5.1863) + (euro * 6.1261) + (libras * 7.15486);
			printf ("\n\nA pessoa %d possui, após conversão, R$ %.2f! :)", i, valorRealPessoa);
			valorRealTotal += valorRealPessoa;
		}
		else
			printf ("\n\nInforme valores válidos para cada uma das moedas! :)");
		printf ("\n\n");
		system ("Pause");
		system ("cls");
	}
	printf ("\n\nAs %d pessoas possuem, após conversão, R$ %.2f! :)", n, valorRealTotal);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
