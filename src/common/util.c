#include <stdlib.h>
#include <unistd.h>
#include "backend_internal.h"

int util_is_root(void) {
    return geteuid() == 0;
}

epm_status_t util_extract_tar(const char *archive, const char *dst) {
    char cmd[1024];
    snprintf(cmd,sizeof(cmd),"tar -xf \"%s\" -C \"%s\"",archive,dst);
    return system(cmd)==0 ? EPM_OK : EPM_ERR_EXECUTION;
}
