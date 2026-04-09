#include "send_file.h"
#include <my_header.h>

void send_file(int fd){
    
    // ================= 接收客户端想要下载的文件名 =================
    int req_file_name_len = 0;
    //使用 MSG_WAITALL 保证完整读到一个int。如果客户端直接断开，ret <= 0
    int ret = recv(fd, &req_file_name_len, sizeof(int), MSG_WAITALL);
    if (ret <= 0) {
        printf("客户端断开连接或接收客户端发送的请求文件名长度失败\n");
        return;
    }
    
    char file_name[256] = {0};
    ret = recv(fd, file_name, req_file_name_len, MSG_WAITALL);
    if (ret <= 0) {
        printf("接收客户端发送的请求文件名失败\n");
        return;
    }
    // 确保字符串以 \0 结尾，并用 [] 包裹打印，排查是否带有隐藏空格或换行符
    file_name[req_file_name_len] = '\0';
    printf(">> 客户端请求下载文件: [%s]\n", file_name);
    // ==================================================================
    
    //尝试打开客户端请求的文件
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1) {
        perror("open file error");
        //文件不存在时，发送一个长度为 0 给客户端，通知其文件不存在
        int err_len = 0;
        send(fd, &err_len, sizeof(int), MSG_NOSIGNAL);
        return; 
    }    

    struct stat st;
    fstat(file_fd, &st);
    off_t file_len = st.st_size; //获取文件内容的长度
    
    //这里还要注意：如果对一个已经断开的文件描述符执行两次以及以上的
    //写操作会触发SIGPIPE信号，导致程序崩溃，为了解决这个问题，
    //可以将send的第四个参数设置为MSG_NOSIGNAL
    int len = strlen(file_name);
    //需要将文件名的长度 + 文件名 + 文件内容(文件长度)发送给客户端
    //发送文件名的长度
    send(fd, &len, sizeof(int), MSG_NOSIGNAL);
    //发送文件名
    send(fd, file_name, len, MSG_NOSIGNAL);
    //发送文件内容长度
    send(fd, &file_len, sizeof(off_t), MSG_NOSIGNAL);
    
    //发送文件内容(为保证效率,使用零拷贝操作)
    //零拷贝操作: 数据在内核之间进行拷贝, 没有内核态和用户态的切换
    sendfile(fd, file_fd, NULL, file_len);
    printf(">> 文件 [%s] 发送完毕!\n\n", file_name);

    close(file_fd);

}
