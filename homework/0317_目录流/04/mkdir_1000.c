#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 定义用来存放完整路径字符串的数组
    char outer_path[1024];  // 外层目录完整路径
    char inner_path[1024];  // 内层目录完整路径
    
    int i,j;

    // 检查命令行参数个数是否正确
    ARGS_CHECK(argc, 2);
    // 用户输入的最外层路径
    // /home/lws/my_project/homework/0317/mkdir_1000
    char *base_path = argv[1];

    // 循环创建目录test1~test1000
    for(i=1;i<=1000;i++){
        // 构造外层目录完整路径
        // /home/lws/my_project/homework/0317/mkdir_1000/test1
        sprintf(outer_path, "%s/test%d", base_path, i);

        // 创建一级文件夹
        if(mkdir(outer_path, 0775) == 0){
            // 内层循环：创建子文件夹
            // 规律：test1 创建 10-20，test2 创建 20-30...
            int start = i * 10;
            int end = start + 10;
            
            for (int j = start; j <= end; j++){
                // 构造二级路径，例如:/home/lws/my_project/homework/0317/mkdir_1000/test1/test10
                sprintf(inner_path, "%s/test%d", outer_path, j);
                mkdir(inner_path, 0775);
            }
        }
    }
    printf("任务完成：已在 %s 下批量创建文件夹。\n", base_path);
    return 0;
}

