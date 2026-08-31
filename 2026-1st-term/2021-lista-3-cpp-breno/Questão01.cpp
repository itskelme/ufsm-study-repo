
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "portuguese");
        
        //Declaração de Variáveis
		double lowerBase, greaterBase, height, area;
			
            area = 0.0; //Variável de Processamento
			lowerBase = 0.0;
			greaterBase = 0.0;
			height = 0.0;
			
		int op = 0;
			//Introdução
			printf ("\n\nEste programa calcula a �rea de um trap�zio, em cent�metros ou metros.");
				
				//Menu de opções
				printf ("\n\nEscolha um dos n�meros do menu abaixo para selecionar a op��o correspondente!");
					printf ("\n\n1 - �rea do trap�zio em centímetros quadrados");
					printf ("\n\n2 - �rea do trap�zio em metros quadrados");

				//Escolha de opção
				printf ("\n\nDigite a op��o desejada: ");
					scanf ("%d", &op);
					system ("cls");
						
				switch (op){
					case 1:
						do {
							printf ("\n\nPor favor, digite o tamanho da base maior (em cent�metros positivos e n�o nulos): ");
								scanf ("%lf", &greaterBase); //Entrada
							} while (greaterBase <= 0);
							
						do {
							printf ("\n\nPor favor, digite o tamanho da base menor (em cent�metros positivos e n�o nulos): ");
								scanf ("%lf", &lowerBase); //Entrada
							} while (lowerBase <= 0);
							
						do {
							printf ("\n\nPor favor, digite o tamanho da altura (em cent�metros positivos e n�o nulos): ");
								scanf ("%lf", &height); //Entrada
							} while (height <= 0);
						
						if (lowerBase > greaterBase)
							printf ("\n\nVoc� digitou valores indevidos para as bases, entretanto isso n�o afetar� o c�lculo para voc�!");
						if (lowerBase == greaterBase);
							printf ("\n\nVoc� digitou valores iguais para as bases, entretanto isso n�o afetar� o c�lculo para voc�!");
							//Processamento
							area = (((greaterBase + lowerBase)*height)/2);

						//Saída
						printf ("\n\nO resultado do c�lculo da �rea deste trap�zio, em cent�metros quadrados, � de %.2f cm�!", area);
							break;
							
					case 2:

						do {
							printf ("\n\nPor favor, digite o tamanho da base maior (em metros positivos e n�o nulos): ");
								scanf ("%lf", &greaterBase); //Entrada
							} while (greaterBase <= 0);
							
						do {
							printf ("\n\nPor favor, digite o tamanho da base menor (em metros positivos e n�o nulos): ");
								scanf ("%lf", &lowerBase); //Entrada
							} while (lowerBase <= 0);
							
						do {
							printf ("\n\nPor favor, digite o tamanho da altura (em metros positivos e n�o nulos): ");
								scanf ("%lf", &height); //Entrada
						break;
							} while (height <= 0);

						if (lowerBase > greaterBase);
							printf ("\n\nVoc� digitou valores indevidos para as bases, entretanto isso n�o afetar� o c�lculo para voc�!");
						if (lowerBase == greaterBase);
							printf ("\n\nVoc� digitou valores iguais para as bases, entretanto isso n�o afetar� o c�lculo para voc�!");
										
						//Processamento
						area = (((greaterBase + lowerBase)*height)/2);
	
							//Saída
							printf ("\n\nO resultado do c�lculo da �rea deste trap�zio, em metros quadrados, � de %.2f m�!\n", area);
                                break;


					if (op <= 0 or op >=2) {
                         printf ("\n\nDigite uma op��o v�lida de acordo com o menu!");    
                    }
						
				
                    printf ("\n\nObrigado por utilizar o nosso programa!\n\n");
		
		        }
		
   

	#ifdef WIN32
	system ("pause");
	#endif
	return 0;	
}
