#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tela.h"

// Trabalho 3 - Laboratorio de Programacao 1 - Diego Kelme
// Gerenciador de notas (post-its), conforme https://github.com/BenhurUFSM/l126a/blob/main/Trabalhos/t3.md

// Cada nota tem um texto, uma etiqueta de 3 letras, uma cor (rgb) e um
// retangulo (posicao e tamanho na tela). As notas ficam num vetor alocado
// dinamicamente, que cresce quando enche e encolhe quando esvazia. No inicio
// a gente le o vetor de um arquivo, e grava de volta no fim (e na tecla 'g'),
// pra nao perder nada.
//
// O programa eh modal: a cada momento ele esta em um dos modos abaixo. Cada
// modo eh uma funcao com um laco while que (1) desenha a tela, (2) le uma
// tecla e (3) faz a acao correspondente num switch. A main so fica chamando a
// funcao do modo atual ate o modo virar "fim".
//   - principal       : navegacao 2D com o cursor por cima dos retangulos
//   - edita_texto      : altera o texto da nota corrente
//   - edita_etiqueta   : altera a etiqueta da nota corrente
//   - edita_cor        : altera a cor da nota corrente
//   - busca_texto      : altera o texto de busca (filtra as notas)
//   - busca_etiqueta   : altera a etiqueta de busca (filtra as notas)
//
// Esta versao suporta tudo: retangulos + cor + etiquetas.
//
// compila com: gcc -o l1-t3 l1-t3-diego-kelme.c tela.c
// executa com: ./l1-t3 [arquivo]


// =============================================================
// constantes e tipos
// =============================================================

#define MAX_TEXTO     100   // limite de bytes do texto (o enunciado pede pelo menos 100)
#define TAM_ETIQUETA    3   // toda etiqueta tem exatamente 3 caracteres
#define CAP_INICIAL     8   // capacidade inicial do vetor de notas
#define MIN_LADO        1   // menor largura/altura que um retangulo pode ter

#define ARQ_PADRAO    "notas.txt"
#define ARQ_PROBLEMAS "notas-problemas.txt"

// uma cor, com os tres componentes rgb entre 0 e 255
typedef struct {
	int r, g, b;
} cor;

// um ponto na tela (x eh a coluna, y eh a linha)
typedef struct {
	int x, y;
} ponto;

// um tamanho (largura e altura)
typedef struct {
	int larg, alt;
} tamanho;

// um retangulo: o canto superior esquerdo mais o tamanho
typedef struct {
	ponto pos;
	tamanho tam;
} retangulo;

// uma nota
typedef struct {
	char etiqueta[TAM_ETIQUETA + 1];   // 3 letras mais o '\0'
	cor c;
	retangulo ret;
	char texto[MAX_TEXTO + 1];
} nota;

// os modos de operacao (mesma ideia do ex2_tela.c do professor)
typedef enum {
	principal,
	edita_texto,
	edita_etiqueta,
	edita_cor,
	busca_texto,
	busca_etiqueta,
	fim,
} modo_t;

// o registro com tudo que o programa precisa, declarado na main e passado
// por referencia pras funcoes que precisam mexer nele
typedef struct {
	modo_t modo;

	nota *notas;        // vetor alocado dinamicamente
	int n;              // quantas notas estao em uso
	int cap;            // capacidade atual do vetor

	int tela_larg;      // tamanho da tela, em caracteres
	int tela_alt;
	ponto cursor;       // onde esta o cursor na tela

	bool tem_removida;  // existe uma "ultima nota removida"?
	nota removida;

	char busca_t[MAX_TEXTO + 1];        // texto de busca
	char busca_e[TAM_ETIQUETA + 1];     // etiqueta de busca

	// coisas que os modos de edicao usam
	char edt_texto[MAX_TEXTO + 1];      // texto que esta sendo editado
	int edt_cursor;                     // cursor dentro do texto editado
	char edt_etiqueta[TAM_ETIQUETA + 1];// etiqueta sendo editada
	cor edt_cor;                        // cor sendo editada
	int comp_sel;                       // componente de cor selecionado (0=r 1=g 2=b)
	bool ultimo_digito;                 // a tecla anterior foi um digito? (edicao de cor)
	int busca_cursor;                   // cursor dentro do texto de busca

	char nome_arquivo[256];
} estado_t;


// =============================================================
// funcoes utilitarias pequenas
// =============================================================

// deixa v dentro do intervalo [lo, hi]
int limita(int v, int lo, int hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

// o caractere serve pro texto de uma nota? imprimivel ascii (32 a 126),
// menos as aspas duplas
bool valido_texto(int c)
{
	return c >= 32 && c <= 126 && c != '"';
}

// o caractere serve pra uma etiqueta? so letra maiuscula ou digito
bool valido_etiqueta(int c)
{
	return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// devolve uma cor de texto que contrasta com o fundo c.
// se o fundo for escuro (media dos rgb menor que 120), o texto vai claro,
// e se for claro, o texto vai escuro
cor cor_contraste(cor c)
{
	int media;
	cor clara = { 240, 240, 240 };
	cor escura = { 16, 16, 16 };

	media = (c.r + c.g + c.b) / 3;
	if (media < 120) {
		return clara;
	}
	return escura;
}

// o ponto p esta dentro do retangulo r?
bool dentro(retangulo r, ponto p)
{
	return p.x >= r.pos.x && p.x < r.pos.x + r.tam.larg
	    && p.y >= r.pos.y && p.y < r.pos.y + r.tam.alt;
}


// =============================================================
// vetor de notas com alocacao dinamica
// =============================================================

// garante que cabe pelo menos mais uma nota no vetor. se estiver cheio,
// dobra a capacidade. retorna false se a realocacao falhar (ai a gente nega
// a inclusao e segue com o que tinha)
bool garante_espaco(estado_t *e)
{
	int nova;
	nota *novo;

	if (e->n < e->cap) {
		return true;
	}
	nova = (e->cap < CAP_INICIAL) ? CAP_INICIAL : e->cap * 2;
	novo = realloc(e->notas, nova * sizeof(nota));
	if (novo == NULL) {
		return false;
	}
	e->notas = novo;
	e->cap = nova;
	return true;
}

// encolhe o vetor quando a ocupacao fica muito baixa (menos de 30% da
// capacidade), pra nao desperdicar memoria. nunca abaixo de CAP_INICIAL
void talvez_encolhe(estado_t *e)
{
	int nova;
	nota *novo;

	if (e->cap <= CAP_INICIAL) return;
	if (e->n >= e->cap * 3 / 10) return;

	nova = e->cap / 2;
	if (nova < CAP_INICIAL) {
		nova = CAP_INICIAL;
	}
	novo = realloc(e->notas, nova * sizeof(nota));
	if (novo != NULL) {
		e->notas = novo;
		e->cap = nova;
	}
}

// insere a nota nt na posicao idx, empurrando as outras pra frente.
// mantem a ordem das demais. retorna false se faltar memoria
bool insere_nota(estado_t *e, nota nt, int idx)
{
	int i;

	if (!garante_espaco(e)) {
		return false;
	}
	idx = limita(idx, 0, e->n);
	for (i = e->n; i > idx; i--) {
		e->notas[i] = e->notas[i - 1];
	}
	e->notas[idx] = nt;
	e->n++;
	return true;
}

// remove a nota da posicao idx, puxando as outras pra tras
void remove_nota(estado_t *e, int idx)
{
	int i;

	if (idx < 0 || idx >= e->n) return;
	for (i = idx; i < e->n - 1; i++) {
		e->notas[i] = e->notas[i + 1];
	}
	e->n--;
	talvez_encolhe(e);
}

// tira a nota de "origem" e coloca em "destino", mantendo a ordem das outras
void move_nota(estado_t *e, int origem, int destino)
{
	nota tmp;

	if (origem < 0 || origem >= e->n) return;
	tmp = e->notas[origem];
	remove_nota(e, origem);
	insere_nota(e, tmp, destino);
}


// =============================================================
// filtragem pela busca e escolha da nota corrente
// =============================================================

// a nota de indice i deve ser ignorada por causa do texto ou da etiqueta de
// busca? (se a busca de texto nao bater, ou a etiqueta nao comecar igual)
bool filtrada(estado_t *e, int i)
{
	nota *nt = &e->notas[i];

	if (e->busca_t[0] != '\0' && strstr(nt->texto, e->busca_t) == NULL) {
		return true;
	}
	if (e->busca_e[0] != '\0'
	    && strncmp(nt->etiqueta, e->busca_e, strlen(e->busca_e)) != 0) {
		return true;
	}
	return false;
}

// conta quantas notas estao visiveis (nao filtradas)
int conta_visiveis(estado_t *e)
{
	int i;
	int q = 0;

	for (i = 0; i < e->n; i++) {
		if (!filtrada(e, i)) q++;
	}
	return q;
}

// indice da nota corrente: a nota nao filtrada mais proxima do fim do vetor
// que esteja debaixo do cursor. retorna -1 se nao tiver nota no cursor
int nota_corrente(estado_t *e)
{
	int i;

	for (i = e->n - 1; i >= 0; i--) {
		if (filtrada(e, i)) continue;
		if (dentro(e->notas[i].ret, e->cursor)) return i;
	}
	return -1;
}


// =============================================================
// leitura e escrita do arquivo
// =============================================================

// le uma linha do arquivo pra uma string alocada dinamicamente (sem o '\n').
// retorna NULL no fim do arquivo. quem chama tem que dar free
char *le_linha(FILE *f)
{
	int cap = 64;
	int n = 0;
	int c;
	char *s = malloc(cap);
	char *t;

	if (s == NULL) return NULL;
	while ((c = fgetc(f)) != EOF && c != '\n') {
		if (n + 1 >= cap) {
			cap = cap * 2;
			t = realloc(s, cap);
			if (t == NULL) { free(s); return NULL; }
			s = t;
		}
		s[n++] = c;
	}
	if (c == EOF && n == 0) {
		free(s);
		return NULL;
	}
	s[n] = '\0';
	return s;
}

// le um inteiro a partir de *p (pulando os espacos da frente) e avanca *p.
// retorna false se nao tiver um numero ali
bool le_inteiro(const char **p, int *valor)
{
	const char *s = *p;
	int n = 0;
	bool neg = false;

	while (*s == ' ' || *s == '\t') s++;
	if (*s == '-') { neg = true; s++; }
	if (*s < '0' || *s > '9') {
		return false;
	}
	while (*s >= '0' && *s <= '9') {
		n = n * 10 + (*s - '0');
		s++;
	}
	*valor = neg ? -n : n;
	*p = s;
	return true;
}

// interpreta uma linha do arquivo e preenche *nt.
// retorna false se a linha estiver mal formatada.
// poe *trunc em true se o texto teve que ser cortado por ser grande demais
bool interpreta_linha(const char *linha, nota *nt, bool *trunc)
{
	const char *p = linha;
	int r, g, b, x, y, larg, alt;
	int i, k;
	unsigned char ch;

	*trunc = false;

	// pula os espacos do comeco
	while (*p == ' ' || *p == '\t') p++;

	// etiqueta: exatamente 3 caracteres validos
	for (i = 0; i < TAM_ETIQUETA; i++) {
		if (!valido_etiqueta(p[i])) {
			return false;
		}
		nt->etiqueta[i] = p[i];
	}
	nt->etiqueta[TAM_ETIQUETA] = '\0';
	p += TAM_ETIQUETA;

	// a cor (3 numeros) e o retangulo (4 numeros)
	if (!le_inteiro(&p, &r))    return false;
	if (!le_inteiro(&p, &g))    return false;
	if (!le_inteiro(&p, &b))    return false;
	if (!le_inteiro(&p, &x))    return false;
	if (!le_inteiro(&p, &y))    return false;
	if (!le_inteiro(&p, &larg)) return false;
	if (!le_inteiro(&p, &alt))  return false;

	// o texto comeca depois da aspa de abertura
	while (*p == ' ' || *p == '\t') p++;
	if (*p != '"') {
		return false;
	}
	p++;
	k = 0;
	while (*p != '\0' && *p != '"') {
		ch = (unsigned char) *p;
		p++;
		// sem suporte a utf8: ignora codigos acima de 126 e os de controle
		if (ch < 32 || ch > 126) continue;
		if (k < MAX_TEXTO) {
			nt->texto[k++] = ch;
		} else {
			*trunc = true;   // sobrou texto, a linha vai pro arquivo de problemas
		}
	}
	if (*p != '"') {
		return false;   // faltou fechar as aspas
	}
	nt->texto[k] = '\0';

	// guarda a cor e o retangulo, ja ajustando aos limites validos
	nt->c.r = limita(r, 0, 255);
	nt->c.g = limita(g, 0, 255);
	nt->c.b = limita(b, 0, 255);
	nt->ret.pos.x = x;
	nt->ret.pos.y = y;
	nt->ret.tam.larg = (larg < MIN_LADO) ? MIN_LADO : larg;
	nt->ret.tam.alt  = (alt  < MIN_LADO) ? MIN_LADO : alt;
	return true;
}

// joga uma linha problematica no fim do arquivo de problemas
void grava_problema(const char *linha)
{
	FILE *f = fopen(ARQ_PROBLEMAS, "a");

	if (f == NULL) return;
	fprintf(f, "%s\n", linha);
	fclose(f);
}

// le todas as notas do arquivo pro estado.
// le uma linha de cada vez: se a linha estiver boa, inclui a nota; se estiver
// estragada (ou o texto for longo demais), copia a linha pro arquivo de
// problemas e segue lendo as outras
void carrega_arquivo(estado_t *e)
{
	FILE *f = fopen(e->nome_arquivo, "r");
	char *linha;
	nota nt;
	bool trunc;

	if (f == NULL) return;   // arquivo ainda nao existe, comeca vazio
	while ((linha = le_linha(f)) != NULL) {
		if (interpreta_linha(linha, &nt, &trunc)) {
			if (insere_nota(e, nt, e->n)) {
				if (trunc) grava_problema(linha);
			}
		} else {
			grava_problema(linha);
		}
		free(linha);
	}
	fclose(f);
}

// grava uma nota no arquivo, no formato que o enunciado pede
void grava_nota(FILE *f, nota *nt)
{
	fprintf(f, "%s %d %d %d %d %d %d %d \"%s\"\n",
	        nt->etiqueta, nt->c.r, nt->c.g, nt->c.b,
	        nt->ret.pos.x, nt->ret.pos.y, nt->ret.tam.larg, nt->ret.tam.alt,
	        nt->texto);
}

// grava todas as notas no arquivo
void grava_arquivo(estado_t *e)
{
	FILE *f = fopen(e->nome_arquivo, "w");
	int i;

	if (f == NULL) return;
	for (i = 0; i < e->n; i++) {
		grava_nota(f, &e->notas[i]);
	}
	fclose(f);
}


// =============================================================
// acoes (uma funcao por acao; estas funcoes NAO escrevem na tela,
// so mexem nos dados)
// =============================================================

// anda com o cursor um passo na direcao dada, sem deixar sair da tela
void move_cursor(estado_t *e, int dx, int dy)
{
	e->cursor.x = limita(e->cursor.x + dx, 1, e->tela_larg);
	e->cursor.y = limita(e->cursor.y + dy, 1, e->tela_alt);
}

// ajeita o retangulo r pra ele conter o cursor, sem sair da tela.
// usado quando a gente cria ou reinsere uma nota
void posiciona_no_cursor(estado_t *e, retangulo *r)
{
	r->pos.x = e->cursor.x;
	r->pos.y = e->cursor.y;
	if (r->pos.x + r->tam.larg - 1 > e->tela_larg) {
		r->pos.x = e->tela_larg - r->tam.larg + 1;
	}
	if (r->pos.y + r->tam.alt - 1 > e->tela_alt) {
		r->pos.y = e->tela_alt - r->tam.alt + 1;
	}
	r->pos.x = limita(r->pos.x, 1, e->tela_larg);
	r->pos.y = limita(r->pos.y, 1, e->tela_alt);
}

// cria uma nota nova, com conteudo padrao, no fim do vetor, em cima do cursor
void nova_nota(estado_t *e)
{
	nota nt;
	cor padrao = { 200, 200, 120 };

	strcpy(nt.etiqueta, "AAA");
	nt.c = padrao;
	nt.ret.tam.larg = 10;
	nt.ret.tam.alt = 4;
	nt.texto[0] = '\0';
	posiciona_no_cursor(e, &nt.ret);
	insere_nota(e, nt, e->n);   // se faltar memoria, simplesmente nao inclui
}

// remove a nota corrente e guarda ela como "ultima nota removida"
void remove_corrente(estado_t *e)
{
	int corr = nota_corrente(e);

	if (corr < 0) return;
	e->removida = e->notas[corr];
	e->tem_removida = true;
	remove_nota(e, corr);
}

// reinsere a ultima nota removida, no fim do vetor, em cima do cursor
void reinsere_removida(estado_t *e)
{
	nota nt;

	if (!e->tem_removida) return;
	nt = e->removida;
	posiciona_no_cursor(e, &nt.ret);
	if (insere_nota(e, nt, e->n)) {
		e->tem_removida = false;
	}
}

// poe o cursor em cima da nota mais proxima do fim do vetor
void pula_para_nota(estado_t *e)
{
	int i;
	retangulo r;

	for (i = e->n - 1; i >= 0; i--) {
		if (filtrada(e, i)) continue;
		r = e->notas[i].ret;
		e->cursor.x = limita(r.pos.x + r.tam.larg / 2, 1, e->tela_larg);
		e->cursor.y = limita(r.pos.y + r.tam.alt / 2, 1, e->tela_alt);
		return;
	}
}

// move o retangulo da nota corrente um passo, levando o cursor junto
void move_retangulo(estado_t *e, int dx, int dy)
{
	int corr = nota_corrente(e);
	retangulo *r;

	if (corr < 0) {
		move_cursor(e, dx, dy);
		return;
	}
	r = &e->notas[corr].ret;
	// so move se o retangulo continuar inteiro dentro da tela
	if (r->pos.x + dx >= 1 && r->pos.x + r->tam.larg - 1 + dx <= e->tela_larg
	 && r->pos.y + dy >= 1 && r->pos.y + r->tam.alt - 1 + dy <= e->tela_alt) {
		r->pos.x += dx;
		r->pos.y += dy;
	}
	move_cursor(e, dx, dy);
}

// aumenta o retangulo da nota corrente na direcao indicada (sem mexer no
// outro lado), se tiver espaco na tela
void aumenta_retangulo(estado_t *e, char dir)
{
	int corr = nota_corrente(e);
	retangulo *r;

	if (corr < 0) return;
	r = &e->notas[corr].ret;
	if (dir == 'h' && r->pos.x > 1)                              { r->pos.x--; r->tam.larg++; }
	if (dir == 'l' && r->pos.x + r->tam.larg - 1 < e->tela_larg) { r->tam.larg++; }
	if (dir == 'k' && r->pos.y > 1)                             { r->pos.y--; r->tam.alt++; }
	if (dir == 'j' && r->pos.y + r->tam.alt - 1 < e->tela_alt)   { r->tam.alt++; }
}

// diminui o retangulo da nota corrente na direcao indicada (sem mexer no
// outro lado), enquanto for maior que o minimo. se o retangulo sair de baixo
// do cursor, o cursor anda junto pra continuar dentro
void diminui_retangulo(estado_t *e, char dir)
{
	int corr = nota_corrente(e);
	retangulo *r;

	if (corr < 0) return;
	r = &e->notas[corr].ret;
	if (dir == 'h' && r->tam.larg > MIN_LADO) {        // encolhe pela esquerda
		r->pos.x++; r->tam.larg--;
		if (e->cursor.x < r->pos.x) e->cursor.x = r->pos.x;
	}
	if (dir == 'l' && r->tam.larg > MIN_LADO) {        // encolhe pela direita
		r->tam.larg--;
		if (e->cursor.x > r->pos.x + r->tam.larg - 1) e->cursor.x = r->pos.x + r->tam.larg - 1;
	}
	if (dir == 'k' && r->tam.alt > MIN_LADO) {         // encolhe por cima
		r->pos.y++; r->tam.alt--;
		if (e->cursor.y < r->pos.y) e->cursor.y = r->pos.y;
	}
	if (dir == 'j' && r->tam.alt > MIN_LADO) {         // encolhe por baixo
		r->tam.alt--;
		if (e->cursor.y > r->pos.y + r->tam.alt - 1) e->cursor.y = r->pos.y + r->tam.alt - 1;
	}
}

// leva a nota corrente pro inicio do vetor
void manda_inicio(estado_t *e)
{
	int corr = nota_corrente(e);

	if (corr >= 0) move_nota(e, corr, 0);
}

// leva a nota corrente pro fim do vetor
void manda_fim(estado_t *e)
{
	int corr = nota_corrente(e);

	if (corr >= 0) move_nota(e, corr, e->n);
}

// remove o caractere da posicao pos da string s
void remove_char(char *s, int pos)
{
	int i;
	int tam = strlen(s);

	if (pos < 0 || pos >= tam) return;
	for (i = pos; i < tam; i++) {
		s[i] = s[i + 1];
	}
}

// insere o caractere ch na posicao pos da string s, se couber (limite max)
bool insere_char(char *s, int pos, char ch, int max)
{
	int i;
	int tam = strlen(s);

	if (tam >= max) return false;
	for (i = tam; i > pos; i--) {
		s[i] = s[i - 1];
	}
	s[pos] = ch;
	s[tam + 1] = '\0';
	return true;
}


// =============================================================
// desenho (uma funcao por modo; TODO comando de saida pra tela mora aqui)
// =============================================================

// imprime um caractere na posicao (lin, col), so se estiver dentro da tela
void poe_char(estado_t *e, int lin, int col, char ch)
{
	if (lin < 1 || lin > e->tela_alt) return;
	if (col < 1 || col > e->tela_larg) return;
	t_lincol(lin, col);
	putchar(ch);
}

// desenha uma nota: pinta o retangulo com a cor dela, escreve o texto fluindo
// pelas linhas e poe a etiqueta no canto de cima a esquerda
void desenha_nota(estado_t *e, int idx)
{
	nota *nt = &e->notas[idx];
	cor fundo = nt->c;
	cor letra = cor_contraste(fundo);
	retangulo r = nt->ret;
	int tam = strlen(nt->texto);
	int p = 0;
	int dl, dc, i;
	char ch;

	t_corfundo(fundo.r, fundo.g, fundo.b);
	t_cortexto(letra.r, letra.g, letra.b);

	// o texto usa o retangulo inteiro, uma linha depois da outra
	for (dl = 0; dl < r.tam.alt; dl++) {
		for (dc = 0; dc < r.tam.larg; dc++) {
			ch = (p < tam) ? nt->texto[p] : ' ';
			if (p < tam) p++;
			poe_char(e, r.pos.y + dl, r.pos.x + dc, ch);
		}
	}

	// a etiqueta vai no canto, com as cores trocadas pra destacar
	t_corfundo(letra.r, letra.g, letra.b);
	t_cortexto(fundo.r, fundo.g, fundo.b);
	for (i = 0; i < TAM_ETIQUETA; i++) {
		poe_char(e, r.pos.y, r.pos.x + i, nt->etiqueta[i]);
	}
}

// desenha uma moldura em volta da nota corrente, pra ela ficar facil de achar
void desenha_moldura(estado_t *e, int idx)
{
	retangulo r = e->notas[idx].ret;
	cor c = e->notas[idx].c;
	cor m = cor_contraste(c);
	int l = r.pos.y, col = r.pos.x, dl = r.tam.alt, dc = r.tam.larg;
	int i;

	t_corfundo(m.r, m.g, m.b);
	t_cortexto(c.r, c.g, c.b);
	for (i = 0; i < dc; i++) {
		poe_char(e, l, col + i, '=');
		poe_char(e, l + dl - 1, col + i, '=');
	}
	for (i = 0; i < dl; i++) {
		poe_char(e, l + i, col, '|');
		poe_char(e, l + i, col + dc - 1, '|');
	}
}

// desenha a barra de status na ultima linha da tela
void desenha_status(estado_t *e, const char *texto)
{
	int col;

	t_cornormal();
	t_lincol(e->tela_alt, 1);
	for (col = 1; col <= e->tela_larg; col++) {
		putchar(' ');
	}
	t_lincol(e->tela_alt, 1);
	printf("%s", texto);
}

// desenha uma string a partir de uma linha, quebrando na largura da tela, e
// deixa o cursor no caractere de indice cur. serve pros dois modos de edicao
// de texto (o da nota e o da busca)
void desenha_texto_editavel(estado_t *e, int linha0, const char *s, int cur)
{
	int larg = e->tela_larg;
	int tam = strlen(s);
	int i, lin, col;
	char ch;

	for (i = 0; i <= tam; i++) {
		lin = linha0 + i / larg;
		col = 1 + i % larg;
		ch = (i < tam) ? s[i] : ' ';
		if (lin <= e->tela_alt) {
			t_lincol(lin, col);
			putchar(ch);
		}
	}
	lin = linha0 + cur / larg;
	col = 1 + cur % larg;
	t_cursor(c_barra, c_pisca);
	t_lincol(lin, col);
}

// tela do modo principal
void desenha_principal(estado_t *e)
{
	int i, corr;
	char info[256];

	t_limpa();

	// as notas sao desenhadas na ordem do vetor, entao as do fim aparecem
	// por cima das anteriores quando ha sobreposicao
	for (i = 0; i < e->n; i++) {
		if (!filtrada(e, i)) desenha_nota(e, i);
	}

	// destaca a nota corrente
	corr = nota_corrente(e);
	if (corr >= 0) desenha_moldura(e, corr);

	// status com a contagem de notas, os filtros e as teclas
	snprintf(info, sizeof info,
	         "notas:%d/%d  busca:\"%s\"  etiq:\"%s\"  "
	         "[hjkl]move [n]ova [e]dita [c]or [t]ag [b]usca [g]rava [ESC]sai",
	         conta_visiveis(e), e->n, e->busca_t, e->busca_e);
	desenha_status(e, info);

	// o cursor por cima de tudo
	t_cursor(c_bloco, c_pisca);
	t_lincol(e->cursor.y, e->cursor.x);
	fflush(stdout);
}

// tela do modo de edicao de texto
void desenha_edita_texto(estado_t *e)
{
	t_limpa();
	t_cornormal();
	t_lincol(1, 1);
	printf("EDICAO DE TEXTO   [Enter] confirma   [ESC] cancela");
	desenha_texto_editavel(e, 3, e->edt_texto, e->edt_cursor);
	fflush(stdout);
}

// tela do modo de edicao de etiqueta
void desenha_edita_etiqueta(estado_t *e)
{
	t_limpa();
	t_cornormal();
	t_lincol(1, 1);
	printf("EDICAO DE ETIQUETA   [Enter] aplica   "
	       "[ctrl+t] aplica a todas visiveis   [ESC] cancela");
	t_lincol(3, 1);
	printf("Etiqueta: %s", e->edt_etiqueta);
	t_cursor(c_sublinha, c_pisca);
	t_lincol(3, 11 + (int) strlen(e->edt_etiqueta));
	fflush(stdout);
}

// tela do modo de edicao de cor
void desenha_edita_cor(estado_t *e)
{
	cor c = e->edt_cor;
	const char *nome[3] = { "R", "G", "B" };
	int val[3];
	int l, i;

	val[0] = c.r; val[1] = c.g; val[2] = c.b;

	t_limpa();
	t_cornormal();
	t_lincol(1, 1);
	printf("EDICAO DE COR   [r/g/b] seleciona   [setas/jk] ajusta   "
	       "[Enter] aplica   [ctrl+t] todas   [ESC] cancela");

	// uma amostra da cor
	t_corfundo(c.r, c.g, c.b);
	for (l = 3; l <= 8; l++) {
		t_lincol(l, 4);
		printf("              ");
	}

	// os valores dos componentes, com o selecionado destacado
	t_cornormal();
	for (i = 0; i < 3; i++) {
		t_lincol(3 + i, 22);
		if (i == e->comp_sel) {
			printf("> %s: %3d <", nome[i], val[i]);
		} else {
			printf("  %s: %3d", nome[i], val[i]);
		}
	}
	t_cursor(c_nada, c_naopisca);
	fflush(stdout);
}

// tela do modo de busca por texto
void desenha_busca_texto(estado_t *e)
{
	t_limpa();
	t_cornormal();
	t_lincol(1, 1);
	printf("BUSCA POR TEXTO   [Enter] aplica   [ESC] limpa e sai");
	desenha_texto_editavel(e, 3, e->busca_t, e->busca_cursor);
	fflush(stdout);
}

// tela do modo de busca por etiqueta
void desenha_busca_etiqueta(estado_t *e)
{
	t_limpa();
	t_cornormal();
	t_lincol(1, 1);
	printf("BUSCA POR ETIQUETA   [Enter] aplica   [ESC] limpa e sai");
	t_lincol(3, 1);
	printf("Etiqueta: %s", e->busca_e);
	t_cursor(c_sublinha, c_pisca);
	t_lincol(3, 11 + (int) strlen(e->busca_e));
	fflush(stdout);
}


// =============================================================
// os modos (cada um eh um while que desenha a tela, le uma tecla e faz a
// acao no switch). a tecla so e lida depois de um do/while pra esperar o
// usuario, ja que o t_tecla nao bloqueia (devolve T_NADA se nada foi teclado)
// =============================================================

// modo principal: navegacao 2D com o cursor por cima dos retangulos
void executa_principal(estado_t *e)
{
	tecla_t tecla;

	while (e->modo == principal) {
		desenha_principal(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		switch ((int) tecla) {
			// sair do programa
			case T_ESC: case 'q': e->modo = fim; break;

			// mover o cursor
			case 'h': case T_ESQUERDA: move_cursor(e, -1,  0); break;
			case 'l': case T_DIREITA:  move_cursor(e, +1,  0); break;
			case 'k': case T_CIMA:     move_cursor(e,  0, -1); break;
			case 'j': case T_BAIXO:    move_cursor(e,  0, +1); break;

			// mover o retangulo da nota corrente (shift)
			case 'H': case T_S_ESQUERDA: move_retangulo(e, -1,  0); break;
			case 'L': case T_S_DIREITA:  move_retangulo(e, +1,  0); break;
			case 'K': case T_S_CIMA:     move_retangulo(e,  0, -1); break;
			case 'J': case T_S_BAIXO:    move_retangulo(e,  0, +1); break;

			// aumentar o retangulo (ctrl + direcao)
			case T_CTRL_H: case T_C_ESQUERDA: aumenta_retangulo(e, 'h'); break;
			case T_CTRL_L: case T_C_DIREITA:  aumenta_retangulo(e, 'l'); break;
			case T_CTRL_K: case T_C_CIMA:     aumenta_retangulo(e, 'k'); break;
			case T_CTRL_J: case T_C_BAIXO:    aumenta_retangulo(e, 'j'); break;

			// diminuir o retangulo (ctrl+y/u/i/o ou alt + direcao);
			// as letras y/u/i/o sao so a direcao (esq/baixo/cima/dir)
			case T_CTRL_Y: case T_A_ESQUERDA: diminui_retangulo(e, 'h'); break;
			case T_CTRL_U: case T_A_BAIXO:    diminui_retangulo(e, 'j'); break;
			case T_CTRL_I: case T_A_CIMA:     diminui_retangulo(e, 'k'); break;
			case T_CTRL_O: case T_A_DIREITA:  diminui_retangulo(e, 'l'); break;

			// ordem da nota corrente no vetor
			case 'i': manda_inicio(e); break;
			case 'f': manda_fim(e);    break;

			// remover, reinserir, criar
			case 'd': case T_DEL: remove_corrente(e);   break;
			case 'I': case T_INS: reinsere_removida(e); break;
			case 'n':             nova_nota(e);         break;

			// poe o cursor em cima da ultima nota
			case 'p': pula_para_nota(e); break;

			// gravar o arquivo
			case 'g': grava_arquivo(e); break;

			// trocar de modo
			case 'e': e->modo = edita_texto;    break;
			case 'c': e->modo = edita_cor;      break;
			case 't': e->modo = edita_etiqueta; break;
			case 'b': e->modo = busca_texto;    break;
			case 'B': e->modo = busca_etiqueta; break;

			default: ;   // qualquer outra tecla eh ignorada
		}
	}
}

// modo de edicao do texto da nota corrente
void executa_edita_texto(estado_t *e)
{
	tecla_t tecla;
	int corr;
	int tam;

	// ao entrar no modo, copia o texto da nota corrente pra edicao.
	// se nao tem nota corrente, volta pro principal
	corr = nota_corrente(e);
	if (corr < 0) {
		e->modo = principal;
		return;
	}
	strcpy(e->edt_texto, e->notas[corr].texto);
	e->edt_cursor = strlen(e->edt_texto);

	while (e->modo == edita_texto) {
		desenha_edita_texto(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		tam = strlen(e->edt_texto);
		switch ((int) tecla) {
			case T_ENTER:   // confirma: copia o texto editado pra nota
				corr = nota_corrente(e);
				if (corr >= 0) strcpy(e->notas[corr].texto, e->edt_texto);
				e->modo = principal;
				break;
			case T_ESC: case T_CTRL_C:   // cancela, nao mexe na nota
				e->modo = principal;
				break;
			case T_BS: case T_CTRL_B:    // apaga o caractere antes do cursor
				if (e->edt_cursor > 0) {
					e->edt_cursor--;
					remove_char(e->edt_texto, e->edt_cursor);
				}
				break;
			case T_DEL: case T_CTRL_D:   // apaga o caractere em cima do cursor
				if (e->edt_cursor < tam) remove_char(e->edt_texto, e->edt_cursor);
				break;
			case T_ESQUERDA: case T_CTRL_H:
				if (e->edt_cursor > 0) e->edt_cursor--;
				break;
			case T_DIREITA: case T_CTRL_L:
				if (e->edt_cursor < tam) e->edt_cursor++;
				break;
			case T_HOME: case T_CTRL_K:
				e->edt_cursor = 0;
				break;
			case T_END: case T_CTRL_J:
				e->edt_cursor = tam;
				break;
			default:
				if (valido_texto(tecla)) {
					if (insere_char(e->edt_texto, e->edt_cursor, tecla, MAX_TEXTO)) {
						e->edt_cursor++;
					}
				}
		}
	}
}

// modo de edicao da etiqueta da nota corrente
void executa_edita_etiqueta(estado_t *e)
{
	tecla_t tecla;
	int corr, i, tam;

	// ao entrar, copia a etiqueta da nota corrente (se houver)
	corr = nota_corrente(e);
	if (corr >= 0) strcpy(e->edt_etiqueta, e->notas[corr].etiqueta);
	else e->edt_etiqueta[0] = '\0';

	while (e->modo == edita_etiqueta) {
		desenha_edita_etiqueta(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		tam = strlen(e->edt_etiqueta);
		switch ((int) tecla) {
			case T_ESC: case T_CTRL_C:
				e->modo = principal;
				break;
			case T_BS: case T_CTRL_B:
				if (tam > 0) e->edt_etiqueta[tam - 1] = '\0';
				break;
			case T_ENTER:   // aplica na nota corrente
				if (tam == TAM_ETIQUETA) {
					corr = nota_corrente(e);
					if (corr >= 0) strcpy(e->notas[corr].etiqueta, e->edt_etiqueta);
					e->modo = principal;
				}
				break;
			case T_CTRL_T: case T_ALT_ENTER:   // aplica em todas as visiveis
				if (tam == TAM_ETIQUETA) {
					for (i = 0; i < e->n; i++) {
						if (!filtrada(e, i)) strcpy(e->notas[i].etiqueta, e->edt_etiqueta);
					}
					e->modo = principal;
				}
				break;
			default:
				if (valido_etiqueta(tecla) && tam < TAM_ETIQUETA) {
					e->edt_etiqueta[tam] = tecla;
					e->edt_etiqueta[tam + 1] = '\0';
				}
		}
	}
}

// devolve um ponteiro pro componente de cor selecionado
int *componente(estado_t *e)
{
	if (e->comp_sel == 0) return &e->edt_cor.r;
	if (e->comp_sel == 1) return &e->edt_cor.g;
	return &e->edt_cor.b;
}

// modo de edicao da cor da nota corrente
void executa_edita_cor(estado_t *e)
{
	tecla_t tecla;
	int corr, i;
	int *comp;
	bool digitou;

	// ao entrar, copia a cor da nota corrente (se houver)
	corr = nota_corrente(e);
	if (corr >= 0) e->edt_cor = e->notas[corr].c;
	e->comp_sel = 0;
	e->ultimo_digito = false;

	while (e->modo == edita_cor) {
		desenha_edita_cor(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		comp = componente(e);
		digitou = false;
		switch ((int) tecla) {
			case 'e': case 'r': e->comp_sel = 0; break;
			case 'v': case 'g': e->comp_sel = 1; break;
			case 'a': case 'b': e->comp_sel = 2; break;
			case 'l': case T_DIREITA:  e->comp_sel = (e->comp_sel + 1) % 3; break;
			case 'h': case T_ESQUERDA: e->comp_sel = (e->comp_sel + 2) % 3; break;

			case 'k': case T_CIMA:    if (*comp < 255) (*comp)++; break;
			case 'j': case T_BAIXO:   if (*comp > 0)   (*comp)--; break;
			case 'K': case T_S_CIMA:  *comp = limita(*comp + 16, 0, 255); break;
			case 'J': case T_S_BAIXO: *comp = limita(*comp - 16, 0, 255); break;

			case T_ENTER:   // aplica na nota corrente
				corr = nota_corrente(e);
				if (corr >= 0) e->notas[corr].c = e->edt_cor;
				e->modo = principal;
				break;
			case T_CTRL_T: case T_ALT_ENTER:   // aplica em todas as visiveis
				for (i = 0; i < e->n; i++) {
					if (!filtrada(e, i)) e->notas[i].c = e->edt_cor;
				}
				e->modo = principal;
				break;
			case T_ESC: case T_CTRL_B:
				e->modo = principal;
				break;

			default:
				if (tecla >= '0' && tecla <= '9') {   // digita o valor do componente
					int d = tecla - '0';
					if (e->ultimo_digito && *comp * 10 + d <= 255) {
						*comp = *comp * 10 + d;
					} else {
						*comp = d;
					}
					digitou = true;
				}
		}
		e->ultimo_digito = digitou;
	}
}

// modo de edicao do texto de busca
void executa_busca_texto(estado_t *e)
{
	tecla_t tecla;
	int tam;

	e->busca_cursor = strlen(e->busca_t);

	while (e->modo == busca_texto) {
		desenha_busca_texto(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		tam = strlen(e->busca_t);
		switch ((int) tecla) {
			case T_ESC: case T_CTRL_C:   // esvazia a busca e volta
				e->busca_t[0] = '\0';
				e->busca_cursor = 0;
				e->modo = principal;
				break;
			case T_ENTER:
				e->modo = principal;
				break;
			case T_BS: case T_CTRL_B:
				if (e->busca_cursor > 0) {
					e->busca_cursor--;
					remove_char(e->busca_t, e->busca_cursor);
				}
				break;
			case T_DEL: case T_CTRL_D:
				if (e->busca_cursor < tam) remove_char(e->busca_t, e->busca_cursor);
				break;
			case T_ESQUERDA: case T_CTRL_H:
				if (e->busca_cursor > 0) e->busca_cursor--;
				break;
			case T_DIREITA: case T_CTRL_L:
				if (e->busca_cursor < tam) e->busca_cursor++;
				break;
			case T_HOME: case T_CTRL_K:
				e->busca_cursor = 0;
				break;
			case T_END: case T_CTRL_J:
				e->busca_cursor = tam;
				break;
			default:
				if (valido_texto(tecla)) {
					if (insere_char(e->busca_t, e->busca_cursor, tecla, MAX_TEXTO)) {
						e->busca_cursor++;
					}
				}
		}
	}
}

// modo de edicao da etiqueta de busca
void executa_busca_etiqueta(estado_t *e)
{
	tecla_t tecla;
	int tam;

	while (e->modo == busca_etiqueta) {
		desenha_busca_etiqueta(e);

		do {
			tecla = t_tecla();
		} while (tecla == T_NADA);

		tam = strlen(e->busca_e);
		switch ((int) tecla) {
			case T_ESC: case T_CTRL_C:   // esvazia a etiqueta de busca e volta
				e->busca_e[0] = '\0';
				e->modo = principal;
				break;
			case T_ENTER:
				e->modo = principal;
				break;
			case T_BS: case T_CTRL_B:
				if (tam > 0) e->busca_e[tam - 1] = '\0';
				break;
			default:
				if (valido_etiqueta(tecla) && tam < TAM_ETIQUETA) {
					e->busca_e[tam] = tecla;
					e->busca_e[tam + 1] = '\0';
				}
		}
	}
}


// =============================================================
// inicializacao e laco principal
// =============================================================

// inicializa o estado e le as notas do arquivo
void inicializa(estado_t *e, const char *nome)
{
	e->modo = principal;
	e->notas = NULL;
	e->n = 0;
	e->cap = 0;
	e->tem_removida = false;
	e->busca_t[0] = '\0';
	e->busca_e[0] = '\0';
	e->edt_cursor = 0;
	e->busca_cursor = 0;
	e->comp_sel = 0;
	e->ultimo_digito = false;
	strncpy(e->nome_arquivo, nome, sizeof(e->nome_arquivo) - 1);
	e->nome_arquivo[sizeof(e->nome_arquivo) - 1] = '\0';
	carrega_arquivo(e);
}

int main(int argc, char *argv[])
{
	estado_t e;

	// le as notas do arquivo (o nome pode vir como argumento)
	inicializa(&e, (argc > 1) ? argv[1] : ARQ_PADRAO);

	t_inicia();

	// descobre o tamanho da tela, com um valor de reserva caso nao responda
	t_tamanho(&e.tela_larg, &e.tela_alt);
	if (e.tela_larg <= 0) e.tela_larg = 80;
	if (e.tela_alt  <= 0) e.tela_alt  = 24;
	e.cursor.x = e.tela_larg / 2;
	e.cursor.y = e.tela_alt / 2;

	// laco principal: chama a funcao do modo atual ate pedir pra terminar
	while (e.modo != fim) {
		switch (e.modo) {
			case principal:      executa_principal(&e);      break;
			case edita_texto:    executa_edita_texto(&e);    break;
			case edita_etiqueta: executa_edita_etiqueta(&e); break;
			case edita_cor:      executa_edita_cor(&e);      break;
			case busca_texto:    executa_busca_texto(&e);    break;
			case busca_etiqueta: executa_busca_etiqueta(&e); break;
			default: ;
		}
	}

	grava_arquivo(&e);   // garante que as notas nao se percam ao sair
	t_fim();
	free(e.notas);
	return 0;
}
