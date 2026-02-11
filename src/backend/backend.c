#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "backend_internal.h"

epm_status_t epm_init(epm_ctx_t *ctx, const char *db_root, int verbose) {
    if (!ctx) return EPM_ERR_UNKNOWN;

    ctx->distro = epm_get_distro();
    ctx->verbose = verbose;

    if (db_root)
        strncpy(ctx->db_root, db_root, sizeof(ctx->db_root)-1);
    else
        strncpy(ctx->db_root, EPM_DEFAULT_DB_ROOT, sizeof(ctx->db_root)-1);

    return db_init(ctx);
}

epm_distro_t epm_get_distro(void) {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) return EPM_DISTRO_UNKNOWN;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "ID=arch")) return EPM_DISTRO_ARCH;
        if (strstr(line, "ID=debian") || strstr(line, "ID=ubuntu")) return EPM_DISTRO_DEBIAN;
        if (strstr(line, "ID=fedora")) return EPM_DISTRO_FEDORA;
        if (strstr(line, "ID=opensuse")) return EPM_DISTRO_OPENSUSE;
    }
    fclose(f);
    return EPM_DISTRO_UNKNOWN;
}

const char *epm_strerror(epm_status_t s) {
    switch (s) {
        case EPM_OK: return "ok";
        case EPM_ERR_PERMISSION: return "permission denied (need root)";
        case EPM_ERR_INVALID_PACKAGE: return "invalid atom package";
        case EPM_ERR_IO: return "I/O error";
        case EPM_ERR_EXECUTION: return "execution failed";
        default: return "unknown error";
    }
}

const char *epm_distro_name(epm_distro_t d) {
    switch (d) {
        case EPM_DISTRO_ARCH: return "arch";
        case EPM_DISTRO_DEBIAN: return "debian";
        case EPM_DISTRO_FEDORA: return "fedora";
        case EPM_DISTRO_OPENSUSE: return "opensuse";
        default: return "unknown";
    }
}
