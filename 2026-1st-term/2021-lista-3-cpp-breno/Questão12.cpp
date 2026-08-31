#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

int main (void)
{
	system ("cls");
	setlocale (LC_ALL, "Portuguese");
	int protons1, neutrons1, massa1, protons2, neutrons2, massa2, protons3, neutrons3, massa3, vazio;
	printf("\n> Caro usuário, este programa lhe determinará dentre três elementos quais são isótopos, isótonos ou isobáros entre si!");
	
	printf("\n\n>> Por favor usuário, digite o número atômico do primeiro elemento: ");
	scanf("%d", &protons1);
	printf("\n\n>> Por favor usuário, digite o número de massa do primeiro elemento: ");
	scanf("%d", &massa1);
	printf("\n\n>> Por favor usuário, digite o número atômico do segundo elemento: ");
	scanf("%d", &protons2);
	printf("\n\n>> Por favor usuário, digite o número de massa do segundo elemento: ");
	scanf("%d", &massa2);
	printf("\n\n>> Por favor usuário, digite o número atômico do terceiro elemento: ");
	scanf("%d", &protons3);
	printf("\n\n>> Por favor usuário, digite o número de massa do terceiro elemento: ");
	scanf("%d", &massa3);
	
	neutrons1 = massa1 - protons1;
	neutrons2 = massa2 - protons2;
	neutrons3 = massa3 - protons3;
	
	if(massa1 == massa2){
		printf("\n\n> O primeiro e o segundo elemento são isóbaros entre si!");
	}
	if(massa2 == massa3){
		printf("\n\n> O segundo e o teceiro elemento são isóbaros entre si!");
	}
	if(massa3 == massa1){
		printf("\n\n> O terceiro e o primeiro elemento são isóbaros entre si!");
	}
	if(neutrons1 == neutrons2){
		printf("\n\n> O primeiro e o segundo elemento são isótonos entre si!");
	}
	if(neutrons2 == neutrons3){
		printf("\n\n> O segundo e o teceiro elemento são isótonos entre si!");
	}
	if(neutrons3 == neutrons1){
		printf("\n\n> O terceiro e o primeiro elemento são isótonos entre si!");
	}
	if(protons1 == protons2){
		printf("\n\n> O primeiro e o segundo elemento são isótopos entre si!");
	}
	if(protons2 == protons3){
		printf("\n\n> O segundo e o teceiro elemento são isótopos entre si!");
	}
	if(protons3 == protons1){
		printf("\n\n> O terceiro e o primeiro elemento são isótopos entre si!");
	}
	if(massa1 != massa2 && massa2 != massa3 && massa3 != massa1){
		printf("\n\nNenhum dos três elementos não são isóbaros entre si");
	}
	if(neutrons1 != neutrons2 && neutrons2 != neutrons3 && neutrons3 != neutrons1){
		printf("\n\nNenhum dos três elementos não são isótonos entre si");
	}
	if(protons1 != protons2 && protons2 != protons3 && protons3 != protons1){
		printf("\n\nNenhum dos três elementos não são isótopos entre si");
	}
	printf("\n\n> Agradeço a sua prefêrencia em utilizar este programa, disponha! ;-)\n");
	#ifdef WIN32
	system ("pause");
	#endif
	return 0;
	
}
	
