#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int read_fd = open("1.pipe", O_RDONLY);
    int write_fd = open("2.pipe", O_WRONLY);

    while(1){
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(read_fd, &set);

        int ret = select(read_fd + 1, &set, NULL, NULL, NULL);
        // if stdin 可用
        if(FD_ISSET(STDIN_FILENO, &set)){
            // 说明。 
            char buf[100] = {0};
            ssize_t ret = read(STDIN_FILENO, buf, sizeof(buf));

            write(write_fd, buf, ret);
        }

        if(FD_ISSET(read_fd, &set)){
            char buf[100] = {0};
            ssize_t count = read(read_fd, buf, sizeof(buf));
            
            if(count == 0){
                printf("userb close.\n");
                break;
            }
            printf("msg from userb: %s\n", buf);
        }
    }

    close(read_fd);
    close(write_fd);

    return 0;
}

