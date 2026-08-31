#include <stdio.h>

// Trabalho 1 - Laboratorio de Programacao 1 - Diego Kelme

// Arquivo unico com a solucao do trabalho conforme orientado no https://github.com/BenhurUFSM/l126a/blob/main/Assuntos/t1.md
// Para organizar em um arquivo so, cada questao tem sua funcao correspondente implementada aqui em cima dai,
// e em baixo tem uma funcao de teste por questao (questao1, questao2, ...)
// que eh chamada pela main


// funcao auxiliar pra pegar o valor absoluto de um double
// eu uso ela na serie de Taylor do seno e no criterio de parada da raiz
double valor_absoluto(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}


// Questao 1: calcula x elevado na n-esima potencia
// se o expoente for negativo, eh o inverso de x elevado a -n
// se for 0, retorna 1
// se for positivo, multiplica x por ele mesmo n vezes
double potencia(double x, int n)
{
	double resultado;
	int i;

	if (n < 0) {
		return 1.0 / potencia(x, -n);
	}

	resultado = 1.0; // se n for 0 o laco nao executa e ja retorna 1
	for (i = 1; i <= n; i++) {
		resultado = resultado * x;
	}

	return resultado;
}


// Questao 2: fatorial
// usei double no retorno conforme dica do professor, pois long pode nao
// aguentar o fatorial dos valores que precisamos na serie do seno
double fatorial(long n)
{
	double resultado;
	long i;

	if (n < 0) {
		return -1; // fatorial de negativo nao existe, retorna -1 como aviso
	}

	resultado = 1; // se n=0 o laco nao roda e ja retorna 1
	for (i = 1; i <= n; i++) {
		resultado = resultado * i;
	}

	return resultado;
}


// Questao 3: seno usando a serie de Taylor
// seno(x) = x - x^3/3! + x^5/5! - x^7/7! ...
// a gente para de somar quando o termo tiver valor absoluto menor que 1e-10
double seno(double x)
{
	double soma = 0;  // soma eh acumulador, comeca em 0 (padrao de somatorio)
	double termo;     // guarda cada termo da serie
	int n = 1;        // expoente da serie: 1, 3, 5, 7, ...
	int sinal = 1;    // alterna entre +1 e -1 a cada passo

	do {
		termo = sinal * potencia(x, n) / fatorial(n);
		soma = soma + termo;

		// prepara o proximo termo
		n = n + 2;
		sinal = -sinal;
	} while (valor_absoluto(termo) >= 1e-10);

	return soma;
}


// Questao 4: raiz quadrada pelo metodo de Heron
// comeca com um chute positivo qualquer
// o proximo chute eh a media entre o chute atual e x dividido pelo chute
// a gente para quando a diferenca entre dois chutes seguidos for menor que 1e-10
double raiz(double x)
{
	double chute;
	double chuteSeguinte;

	// caso especial raiz de 0 eh 0 (se nao tratar aqui daria divisao por zero)
	if (x == 0) {
		return 0;
	}

	chute = 1.0; // chute inicial, qualquer valor positivo serve

	do {
		chuteSeguinte = (chute + x / chute) / 2.0;
		if (valor_absoluto(chuteSeguinte - chute) < 1e-10) {
			return chuteSeguinte;
		}
		chute = chuteSeguinte;
	} while (1);
}


// Questao 5: coseno usando a identidade sen^2(x) + cos^2(x) = 1
// entao cos(x) = raiz(1 - seno^2(x))
// (da sempre valor positivo, que pra tabela de 0 a 89 graus eh suficiente)
double cosseno(double x)
{
	double s;
	double dentroRaiz;

	s = seno(x);
	dentroRaiz = 1 - s * s;

	// detalhe bem curioso prof: as vezes por causa de arredondamento do ponto
	// flutuante o 1 - s*s da um valor negativo bem pequenininho (tipo -1e-16),
	// ai a raiz fica iterando sem parar. Eu trato isso como zero.
	if (dentroRaiz < 0) {
		dentroRaiz = 0;
	}

	return raiz(dentroRaiz);
}


// questao 6: tabela de senos e cossenos (auxiliares para montar o quadro)
// fiz uma tabela ASCII parecida com a que fiz no exercicio de Fibonacci,
// usando os caracteres +, - e |

// imprime a linha de separacao do quadro (tipo +----+---+)
void imprime_separador()
{
	int i;
	printf("+-----");
	// sao 5 colunas de seno/cosseno e cada uma ocupa 15 tracos
	for (i = 0; i < 5; i++) {
		printf("+---------------");
	}
	printf("+\n");
}


// imprime a linha de cabecalho
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


// questao 6 (continuacao) imprime a tabela completa
void imprime_tabela()
{
	double pi = 3.14159265;
	int ang;
	int j;
	int g;
	double rad;
	double s;
	double c;

	imprime_cabecalho();

	// as linhas comecam nos multiplos de 5 (0, 5, 10, ... 85)
	// e cada linha mostra 5 pares de seno/cosseno: ang, ang+1, ang+2, ang+3, ang+4
	// com isso a ultima linha (85) chega ate 89 graus (85+4)
	for (ang = 0; ang <= 85; ang = ang + 5) {
		printf("| %3d ", ang);

		for (j = 0; j < 5; j++) {
			g = ang + j;              // angulo atual em graus
			rad = g * pi / 180.0;     // converte pra radianos
			s = seno(rad);
			c = cosseno(rad);
			printf("| %6.4lf %6.4lf ", s, c);
		}

		printf("|\n");
	}

	imprime_separador();
}


// a partir daqui sao as funcoes de teste de cada questao
// cada uma chama as funcoes da questao com alguns valores e mostra o resultado

void questao1()
{
	printf("\nQuestao 1 - Potencia\n");
	printf("%lf = %lf\n", 25.0,  potencia(5.0, 2));
	printf("%lf = %lf\n", 6.25,  potencia(2.5, 2));
	printf("%lf = %lf\n", 1.0/8, potencia(2.0, -3));
	printf("%lf = %lf\n", -8.0,  potencia(-2.0, 3));
	printf("%lf = %lf\n", 1.0,   potencia(10.0, 0));
}


void questao2()
{
	printf("\nQuestao 2 - fatorial\n");
	printf("0!  = %.0lf (esperado 1)\n",       fatorial(0));
	printf("1!  = %.0lf (esperado 1)\n",       fatorial(1));
	printf("5!  = %.0lf (esperado 120)\n",     fatorial(5));
	printf("10! = %.0lf (esperado 3628800)\n", fatorial(10));
	printf("17! = %.0lf\n",                    fatorial(17));
	printf("-3! = %.0lf (negativo nao existe, retorna -1)\n", fatorial(-3));
}


void questao3()
{
	double pi = 3.14159265;
	printf("\nQuestao 3 - Seno\n");
	printf("seno(0)    = %lf (esperado 0)\n",      seno(0));
	printf("seno(pi/6) = %lf (esperado 0.5)\n",    seno(pi/6));
	printf("seno(pi/4) = %lf (esperado 0.7071)\n", seno(pi/4));
	printf("seno(pi/2) = %lf (esperado 1)\n",      seno(pi/2));
}


void questao4()
{
	printf("\nQuestao 4 - Raiz quadrada\n");
	printf("raiz(4)    = %lf (esperado 2)\n",       raiz(4));
	printf("raiz(9)    = %lf (esperado 3)\n",       raiz(9));
	printf("raiz(2)    = %lf (esperado 1.4142)\n",  raiz(2));
	printf("raiz(16)   = %lf (esperado 4)\n",       raiz(16));
	printf("raiz(100)  = %lf (esperado 10)\n",      raiz(100));
	printf("raiz(0.25) = %lf (esperado 0.5)\n",     raiz(0.25));
	printf("raiz(0)    = %lf (esperado 0)\n",       raiz(0));
}


void questao5()
{
	double pi = 3.14159265;
	printf("\nQuestao 5 - Cosseno\n");
	printf("cosseno(0)    = %lf (esperado 1)\n",      cosseno(0));
	printf("cosseno(pi/6) = %lf (esperado 0.8660)\n", cosseno(pi/6));
	printf("cosseno(pi/4) = %lf (esperado 0.7071)\n", cosseno(pi/4));
	printf("cosseno(pi/3) = %lf (esperado 0.5)\n",    cosseno(pi/3));
	printf("cosseno(pi/2) = %lf esperado 0)\n",      cosseno(pi/2));
}


void questao6()
{
	printf("\nQuestao 6 - tabela de senos e cossenos\n");
	imprime_tabela();
}


int main()
{
	questao1();
	questao2();
	questao3();
	questao4();
	questao5();
	questao6();
	return 0;
}
