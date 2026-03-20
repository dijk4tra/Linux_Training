#include <my_header.h>

int main(int argc, char *argv[]){

    // 程序启动的时候。系统会开三个流。 
    // stdin stdout stderr
    // 它们分别占据了三个文件描述符: 0  1  2

    printf("111\n");
    write(1, "222", 3);

    // fwrite ---> ISO-C---> write--->系统调用 
    // ISO-C POSIX-C 在Linux中完成自己的任务时候，使用的是系统调用 

    return 0;
}


