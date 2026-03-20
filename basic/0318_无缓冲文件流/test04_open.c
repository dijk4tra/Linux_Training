#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("1.txt", O_RDONLY);
    printf("fd = %d\n", fd);

    ERROR_CHECK(fd, -1, "open failed");

    
    return 0;
}

