#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open read before.\n");
    int fd = open("1.pipe", O_RDONLY);
    printf("open read after.\n");
    ERROR_CHECK(fd, -1, "open read failed");

    char buf[4096] = {0};
    for(int i=0; i<4096; i++){
        /* printf("read once before ----- %d-----\n", i); */
        ssize_t ret = read(fd, buf, sizeof(buf));
        /* printf("read once end-------%d -----\n", i); */
        sleep(5);
    }
    
    close(fd);
    
    return 0;
}

