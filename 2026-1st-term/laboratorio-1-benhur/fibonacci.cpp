#include <stdio.h>

// Uma função que lê um número e retorna o enésimo número da sequência de Fibonacci
// (ou seja, o número de entrada corresponde a posição do número da sequência de Fibonacci)


int fibo(int n) {
	int numAnterior = 0, numAtual = 1, numProximo;
	
	
// Se n for 1, o resultado é o próprio 'atual' (1)
    // O laço começa a contar a partir do segundo termo
    for (int i = 1; i < n; i++) {
        numProximo = numAtual + numAnterior; //  soma os dois anteriores
        numAnterior = numAtual;           // atual vira o anterior para o próximo passo
        numAtual = numProximo;            // próximo vira o atual
    }
    return numAtual; // manda o valor para a main
}
 
int main()
{
	

  puts("+----+------+");
  puts("|  n | fibo |");
  puts("+----+------+");
  for (int i = 1; i < 30; i++) {
    printf("| %2d | %4d |\n", i, fibo(i));
  }
  puts("+----+------+");
  
  return 0;
}
