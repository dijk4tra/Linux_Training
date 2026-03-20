#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    ARGS_CHECK(argc, 2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir failed");

    // 切换工作目录为参数传入的目录
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret, -1, "chdir failed");

    struct dirent *dirent_p;
    // 读取目录中的每个目录项
    while((dirent_p = readdir(dirp)) != NULL){
        // 获取文件相关的信息
        struct stat stat_buf;
        int ret = stat(dirent_p, &stat_buf);
        ERROR_CHECK(ret, -1, "stat failed");
        
        // 输出文件相关的信息
        printf("%o %lu %u %u %lu %lu %s\n", 
               stat_buf.st_mode,        // 八进制无符号数
               stat_buf.st_nlink,       // 无符号长整型          
               stat_buf.st_uid,         // 无符号整型
               stat_buf.st_gid,         // 无符号整型
               stat_buf.st_size,        // 无符号长整型   
               stat_buf.st_mtim.tv_sec,  // 时间戳，以无符号长整型打印
               dirent_p->d_name);

    }
    
    // 关闭目录流
    closedir(dirp);
    
    return 0;
}

