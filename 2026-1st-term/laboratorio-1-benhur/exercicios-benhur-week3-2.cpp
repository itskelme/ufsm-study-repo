#include <stdio.h>
#include <stdlib.h>


void impnum(int num)
{
  if (num < 0) {
    putchar('-');
    num = -num;
  }
  if (num >= 10) {
    impnum(num / 10);
  }
  putchar('0' + num % 10);
}


int main(void)
{
	int iNum = 0;
	printf("\nNeste programa o usuário deve digitar o número para calcularmos o: ");
	scanf("%f", &iNum);
	
    impnum(iNum);
    return 0;
}
