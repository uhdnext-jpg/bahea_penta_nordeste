# RELATÓRIO — Depuração e Tratamento de Erros

## Onde foram usados assert, logs e tratamento de erros
- assert em criação/enfileiramento/empilhamento e abertura do logger.
- Logs em log.txt (INFO/ERRO): enfileirar, remover, empilhar, falhas de alocação, não encontrado.
- Tratamento de erros: checagem de malloc/realloc e validações de entrada.

## gdb — evidências sugeridas
gdb ./helpdesk
(gdb) break main
(gdb) run
(gdb) step
(gdb) print fila
(gdb) continue

## Valgrind — evidências sugeridas
make
valgrind --leak-check=full --show-leak-kinds=all ./helpdesk

Cole prints demonstrando ausência de vazamentos.
