#include <stdio.h>

// Funcao pra calcular o cosseno de um angulo em radianos
// o professor pediu pra usar a formula: sen^2(x) + cos^2(x) = 1
// entao: cos(x) = raiz(1 - sen^2(x))

// pra isso eu preciso das funcoes seno e raiz que ja fiz
// (e a funcao seno precisa de potencia e fatorial, entao trago todas)


// funcao potencia (do execicio 1)
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


// funcao auxiliar com valor absoluto
double valor_absoluto(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}


// funcao seno (exercicio 3)
double seno(double x)
{
	double soma = 0;
	double termo;
	int n = 1;
	int sinal = 1;

	do {
		termo = sinal * potencia(x, n) / fatorial(n);
		soma = soma + termo;
		n = n + 2;
		sinal = -sinal;
	} while (valor_absoluto(termo) >= 1e-10);

	return soma;
}


// funcao raiz (execicio 4)
double raiz(double x)
{
	double chute;
	double chuteSeguinte;

	if (x == 0) {
		return 0;
	}
	chute = 1.0;
	do {
		chuteSeguinte = (chute + x / chute) / 2.0;
		if (valor_absoluto(chuteSeguinte - chute) < 1e-10) {
			return chuteSeguinte;
		}
		chute = chuteSeguinte;
	} while (1);
}


// funcao cosseno usando a identidade trigonometrica
// cos(x) = raiz(1 - sen^2(x))
// obs: essa formula da sempre o valor positivo do cosseno
// pra tabela do trabalho (0 a 89 graus) funciona perfeitamente
// pq nesse intervalo o cosseno eh sempre positivo mesmo
double cosseno(double x)
{
	double s;
	double dentroRaiz;

	s = seno(x);            // calcula seno do angulo
	dentroRaiz = 1 - s * s; // o que vai dentro da raiz

	// detalhe bem curioso prof, as vezes por causa de arredondamento do ponto flutuante
	// a conta 1 - s*s da um valor negativo bem pequenininho (tipo -1e-16)
	// isso faz a raiz travar, entao eu trato como zero nesse caso
	
	if (dentroRaiz < 0) {
		dentroRaiz = 0;
	}

	return raiz(dentroRaiz);
}


void testa_cosseno()
{
	double pi = 3.14159265;
	printf("Teste do cosseno\n");
	printf("cosseno(0)    = %lf (esperado 1)\n",      cosseno(0));
	printf("cosseno(pi/6) = %lf (esperado 0.8660)\n", cosseno(pi/6));
	printf("cosseno(pi/4) = %lf (esperado 0.7071)\n", cosseno(pi/4));
	printf("cosseno(pi/3) = %lf (esperado 0.5)\n",    cosseno(pi/3));
	printf("cosseno(pi/2) = %lf (esperado 0)\n",      cosseno(pi/2));
}


int main()
{
	testa_cosseno();
	return 0;
}
