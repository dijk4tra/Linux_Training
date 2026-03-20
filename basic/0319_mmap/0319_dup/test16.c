#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 现在都是在重定向输出，能不能重定向输入？
    // scanf：本来都是从键盘输入，从文件描述符0读数据
    // 如果这时候将0关掉，重新打开一个文件，占用文件描述符0
    // 这时候如果使用scanf直接从文件读数据

    close(STDIN_FILENO);

    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    
    printf("fd = %d\n", fd);
    
    int num1 = 0;
    double d1 = 0.0;

    scanf("%d %lf", &num1, &d1);
    printf("num1 = %d\nd1 = %lf", num1, d1);

    // 输入重定向

    return 0;
}

