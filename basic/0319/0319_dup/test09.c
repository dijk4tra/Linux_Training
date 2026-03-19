#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // printf就是往文件描述符为1的地方输出
    printf("111111\n");
    // 现在1对应的是屏幕
    // close(1); 只是关掉了文件描述符和文件对象之间的映射关系
    // 把1关掉，即切断了内核缓冲区和屏幕的联系
    close(1);
    
    // 系统会从小到大分配文件描述符
    // 如果该文件描述符未被使用，就直接再次利用
    // 现在1重新和1.txt对上了
    int fd = open("1.txt", O_WRONLY);
    // printf还是向文件描述符1输出
    // 但是文件描述符1联系的对象已经变成了1.txt
    printf("fd = %d\n", fd);

    close(fd);
    
    return 0;
}

