#ifndef PILHA_H
#define PILHA_H
#include "chamado.h"
#include <stddef.h>

typedef struct NoPilha {
    Chamado *valor;
    struct NoPilha *baixo;
} NoPilha;

typedef struct {
    NoPilha *topo;
    size_t tamanho;
} Pilha;

Pilha* pilha_criar(void);
int pilha_empilhar(Pilha *p, Chamado *c);
Chamado* pilha_desempilhar(Pilha *p);
Chamado* pilha_buscar_rec(const NoPilha *no, int id);
void pilha_imprimir(const Pilha *p, FILE *out);
void pilha_destruir(Pilha *p);

#endif
