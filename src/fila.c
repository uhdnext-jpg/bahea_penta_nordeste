#include "fila.h"
#include "logger.h"
#include <stdlib.h>
#include <assert.h>

Fila* fila_criar(void) {
    Fila *f = (Fila*)malloc(sizeof(Fila));
    if (!f) return NULL;
    f->ini = f->fim = NULL;
    f->tamanho = 0;
    return f;
}

int fila_enfileirar(Fila *f, Chamado *c) {
    assert(f != NULL);
    assert(c != NULL);
    NoFila *no = (NoFila*)malloc(sizeof(NoFila));
    if (!no) {
        log_erro("Erro de alocacao ao enfileirar chamado ID=%d", c->id);
        return 0;
    }
    no->valor = c;
    no->prox = NULL;
    if (!f->fim) {
        f->ini = f->fim = no;
    } else {
        f->fim->prox = no;
        f->fim = no;
    }
    f->tamanho++;
    log_info("Enfileirado chamado ID=%d (prio=%d, status=%s)", c->id, c->prioridade, status_to_str(c->status));
    return 1;
}

Chamado* fila_desenfileirar(Fila *f) {
    assert(f != NULL);
    if (!f->ini) return NULL;
    NoFila *no = f->ini;
    f->ini = no->prox;
    if (!f->ini) f->fim = NULL;
    f->tamanho--;
    Chamado *c = no->valor;
    free(no);
    return c;
}

Chamado* fila_remover_maior_prioridade(Fila *f) {
    assert(f != NULL);
    if (!f->ini) return NULL;

    NoFila *ant_min = NULL;
    NoFila *min = f->ini;

    NoFila *ant = NULL;
    for (NoFila *cur = f->ini; cur; ant = cur, cur = cur->prox) {
        if (cur->valor->prioridade < min->valor->prioridade) {
            min = cur;
            ant_min = ant;
        }
    }

    if (ant_min) ant_min->prox = min->prox;
    else f->ini = min->prox;

    if (min == f->fim) f->fim = ant_min;

    f->tamanho--;
    Chamado *ret = min->valor;
    free(min);
    log_info("Removido para atendimento ID=%d (prio=%d)", ret->id, ret->prioridade);
    return ret;
}

Chamado* fila_buscar_rec(const NoFila *no, int id) {
    if (!no) return NULL;
    if (no->valor && no->valor->id == id) return no->valor;
    return fila_buscar_rec(no->prox, id);
}

void fila_imprimir(const Fila *f, int filtro_status, FILE *out) {
    if (!out) out = stdout;
    fprintf(out, "=== FILA (%zu itens) ===\n", f ? f->tamanho : 0);
    if (!f || !f->ini) {
        fprintf(out, "(vazia)\n");
        return;
    }
    for (NoFila *cur = f->ini; cur; cur = cur->prox) {
        if (filtro_status < 0 || (int)cur->valor->status == filtro_status) {
            chamado_imprimir(cur->valor, out);
        }
    }
}

Chamado* fila_mais_antigo_aberto(Fila *f) {
    if (!f) return NULL;
    Chamado *mais_antigo = NULL;
    for (NoFila *cur = f->ini; cur; cur = cur->prox) {
        if (cur->valor->status == STATUS_ABERTO) {
            if (!mais_antigo || cur->valor->criado_em < mais_antigo->criado_em) {
                mais_antigo = cur->valor;
            }
        }
    }
    return mais_antigo;
}

void fila_destruir(Fila *f) {
    if (!f) return;
    NoFila *cur = f->ini;
    while (cur) {
        NoFila *prox = cur->prox;
        chamado_destruir(cur->valor);
        free(cur);
        cur = prox;
    }
    free(f);
}
