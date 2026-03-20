#include <my_header.h>

int main(int argc, char *argv[]){

    // 工作路径，就是相对路径的起点。 
    // 刚开始程序启动的事后，我们的工作路径，就是shell所在位置
    // 使用chdir 会切换工作路径。 切换工作路径，会影响起点。 
    FILE *fp1 = fopen("1.txt", "r");
    chdir("..");
    FILE *fp2 = fopen("1.txt", "r");

    return 0;
}
