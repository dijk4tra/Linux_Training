#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    //需要连接的服务器的 IP 和端口
    char *server_ip = "192.168.193.128";
    char *server_port = "11111";

    //创建客户端套接字
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sock_fd, -1, "socket");

    //准备服务器地址结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    //设置服务器地址结构体
    //IPv4
    server_addr.sin_family = AF_INET;
    //服务器IP: 字符串 -> 网络字节序二进制地址
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    //服务器端口: 字符串 -> 整数 -> 网络字节序
    server_addr.sin_port = htons(atoi(server_port));
    
    //主动发起 TCP 连接
    int ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "connect");

    //客户端发送数据给服务器
    int cnt = send(sock_fd, "---hello from client---", 23, 0);
    ERROR_CHECK(cnt, -1, "send");
    printf("send cnt = %d\n", cnt);

    //接收服务器回复的数据
    char buf[100] = {0};
    cnt = recv(sock_fd, buf, sizeof(buf)-1, 0);
    ERROR_CHECK(cnt, -1, "recv");

    if (cnt == 0) {
        printf("server has closed the connection\n");
    } else {
        buf[cnt] = '\0';
        printf("recv cnt = %d\n", cnt);
        printf("client recv: %s\n", buf);
    }
    
    //关闭套接字
    close(sock_fd);
    
    printf("Client closed. It should be in TIME_WAIT soon. Sleeping for 30s ...\n");
    sleep(30);

    return 0;
}

