#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("2.txt", O_WRONLY | O_CREAT | O_EXCL, 0666);
    ERROR_CHECK(fd, -1, "open failed");
    // 独占

    return 0;
}

