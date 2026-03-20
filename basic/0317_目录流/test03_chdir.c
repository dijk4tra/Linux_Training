#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // chdir 切换工作路径
    // 每一个程序都有自己的工作路径，工作路径一般是继承的
    // 工作路径，就是相当路径的起点

    FILE *fp = fopen("1.txt","r");
    ERROR_CHECK(fp, NULL, "fopen first failed");
    int ret = chdir("/");
    printf("ret = %d\n", ret);

    FILE *fp2 = fopen("1.txt","r");
    ERROR_CHECK(fp2, NULL, "fopen second failed");
    

    return 0;
}

