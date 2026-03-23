#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    pid_t ret = fork();

    if(ret == 0){
        printf("child process.\n");

        sleep(5);
        printf("child prcess over.\n");
    }else{
        printf("father process.\n");

        while(1){
            printf("father process runnig.\n");
            sleep(1);
        }
    }

    return 0;
}

