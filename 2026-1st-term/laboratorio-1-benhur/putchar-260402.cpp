#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

	
int lerNum(int valor) {
	
	
	
	char caractere =  (valor + '0');
	
	
	
	int contador = 0;

	return valor;
}

int main(void) {
	
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	
	int valor;
	
	printf("Digite um número: ");
	scanf("%d", &valor);
	printf("Você digitou: %d", lerNum(valor));
	
	return 0;
	
}
