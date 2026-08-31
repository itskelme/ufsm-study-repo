#include <stdio.h> // standard input e output "h" de header file p printf scanf e outros
#include <stdlib.h>  // pra usar a funcao srand e rand
#include <stdbool.h> // pra usar true e false com bool
#include <time.h>   // para poder usar o time como argum. no srand q to usando lá na main

// Trabalho 2 - Laboratorio de Programacao 1 - Diego Kelme
// Quebra-cabecas de pecas deslizantes (15-puzzle)

// O tabuleiro eh um vetor de 16 posicoes que representa uma matriz 4x4.
// As primeiras 4 posicoes (0..3) sao a primeira linha
// as 4 seguintes (4..7) a segunda linha, e assim por diante
// O numero 15 representa o espaco vazio do puzzle.

// funcoes de cor e terminal (peguei no material do professor) OK

void cor_letra(int r, int g, int b)
{
	printf("%c[38;2;%d;%d;%dm", 27, r, g, b);
}

void cor_fundo(int r, int g, int b)
{
	printf("%c[48;2;%d;%d;%dm", 27, r, g, b);
}

void cor_normal()
{
	printf("%c[0m", 27);
}

void cor_tabuleiro()
{
	cor_fundo(210, 200, 200);   // branco palmito
}

void cor_peca()
{
	cor_fundo(100, 100, 200); // branco azulado
	cor_letra(40, 50, 70);    // cinza escuro meio azulado
}

void cor_vazio()
{
	cor_fundo(0, 0, 0);       // preto
}

// em modo raw, \n so avanca a linha mas nao volta ao inicio
// entao uso \r\n em todo lugar
void pula_linha()
{
	printf("\r\n"); // bem importante para a matriz exibir corretinha viu
}

void limpa_tela()
{
	cor_normal();
	printf("%c[2J", 27); // limpa a tela
	printf("%c[H", 27);  // cursor no inicio
}


// funcao 1 -> preenche o vetor com  0..15 em ordem

void preenche_vetor(int v[])
{
	int i;
	for (i = 0; i < 16; i++) {
		v[i] = i;
	}
}


// funcao 2 -- procura um numero no  vetor e retorna o indice
// se nao encontrar, retorna -1
int acha_numero(int v[], int num) // TODO: verificar OK
{
	int i;
	for (i = 0; i < 16; i++) {
		if (v[i] == num) {
			return i;
		}
	}
	return -1;
}


// funcao 3 -> retorna true se o vetor estiver em ordem
bool em_ordem(int v[])
{
	int i;
	for (i = 0; i < 16; i++) {
		if (v[i] != i) {
			return false;
		}
	}
	return true;
}


// funcao 4 -- troca o 15 com o numero a esquerda dele
// se o 15 estiver no inicio de uma linha, nao faz nada

void move_esquerda(int v[])
{
	int pos;
	int aux;

	pos = acha_numero(v, 15);

	// se o 15 esta na coluna 0 (inicio da linha), nao da pra mover
	if (pos % 4 == 0) {
		return;
	}

	//  a troca eu faco utilizando uma variavel auxiliar
	aux = v[pos - 1];
	v[pos - 1] = v[pos];
	v[pos] = aux;
}


// funcao 5 -- troca o 15 com o numero a direita dele
 // se o 15 estiver no fim de uma linha, nao faz nada

void move_direita(int v[])
{
	int pos;
	int aux;

	pos = acha_numero(v, 15);

	// se o 15 esta na coluna 3 (fim da linha), nao da pra mover
	if (pos % 4 == 3) {
		return;
	}

	aux = v[pos + 1];
	v[pos + 1] = v[pos];
	v[pos] = aux;
}


//funcao 6 -- troca o 15 com o numero acima dele
// e se o 15 estiver na primeira linha, nao faz nada

void move_cima(int v[])
{
	int pos;
	int aux;

	pos = acha_numero(v, 15);

	// se o 15 esta na primeira linha (indices 0..3), nao da pra mover
	if (pos < 4) {
		return;
	}

	aux = v[pos - 4];
	v[pos - 4] = v[pos];
	v[pos] = aux;
}


// funcao 7 -- troca o 15 com o numero abaixo dele
// se o 15 estiver na  ultima linha, nao faz nada
void move_baixo(int v[])
{
	int pos;
	int aux;

	pos = acha_numero(v, 15);

	 // se o 15 esta na ultima linha (indices  12...15),  nao da pra mover
	if (pos >= 12) {
		return;
	}

	aux = v[pos + 4];
	v[pos + 4] = v[pos];
	v[pos] = aux;
}




// funcao 8 -- chama aleatoriamente uma das 4 funcoes de movimento


void move_aleatorio(int v[])
{ 

// bem que eu poderia usar um switch(r) { case 0: + move(v) + break e prox case }...  em, mas vamos deixar simples
 // o r a variavel responsavel pelo rand e dai isso permite fazer a movimentacao ser randomica
	int r;
	r = rand() % 4;

	if (r == 0) {
		move_esquerda(v);
	} else if (r == 1) {
		move_direita(v);
	} else if (r == 2) {
		move_cima(v);
	} else {
		move_baixo(v);
	}
}


// ===========================================================
// funcao 9 -- embaralha o vetor chamando move_aleatorio varias vezes
// como cada movimento parte de um estado valido,
// o resultado sempre vai ser um puzzle que tem solucao

void embaralha(int v[])
{
	int i;
	for (i = 0; i < 200; i++) {
		move_aleatorio(v);
	}
}


// funcao 10 -- executa o movimento de acordo com o caractere 
// (achei estranho viu, essa movimentação ai professor, mas ta igual tá no t2.txt)
//   w ou c ou A (seta cima)-->   peca sobe   (o 15 troca com o de baixo)
//   a ou e ou D (seta esq) -->   peca vai à esquerda (15 troca com o da direita)
//   s ou b ou B (seta baixo) ->  peca desce (15 troca com o de cima)
//   d ou C     (seta dir) ->     peca vai pra direita  (15 troca com o da esquerda)
//   e outros caracteres -> a gente nao faz nada


void move_caractere(int v[], char c)
{
	if (c == 'w' || c == 'W' || c == 'c' || c == 'A') {
		move_baixo(v);
	} else if (c == 'a' || c == 'e' || c == 'D') {
		move_direita(v);
	} else if (c == 's' ||c == 'b' || c == 'B') {
		move_cima(v);
	} else if (c == 'd' || c == 'C') {
		move_esquerda(v);
	}
	// outros caracteres: nao faz nada
}


// funcao 11 -- move a peca  na posicao informada, se possivel
// (so eh possivel se uma  posicao vizinha tiver o 15)
//
//
// obs.: implementei pelo raciocinio fisico em vez de seguir
// a letra do enunciado, porque a  movimentacao a esquerda troca
// o 15 com o numero a esquerda DELE,e nao da peca.
// Entao: se o 15  esta a esquerda da peca, chamo move_direita
// pra o 15 se deslocar pra direita (onde esta a peca.

void move_posicao(int v[], int pos)
{
	int pos15;
	int linha;
	int col;
	int linha15;
	int col15;

	pos15 = acha_numero(v, 15);

	//descobre linha e coluna da peca e do 15 na matriz 4x4 OK
	linha = pos / 4;
	col = pos % 4;
	linha15 = pos15 / 4;
	col15 = pos15 % 4;

	// 15 a esquerda da peca: peca vai pra esquerda, 15 anda pra direita
	if (linha == linha15 && col15 == col - 1) {
		move_direita(v);
	}
	// 15 a direita a peca vai pra direita, 15 anda pra esquerda
	else if (linha == linha15 && col15 == col + 1) {
		move_esquerda(v);
	}
	// 15 acima: peca  sobe, e o 15 desce
	else if (col == col15 && linha15 == linha - 1) {
		move_baixo(v);
	}
	// 15 abaixo a peca desce, o 15 sobe
	else if (col == col15 && linha15 == linha + 1) {
		move_cima(v);
	}
	// se 15 nao for vizinho da peca, a funcao nao faz nada!!
}



// funcao 12 -- desenha o tabuleiro com cores ANSI
// cada peca  e ocupa 3 linha de altura pra ficar mais legivel OK
//baseado na função imprime_tabuleiro_v8 do github do professor


void desenha_borda()
{
	cor_normal(); //a gente declarou no inicio
	printf("%3s", "");
	cor_tabuleiro();
	printf("%32s", "");
	cor_normal();
	pula_linha();
}

void imprime_pedaco_linha(int v[], int l, bool com_peca)
{
	int i;

	cor_normal();
	printf("%3s", "");
	cor_tabuleiro();
	printf("  ");
	for (i = 0; i < 4; i++) {
		int val = v[l * 4 + i];
		if (val == 15) {
			cor_vazio();
			printf("       "); // 15 eh casa vazia
		} else {
			cor_peca();
			// mostra o numero (1 ... 15) centralizado; pecas vao de 0 a 14,
			// e dai somei 1 pra ficar igual ao puzzle classico (1 a 15)
			
			
			if (com_peca) {
				printf("  %2d   ", val + 1);
			} else {
				printf("       ");
			}
		}
	}
	cor_tabuleiro();
	printf("  ");
	cor_normal();
	pula_linha();
}

void imprime_linha(int v[], int l)
{
	// cada peca tem 3 linhas: espaco, letra,  espaco
	imprime_pedaco_linha(v, l, false);
	imprime_pedaco_linha(v, l, true);
	imprime_pedaco_linha(v, l, false);
}

void desenha(int v[])
{
	int linha;
	limpa_tela();
	pula_linha();
	desenha_borda();
	for (linha = 0; linha < 4; linha++) {
		imprime_linha(v, linha);
	}
	desenha_borda();
	pula_linha();
}


// le uma tecla em modo raw, tratando sequencias de seta OK
// setas enviam ESC [ A/B/C/D -- retornamos so a letra final

char le_tecla()
{
	char c = getchar();
	if (c == 27) {        // ESC
		char c2 = getchar();
		if (c2 == '[') {
			return getchar(); // A, B, C ou D
		}
	}
	return c;
}


 // funcoes de teste OK e se for corrigir ignora essa parte ks
// DESATUALIZADO
void testa_preenche()
{
	int v[16];
	int i;
	preenche_vetor(v);
	for (i = 0; i < 16; i++) {
		if (v[i] != i) {
			printf("ERRO testa_preenche posicao %d\n", i);
		}
	}
}

void testa_acha_numero()
{
	//peguei a ideia desse teste das dicas do professor, adaptei pra
	// usar so numeros validos do puzzle (0 a 15)
	
	int v[] = { 1, 12,  3, 14,  5,  9,  7,  8, 6,  0, 11,  2, 13,  4, 15, 10 };
	if (acha_numero(v,  1) !=  0) printf("ERRO testa_acha_numero 1\n");
	if (acha_numero(v, 15) != 14) printf("ERRO testa_acha_numero 2\n");
	if (acha_numero(v, 99) != -1) printf("ERRO testa_acha_numero 3\n");
}

void testa_em_ordem()
{
	int ordenado[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 };
	int trocado[]  = { 1, 0, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 };
	if (!em_ordem(ordenado)) printf("ERRO testa_em_ordem 1\n");
	if ( em_ordem(trocado))  printf("ERRO testa_em_ordem 2\n");
}

void testa_movimentos()
{
	int v[16];
	int w[16];
	int i;

	// exemplo do enunciado da funcao 4
	// 1 3 2 4 5 8 7 6 9  15 10 11 12 0 14 13
	
	// apos move_esquerda:
	// 1 3 2 4 5 8 7 6 15  9 10 11 12 0 14 13
	
	
	{
		int t[] = { 1, 3, 2, 4, 5, 8, 7, 6, 9,15,10,11,12, 0,14,13 };
		for (i = 0; i < 16; i++) v[i] = t[i];
		move_esquerda(v);
		if (v[8] != 15 || v[9] != 9) {
			printf("ERRO testa_movimentos move_esquerda\n");
		}
	}

	// exemplo do enunciado da funcao 6 (cima)
	// 1 3 2 4 5 8 7 6 9 15 10 11 12 0 14 13
	
	// apos move_cima:
	// 1 3 2 4 5 15 7 6 9 8 10 11 12 0 14 13
	
	
	{
		int t[] = { 1, 3, 2, 4, 5, 8, 7, 6, 9,15,10,11,12, 0,14,13 };
		for (i = 0; i < 16; i++) w[i] = t[i];
		move_cima(w);
		if (w[5] != 15 || w[9] != 8) {
			printf("ERRO testa_movimentos move_cima\n");
		}
	}

	// testa que move_esquerda nao faz nada se 15 esta no inicio da linha
	for (i = 0; i < 16; i++) v[i] = i;
	v[0] = 15; v[15] = 0; // poe  o 15 no canto superior esquerdo
	move_esquerda(v);
	if (v[0] != 15) {
		printf("ERRO testa_movimentos move_esquerda na borda\n");
	}

	// testa que movecima nao faz nada se 15 esta na primeira linha
	for (i = 0; i < 16; i++) v[i] = i;
	v[2] = 15; v[15] = 2;
	move_cima(v);
	if (v[2] != 15) {
		printf("ERRO testa_movimentos move_cima na borda\n");
	}
}

void testa_move_caractere()
{
	int v[16];
	int i;

	// monta um estado co 15 no meio (posicao  5, linha 1 col 1)
	for (i = 0; i < 16; i++) v[i] = i;
	v[5] = 15; v[15] = 5;

	move_caractere(v, 'd'); //  'd'  peca pra direita = 15 pra esquerda
	if (v[4] != 15) printf("ERRO testa_move_caractere d\n");

	move_caractere(v, 'a'); 
	// peca pra esquerda = 15 pra direita
	if (v[5] != 15) printf("ERRO testa_move_caractere a\n");

	move_caractere(v, 'w'); // peca pra cima = 15 pra baixo
	if (v[9] != 15) printf("ERRO testa_move_caractere w\n");

	move_caractere(v, 's'); // peca pra baixo = 15 pra cima
	if (v[5] != 15) printf("ERRO testa_move_caractere s\n");
}

void testa_move_posicao()
{
	int v[16];
	int i;

	// 15 na posicao 5, peca na posicao 6
	for (i = 0; i < 16; i++) v[i] = i;
	v[5] = 15; v[15] = 5;

	// movendo a peca da posicao 6 (15 esta a esquerda dela na mesma linha)
	move_posicao(v, 6);
	if (v[5] != 6)  printf("ERRO testa_move_posicao 1\n");
	if (v[6] != 15) printf("ERRO testa_move_posicao 2\n");
}

void roda_testes()
{
	printf("heyy querido, estamos rodando os testes ... \n");
	testa_preenche();
	testa_acha_numero();
	testa_em_ordem();
	testa_movimentos();
	testa_move_caractere();
	testa_move_posicao();
	printf("Testes terminados (se nao apareceu ERRO acima, ta tudo certo viu CONFIA kk)\n\n");
}


// ========================================================
int main()
{
	int v[16];
	char c;

	// inicializa o gerador de numeros aleatorios com a hora atual
	// sem isso, o embaralhamento seria sempre igual a cada execucao OK
	//e so para eu n esquecer, o rand() gera números “aleatórios” e o srand() define a semente dessa aleatoriedade,
	// mas o srand vem na main caso eu queira usar rand em qualquer outra parte já tá no escopo, mas creio que n teria problema
	// anexar 
	srand((unsigned int)time(NULL));

	// eu TODO: comentar essa linha quando ja estiver funcionando OK
	// roda_testes();

	// prepara o tabuleiro
	preenche_vetor(v);
	embaralha(v);

	// coloca o terminal em modo raw -> e dai cada tecla chega imediatamente,
	// sem precisar apertar Enter, e sem echo o que foi digitado
	system("stty raw -echo");

	//roda ate resolver ou apertar Q Ok
	while (!em_ordem(v)) {
		desenha(v);
		printf("   Use wasd ou setas para mover  |  Q para sair\r\n");

		c = le_tecla();

		if (c == 'Q') {
			break;
		}

		move_caractere(v, c);
	}

	// restaura o terminal  antes de sair OK
	system("stty sane");


	//TODO: eu tenho que verificar o em ordem se está funcionando ou n
	if (em_ordem(v)) {
		desenha(v);
		printf("Parabens! Voce resolveu o quebra-cabecas e agora merece um 10!\n");
	} else {
		printf("\nObrigado por utilizar nosso programa. Ate logo!\n");
			printf("\nAte logo!\n");
	}
	

	return 0;
}
