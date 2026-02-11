#include <stdio.h>
#include <string.h>
#include "backend_internal.h"

/* very simple json parser fr fr */
static int extract(const char *buf, const char *key, char *out, size_t max) {
    char pat[64];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    char *p = strstr(buf, pat);
    if (!p) return 0;
    p = strchr(p + strlen(pat), ':');
    if (!p) return 0;
    p = strchr(p, '"');
    if (!p) return 0;
    p++;
    char *e = strchr(p, '"');
    if (!e) return 0;
    size_t len = e - p;
    if (len >= max) len = max - 1;
    strncpy(out, p, len);
    out[len] = 0;
    return 1;
}

epm_status_t atom_parse(const char *json_path, epm_atom_t *out) {
    FILE *f = fopen(json_path, "r");
    if (!f) return EPM_ERR_INVALID_PACKAGE;

    char buf[4096];
    size_t n = fread(buf, 1, sizeof(buf)-1, f);
    fclose(f);
    buf[n] = 0;

    if (!extract(buf, "name", out->name, sizeof(out->name))) return EPM_ERR_INVALID_PACKAGE;
    if (!extract(buf, "version", out->version, sizeof(out->version))) return EPM_ERR_INVALID_PACKAGE;

    if (!extract(buf, "prefix", out->prefix, sizeof(out->prefix)))
        strcpy(out->prefix, "/");

    return EPM_OK;
}
