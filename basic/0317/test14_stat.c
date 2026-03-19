#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // stat 是用于获取详细信息的函数
    
    /* struct stat *stat_p; */
    /* struct stat *stat_p = NULL; */
    // 上面的两个写法是错误的(野指针和空指针)，下面的写法才是对的
    // struct stat *stat_p = malloc(sizeof(struct stat));
    
    struct stat stat_buf;
    struct stat *stat_p = &stat_buf;
    
    int ret = stat("1.txt", stat_p);
    
    ERROR_CHECK(ret, -1 , "stat failed");

    stat_p->st_gid;
    // stat中，st_mode: 文件类型+权限
    stat_p->st_mode;

    // 硬链接数
    stat_p->st_nlink;
    
    stat_p->st_uid;
    stat_p->st_gid;
    stat_p->st_size;
    stat_p->st_mtim.tv_sec;
    

    return 0;
}

