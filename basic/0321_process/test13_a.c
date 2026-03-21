#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    /* for(int i=0; i<3; i++){ */
    /*     printf("a\n"); */
    /*     fork(); */    
    /* } */

    for(int i=0; i<3; i++){
        fork();
        printf("a\n");
    }
    printf("b\n");

    for(int i=0; i<3; i++){
        fork();
        printf("a");
    }
    printf("\n");

    for(int i=0; i<3; i++){
        printf("a");
        fork();
    }


    return 0;
}

