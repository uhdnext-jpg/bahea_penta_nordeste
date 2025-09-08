#ifndef LOG_H
#define LOG_H
#include <stdio.h>

int log_init(const char *path);
void log_close(void);
void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif
