#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("test5_execl \n");
    // 参数结束有一个标志:  (void *)0;  NULL
    execl("./test5_while", "111", "222", (void *)0);

    return 0;
}

