#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
   
    DIR *dirp = opendir("/home/lws/my_project");
    
    ERROR_CHECK(dirp, NULL, "opendir failed");

    // 使用readdir读目录流，读出来的是这个目录中装了哪些子项、目录项
    struct dirent *dirent_p = readdir(dirp);

    printf("%ld %s %ld\t  %d %d\n", 
           dirent_p->d_ino, dirent_p->d_name, dirent_p->d_off, 
           dirent_p->d_reclen, dirent_p->d_type);    

    // 及时释放资源，关闭目录流
    closedir(dirp);
    return 0;
}

