#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    for(int i=0; i<3 ; i++){
        fork();
        printf("a");
    }

    printf("\n");

    return 0;
}

