#ifndef CHAMADO_H
#define CHAMADO_H

#include <stdio.h>
#include <time.h>

typedef enum {
    STATUS_ABERTO = 0,
    STATUS_ANDAMENTO = 1,
    STATUS_FECHADO = 2
} StatusChamado;

typedef struct Chamado {
    int id;
    char *titulo;
    int prioridade;          
    StatusChamado status;
    time_t criado_em;        
} Chamado;

Chamado *chamado_criar(int id, const char *titulo, int prioridade, StatusChamado status);
void chamado_destruir(Chamado *c);
void chamado_imprimir(const Chamado *c, FILE *out);
Chamado *chamado_clonar(const Chamado *orig);
int parse_status(const char *s);
const char* status_to_str(StatusChamado st);

#endif
