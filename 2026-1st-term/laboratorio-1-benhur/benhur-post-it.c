





#include <stdio.h>

#include <stdbool.h>


typedef struct {

  int r;

  int g;

  int b;

} cor;

typedef struct {

  int x;

  int y;

} ponto;

typedef struct {

  ponto inf_esq;

  ponto sup_dir;

} retangulo;

typedef struct {

  retangulo r;

  cor c;

  char t[20];

} postit;


int acha_postit(int n, postit v[n], ponto p)

{

  

}


void cor_letra(cor c)

{

  printf("%c[38;2;%d;%d;%dm", 27,

         c.r*256/100, c.g*256/100, c.b*256/100);

}


void cor_fundo(cor c)

{

  printf("%c[48;2;%d;%d;%dm", 27,

         c.r*256/100, c.g*256/100, c.b*256/100);

}


void cor_normal()

{

  printf("%c[0m", 27);

}


void desenha_postits(int n, postit v[n])

{

  char letra;

  for (int lin = 23; lin >= 0; lin-=1) {

    for (int col = 0; col < 80; col++) {

      ponto pos = { col, lin };

      int i = acha_postit(n, v, pos);

      if (i != -1) {

        cor_fundo(v[i].c);

        cor_letra(v[i].c);

        letra = v[i].t[0];

      } else {

        cor_fundo((cor){0,0,0});

        letra = ' ';

      }

      putchar(letra);

    }

    cor_normal();

    putchar('\n');

  }

}

int acha_postit(int n, postit v[n], ponto p)
{
  // de tras para frente (n-1 até 0)
  for (int i = n - 1; i >= 0; i--) {
    // Verifica se a coordenada x e y do ponto está dentro dos limites do retângulo
    if (p.x >= v[i].r.inf_esq.x && p.x <= v[i].r.sup_dir.x &&
        p.y >= v[i].r.inf_esq.y && p.y <= v[i].r.sup_dir.y) {
      return i; // Retorna o índice do post-it encontrado
    }
  }
  
  // quando 0 post-it é encontrado, a gente retorna -1
  return -1;
}

int main()

{

  postit v[] = {

    {{{ 0, 0}, {49,23}}, { 0, 0,50}, "escuro"   },

    {{{15, 9}, {40,30}}, {10,10,80}, "azul"    },

    {{{10,10}, {20,20}}, {99, 0, 0}, "vermelho"},

    {{{30,15}, {70,22}}, {99,70, 0}, "laranja" },

  };

  desenha_postits(4, v);

}

Aqui está a implementação da função acha_postit.
