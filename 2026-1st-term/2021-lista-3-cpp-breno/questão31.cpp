#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int op;
	double tempo, horas, velocidade;
	printf("\n\nEste programa calcúla quanto tempo você demora para chegar em qualquer capital");
	printf("brasileira, saindo de Rio Branco em um movimento uniforme.");
	printf("\n\nEscreva a velocidade em KM/H: ");
	scanf("%lf", &velocidade);
	printf("\n\nCapitais:");
	printf("\n\n1-Porto Velho\n-2 Manaus\n3-Campo Grande\n4-Macapá\n5-Brasília\n6-Boa Vista");
	printf("\n7-Cuiabá\n8-Palmas\n9-São Paulo\n10-Teresina\n11-Rio de Janeiro\n12-Bélem\n");
	printf("13-Goiânia\n14-Salvador\n15-Florianópolis\n16-São Luís\n17-Maceió\n18-Porto Alegre\n");
	printf("19-Curitiba\n20-Belo Horizonte\n21-Fortaleza\n22-Recife\n23-João Pessoa\n24-Aracaju\n");
	printf("25-Natal\n26-Vitória\n");
	printf("\nDigite o número da Capital desejada:");
	scanf("%d", &op);
		switch(op){
			case 1:
			horas = 449 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 2:
			horas = 1149 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 3: 
			horas = 1828 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 4:
			horas = 2160 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 5:
			horas = 2247 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 6:
			horas = 1627 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 7:
			horas = 1415 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 8:
			horas = 2132 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 9:
			horas = 2707 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 10:
			horas = 2808 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 11:
			horas = 2983 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 12:
			horas = 2334 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 13:
			horas = 2138 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 14:
			horas = 3207 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 15:
			horas = 2811 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 16:
			horas = 2725 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 17:
			horas = 3512 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 18:
			horas = 2815 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 19:
			horas = 2602 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 20:
			horas = 2787 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 21:
			horas = 3301 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 22:
			horas = 3620 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 23:
			horas = 3635 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 24:
			horas = 3360 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 25:
			horas = 3618 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			case 26:
			horas = 3161 / velocidade;
			tempo = horas / 24;
			printf("\n\nO tempo de Rio Branco para a capital selecionada é de %.2f dias.", tempo);
			break;
			default:
			printf("\n\nDigite um valor válido.");
		}
	printf("\n\nObrigado por ultilizar ester programa!\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
