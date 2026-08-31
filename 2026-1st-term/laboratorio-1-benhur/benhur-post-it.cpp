#include <stdio.h>
#include <stdbool.h>

// 

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

// Implemente a função acha_postit abaixo.
// Ela recebe um vetor de postits e seu tamanho,
// mais um ponto, e retorna o índice no vetor
// onde está o postit que contém nesse ponto.
// Caso não haja postit nesse ponto, retorna -1;
// caso haja mais de um, retorna o mais próximo ao
// final do vetor.

int acha_postit(int n, postit v[], ponto p)
{
    // percorre de tras para frente (n-1 até 0)
    for (int i = n - 1; i >= 0; i--) {
    	
        // sabe se a coordenada x e y do ponto está dentro dos limites do retângulo
        int esq   = v[i].r.inf_esq.x;
        int dir   = v[i].r.sup_dir.x;
        int baixo = v[i].r.inf_esq.y;
        int cima  = v[i].r.sup_dir.y;
        
        // ponto maior ou igual a esquerda ... 
        if (p.x >= esq && p.x <= dir && p.y >= baixo && p.y <= cima) {
            return i; // manda de volta o índice do post-it encontrado
        }
    }
    
    // quando nenhum postit é encontrado a gente retorna -1
    return -1;
}

void desenha_postits(int n, postit v[])
{
  char letra;
  
  // O lin-=1 é apenas outra forma de escrever lin-- (subtrai 1 a cada reoetição)
  // Varre da linha mais alta (23) até a base (0)
  for (int lin = 23; lin >= 0; lin-=1) {
      
    // 80 colunas
    for (int col = 0; col <= 79; col++) {
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

int main()
{
  // {{{inf_esq_x, inf_esq_y}, {sup_dir_x, sup_dir_y}}, {r, g, b}, "nome da cor"}
  postit v[] = {
    {{{ 1, 1}, {40,10}}, { 0, 40,0}, "green"     },
    {{{10,10}, {20,20}}, {99, 0, 0}, "vermelho"  },
    {{{15, 9}, {40,30}}, {00,00,80}, "azul"      },
    {{{30,15}, {70,22}}, {99,70, 0}, "laranja"   },
  };
  desenha_postits(4, v);
  
  return 0;
}
