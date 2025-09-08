#ifndef FILA_H
#define FILA_H
#include "chamado.h"
#include <stddef.h>

typedef struct NoFila {
    Chamado *valor;
    struct NoFila *prox;
} NoFila;

typedef struct {
    NoFila *ini;
    NoFila *fim;
    size_t tamanho;
} Fila;

Fila* fila_criar(void);
int fila_enfileirar(Fila *f, Chamado *c);
Chamado* fila_desenfileirar(Fila *f);
Chamado* fila_remover_maior_prioridade(Fila *f);
Chamado* fila_buscar_rec(const NoFila *no, int id);
void fila_imprimir(const Fila *f, int filtro_status, FILE *out);
Chamado* fila_mais_antigo_aberto(Fila *f);
void fila_destruir(Fila *f);

#endif
