#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int n, maior, menor, par, impar, i, atual, anterior, op, div;
	double valor, soma, media;
	soma = 0.0;
	media = 0.0;
	n = 0;
	maior = -2147483647;
	menor = 2147483647;
	i = 1 ;
	atual = 0 ;
	anterior = 0 ;
	op = 0;
	div = 0;
	par = 0;
	impar = 0;
	par = 0;
	printf("\n> Este programa basicamente irá ler 'n' números inteiros não negativos e determinará qual é o maior, o menor, ");
	printf("\na quantidade de pares, a quantidade de ímpares e a média de todos os números!");
	printf("\n\n>> Por favor usuário, você deverá digitar um valor (inteiro e não negativo) para quantidade de números que quer determinar: ");
	scanf("%d", &n);
	
	while(i <= n){
		printf("\n\nDigite o valor do %dº número: ", i);
		scanf("%lf", &valor);
		soma = soma + valor;
		if(valor > maior){
			maior = valor;
		}
		if(valor < menor){
		menor = valor;
		}
		if(valor == 0)
		printf("\n\nO número é 0!");
		if( valor != 0){
			div = (valor / 2);
		
			if((div * 2) == valor){
			par = par + 1;
			}
			if((div * 2) != valor){
			impar = impar + 1;
			}
		}
		i++;
	}
	media = soma/n;
	
	printf("\n\nSão(é) %d número(s) par(es)!", par);
	printf("\n\nSão(é) %d número(s) ímpar(es)!", impar);
	printf("\n\nA média do(s) %d número(s) é %.2f!", n, media);
	printf("\n\nO maior do(s) %d número(s) é %d!", n, maior);
	printf("\n\nO menor do(s) %d número(s) é %d!", n, menor);
	
	printf("\n\nObrigado por utilizar nosso programa!\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
	
