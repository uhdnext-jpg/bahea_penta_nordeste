#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "chamado.h"
#include "fila.h"
#include "pilha.h"
#include "utils.h"
#include "logger.h"

static void menu_listar(const Fila *fila) {
    printf("1) Listar todos\n2) Filtrar por status (0=aberto,1=andamento,2=fechado)\nOpcao: ");
    int op;
    scanf("%d", &op);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    if (op == 1) {
        fila_imprimir(fila, -1, stdout);
    } else if (op == 2) {
        int st = ler_int("Status: ", 0, 2);
        fila_imprimir(fila, st, stdout);
    } else {
        printf("Opcao invalida.\n");
    }
}

static void menu_cadastrar(Fila *fila) {
    int id = ler_int("ID: ", 1, 1000000000);
    char *titulo = ler_linha("Titulo: ");
    if (!titulo) { log_erro("Falha ao ler titulo"); return; }
    int prio = ler_int("Prioridade (1-5, 1=maior): ", 1, 5);
    printf("Status (aberto/andamento/fechado): ");
    char *s = ler_linha(NULL);
    if (!s) { free(titulo); log_erro("Falha ao ler status"); return; }
    int ps = parse_status(s);
    if (ps < 0) {
        printf("Status invalido. Usando 'aberto'.\n");
        ps = STATUS_ABERTO;
    }
    Chamado *c = chamado_criar(id, titulo, prio, (StatusChamado)ps);
    free(titulo);
    free(s);
    if (!c) {
        log_erro("Erro de alocacao ao criar chamado");
        printf("Falha ao criar chamado.\n");
        return;
    }
    if (!fila_enfileirar(fila, c)) {
        chamado_destruir(c);
        printf("Falha ao enfileirar chamado.\n");
    }
}

static void menu_buscar(const Fila *fila, const Pilha *pilha) {
    int id = ler_int("ID para buscar: ", 1, 1000000000);
    Chamado *c = fila_buscar_rec(fila ? fila->ini : NULL, id);
    if (!c) c = pilha_buscar_rec(pilha ? pilha->topo : NULL, id);
    if (c) {
        printf("Encontrado:\n");
        chamado_imprimir(c, stdout);
    } else {
        printf("Chamado nao encontrado.\n");
        log_info("Chamado nao encontrado ID=%d", id);
    }
}

static void menu_atender(Fila *fila, Pilha *finalizados) {
    Chamado *c = fila_remover_maior_prioridade(fila);
    if (!c) {
        printf("Fila vazia.\n");
        return;
    }
    printf("Atendendo chamado:\n");
    chamado_imprimir(c, stdout);
    if (c->status == STATUS_FECHADO) {
        if (!pilha_empilhar(finalizados, c)) {
            printf("Falha ao mover para pilha de finalizados. Descartando chamado.\n");
            chamado_destruir(c);
        }
    } else {
        printf("Chamado nao fechado. Retornando para fila ao final.\n");
        if (!fila_enfileirar(fila, c)) {
            printf("Falha ao re-enfileirar. Descartando chamado.\n");
            chamado_destruir(c);
        }
    }
}

static void menu_estatisticas(Fila *fila, Pilha *pilha) {
    Estatisticas e;
    calcular_estatisticas(fila, pilha, &e);
    printf("Abertos: %d | Andamento: %d | Fechados: %d | %% Concluidos: %.2f%%\n",
           e.total_abertos, e.total_andamento, e.total_fechados, e.pct_concluidos);
    Chamado *antigo = fila_mais_antigo_aberto(fila);
    if (antigo) {
        printf("Mais antigo aberto: ");
        chamado_imprimir(antigo, stdout);
    } else {
        printf("Nao ha chamados abertos.\n");
    }
}

static void relatorio_final(Fila *fila, Pilha *pilha) {
    printf("\n===== RELATORIO FINAL =====\n");
    menu_estatisticas(fila, pilha);
    printf("Chamados restantes na fila:\n");
    fila_imprimir(fila, -1, stdout);
    printf("Chamados finalizados na pilha:\n");
    pilha_imprimir(pilha, stdout);
}

int main(void) {
    assert(logger_init("log.txt") && "Falha ao abrir log.txt");
    log_info("=== Inicio do programa ===");

    Fila *fila = fila_criar();
    Pilha *finalizados = pilha_criar();
    if (!fila || !finalizados) {
        log_erro("Falha de alocacao ao criar estruturas principais");
        fprintf(stderr, "Erro grave de alocacao.\n");
        return EXIT_FAILURE;
    }

    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1) Cadastrar chamado\n");
        printf("2) Listar chamados\n");
        printf("3) Executar atendimento (remove maior prioridade)\n");
        printf("4) Buscar chamado por ID (recursivo)\n");
        printf("5) Estatisticas\n");
        printf("0) Sair\n");
        opcao = ler_int("Escolha: ", 0, 5);

        switch (opcao) {
            case 1: menu_cadastrar(fila); break;
            case 2: menu_listar(fila); break;
            case 3: menu_atender(fila, finalizados); break;
            case 4: menu_buscar(fila, finalizados); break;
            case 5: menu_estatisticas(fila, finalizados); break;
            case 0: break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    relatorio_final(fila, finalizados);

    fila_destruir(fila);
    pilha_destruir(finalizados);
    logger_close();
    return EXIT_SUCCESS;
}
