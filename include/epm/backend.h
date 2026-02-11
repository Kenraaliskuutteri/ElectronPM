#ifndef EPM_BACKEND_H
#define EPM_BACKEND_H

#include <stddef.h>

typedef enum {
    EPM_DISTRO_UNKNOWN = 0,
    EPM_DISTRO_ARCH,
    EPM_DISTRO_DEBIAN,
    EPM_DISTRO_FEDORA,
    EPM_DISTRO_OPENSUSE
} epm_distro_t;

typedef enum {
    EPM_OK = 0,
    EPM_ERR_UNKNOWN,
    EPM_ERR_UNSUPPORTED_DISTRO,
    EPM_ERR_INVALID_PACKAGE,
    EPM_ERR_DEPENDENCY,
    EPM_ERR_PERMISSION,
    EPM_ERR_EXECUTION,
    EPM_ERR_IO
} epm_status_t;

typedef struct {
    epm_distro_t distro;
    char db_root[512];
    int verbose;
} epm_ctx_t;

/* lifecycle */
epm_status_t epm_init(epm_ctx_t *ctx, const char *db_root, int verbose);
epm_distro_t epm_get_distro(void);

/* operations */
epm_status_t epm_install_atom(epm_ctx_t *ctx, const char *atom_file);
epm_status_t epm_remove(epm_ctx_t *ctx, const char *package_name);

/* list */
typedef struct {
    char name[128];
    char version[64];
} epm_pkg_info_t;

epm_status_t epm_list(epm_ctx_t *ctx, epm_pkg_info_t *out, size_t cap, size_t *out_len);

/* utils */
const char *epm_strerror(epm_status_t status);
const char *epm_distro_name(epm_distro_t distro);

#endif

