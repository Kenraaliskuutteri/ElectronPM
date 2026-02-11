#include "backend_internal.h"

epm_status_t epm_list(epm_ctx_t *ctx, epm_pkg_info_t *out, size_t cap, size_t *out_len) {
    return db_list(ctx, out, cap, out_len);
}
