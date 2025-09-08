# SAJ-COM07 — Sistema de Gerenciamento de Chamados (C)

Projeto em C com estruturas dinâmicas (fila/pilha), alocação dinâmica, recursividade, tratamento de erros com logs, modularização e depuração com gdb/valgrind.

## Como compilar e executar
make
./helpdesk

Alvos úteis:
- make clean
- make run
- make valgrind

## Estrutura
include/ (headers)
src/     (fontes C)
Makefile
README.md
RELATORIO.md

## Requisitos atendidos
- Fila (lista ligada) e Pilha (lista ligada).
- Remoção por maior prioridade (menor número), estável por ordem de chegada.
- Busca recursiva em fila e pilha.
- Logger com timestamps em log.txt; asserts em pontos críticos.
- malloc/realloc/free com checagem; Makefile com flags de warnings.
