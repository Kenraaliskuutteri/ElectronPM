#ifndef EPM_BACKEND_INTERNAL_H
#define EPM_BACKEND_INTERNAL_H

#include "epm/backend.h"

#define EPM_DEFAULT_DB_ROOT "/var/lib/electronpm"
#define EPM_TMP_TEMPLATE    "/tmp/epmXXXXXX"

typedef struct {
    char name[128];
    char version[64];
    char prefix[256];
} epm_atom_t;

/* db */
epm_status_t db_init(const epm_ctx_t *ctx);
epm_status_t db_add(const epm_ctx_t *ctx, const epm_atom_t *atom);
epm_status_t db_remove(const epm_ctx_t *ctx, const char *name);
epm_status_t db_list(const epm_ctx_t *ctx, epm_pkg_info_t *out, size_t cap, size_t *out_len);
epm_status_t db_write_filelist(const epm_ctx_t *ctx, const char *pkg, char **files, size_t count);
epm_status_t db_read_filelist(const epm_ctx_t *ctx, const char *pkg, char ***files, size_t *count);

/* atom */
epm_status_t atom_parse(const char *json_path, epm_atom_t *out);

/* fs */
int fs_exists(const char *path);
int fs_mkdir_p(const char *path);
epm_status_t fs_copy_tree(const char *src, const char *dst, char ***files, size_t *count);

/* util */
epm_status_t util_extract_tar(const char *archive, const char *dst);
int util_is_root(void);

#endif
