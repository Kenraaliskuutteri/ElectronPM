#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <epm/backend.h>

#define EPM_VERSION "0.1.0"

static void help(void) {
    printf(
        "ElectronPM %s\n"
        "Universal package manager for .atom files\n\n"
        "Usage:\n"
        "  electronpm install <file>.atom\n"
        "  electronpm remove <package>\n"
        "  electronpm list\n"
        "  electronpm ascii\n"
        "  electronpm --help\n"
        "  electronpm --version\n\n"
        "Env:\n"
        "  EPM_DB_ROOT=/custom/db/root\n"
        "  EPM_TEST_MODE=1   (bypass root check for sandbox testing)\n",
           EPM_VERSION
    );
}

int main(int argc, char **argv) {
    if (argc < 2) { help(); return 0; }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) { help(); return 0; }
    if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-V")) {
        printf("%s\n", EPM_VERSION);
        return 0;
    }

    const char *db_root = getenv("EPM_DB_ROOT");
    int verbose = getenv("EPM_VERBOSE") ? 1 : 0;

    epm_ctx_t ctx;
    epm_status_t st = epm_init(&ctx, db_root, verbose);
    if (st != EPM_OK) {
        fprintf(stderr, "init: %s\n", epm_strerror(st));
        return 2;
    }

    if (!strcmp(argv[1], "install")) {
        if (argc < 3) { fprintf(stderr, "missing atom file\n"); return 2; }
        st = epm_install_atom(&ctx, argv[2]);
        if (st != EPM_OK) { fprintf(stderr, "install: %s\n", epm_strerror(st)); return 2; }
        printf("ok\n");
        return 0;
    }

    if (!strcmp(argv[1], "remove")) {
        if (argc < 3) { fprintf(stderr, "missing package name\n"); return 2; }
        st = epm_remove(&ctx, argv[2]);
        if (st != EPM_OK) { fprintf(stderr, "remove: %s\n", epm_strerror(st)); return 2; }
        printf("ok\n");
        return 0;
    }

    if (!strcmp(argv[1], "list")) {
        epm_pkg_info_t pkgs[512];
        size_t n = 0;
        st = epm_list(&ctx, pkgs, 512, &n);
        if (st != EPM_OK) { fprintf(stderr, "list: %s\n", epm_strerror(st)); return 2; }

        for (size_t i = 0; i < n; i++) {
            printf("%s %s\n", pkgs[i].name, pkgs[i].version);
        }
        return 0;
    }

    if (!strcmp(argv[1], "electronpm ascii")) {
printf("███████╗██╗     ███████╗ ██████╗████████╗██████╗  ██████╗ ███╗   ██╗██████╗ ███╗   ███╗\n");
printf("██╔════╝██║     ██╔════╝██╔════╝╚══██╔══╝██╔══██╗██╔═══██╗████╗  ██║██╔══██╗████╗ ████║\n");
printf("█████╗  ██║     █████╗  ██║        ██║   ██████╔╝██║   ██║██╔██╗ ██║██████╔╝██╔████╔██║\n");
printf("██╔══╝  ██║     ██╔══╝  ██║        ██║   ██╔══██╗██║   ██║██║╚██╗██║██╔═══╝ ██║╚██╔╝██║\n");
printf("███████╗███████╗███████╗╚██████╗   ██║   ██║  ██║╚██████╔╝██║ ╚████║██║     ██║ ╚═╝ ██║\n");
printf("╚══════╝╚══════╝╚══════╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝     ╚═╝\n");
        printf("Welcome to ElectronPM!\n");
        return 0;
    }

    fprintf(stderr, "unknown command: %s\n", argv[1]);
    help();
    return 2;
}
