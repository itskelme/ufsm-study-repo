#include <stdio.h>

// Funcao que calcula o fatorial de um numero
// o professor avisou nas dicas que o long pode ser pequeno demais
// pra calcular o fatorial que a gente precisa na serie do seno
// entao segui a dica e usei double pro retorno (que aguenta numeros enormes)

// regras:
// fatorial de numero negativo nao existe (retornamos -1 como aviso)
// fatorial de 0 eh 1
// fatorial de n positivo eh n * fatorial de n-1


double fatorial(long n)
{
	double resultado;
	long i;

	// fatorial de negativo nao existe, retorno -1 como aviso de erro
	if (n < 0) {
		return -1;
	}

	resultado = 1; // comeca com 1, assim se n=0 ja retorna certo

	// multiplica de 1 ate n, um por um
	// se n for 0 o laco nao executa e ja retorna 1
	for (i = 1; i <= n; i++) {
		resultado = resultado * i;
	}

	return resultado;
}


void testa_fatorial()
{
	printf("Teste do fatorial\n");
	printf("0!  = %.0lf (esperado 1)\n",       fatorial(0));
	printf("1!  = %.0lf (esperado 1)\n",       fatorial(1));
	printf("5!  = %.0lf (esperado 120)\n",     fatorial(5));
	printf("10! = %.0lf (esperado 3628800)\n", fatorial(10));
	printf("17! = %.0lf\n",                    fatorial(17));
	printf("-3! = %.0lf (negativo nao existe, retorna -1)\n", fatorial(-3));
}


int main()
{
	testa_fatorial();
	return 0;
}
