#include <stdio.h>

// Funcao que calcula o seno de um angulo em radianos usando a serie de Taylor
// seno(x) = x - x^3/3! + x^5/5! - x^7/7! ...
// paramos de somar quando o termo tiver valor absoluto menor que 1e-10

// pra isso eu reaproveitei as funcoes de potencia e fatorial que ja fiz


// funcao potencia (do exercicio 1)
double potencia(double x, int n)
{
	double resultado;
	int i;

	if (n < 0) {
		return 1.0 / potencia(x, -n);
	}
	resultado = 1.0;
	for (i = 1; i <= n; i++) {
		resultado = resultado * x;
	}
	return resultado;
}


// funcao fatorial (do exercicio 2)
// usando double pro retorno conforme dica do professor
double fatorial(long n)
{
	double resultado;
	long i;

	if (n < 0) {
		return -1;
	}
	resultado = 1;
	for (i = 1; i <= n; i++) {
		resultado = resultado * i;
	}
	return resultado;
}


// funcao auxiliar pra calcular o valor absoluto de um double
// (o professor comentou nas dicas que essa funcao ajuda)
double valor_absoluto(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}


// funcao seno usando serie de Taylor
double seno(double x)
{
	double soma = 0;  // soma eh acumulador, comeca em 0 (padrao de somatori)
	double termo;     // vai guardar cada termo da serie
	int n = 1;        // expoente da serie: 1, 3, 5, 7, ...
	int sinal = 1;    // vai alternar entre +1 e -1 a cada passo

	// repete ate o termo ficar muito pequeno (menor que 1e-10 em absoluto)
	do {
		// termo = sinal * x^n / n!
		termo = sinal * potencia(x, n) / fatorial(n);
		soma = soma + termo;

		// prepara o proximo termo: expoente aumenta 2 em 2, sinal inverte
		n = n + 2;
		sinal = -sinal;
	} while (valor_absoluto(termo) >= 1e-10);

	return soma;
}


void testa_seno()
{
	double pi = 3.14159265;
	printf("Teste do seno\n");
	printf("seno(0)      = %lf (esperado 0)\n",  seno(0));
	printf("seno(pi/6)   = %lf (esperado 0.5)\n", seno(pi/6));
	printf("seno(pi/4)   = %lf (esperado 0.7071)\n", seno(pi/4));
	printf("seno(pi/2)   = %lf (esperado 1)\n", seno(pi/2));
	printf("seno(pi)     = %lf (esperado 0)\n", seno(pi));
}


int main()
{
	testa_seno();
	return 0;
}
