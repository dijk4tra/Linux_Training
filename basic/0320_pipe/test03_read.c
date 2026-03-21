#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("1.pipe", O_RDONLY);
    ERROR_CHECK(fd, -1, "open read failed");

    char buf[1024] = {0};
    ssize_t ret = read(fd, buf, sizeof(buf));
    printf("read_count = %ld\n", ret);
    printf("msg = %s\n", buf);

    close(fd);   
    
    return 0;
}

