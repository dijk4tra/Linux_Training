#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // ls /home/lws/test_my_project
    // ./test12_ls /home/lws

    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir failed");

    struct dirent *dirent_p = NULL;
    while((dirent_p = readdir(dirp)) != NULL){
        printf("%ld %d %s\n", dirent_p->d_ino, dirent_p->d_type, 
               dirent_p->d_name);
    }

    closedir(dirp);

    return 0;
}

