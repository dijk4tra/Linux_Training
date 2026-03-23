#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    for(int i=0; i<4; i++){
        printf("a");

        if(i == 2){
            printf("b");
            continue;
        }
        fork();
    }

    return 0;
}

