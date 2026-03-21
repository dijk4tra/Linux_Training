#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // pid是进程ID, ppid是进程的父进程的ID
    // ./test1
    // getpid()  getppid()

    printf("pid = %d\n", getpid());
    printf("ppid = %d\n", getppid());

    sleep(60);

    return 0;
}

