#include <bits/types/stack_t.h>
#include <dirent.h>
#include <my_header.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir failed");

    // argv[1]/test10_opendir.c
    // ../../test10_opendir.c
    // sprintf
    char path[100] = {0};

    struct dirent *dirent_p = NULL;
    struct stat *stat_p = (struct stat *)calloc(1, sizeof(struct stat));
    // 绝对路径和相对路径都要掌握 
    while((dirent_p = readdir(dirp)) != NULL){
        sprintf(path, "%s/%s", argv[1], dirent_p->d_name);

        int ret = stat(path, stat_p);
        ERROR_CHECK(ret, -1, "stat failed");

        printf("%o %ld %d %d %ld %ld %s\n", stat_p->st_mode, stat_p->st_nlink, 
               stat_p->st_uid, stat_p->st_gid,
               stat_p->st_size, stat_p->st_mtime, 
               dirent_p->d_name);
    }

    closedir(dirp);

    return 0;
}
