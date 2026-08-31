#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	char caracter;
	int n, i, quantVogais = 0;
	printf ("\n\nEste programa lê uma frase e define a quantidade de vogais não acentuadas!");
	printf ("\n\nDigite a quantidade de caracteres da frase: ");
	scanf ("%d", &n);
	i = 1;
	fflush(stdin);
	do {
		system ("cls");
		printf ("\n\nDigite o %d° caracter: ", i);
		scanf ("%c", &caracter);
		switch (caracter) {
			case 'A':
			case 'a':
			case 'E':
			case 'e':
			case 'I':
			case 'i':
			case 'O':
			case 'o':
			case 'U':
			case 'u':
				quantVogais++;
				break;
		}
		fflush(stdin);
		i++;
	} while (i <= n);
	printf ("\n\nA quantidade de vogais não acentuadas na frase é de %d!", quantVogais);	 
	printf ("\n\nObrigado por utilizar nosso programa!\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
