#include <my_header.h>

int main(int argc, char *argv[]){

    ARGS_CHECK(argc, 2);
    mkdir(argv[1], 0777);
    chdir(argv[1]);

    char father_path[100] = {0};
    // 循环 1000次。 创建  test1 test2 
    for(int i=1; i<= 1000; i++){
        // i-> 1----1000
        // test1 : sprintf 
        sprintf(father_path, "%s%d", "test", i);
        mkdir(father_path, 0777);

        chdir(father_path);
        char child_path[50] = {0};
        for(int j=1; j<=10; j++){
            // 1--10
            /* i * 10 + j; */
            sprintf(child_path, "%s%d", "test", i * 10 + j);
            mkdir(child_path, 0777);
        }
        chdir("..");

    }

    return 0;
}
