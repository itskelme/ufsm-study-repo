#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void) 
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	float H, n, i;
	printf ("\n\nEste programa determina o valor de H = 1 * 1/4 * 1/9 * ... * 1/N");
	printf ("\n\nDigite o valor de n para determinação de N: ");
	scanf ("%f", &n);
	H = 1.0; // 1/1
	i = 2.0;
	while (i <= n) {
		H = H * (1.0/(i*i));
		i++;	
	}
	printf ("\n\nO valor de H é %.4f!", H);
	printf ("\n\nObrigado por utilizar nosso programa! :)\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
