#include <my_header.h>

int main(int argc, char *argv[]){

    // printf : 行缓冲区的输出。 

    printf("hello world.\n");

    fwrite("111\n", 1, 4, stdout);

    // 程序在启动的时候。系统会自动开三个流。 stdin stdout stderr

    return 0;
}
