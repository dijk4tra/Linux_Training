#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // int fd = open("1.txt", O_WRONLY);
    // int fd = open("1.txt", O_WRONLY, O_CREAT); // 要加权限！
    int fd = open("1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    // 如果加了O_CREAT，一定要给权限，否则就是随机权限！

    // 如果只写一个O_WRONLY
    ERROR_CHECK(fd, -1, "open failed");

    
    return 0;
}

