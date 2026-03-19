#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 目录中，是存放目录下有什么内容的，也就是它的子项
    // 比如cpp71这个目录下，装了 day1 day2 day3 test1.c test2.c
    // 不断的读，会一个一个拿回来，这个叫做目录项 struct dirent
    // 直到读到末尾，读完时会返回特殊值

    // 文件流中的标准操作：
    // 1.打开文件流
    FILE *fp = fopen("1.txt", "r");
    // 2.判断文件指针是否为NULL
    ERROR_CHECK(fp, NULL, "fopen failed.");
    // 3.使用
    int ch;
    while((ch = fgetc(fp)) != EOF){
        printf("--------%c", ch);
    }
    // 4.关闭文件流
    fclose(fp);
    
    return 0;
}

