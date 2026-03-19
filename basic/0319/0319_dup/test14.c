#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("----------------------\n");
    int stdout_backup_fd = dup(STDOUT_FILENO);
    int file_fd = open("1.txt", O_WRONLY | O_TRUNC);

    dup2(file_fd, STDOUT_FILENO);
    printf("11111.\n");
    dup2(stdout_backup_fd, STDOUT_FILENO);
    printf("22222.\n");
    dup2(file_fd, STDOUT_FILENO);
    printf("33333.\n");
    dup2(stdout_backup_fd, STDOUT_FILENO);
    printf("44444.\n");

    close(stdout_backup_fd);
    close(file_fd);
    close(STDOUT_FILENO);
    
    return 0;
}

