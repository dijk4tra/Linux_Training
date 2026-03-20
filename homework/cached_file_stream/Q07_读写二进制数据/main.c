#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

/*
在网盘项目中，常需要保存文件的元数据（如文件名、大小）。
定义一个文件信息结构体 `FileInfo`，将其实例以二进制块的方式写入磁盘，再重新读入内存。
要求：
    1. 定义结构体 `FileInfo`，包含 `char filename[64]` 和 `long filesize`。
    2. 使用 `fwrite()` 将结构体变量整体写入 `index.db`。
    3. 使用 `fread()` 将数据读回到新的结构体变量中并输出。
*/

// 1.定义结构体 `FileInfo`
typedef struct {
    char filename[64]; // 文件名
    long filesize;     // 文件大小（单位：字节）
} FileInfo;

int main(void) {
    FileInfo write_data = { "lesson_by_kongling.mp4",114514 }; // 初始化一个结构体变量
    FileInfo read_data = { 0 }; // 创建一个空的结构体，用于接收读取的数据

    FILE *fp_out = fopen("index.db", "wb");
    if (fp_out == NULL) {
        perror("fopen failed");
        return 1;
    }

    // 2.使用 `fwrite()` 将结构体变量整体写入 `index.db`
    fwrite(&write_data, sizeof(FileInfo), 1, fp_out);
    fclose(fp_out);
    printf("fwrite succeeded.\n");

    FILE *fp_in = fopen("index.db", "rb");
    if (fp_in == NULL) {
        perror("fopen failed");
        return 1;
    }
    //3.使用 `fread()` 将数据读回到新的结构体变量中并输出
    if ((fread(&read_data, sizeof(FileInfo), 1, fp_in)) == 1) {
        printf("\nfread succeeded: \n");
        printf("filename: %s\n", read_data.filename);
        printf("filesize: %ld\n", read_data.filesize);
    }

    fclose(fp_in);

    return 0;
}