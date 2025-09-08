#include "utils.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void calcular_estatisticas(const Fila *fila, const Pilha *finalizados, Estatisticas *out) {
    assert(out != NULL);
    out->total_abertos = 0;
    out->total_andamento = 0;
    out->total_fechados = 0;

    if (fila) {
        for (NoFila *cur = fila->ini; cur; cur = cur->prox) {
            switch (cur->valor->status) {
                case STATUS_ABERTO: out->total_abertos++; break;
                case STATUS_ANDAMENTO: out->total_andamento++; break;
                case STATUS_FECHADO: out->total_fechados++; break;
            }
        }
    }
    if (finalizados) {
        for (NoPilha *cur = finalizados->topo; cur; cur = cur->baixo) {
            out->total_fechados++;
        }
    }

    int total = out->total_abertos + out->total_andamento + out->total_fechados;
    out->pct_concluidos = (total > 0) ? (100.0 * out->total_fechados / total) : 0.0;
}

char* ler_linha(const char *prompt) {
    if (prompt) printf("%s", prompt);
    size_t cap = 64;
    size_t len = 0;
    char *buf = (char*)malloc(cap);
    if (!buf) return NULL;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = (char*)realloc(buf, cap);
            if (!tmp) {
                free(buf);
                log_erro("Erro de alocacao ao ler_linha");
                return NULL;
            }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }
    if (c == EOF && len == 0) {
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
}

int ler_int(const char *prompt, int min, int max) {
    char *linha = NULL;
    int val;
    while (1) {
        if (prompt) printf("%s", prompt);
        linha = ler_linha(NULL);
        if (!linha) {
            printf("Entrada invalida.\n");
            continue;
        }
        char *end;
        val = (int)strtol(linha, &end, 10);
        if (end == linha || *end != '\0') {
            printf("Digite um numero inteiro.\n");
            free(linha);
            continue;
        }
        if (val < min || val > max) {
            printf("Valor deve estar entre %d e %d.\n", min, max);
            free(linha);
            continue;
        }
        free(linha);
        return val;
    }
}

void pause_enter(void) {
    printf("Pressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
