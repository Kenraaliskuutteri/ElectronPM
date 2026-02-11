#include <stdio.h>
#include <string.h>
#include "backend_internal.h"

static void path(char *out, const epm_ctx_t *ctx, const char *sub) {
    snprintf(out, 512, "%s/%s", ctx->db_root, sub);
}

epm_status_t db_init(const epm_ctx_t *ctx) {
    fs_mkdir_p(ctx->db_root);
    char f[512];
    path(f, ctx, "files");
    fs_mkdir_p(f);
    return EPM_OK;
}

epm_status_t db_add(const epm_ctx_t *ctx, const epm_atom_t *a) {
    char p[512];
    path(p, ctx, "installed.db");
    FILE *f = fopen(p, "a");
    if (!f) return EPM_ERR_IO;
    fprintf(f, "%s|%s\n", a->name, a->version);
    fclose(f);
    return EPM_OK;
}

epm_status_t db_remove(const epm_ctx_t *ctx, const char *name) {
    char p[512], t[512];
    path(p, ctx, "installed.db");
    path(t, ctx, "installed.tmp");

    FILE *in = fopen(p, "r");
    FILE *out = fopen(t, "w");
    if (!in || !out) return EPM_ERR_IO;

    char line[256];
    while (fgets(line, sizeof(line), in))
        if (strncmp(line, name, strlen(name)) != 0)
            fputs(line, out);

    fclose(in); fclose(out);
    rename(t, p);
    return EPM_OK;
}

epm_status_t db_list(const epm_ctx_t *ctx, epm_pkg_info_t *out, size_t cap, size_t *out_len) {
    char p[512];
    path(p, ctx, "installed.db");
    FILE *f = fopen(p, "r");
    if (!f) return EPM_OK;

    size_t i = 0;
    while (i < cap && fscanf(f, "%127[^|]|%63s\n", out[i].name, out[i].version) == 2)
        i++;

    fclose(f);
    *out_len = i;
    return EPM_OK;
}
