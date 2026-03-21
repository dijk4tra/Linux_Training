#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open read before.\n");
    int fd = open("1.pipe", O_RDONLY);
    printf("open read after.\n");
    ERROR_CHECK(fd, -1, "open read  failed");

    char buf[1024] = {0};
    for(int i=1; i<1000; i++){
        printf("read once before ---- %d ----\n", i);
        ssize_t ret = read(fd, buf, sizeof(buf));
        printf("read once end ---- %d ----\n", i);
    }
    
    close(fd);

    return 0;
}

