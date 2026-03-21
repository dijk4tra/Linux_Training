#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open write before.\n");
    int fd = open("1.pipe", O_WRONLY);
    printf("open write after.\n");
    ERROR_CHECK(fd, -1, "open write failed");

    char buf[1024] = {0};
    for(int i=0; i<1000; i++){
        write(fd, buf, sizeof(buf));
        sleep(4);
    }

    close(fd);
    
    return 0;
}

