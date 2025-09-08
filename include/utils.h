#ifndef UTILS_H
#define UTILS_H

#include "fila.h"
#include "pilha.h"

typedef struct {
    int total_abertos;
    int total_andamento;
    int total_fechados;
    double pct_concluidos; 
} Estatisticas;

void calcular_estatisticas(const Fila *fila, const Pilha *finalizados, Estatisticas *out);
char* ler_linha(const char *prompt);
int ler_int(const char *prompt, int min, int max);
void pause_enter(void);

#endif
