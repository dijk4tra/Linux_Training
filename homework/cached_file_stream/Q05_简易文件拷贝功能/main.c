#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define BUFFER_SIZE 4096

/*
编写一个C程序，实现一个简易的文件拷贝功能。要求以字节为单位从 `source.dat` 读取内容，并将其逐个写入到 `dest.dat` 中。
要求：
    1. 使用 `fopen()` 以二进制模式打开文件。
    2. 使用 `fgetc()` 和 `fputc()` 函数进行字节级的读写操作。
    3. 循环读取直到文件末尾（EOF）。
    4. 操作完成后务必关闭所有文件流。
*/

void bfile_cpy(const char *src_file, const char *dest_file) {
    // 只读模式打开源文件用于读取，注意要明确以b为后缀
    FILE *src_fp = fopen(src_file, "rb");
    if (src_file == NULL) {
        fprintf(stderr, "fopen src_file %s failed", src_file);
        exit(1);
    }

    // 只写模式打开目标文件用于写入，注意要明确以b为后缀
    FILE *dest_fp = fopen(dest_file, "wb");
    if (dest_file == NULL) {
        fprintf(stderr, "fopen dest_file %s failed.", dest_file);
        fclose(src_fp); // 关闭已经打开的源文件
        exit(1);
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
        fwrite(buffer, 1, read_count, dest_fp);
    }

    // 写操作后及时刷新
    fflush(dest_fp);  // 强制将缓冲区内容写入磁盘
    // 关闭流资源
    fclose(src_fp);
    fclose(dest_fp);

}

int main(void) {
    bfile_cpy("source.dat", "dest.dat");
    return 0;
}