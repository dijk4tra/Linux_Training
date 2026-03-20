#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
用户输入文件名，将整个文件的内容读入到字符数组中，并在后面添加空字符'\0'。
    char* read_file(const char* path);
分析思路：
    1.打开文件，以只读模式打开即可。不要忘记进行打开失败，错误处理。（在Windows平台下，应该以rb只读模式打开）
    2.此案例中文件的大小只能在运行时期确定，所以需要动态内存分配一个字符数组用于存储结果字符串。
    3.结果字符串的长度 = 文件字节大小 + 1
    4.文件字节大小怎么算？用ftell函数配合fseek函数即可计算。
    5.利用fread函数一次性将整个文件中的数据，全部读到结果字符数组中，然后自己在末尾加一个空字符。
*/

char *read_file(const char *path) {
    // 1.以二进制只读模式打开文件
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        perror("fopen file failed.");
        return NULL;
    }

    // 2.计算文件字节大小
    fseek(fp, 0, SEEK_END);    // 将文件指针移动到文件末尾
    long filesize = ftell(fp); // 获取当前指针相对于文件头部的偏移量（即文件总字节数）
    rewind(fp);                // 测量完后，必须将指针重置回文件开头，否则无法读取内容

    // 3. 动态分配内存
    // 大小为文件字节数 + 1 个结束符位
    char *buffer = malloc(filesize + 1);
    if (buffer == NULL) {
        printf("malloc failed.\n");
        fclose(fp);
        return NULL;
    }

    // 4.一次性读取整个文件
    // fread 参数：目标地址, 每个元素大小, 元素个数, 文件指针
    size_t read_count = fread(buffer, 1, filesize, fp);

    // 5. 添加字符串结束符
    // buffer 的索引从 0 到 filesize-1 是数据，filesize 位置放 \0
    buffer[read_count] = '\0';

    // 6. 清理并返回
    fclose(fp);
    return buffer;
}


int main(void) {
    char *content = read_file("1.txt");

    if (content != NULL) {
        printf("%s\n", content);
        free(content);   // 释放 read_file 函数中 malloc 分配的堆内存
        content = NULL;  // 别忘记指针置空
    }
    else {
        printf("read_file failed.\n");
    }

    return 0;
}