#include <stdio.h>
#include <string.h>
#include <epm/backend.h>

#define EPM_VERSION "0.1.0"

static void print_help(void) {
    printf(
        "ElectronPM %s\n"
        "Universal package manager for .atom files\n\n"
        "Usage:\n"
        "  electronpm install <file>.atom\n"
        "  electronpm remove <package>\n"
        "  electronpm list\n"
        "  electronpm --help\n"
        "  electronpm --version\n",
        EPM_VERSION
    );
}

// The support for commands will be done once the backend gets started, hopefully soon - Kenraali :D
