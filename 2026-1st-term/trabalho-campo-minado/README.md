# Campo Minado (C99 - Modo No-Guess)

Implementacao em C baseada no modelo do professor (`Tabuleiro`, `alocar`, `liberar`, `preencher`, `imprimir`) com resolucao 100% deterministica sem situacoes de puro azar (50/50), conforme o artigo da Wikipedia.

## Estrutura do Projeto

- `tabuleiro.h` e `tabuleiro.c`: Estrutura `Tabuleiro`, alocacao dinamica bidimensional (`int **celulas`, `int **estado`), abertura recursiva (flood fill), contagem de vizinhanca e gerador No-Guess.
- `solver.h` e `solver.c`: Motor de deducao logica (analise de 1 quadrado e reducao de subconjuntos de 2 quadrados) e verificador de solvabilidade estrita.
- `main.c`: Menu principal, leitura das dimensoes e numero de bombas, cronometro em tempo real e loop de execucao.

## Compilacao e Execucao

```bash
make
./campo_minado
```
