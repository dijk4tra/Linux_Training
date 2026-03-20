#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 1024
#define BUFFER_SIZE 4096

/*
请自行熟悉文件流的操作函数，实现以下文件流基础操作：

1.逐字符文本文件复制，函数声明是：
void copy_file_char(const char* src_file, const char* dest_file);

2.逐行文本文件复制，函数声明式：
void copy_file_line(const char* src_file,const char* dest_file);

3.用fread和fwrite函数实现二进制文件的复制, 函数声明如下：
void binary_file_cpy(const char* src_file, const char* dest_file);

4.用fread和fwrite函数实现二进制的序列化和反序列化操作。
此操作可以不提取函数，直接在main函数中实现即可。但只允许使用一个FILE流。
*/

// ====================================================================
// 1. 逐字符文本文件复制
// 原理：通过 fgetc 每次从源文件读取一个字节，直到遇到 EOF（文件结束符）
// ====================================================================
void copy_file_char(const char *src_file, const char *dest_file) {
    // 以“只读”模式打开源文件
    FILE *src_fp = fopen(src_file, "r");
    if (src_fp == NULL) {
        perror("打开源文件失败"); // perror 能打印出具体的系统错误原因
        return;
    }

    // 以“只写”模式打开目标文件（若不存在则创建，若存在则清空）
    FILE *dest_fp = fopen(dest_file, "w");
    if (dest_fp == NULL) {
        perror("创建目标文件失败");
        fclose(src_fp);  // 记得关闭已打开的文件流
        return;
    }

    int ch; 
    /* 关键点：为什么用 int 而不是 char？
     * EOF 在 C 语言中通常定义为 -1。char 可能被视为无符号数，无法表示 -1。
     * 使用 int 可以确保正确判断是否到达文件末尾。
     */
    while ((ch = fgetc(src_fp)) != EOF) {
        fputc(ch, dest_fp); // 将字符写入目标文件
    }

    // 关闭文件流释放资源
    fclose(src_fp);
    fclose(dest_fp);
    printf("copy_file_char succeeded.\n");

}

// ====================================================================
// 2. 逐行文本文件复制
// 原理：利用缓冲区一次读取一行（遇到 \n 或达到长度限制为止）
// ====================================================================
void copy_file_line(const char *src_file, const char *dest_file) {
    FILE *src_fp = fopen(src_file, "r");
    if (src_fp == NULL) {
        printf("fopen src failed.\n");
        return;
    }

    FILE *dest_fp = fopen(dest_file, "w");
    if (dest_fp == NULL) {
        printf("fopen dest failed.\n");
        fclose(src_fp);  // 记得关闭已打开的文件流
        return;
    }

    char buffer[MAX_CHARS]; // 定义一个1KB的缓冲区来暂存每一行数据
    /*
     * fgets 逻辑：
     * 1. 最多读取 MAX_CHARS - 1 个字符。
     * 2. 读取到换行符 '\n' 时会将其一并存入 buffer 并停止。
     * 3. 自动在末尾添加 '\0'。
     */
    // fgets返回NULL代表读取失败或已到文件尾
    while (fgets(buffer, sizeof(buffer), src_fp) != NULL) {
        fputs(buffer, dest_fp);
    }

    fclose(src_fp);
    fclose(dest_fp);
}

// ====================================================================
// 3. 用fread和fwrite函数实现二进制文件的复制
// 原理：不关心文件内容，直接进行内存块的拷贝，效率最高
// ====================================================================
void binary_file_cpy(const char *src_file, const char *dest_file) {
    // 必须使用 "rb" 和 "wb" 模式，防止系统对换行符进行自动转换
    FILE *src_fp = fopen(src_file, "rb");
    if (src_fp == NULL) {
        printf("fopen src failed.\n");
        return;
    }

    FILE *dest_fp = fopen(dest_file, "wb");
    if (dest_fp == NULL) {
        printf("fopen dest failed.\n");
        fclose(src_fp);
        return;
    }

    // 定义缓冲区的大小,一般会稍微定义大一些，这里是4Kb
    char buffer[BUFFER_SIZE];

    /*
     * fread 参数含义：读取到 buffer 中，每个单元 1 字节，最多读 BUFFER_SIZE 个
     * 返回值：实际读到的“单元数”。如果读到文件末尾，返回值会小于 BUFFER_SIZE。
     */
    // 循环读取源文件，然后写入目标文件
    // 二进制文件复制的惯用方法
    int read_count;
    while ((read_count = fread(buffer, 1, BUFFER_SIZE, src_fp)) > 0) {
        // 将实际读到的 read_count 个字节写入目标文件
        fwrite(buffer, 1, read_count, dest_fp);
    }

    // 写操作后及时刷新
    fflush(dest_fp);  // 强制将缓冲区内容写入磁盘

    // 关闭流资源
    fclose(src_fp);
    fclose(dest_fp);

}


typedef struct {
    char name[25];
    int age;
    char gender;
} Student;


int main(void) {
    Student s1 = { "zhangsan", 20, 'm' };

    // 序列化是写的过程, 所以用只写模式打开文件
    FILE *fp = fopen("1.txt", "w");
    if (fp == NULL) {
        fprintf(stderr, "文件 1.txt 打开失败!\n");
        return -1;
    }
    // 序列化
    fprintf(fp, "Name: %s, Age: %d, Gender: %c\n", s1.name, s1.age, s1.gender);
    // 刷新输出缓冲区
    fflush(fp);

    // 反序列化,此时文件指针已指向文件末尾,已经读不到数据了
    // 在不移动文件指针的前提下, 所以要重新打开文件流
    // 反序列化是读的过程, 所以用只读模式打开文件
    // 反序列化结束，直接关闭fp流，重新打开流
    fclose(fp);
    fp = fopen("1.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "文件 1.txt 打开失败!\n");
        return -1;
    }
    Student s2;
    // name是字符数组不需要再取地址了
    // 限制最多读24个字节，避免数组越界
    // "%24[^,]"的意思是读取最多24个字符，直到遇到逗号为止, 并将读取到的字符数据存入目标字符数组以空字符结尾
    fscanf(fp, "Name: %24[^,], Age: %d, Gender: %c", s2.name, &s2.age, &s2.gender);

    // 关闭流
    fclose(fp);

    return 0;
}