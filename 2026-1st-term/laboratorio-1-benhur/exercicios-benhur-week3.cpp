#include <stdio.h>


void coisa()
{
	putchar('+');
	putchar('-');
	putchar('-');
	putchar('-');
	putchar('+');
	putchar('\n');
}

void linha(int c)
{
	putchar('|');
	putchar(' ');
	putchar(c);
	putchar(' ');
	putchar('|');
	putchar('\n');
}

void nome()
{
    coisa();
    linha("J");
    linha('u');
    linha('c');
    linha('a');
    coisa();
}

int main(void)
{
    nome();
    return 0;
}
