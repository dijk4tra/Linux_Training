#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    char *server_ip = "192.168.193.128";
    char *server_port = "11111";
    
    //创建套接字
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");
    
    //设置套接字选项：地址重用 / 端口重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    //准备服务器地址结构体
    struct sockaddr_in addr, client_addr;
    socklen_t len = sizeof(client_addr);

    //初始化结构体
    memset(&addr, 0, sizeof(addr));

    //填写服务器地址
    //1.指定地址族：IPv4
    addr.sin_family = AF_INET;
    //2.指定IP地址
    // - 把点分十进制字符串形式的 IPv4 地址
    // - 转换成 32 位二进制形式
    addr.sin_addr.s_addr = inet_addr(server_ip);
    //3.指定端口号
    //把主机字节序的 16 位整数转成网络字节序
    addr.sin_port = htons(atoi(server_port));

    //绑定本机 IP 和端口
    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //把 socket 变成监听套接字
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");
    printf("server is listening...\n");

    //接受客户端连接
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");
    
    //打印客户端信息
    printf("client ip = %s, client port = %d\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    //接收客户端发送的数据
    char buf[100] = {0};
    //阻塞等待客户端发送数据，返回值 cnt 是实际接收到的字节数
    int cnt = recv(conn_fd, buf, sizeof(buf)-1, 0);
    if(cnt == 0){
        printf("client has closed the connection\n");
    }else{
        buf[cnt] = '\0'; // 把收到的数据补成字符串，便于打印
        printf("recv cnt = %d\n", cnt);
        printf("server recv: %s\n", buf);
    }

    //服务器向客户端发送数据
    cnt = send(conn_fd, "hello from server", 17, 0);
    ERROR_CHECK(cnt, -1, "send");
    printf("send cnt: %d\n", cnt);

    //关闭连接
    close(listen_fd); // 停止监听新的连接
    close(conn_fd);   // 关闭当前通信连接（向客户端发送 FIN 报文，触发四次挥手）
    
    printf("Server has closed fds. Sleeping for 30s ...\n");
    sleep(30);

    return 0;
}

