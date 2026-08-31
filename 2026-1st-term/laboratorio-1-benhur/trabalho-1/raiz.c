#include <stdio.h>

// Funcao pra calcular raiz quadrada usando o metodo de Heron
// e nossa a ideia eh:
//   - comeca com um chute qualquer positivo
//   - proximo chute eh a media entre o chute atual e x/chute
//   - para quando a diferenca entre dois chutes seguidos for < 1e-10


// funcao auxiliar pra valor absoluto (igual a do seno)
double valor_absoluto(double x)
{
	if (x < 0) {
		return -x;
	}
	return x;
}


double raiz(double x)
{
	// caso especial raiz de 0 eh 0
	// (se eu deixar entrar no laco vai dar divisao por zero)
	if (x == 0) {
		return 0;
	}

	double chute = 1.0;        // chute inicial, qualquer valor positivo serve
	double chuteSeguinte;      // vai guardar o proximo chute

	// repete ate os chutes ficarem praticamente iguais
	do {
		// o proximo chute eh a media entre o chute atual e x dividido pelo chute
		chuteSeguinte = (chute + x / chute) / 2.0;

		// se a diferenca entre os dois chutes eh bem pequena, achamos a raiz
		if (valor_absoluto(chuteSeguinte - chute) < 1e-10) {
			return chuteSeguinte;
		}

		// se nao, atualiza o chute e continua
		chute = chuteSeguinte;
	} while (1); // loop infinito, o return la dentro quebra ele
}


void testa_raiz()
{
	printf("Teste da raiz quadrada\n");
	printf("raiz(4)    = %lf (esperado 2)\n",       raiz(4));
	printf("raiz(9)    = %lf (esperado 3)\n",       raiz(9));
	printf("raiz(2)    = %lf (esperado 1.4142)\n",  raiz(2));
	printf("raiz(16)   = %lf (esperado 4)\n",       raiz(16));
	printf("raiz(0.25) = %lf (esperado 0.5)\n",     raiz(0.25));
	printf("raiz(100)  = %lf (esperado 10)\n",      raiz(100));
	printf("raiz(0)    = %lf (esperado 0)\n",       raiz(0));
}


int main()
{
	testa_raiz();
	return 0;
}
