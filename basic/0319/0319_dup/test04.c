#include <my_header.h>

int main(int argc, char *argv[]){

    // 直接open文件描述符上来就是3. 因为 0 1 2 被已经被标准流用了
    int fd = open("1.txt", O_WRONLY | O_CREAT |O_TRUNC, 0666);
    printf("fd = %d\n", fd);


    return 0;
}


