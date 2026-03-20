#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // stat用于获取一个文件/文件夹的详细信息
    // 包括 ls -al的几乎所有输出（除了名字）
    
    // 错误写法
    //struct stat *stat_p = NULL;
    //stat("1.txt", stat_p);

    struct stat stat_buf;
    stat("1.txt", &stat_buf);

    // stat里面的字段有哪些？哪些与ls -al的输出对应？
    
    return 0;
}

