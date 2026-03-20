#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    int fd;             // 文件描述符
    off_t file_size;    // 文件大小，off_t 是专门用于文件大小/偏移量的类型
    char *buffer;       // 用于保存文件内容的堆内存指针
    ssize_t bytes_read; // read 的返回值，表示实际读取的字节数
    
    // 1.检查命令行参数数量
    ARGS_CHECK(argc, 2);

    // 2.以只读方式打开文件
    fd = open(argv[1], O_RDONLY);
    ERROR_CHECK(fd, -1, "open file failed");

    // 3.使用lseek将文件指针移动到文件末尾
    // 返回值就是文件的大小(单位是字节)
    file_size = lseek(fd, 0 ,SEEK_END);
    if(file_size == -1){
        perror("lseek SEEK_END failed");
        close(fd);
        return -1;
    }
    
    // 4.在堆区申请 file_size + 1 字节空间
    // +1 的目的是在最后补一个 '\0'，方便把内容当作字符串输出
    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL)
    {
        perror("malloc failed");
        close(fd);
        return 1;
    }

    // 5.再次使用 lseek 将文件指针移动回文件开头
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek SEEK_SET failed");
        free(buffer);
        close(fd);
        return -1;
    }

    // 6.一次性读取整个文件内容到内存中
    bytes_read = read(fd, buffer, file_size);
    if(bytes_read == -1){
        perror("read file failed");
        free(buffer);
        close(fd);
        return -1;
    }

    // 7. 在读取内容后面补上字符串结束符
    buffer[bytes_read] = '\0';

    // 8. 输出读取结果
    printf("文件大小：%ld 字节\n", (long)file_size);
    printf("实际读取：%ld 字节\n", (long)bytes_read);
    printf("文件内容如下：\n");
    printf("%s\n", buffer);

    // 9. 释放资源
    free(buffer);
    close(fd);

    return 0;
}

