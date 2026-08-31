#include <stdio.h>

// Tabela de senos e cossenos de 0 ate 89 graus
// 5 graus por linha, com 4 casas depois da virgula
// fiz uma tabela ASCII parecida com a de um exercicio de Fibonacci 
// que eu fiz em sala, usando os caracteres +, - e |


// potencia (exercicio 1)
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


// fatorial (do exercicio 2)
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


// valor absoluto (auxiliar)
double valor_absoluto(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}


// seno por serie de Taylor (do exercicio 3)
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


// ---- raiz pelo metodo de Heron (do exercicio 4) ----
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


// ---- cosseno (do exercicio 5) ----
// cos(x) = raiz(1 - sen^2(x))
// (funciona pra 0 a 89 graus que eh o que a tabela precisa)
double cosseno(double x)
{
	double s;
	double dentroRaiz;

	s = seno(x);
	dentroRaiz = 1 - s * s;
	// arredondamento pode dar negativo bem pequenininho, eu trato como zero
	if (dentroRaiz < 0) {
		dentroRaiz = 0;
	}
	return raiz(dentroRaiz);
}


// imprime a linha de separacao do quadro (tipo +----+---+)
// peguei essa ideia do meu codigo do fibonacci
void imprime_separador()
{
	int i;
	printf("+-----");
	// cada uma das 5 colunas de seno/cosseno tem 15 tracos
	for (i = 0; i < 5; i++) {
		printf("+---------------");
	}
	printf("+\n");
}


// imprime o cabecalho da tabela
void imprime_cabecalho()
{
	int i;
	imprime_separador();
	printf("| ang ");
	for (i = 0; i < 5; i++) {
		printf("|  seno cosseno ");
	}
	printf("|\n");
	imprime_separador();
}


// imprime a tabela completa
void imprime_tabela()
{
	// constante pi pra converter graus em radianos
	double pi = 3.14159265;
	int ang;
	int j;
	int g;
	double rad;
	double s;
	double c;

	imprime_cabecalho();

	// cada linha comeca em um multiplo de 5 (0, 5, 10, ... 85)
	// e mostra 5 pares de seno/cosseno seguidos (ang, ang+1, ang+2, ang+3, ang+4)
	// chega ate 89 graus no final (85+4 = 89)
	for (ang = 0; ang <= 85; ang = ang + 5) {
		printf("| %3d ", ang);

		// laco interno pelas 5 colunas da linha
		for (j = 0; j < 5; j++) {
			g = ang + j;              // angulo em graus
			rad = g * pi / 180.0;     // converte pra radianos
			s = seno(rad);            // calcula o seno
			c = cosseno(rad);         // calcula o cosseno
			printf("| %6.4lf %6.4lf ", s, c);
		}

		printf("|\n");
	}

	imprime_separador();
}


int main()
{
	imprime_tabela();
	return 0;
}
