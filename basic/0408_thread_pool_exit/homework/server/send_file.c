#include "send_file.h"
#include <my_header.h>

void send_file(int fd){
    
    int req_file_name_len = 0;
    int ret = recv(fd, &req_file_name_len, sizeof(int), MSG_WAITALL);
    if (ret <= 0) {
        printf("客户端断开连接或接收客户端发送的请求文件名长度失败\n");
        return; // 直接返回，外层 Worker 会负责 close(fd)
    }

    char file_name[256] = {0};
    ret = recv(fd, file_name, req_file_name_len, MSG_WAITALL);
    if (ret <= 0) {
        printf("接收客户端发送的请求文件名失败\n");
        return;
    }
    
    // 手动添加字符串结束符 \0，确保作为 C 字符串使用时不会越界
    // 用 [] 包裹打印，方便排查由于隐藏空格或换行符导致的找不到文件的问题
    file_name[req_file_name_len] = '\0';
    printf(">> 客户端请求下载文件: [%s]\n", file_name);

    // 尝试在服务端本地打开客户端请求的这个文件 (只读模式)
    int file_fd = open(file_name, O_RDONLY);
    if(file_fd == -1){
        perror("open file failed");
        // 如果文件不存在或无法打开，发送一个长度为 0 的标志给客户端，
        // 客户端拿到长度 0 后就会知道文件不存在，从而安全断开。
        int err_len = 0;
        send(fd, &err_len, sizeof(int), MSG_WAITALL);
        return;
    }

    // 使用fstat获取已经打开文件的原信息(文件大小)
    struct stat st;
    fstat(file_fd, &st);
    off_t file_len = st.st_size; // 获取文件内容的实际大小 (字节)

    int len = strlen(file_name);
    // 协议格式设计：将[文件名的长度] + [文件名] + [文件内容总大小]依次发给客户端

    // 这里还要注意：如果对一个已经断开的文件描述符执行两次以及以上的
    // 写操作会触发SIGPIPE信号，导致程序崩溃，为了解决这个问题，
    // 可以将send的第四个参数设置为MSG_NOSIGNAL
    
    // 发送文件名的长度
    send(fd, &len, sizeof(int), MSG_NOSIGNAL);
    
    // 发送文件名字符串
    send(fd, file_name, len, MSG_NOSIGNAL);

    // 发送文件的总大小
    send(fd, &file_len, sizeof(off_t), MSG_NOSIGNAL);

    // 发送文件本体内容
    sendfile(fd, file_fd, NULL, file_len);

    // 释放服务端本地打开的文件描述符
    close(file_fd);

}
