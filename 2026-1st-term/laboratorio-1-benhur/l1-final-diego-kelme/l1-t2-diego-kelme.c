#include <stdio.h>
#include <stdlib.h>  // pra usar a funcao rand

// Trabalho 2 - Laboratorio de Programacao 1 - Diego Kelme
// Quebra-cabecas de pecas deslizantes (15-puzzle)

// O tabuleiro eh um vetor de 16 posicoes que representa uma matriz 4x4.
// As primeiras 4 posicoes (0..3) sao a primeira linha
// as 4 seguintes (4..7) a segunda linha, e assim por diante
// O numero 15 representa o espaco vazio do puzzle.


// funcao 1 -> preenche o vetor com 0..15 em ordem (0 no indice 0, 1 no indice 1, etc)
void preenche_vetor(int v[])
{
	int i;
	for (i = 0; i < 16; i++) {
		v[i] = i;
	}
}


// funcao 2 -- procura um numero no vetor e retorna o indice onde ele esta
// se nao encontrar, retorna -1
int acha_numero(int v[], int num)
{
	int i;
	for (i = 0; i < 16; i++) {
		if (v[i] == num) {
			return i;
		}
	}
	return -1;
}


// funcao 3 -> a gente verifica se o vetor esta em ordem (puzzle resolvido)
// e retorna 1 (true) se sim, 0 (false) se nao
// usei int como bool, ja que a gente nao tinha visto stdbool.h ainda
int em_ordem(int v[])
{
	int i;
	for (i = 0; i < 16; i++) {
		if (v[i] != i) {
			return 0;
		}
	}
	return 1;
}


// funcao 4 -- troca o 15 com o numero a esquerda dele (15 anda pra esquerda)
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

	// faz a troca usando uma variavel auxiliar
	aux = v[pos - 1];
	v[pos - 1] = v[pos];
	v[pos] = aux;
}


// funcao 5
// troca o 15 com o numero a direita dele (15 anda pra direita)
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


// funcao 6 ---- a gente troca o 15 com o numero acima dele (15 anda pra cima)
// se o 15 estiver na primeira linha, nao faz nada
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


// funcao 7
// troca o 15 com o numero abaixo dele (15 anda pra baixo) e
//  se o 15 estiver na ultima linha, nao faz nada
void move_baixo(int v[])
{
	int pos;
	int aux;

	pos = acha_numero(v, 15);
	// se o 15 esta na ultima linha (indices 12 ... 15), nao da pra mover
	
	
	if (pos >= 12) {
		return;
	}

	aux = v[pos + 4];
	v[pos + 4] = v[pos];
	v[pos] = aux;
}


// funcao 8
// recebe  o vetor e chama aleatoriamente uma das 4 funcoes de movimentacao
// uso a funcao rand() pra a gente escolher um numero entre 0 e 3, e cada um
// corresponde a uma das funcoes
void move_aleatorio(int v[])
{
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


// funcao 9 -- recebe o vetor e o embaralha, chamando diversas vezes a funcao acima
// como cada movimento parte de um estado valido (resolvido inicialmente),
// o resultado sempre vai ser um puzzle que	 tem solucao
void embaralha(int v[])
{
	int i;
	for (i = 0; i < 200; i++) {
		move_aleatorio(v);
	}
}


// funcao 10
// chama a funcao de movimentacao baseado no caractere recebido
//   w ou c ou A: peca pra cima    (15 troca com o de baixo)
//   a ou e ou D: peca pra esquerda (15 troca com o da direita)
//   s ou b ou B: peca pra baixo   (15 troca com o de cima)
//   d ou C:   peca pra direita (15 troca com o da esquerda)
// outros caracteres: a funcao nao faz nada
void move_caractere(int v[], char c)
{
	if (c == 'w' || c == 'c' || c == 'A') {
		move_baixo(v);
	} else if (c == 'a' || c == 'e' || c == 'D') {
		move_direita(v);
	} else if (c == 's' || c == 'b' || c == 'B') {
		move_cima(v);
	} else if (c == 'd' || c == 'C') {
		move_esquerda(v);
	} else {
		puts("\n");
	}
}


// funcao 11
// move a peca que esta na  posicao informada, se for possivel
// (so  eh possivel se uma posicao vizinha tiver o 15)

// obs.:  o exemplo do enunciado diz "se a posicao 1 tiver o 15, chama a
// movimentacao a esquerda", mas se eu fizer isso a peca nao se move de
// fato (porque a  movimentacao a esquerda troca o 15 com o numero a
// esquerda DELE proprio, nao da peca). Por isso implementei pelo
// raciocinio fisico: se o 15 esta a esquerda   da peca, a peca tem que
// ir pra esquerda, e pra peca ir pra esquerda eu preciso que o 15 ande
// pra direita, ou seja,  chamar move_direita.
void move_posicao(int v[], int pos)
{
	int pos15;
	int linha;
	int col;
	int linha15;
	int col15;

	pos15 = acha_numero(v, 15);

	// descobre linha e coluna da peca e do 15 na matriz 4x4
	linha = pos / 4;
	col = pos % 4;
	linha15 = pos15 / 4;
	col15 = pos15 % 4;

	// 15 a esquerda da peca: peca vai pra esquerda, 15 anda pra direita
	if (linha == linha15 && col15 == col - 1) {
		move_direita(v);
	}
	// 15 a direita: peca vai pra direita, 15 anda pra esquerda
	else if (linha == linha15 && col15 == col + 1) {
		move_esquerda(v);
	}
	// 15 acima: peca sobe, 15 desce
	else if (col == col15 && linha15 == linha - 1) {
		move_baixo(v);
	}
	// 15 abaixo: peca desce, 15 sobe
	else if (col == col15 && linha15 == linha + 1) {
		move_cima(v);
	}
	// se 15 nao for vizinho da peca, a funcao nao faz nada
}


// funcao 12
// desenha o tabuleiro na tela em formato ASCII
// usei o estilo de quadro com +, -, |  (parecido com o exercicio do fibonacci e
// na lista anterior na questão da tabela de radianos)
// a casa onde esta o 15 fica em branco (eh o espaco vazio do puzzle)
// usei puts pras linhas fixas (separadores),  igual eu fiz no fibonacci,
// pq o puts ja imprime o \n no fim sozinho

void desenha(int v[])
{
	int i;
	int j;
	int idx;

	puts("+----+----+----+----+");
	for (i = 0; i < 4; i++) {
		printf("|");  
		for (j = 0; j < 4; j++) {
			idx = i * 4 + j;
			if (v[idx] == 15) {
				printf("    |");   // essa casa d 15 fica vazia 
			} else {
				printf(" %2d |", v[idx]);
			}
		}
		puts(""); // imprime so o \n depois da linha de pecas
		puts("+----+----+----+----+");
	}
}


// =============================================================
// funcoes de teste que eu uso pra debugar cada funcao isolada
// 
// eu deixei roda_testes() chamado no comeco da main, ai
// antes do jogo comecar os testes rodam, e se algum print "ERRO"
// aparecer eu sei que tem coisa quebrada pra olhar.
// Depois que ja estiver tudo certo, eh so comentar a chamada.

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
	// peguei a ideia desse teste das dicas do professor, adaptei pra
	// usar so numeros validos do puzzle (0 a 15)
	int v[] = { 1, 12,  3, 14,  5,  9,  7,  8,
	            6,  0, 11,  2, 13,  4, 15, 10 };
	if (acha_numero(v,  1) !=  0) printf("ERRO testa_acha_numero 1\n");
	if (acha_numero(v, 15) != 14) printf("ERRO testa_acha_numero 2\n");
	if (acha_numero(v, 99) != -1) printf("ERRO testa_acha_numero 3\n");
}

void testa_em_ordem()
{
	int ordenado[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 };
	int trocado[]  = { 1, 0, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 };
	if (em_ordem(ordenado) != 1) printf("ERRO testa_em_ordem 1\n");
	if (em_ordem(trocado)  != 0) printf("ERRO testa_em_ordem 2\n");
}

void testa_movimentos()
{
	int v[16];
	int w[16];
	int i;

	// exemplo do enunciado da funcao 4
	// 1 3 2 4 5 8 7 6 9 15 10 11 12 0 14 13
	// apos move_esquerda:
	// 1 3 2 4 5 8 7 6 15 9 10 11 12 0 14 13
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

	
	//  testa que move_esquerda nao faz nada se 15 esta no inicio da linha
	for (i = 0; i < 16; i++) v[i] = i;
	v[0] = 15; v[15] = 0; //  poe o 15 no canto superior esquerdo
	move_esquerda(v);
	if (v[0] != 15) {
		printf("ERRO testa_movimentos move_esquerda na borda\n");
	}

	// testa que move_cima e  nao faz nada se 15 esta na primeira linha
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

	// monta um estado com 15 no meio (posicao 5, linha 1 col 1)
	// pra testar todos os movimentos
	for (i = 0; i < 16; i++) v[i] = i;
	v[5] = 15; v[15] = 5;

	move_caractere(v, 'd'); // 'd' = peca pra direita = 15 pra esquerda
	if (v[4] != 15) printf("ERRO testa_move_caractere d\n");

	move_caractere(v, 'a'); // peca pra esquerda = 15 pra direita
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

	// mesmo cenario: 15 na posicao 5, peca na posicao 6
	for (i = 0; i < 16; i++) v[i] = i;
	v[5] = 15; v[15] = 5;

	// movendo a peca da posicao 6 (15 esta a esquerda dela na mesma linha)
	move_posicao(v, 6);
	if (v[5] != 6)  printf("ERRO testa_move_posicao 1\n");
	if (v[6] != 15) printf("ERRO testa_move_posicao 2\n");
}

void roda_testes()
{
	printf("Rodando testes...\n");
	testa_preenche();
	testa_acha_numero();
	testa_em_ordem();
	testa_movimentos();
	testa_move_caractere();
	testa_move_posicao();
	printf("Testes terminados (se nao apareceu ERRO acima, ta tudo certo)\n\n");
}


// =============================================================
// programa main 
// =============================================================
int main()
{
	int v[16];
	char c;

	// roda os testes antes do jogo, ajuda bastante a debugar
	// pode comentar essa linha quando ja estiver funcionando
	roda_testes();

	// prepara o tabuleiro: declara, preenche e embaralha
	preenche_vetor(v);
	embaralha(v);

	printf("Quebra-cabecas de pecas deslizantes\n");
	printf("Use w/a/s/d para mover, Q para sair\n\n");

	// loop principal: roda enquanto o puzzle nao estiver resolvido
	// e o usuario nao tiver digitado Q
	while (em_ordem(v) == 0) {
		desenha(v);
		printf("Movimento: \n");
		scanf(" %c", &c);

		if (c == 'Q') {
			break;
		} 

		move_caractere(v, c);
	}

	// se saiu do loop por ter resolvido e dai mostra mensagem de parabens
	if (em_ordem(v) == 1) {
		desenha(v);
		printf("Parabens! Voce resolveu o nosso quebra-cabecas e agora merece um 10!\n");
	}

	printf("Obrigado por utilizar o nosso programa. Ate logo!\n");
	return 0;
}
