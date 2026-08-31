#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");;
	setlocale (LC_ALL, "Portuguese");;
    
	int termo1, termo2, novoTermo, n, i;

		//	Introdução
		printf ("\n\n> Este programa basicamente lhe imprimirá a Série de Fibonacci até um enésimo termo (n);!");

			//	Entrada
			printf ("\n\n>> Digite a quantidade de termos da Série para impressão: ");
				scanf ("%d", &n);
				system ("cls");
				if (n <= 0){
					printf ("\n\n>> Por favor usuário, digite uma quantidade válida de termos da Série!");
                }
				
                else if (n == 1) {
                    printf ("\n\n> Série de Fibonacci: 1, ");
                }
                    
                else if (n == 2) {
                    printf ("\n\n> Série de Fibonacci: 1, 1, ");
                }
                    
                else if (n > 2){

                    termo1 = 1;
                    termo2 = 1;
                        printf ("\n\n> Série de Fibonacci:  %d, %d, ", termo1, termo2);
                        i = 2;
                        do {
                            novoTermo = termo1 + termo2;
                            printf ("%d, ", novoTermo);
                            termo1 = termo2;
                            termo2 = novoTermo;
                            i++;
                            } while (i < n); 
                    }
				printf ("...");
				
			//	Agradecimentos 
			printf ("\n\n> Agradeço a sua prefêrencia em utilizar este programa, disponha! ;-)\n"); 
				
            //	By TheK'

	#ifdef WIN32
	system ("pause");
	#endif
	return 0;	
}