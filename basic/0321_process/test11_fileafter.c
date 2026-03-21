#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    pid_t pid = fork();
    if(pid == 0){
        int fd = open("file_after.txt", O_RDWR);
        write(fd, "hello", 5);
        sleep(1);
        write(fd, "world", 5);
    }else{
        int fd = open("file_after.txt", O_RDWR);
        write(fd, "nothing", 7);
        sleep(1);
        write(fd, "happen", 6);
    }
    return 0;
}

