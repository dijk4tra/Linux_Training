#include <asm-generic/socket.h>
#include <my_header.h>
#include <unistd.h>

//第一版：基础的单次同步阻塞通信
//特点：严格的一问一答模式，函数是阻塞的(Blocking)，只能服务一个客户端且只能通信一次
//为了打破这种“必须要你发一句、我才能回一句”的死板设定，也就是解决同步阻塞问题，就必须引入 I/O 多路复用技术

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建监听套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");

    //设置端口重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    //配置网络地址结构体,绑定服务器的ip与端口号
    struct sockaddr_in addr, client_addr;
    socklen_t len = sizeof(client_addr);

    memset(&addr, 0, sizeof(addr));//初始化addr
    
    addr.sin_family = AF_INET;
    // inet_addr()：将点分十进制的字符串IP转换为网络字节序的32位整数
    addr.sin_addr.s_addr = inet_addr(ip);
    // htons() (Host TO Network Short)：将16位的主机字节序端口号转换为网络字节序
    addr.sin_port = htons(atoi(port));
    
    //将刚刚配置好的IP地址和端口号与listen_fd绑定在一起
    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    printf("server is listening\n");

    /* accept(listen_fd, NULL, NULL);//不想知晓客户端的ip与端口号 */
    //从全连接队列中取出一条连接，并且返回一个用于通信的文件描述符
    //这是一个【阻塞函数】。服务器执行到这里会暂停，直到有客户端完成TCP三次握手
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");

    //传输数据
    //服务器接收从客户端发送过来的数据
    //注意：当前版本是同步阻塞的，必须等待客户端先发数据，recv才能读到，否则一直卡住
    char buf[100] = {0};
    int cnt = recv(conn_fd, buf, sizeof(buf), 0);
    printf("recv cnt: %d\n", cnt);
    printf("server buf : %s\n", buf);

    //服务器给客户端发送数据
    cnt = send(conn_fd, "byebye", 6, 0);
    printf("read cnt: %d\n", cnt);

    while(1);
    //最后就应该将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;
}

