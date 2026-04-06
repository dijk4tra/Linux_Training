#include <my_header.h>

//第一版：基础的单次同步阻塞通信
//特点：严格的一问一答模式，函数是阻塞的(Blocking)，只能服务一个客户端且只能通信一次

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建客户端专用的文件描述符
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(client_fd, -1, "socket");
    
    //配置网络地址结构体
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//初始化addr
    
    //配置目标服务器的地址信息
    addr.sin_family = AF_INET;
    // inet_addr()：将点分十进制的字符串IP转换为网络字节序的32位整数
    addr.sin_addr.s_addr = inet_addr(ip);
    // htons() (Host TO Network Short)：将16位的主机字节序端口号转换为网络字节序
    addr.sin_port = htons(atoi(port));
    
    /* 
     * 知识点：客户端为什么不需要 bind()？
     * 客户端通常不需要显式绑定固定的端口。
     * 在调用connect时，操作系统内核会自动为客户端分配一个本地可用的随机端口。
     */

    //客户端调用connect()函数，主动向服务器发起TCP的【三次握手】
    //这是一个阻塞调用，直到三次握手成功或超时才会返回
    //此时，服务器端的 accept() 也会因为握手成功而被唤醒
    int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    //客户端与服务器进行数据的收发
    //必须和服务器的逻辑对应：服务器先recv，客户端就必须先send
    int cnt = send(client_fd, "hello", 5, 0);
    printf("read cnt: %d\n", cnt); //此处打印的是发送出去的字节数

    //接收服务器发送过来的数据
    char buf[100] = {0};
    cnt = recv(client_fd, buf, sizeof(buf), 0);
    printf("recv cnt: %d\n", cnt);
    printf("client buf : %s\n", buf);

    while(1);
    //关闭文件描述符(意味着执行了四次挥手)
    close(client_fd);
    return 0;
}

