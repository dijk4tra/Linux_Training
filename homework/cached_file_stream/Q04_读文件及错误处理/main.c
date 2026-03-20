#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h> // 必须包含此头文件才能使用 errno

/*
编写一个C程序，实现以下功能：
    1.程序尝试以"r"模式，打开一个名为 "1.txt" 的文件进行读操作。
    2.如果文件成功打开，程序则读取文件的第一行内容，并将其输出到终端上。
    3.如果文件打开失败，程序应使用 perror 函数打印此文件名以及相关错误信息，并输出 errno 当前的值。
把代码编写完成后，执行以下操作从而测试代码：
    1.一开始，1.txt文件不存在，观察程序的输出。
    2.在工作目录下创建1.txt文件，并在文件开头手动写入一行字符串，再观察程序的输出。
    3.将1.txt文件的权限改成只读模式，然后将打开模式改成"a"模式，尝试写入一行字符串，观察程序的输出。
    4.将1.txt文件的权限重新改为正常模式，再尝试写入一行字符串，观察程序的输出。
*/

void test4() {
    // 模式"a" (append)，如果文件不存在会创建，存在则在末尾追加
    FILE *fp = fopen("1.txt", "a");

    if (fp == NULL) {
        perror("fopen file failed");
        printf("Current errno value: %d\n", errno);
        return -1;
    }

    // 准备要写入的字符串
    const char *text = "This is a new line added by C program.\n";

    // 执行写入操作
    // fputs 返回非负值表示成功，EOF 表示失败
    if (fputs(text, fp) != EOF) {
        printf("fputs successfully.\n");
    }
    else {
        printf("fputs failed.\n");
    }

    // 关闭文件，确保缓冲区内容真正写入磁盘
    fclose(fp);
}


int main(void) {
    
    FILE *fp = fopen("1.txt", "r");
    if (fp == NULL) {
        // 如果打开失败，使用 perror 打印错误信息
        // 它会自动在字符串后加上冒号和具体的错误描述
        perror("fopen file failed");
        // 打印 errno 的当前值
        printf("Current errno value: %d\n", errno);
        return -1;
    }


    // 定义一个字符数组（缓冲区）来存储读取的内容
    char buffer[1024];

    // 使用 fgets 读取第一行
    // 参数：(存储位置, 最大长度, 文件指针)
    if (fgets(buffer, 1024, fp) != NULL) {
        printf("File content: %s", buffer);
    }
    else {
        // 如果文件是空的，fgets 会返回 NULL
        printf("File is empty or fgets failed.\n");
    }
    fclose(fp);

    test4();

    return 0;
}