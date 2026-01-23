#ifndef EPM_BACKEND_H
#define EPM_BACKEND_H

typedef enum {
    EPM_OK = 0,
    EPM_ERR_NOT_FOUND,
    EPM_ERR_DOWNLOAD,
    EPM_ERR_CHECKSUM,
    EPM_ERR_EXTRACT,
    EPM_ERR_IO,
    EPM_ERR_INTERNAL
} epm_status_t;

epm_status_t epm_install(const char *package);
epm_status_t epm_remove(const char *package);
epm_status_t epm_list(void);

const char *epm_strerror(epm_status_t status);

#endif 

// There isn't much here yet. This is mainly vibe coded to have a semi functioning Backend & frontend connection. This may be the most vibe coded thing ever tho. - Kenraali. 
