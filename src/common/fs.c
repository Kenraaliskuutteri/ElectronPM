#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "backend_internal.h"

int fs_exists(const char *p) {
    return access(p, F_OK) == 0;
}

int fs_mkdir_p(const char *path) {
    char tmp[512];
    strncpy(tmp, path, sizeof(tmp));
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    return mkdir(tmp, 0755);
}

static void walk(const char *src, const char *dst, char ***files, size_t *count) {
    DIR *d = opendir(src);
    struct dirent *e;
    fs_mkdir_p(dst);

    while ((e = readdir(d))) {
        if (!strcmp(e->d_name,".") || !strcmp(e->d_name,"..")) continue;
        char s[512], t[512];
        snprintf(s,sizeof(s),"%s/%s",src,e->d_name);
        snprintf(t,sizeof(t),"%s/%s",dst,e->d_name);

        if (e->d_type == DT_DIR)
            walk(s,t,files,count);
        else {
            FILE *in=fopen(s,"rb"),*out=fopen(t,"wb");
            char buf[4096]; size_t n;
            while ((n=fread(buf,1,sizeof(buf),in))) fwrite(buf,1,n,out);
            fclose(in); fclose(out);

            (*files)=realloc(*files,sizeof(char*)*((*count)+1));
            (*files)[*count]=strdup(t);
            (*count)++;
        }
    }
    closedir(d);
}

epm_status_t fs_copy_tree(const char *src, const char *dst, char ***files, size_t *count) {
    *files=NULL; *count=0;
    walk(src,dst,files,count);
    return EPM_OK;
}
