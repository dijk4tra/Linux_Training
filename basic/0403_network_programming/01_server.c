#include <asm-generic/socket.h>
#include <my_header.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.100.128";
    char *port = "12345";

    //创建文件描述符
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");

    //设置端口重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    //绑定服务器的ip与端口号
    struct sockaddr_in addr, client_addr;
    socklen_t len = sizeof(client_addr);

    memset(&addr, 0, sizeof(addr));//初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));

    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    printf("sever is listening\n");

    /* accept(listen_fd, NULL, NULL);//不想知晓客户端的ip与端口号 */
    //从全连接队列中取出一条连接，并且返回一个用于通信的文件描述符
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");

    //传输数据
    //服务器接收从客户端发送过来的数据
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

