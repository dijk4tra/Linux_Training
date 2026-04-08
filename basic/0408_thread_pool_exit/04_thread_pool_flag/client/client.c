#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建文件描述符
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(client_fd, -1, "socket");

    //绑定服务器的ip和端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));
    
    //调用connect函数进行三次握手
    int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    //客户端的代码走到这里就说明三次握手已经建立成功了，
    //已经可以与服务器传输数据
    
    //先读取文件名字长度
    //再根据文件名字长度: 读取文件名
    int file_name_len = 0;
    recv(client_fd, &file_name_len, sizeof(int), MSG_WAITALL);

    char file_name[100] = {0};
    recv(client_fd, file_name, file_name_len, MSG_WAITALL);

    //接收文件大小
    off_t file_size = 0;
    recv(client_fd, &file_size, sizeof(off_t), MSG_WAITALL);
    //MSG_WAITALL: 等到接收完毕指定长度的数据, 如果等不到就一直阻塞
    printf("file_size: %ld \n", file_size);
    
    //接收文件内容
    int file_fd = open(file_name, O_RDWR|O_CREAT, 0600);
    //调整文件需要的大小
    ftruncate(file_fd, file_size);
    //内存映射
    char *p = (char *)mmap(NULL, file_size, PROT_READ|PROT_WRITE, MAP_SHARED, file_fd, 0);
    recv(client_fd, p, file_size, MSG_WAITALL);
    //解除内存映射
    munmap(p, file_size);

    close(file_fd);
    close(client_fd);
     
    return 0;
}

