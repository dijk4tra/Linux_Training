#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
   
    DIR *dirp = opendir("/home/lws/my_project");
    ERROR_CHECK(dirp, NULL, "opendir failed");

    struct dirent *dirent_p = NULL;
    while((dirent_p = readdir(dirp)) != NULL){
        printf("%ld %s %ld\t  %d %d\n",
               dirent_p->d_ino, dirent_p->d_name, dirent_p->d_off,
               dirent_p->d_reclen, dirent_p->d_type);
    }
    
    printf("DT_REG = %d\n", DT_REG);
    printf("DT_DIR = %d\n", DT_DIR);

    closedir(dirp);
    
    return 0;
}

