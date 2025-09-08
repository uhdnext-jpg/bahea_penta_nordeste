#include "log.h"
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

static FILE *g_log = NULL;

// Inicializa o log
int log_init(const char *path) {
    if (g_log) return 1; // já inicializado
    g_log = fopen(path, "a");
    if (!g_log) {
        fprintf(stderr, "Erro ao abrir o arquivo de log: %s\n", path);
        return 0;
    }
    return 1;
}

// Fecha o log
void log_close(void) {
    if (g_log) fclose(g_log);
    g_log = NULL;
}

// Função interna que escreve a mensagem
static void vlog_write(const char *tag, const char *fmt, va_list ap) {
    if (!g_log) return;

    time_t t = time(NULL);
    char ts[32];
    struct tm *tm_info = localtime(&t);
    if (tm_info) {
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);
    } else {
        snprintf(ts, sizeof(ts), "n/a");
    }

    fprintf(g_log, "[%s] %s: ", ts, tag);
    vfprintf(g_log, fmt, ap);
    fprintf(g_log, "\n");
    fflush(g_log);
}

// Função pública para log de informações
void log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vlog_write("INFO", fmt, ap);
    va_end(ap);
}

// Função pública para log de erros
void log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vlog_write("ERROR", fmt, ap);
    va_end(ap);
}