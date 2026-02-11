#include <stdlib.h>
#include <unistd.h>
#include "backend_internal.h"

epm_status_t epm_remove(epm_ctx_t *ctx, const char *pkg) {
    if (!util_is_root()) return EPM_ERR_PERMISSION;

    char **files;
    size_t count;
    if (db_read_filelist(ctx, pkg, &files, &count) != EPM_OK)
        return EPM_ERR_INVALID_PACKAGE;

    for (size_t i = 0; i < count; ++i)
        unlink(files[i]);

    db_remove(ctx, pkg);
    return EPM_OK;
}
