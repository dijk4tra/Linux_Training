#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  

    int fd = open("1.pipe", O_WRONLY);
    ERROR_CHECK(fd, -1, "open write failed");

    char *msg = "msgggggg from test3_write";
    /* write(fd, msg, sizeof(msg)); */
    write(fd, msg, strlen(msg));

    close(fd);
    
    return 0;
}

