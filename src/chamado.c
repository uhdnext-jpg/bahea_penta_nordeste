#include "chamado.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <strings.h>

Chamado *chamado_criar(int id, const char *titulo, int prioridade, StatusChamado status) {
    assert(titulo != NULL);
    Chamado *c = (Chamado*)malloc(sizeof(Chamado));
    if (!c) return NULL;
    c->id = id;
    c->prioridade = prioridade;
    c->status = status;
    c->criado_em = time(NULL);

    size_t len = strlen(titulo);
    c->titulo = (char*)malloc(len + 1);
    if (!c->titulo) {
        free(c);
        return NULL;
    }
    memcpy(c->titulo, titulo, len + 1);
    return c;
}

void chamado_destruir(Chamado *c) {
    if (!c) return;
    free(c->titulo);
    free(c);
}

void chamado_imprimir(const Chamado *c, FILE *out) {
    if (!c) return;
    if (!out) out = stdout;
    char buf[32];
    struct tm *tm = localtime(&c->criado_em);
    if (tm) {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    } else {
        snprintf(buf, sizeof(buf), "n/a");
    }
    fprintf(out, "[ID:%d] %s | prio:%d | status:%s | criado:%s\n",
        c->id, c->titulo, c->prioridade, status_to_str(c->status), buf);
}

Chamado *chamado_clonar(const Chamado *orig) {
    if (!orig) return NULL;
    Chamado *c = chamado_criar(orig->id, orig->titulo, orig->prioridade, orig->status);
    if (c) c->criado_em = orig->criado_em;
    return c;
}

int parse_status(const char *s) {
    if (!s) return -1;
#ifdef _WIN32
    #define strcasecmp _stricmp
#endif
    if (strcasecmp(s, "aberto") == 0) return STATUS_ABERTO;
    if (strcasecmp(s, "andamento") == 0) return STATUS_ANDAMENTO;
    if (strcasecmp(s, "fechado") == 0) return STATUS_FECHADO;
    return -1;
}

const char* status_to_str(StatusChamado st) {
    switch (st) {
        case STATUS_ABERTO: return "aberto";
        case STATUS_ANDAMENTO: return "andamento";
        case STATUS_FECHADO: return "fechado";
        default: return "desconhecido";
    }
}
