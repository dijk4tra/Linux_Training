#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd = open("777_copy.txt", O_RDWR);

    ftruncate(fd, 100);
    
    // 在下载的场景中有用。用于占位。 
    // 防止下载过程中，下载速度比较慢。导致，程序刚启动有空间。 
    // 下载过程中，空间被别人占了。 
    return 0;

}

