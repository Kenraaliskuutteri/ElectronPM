#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "backend_internal.h"

static int g_verbose = 0;

void epm_log_set_verbose(int v) { g_verbose = v; }

static void vlog(const char *lvl, const char *fmt, va_list ap) {
    time_t t = time(NULL);
    struct tm tmv;
    localtime_r(&t, &tmv);

    char ts[32];
    strftime(ts, sizeof(ts), "%H:%M:%S", &tmv);

    fprintf(stderr, "[%s] %s: ", ts, lvl);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

void epm_log_debug(const char *fmt, ...) {
    if (!g_verbose) return;
    va_list ap; va_start(ap, fmt);
    vlog("DEBUG", fmt, ap);
    va_end(ap);
}
void epm_log_info(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vlog("INFO", fmt, ap);
    va_end(ap);
}
void epm_log_warn(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vlog("WARN", fmt, ap);
    va_end(ap);
}
void epm_log_error(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vlog("ERROR", fmt, ap);
    va_end(ap);
}
