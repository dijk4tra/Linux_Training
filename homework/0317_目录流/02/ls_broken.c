#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    DIR * dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir failed");

    struct dirent *dirent_p;
    // 循环读目录项并打印目录信息，循环结束的条件是返回值为NULL
    while((dirent_p = readdir(dirp)) != NULL){
        printf("inode num = %lu, reclen = %hu, type = %u, name = %s\n",
               dirent_p->d_ino,       // inode编号一般是一个64位无符号整数
               dirent_p->d_reclen,    // 无符号短整型  
               dirent_p->d_type,      // 无符号整型
               dirent_p->d_name);
    }
    // 及时释放资源，关闭目录流
    closedir(dirp);
    return 0;
}

