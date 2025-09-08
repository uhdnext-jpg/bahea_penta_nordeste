#include "pilha.h"
#include "logger.h"
#include <stdlib.h>
#include <assert.h>

Pilha* pilha_criar(void) {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    if (!p) return NULL;
    p->topo = NULL;
    p->tamanho = 0;
    return p;
}

int pilha_empilhar(Pilha *p, Chamado *c) {
    assert(p != NULL);
    assert(c != NULL);
    NoPilha *no = (NoPilha*)malloc(sizeof(NoPilha));
    if (!no) {
        log_erro("Erro de alocacao ao empilhar chamado ID=%d", c->id);
        return 0;
    }
    no->valor = c;
    no->baixo = p->topo;
    p->topo = no;
    p->tamanho++;
    log_info("Empilhado (finalizado) chamado ID=%d", c->id);
    return 1;
}

Chamado* pilha_desempilhar(Pilha *p) {
    assert(p != NULL);
    if (!p->topo) return NULL;
    NoPilha *no = p->topo;
    p->topo = no->baixo;
    p->tamanho--;
    Chamado *c = no->valor;
    free(no);
    return c;
}

Chamado* pilha_buscar_rec(const NoPilha *no, int id) {
    if (!no) return NULL;
    if (no->valor && no->valor->id == id) return no->valor;
    return pilha_buscar_rec(no->baixo, id);
}

void pilha_imprimir(const Pilha *p, FILE *out) {
    if (!out) out = stdout;
    fprintf(out, "=== PILHA FINALIZADOS (%zu itens) ===\n", p ? p->tamanho : 0);
    if (!p || !p->topo) {
        fprintf(out, "(vazia)\n");
        return;
    }
    for (NoPilha *cur = p->topo; cur; cur = cur->baixo) {
        chamado_imprimir(cur->valor, out);
    }
}

void pilha_destruir(Pilha *p) {
    if (!p) return;
    NoPilha *cur = p->topo;
    while (cur) {
        NoPilha *baixo = cur->baixo;
        chamado_destruir(cur->valor);
        free(cur);
        cur = baixo;
    }
    free(p);
}
