#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open write before.\n");
    int fd = open("1.pipe", O_WRONLY);
    printf("open write after.\n");
    ERROR_CHECK(fd, -1, "open write failed");
    
    char buf[4096] = {0};
    for(int i=0; i<1000; i++){
        printf("write before ----- %d-----\n", i);
        write(fd, buf, sizeof(buf));
        printf("write end-------%d -----\n", i);
    }

    close(fd);

    return 0;
}

