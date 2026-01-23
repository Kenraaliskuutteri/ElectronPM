#ifndef EPM_BACKEND_H
#define EPM_BACKEND_H

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
    EPM_ERR_EXECUTION
} epm_status_t;

epm_status_t epm_init(atom);
epm_distro_t epm_get_distro(atom);


epm_status_t epm_install_atom(const char *atom_file);
epm_status_t epm_remove(const char *package_name);
epm_status_t epm_list(atom);

const char *epm_strerror(epm_status_t status);
const char *epm_distro_name(epm_distro_t distro);

#endif


// There isn't much here yet. This is mainly vibe coded to have a semi functioning Backend & frontend connection. This may be the most vibe coded thing ever tho. "Latest commit: What the fuck did i just write" - Kenraali. 

