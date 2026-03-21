#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("a\n");

    fork();

    printf("a\n");

    fork();

    printf("a\n");
    
    return 0;
}

