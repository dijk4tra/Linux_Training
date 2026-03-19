#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // mkdir 就是创建目录
    // 也会受到 umask 的控制，也就是将 umask 里面的内容掩去
    // 这里创建的目录，因为收到 umask 影响，其权限实际上是 0765
    mode_t mode = 0767;
    int ret = mkdir("test111", mode);

    printf("ret = %d\n", ret);
    ERROR_CHECK(ret, -1, "mkdir failed");

    return 0;
}

