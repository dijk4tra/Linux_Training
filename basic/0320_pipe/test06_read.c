#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open read before.\n");
    int fd = open("1.pipe", O_RDONLY);
    printf("open read after.\n");
    ERROR_CHECK(fd, -1, "open read failed");
    char buf[1024] = {0};
    for(int i=0; i<1000; i++){
        printf("read before ----- %d-----\n", i);
        ssize_t ret = read(fd, buf, sizeof(buf));
        printf("read end-------%d--ret=%ld-----\n", i,ret);
        sleep(1);
        printf("sleep over --------%d\n",i);
    }

    close(fd);
    
    return 0;
}

