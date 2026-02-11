#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "backend_internal.h"

epm_status_t epm_install_atom(epm_ctx_t *ctx, const char *atom_file) {
    if (!util_is_root()) return EPM_ERR_PERMISSION;

    char tmp[] = EPM_TMP_TEMPLATE;
    if (!mkdtemp(tmp)) return EPM_ERR_IO;

    if (util_extract_tar(atom_file, tmp) != EPM_OK)
        return EPM_ERR_EXECUTION;

    char json[512];
    snprintf(json, sizeof(json), "%s/epm.json", tmp);

    epm_atom_t atom;
    if (atom_parse(json, &atom) != EPM_OK)
        return EPM_ERR_INVALID_PACKAGE;

    char payload[512];
    snprintf(payload, sizeof(payload), "%s/payload", tmp);

    char **files = NULL;
    size_t count = 0;

    if (fs_copy_tree(payload, atom.prefix, &files, &count) != EPM_OK)
        return EPM_ERR_IO;

    db_add(ctx, &atom);
    db_write_filelist(ctx, atom.name, files, count);

    return EPM_OK;
}
