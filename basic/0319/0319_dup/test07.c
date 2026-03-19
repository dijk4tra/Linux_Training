#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 系统启动时，会默认开启三个流：stdin stdout stderr
    // 它们分别对应三个文件描述符：0 1 2
    // STDIN_FILENO  STDOUT_FILENO  STDERR_FILENO
    
    STDIN_FILENO;
    printf("%d/n",STDIN_FILENO);
    printf("%d/n",STDOUT_FILENO); 
    printf("%d/n",STDERR_FILENO); 
    // FILE * 但是也会用到 fd 文件描述符。 

    stdin->_fileno; // 可以使用FILE * 直接箭头函数它的属性。 _fileno
    
    /* fileno(stdout); */
    printf("%d\n", fileno(stdout));

    return 0;
}

