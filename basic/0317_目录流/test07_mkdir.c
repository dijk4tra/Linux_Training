#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // ./test7_mkdir test222 777
    // 权限可写可不写
    // 777
    
    // argv[1];
    // argv[2];
    mode_t mode = 777;
    if(argc == 3){
        sscanf(argv[2], "%o", &mode);
    }

    int ret = mkdir(argv[1], mode);
    ERROR_CHECK(ret, -1, "mkdir failed");

    return 0;
}

