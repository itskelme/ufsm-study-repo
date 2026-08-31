#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int DDI, vazio;
	printf("\n\nEste programa determinará se um país faz parte da Ámerica do Sul!");
	printf("\n\nPor favor usuário, digite o DDI do país: ");
	scanf("%d", &DDI);
	if( DDI != 55 && DDI != 56 && DDI != 54 && DDI != 591 && DDI != 57 && DDI != 593 && DDI != 594 && DDI != 58 && DDI != 51 && DDI != 598 && DDI != 595 && DDI != 592 && DDI != 597){
		printf("\n\nO país não faz parte da Ámerica do Sul! \n\n");		
	}
	if(DDI == 55 || DDI == 56 || DDI == 54 || DDI == 591 || DDI == 57 || DDI == 593 || DDI == 594 || DDI == 58 || DDI == 51 || DDI == 598 || DDI == 595 || DDI == 592 || DDI == 597){
		printf("\n\nO país faz parte da Ámerica do Sul! \n\n");
	}
	printf("\n\nObrigado por utilizar este programa!\n\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
}
