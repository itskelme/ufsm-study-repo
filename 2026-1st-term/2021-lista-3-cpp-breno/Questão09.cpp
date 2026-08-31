#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int a1, r, n, an = 0;
	printf ("\n\nEste programa define o termo an de uma Progressão Aritmética (PA)! :)");
	printf ("\n\nDigite o primeiro termo da PA: ");
	scanf ("%d", &a1);
	printf ("\n\nDigite a razão da PA: ");
	scanf ("%d", &r);
	printf ("\n\nDigite o índice n do termo an desejado na PA: ");
	scanf ("%d", &n);
	
	an = a1 + ((n - 1) * r);
	printf ("\n\nO termo a%d da PA é %d!", n, an);
	
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
