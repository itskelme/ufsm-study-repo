#include <stdio.h>

// Funcao que calcula x elevado na n-esima potencia
// recebe um double (x) e um int (n), retorna um double

// regras:
// x elevado a 0 eh 1
// x elevado a n negativo eh o inverso de x elevado a -n
// x elevado a n positivo eh x multiplicado por ele mesmo n vezes


double potencia(double x, int n)
{
	double resultado;
	int i;

	// se o expoente for negativo, fazemos o inverso de x^-n
	// (troca o sinal e depois inverte o resultado)
	if (n < 0) {
		return 1.0 / potencia(x, -n);
	}

	resultado = 1.0; // comeca em 1, assim se n for 0 ja retorna certo

	// laco que multiplica x por ele mesmo n vezes
	// peguei essa forma das dicas do professor, tem vantagem de nao precisar
	// de if para o caso de n ser 0 (o laco nao executa e resultado ja vale 1)
	for (i = 1; i <= n; i++) {
		resultado = resultado * x;
	}

	return resultado;
}


void testa_potencia()
{
	printf("Teste da potencia\n");
	printf("%lf = %lf\n", 25.0, potencia(5.0, 2));
	printf("%lf = %lf\n", 6.25, potencia(2.5, 2));
	printf("%lf = %lf\n", 1.0/8, potencia(2.0, -3));
	printf("%lf = %lf\n", -8.0, potencia(-2.0, 3));
	printf("%lf = %lf\n", 1.0, potencia(10.0, 0));
	printf("%lf = %lf\n", 100000.0, potencia(10.0, 5));
}


int main()
{
	testa_potencia();
	return 0;
}
