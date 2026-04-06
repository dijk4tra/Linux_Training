#include <my_header.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    /* 
     * 注：虽然变量名起作 listen_fd，但 UDP 其实没有 listen 的概念。
     * 叫 udp_fd 或者 sockfd 可能会更准确，避免和 TCP 混淆。
     */
    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(udp_fd, -1, "socket");

    // 绑定服务器的ip与端口号
    struct sockaddr_in addr, client_addr;
    socklen_t len = sizeof(client_addr);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    int ret = bind(udp_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    // 位图初始化，准备交给 select 管理
    fd_set set;
    FD_ZERO(&set);

    while(1)
    {
        FD_SET(STDIN_FILENO, &set);
        FD_SET(udp_fd, &set);

        int nready = select(10, &set, NULL, NULL, NULL);
        printf("nready: %d\n", nready);

        // 场景 A：服务器端从键盘输入数据
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));

            /*
             * 【重大逻辑陷阱预警】
             * 如果服务器刚启动，没有任何客户端发来过消息，
             * 此时管理员直接在服务器键盘上敲了一句话，会发生什么？
             * 
             * 答案：会发送失败或发送给未知的地址（甚至导致程序崩溃）！
             * 原因：此时 client_addr 结构体里的数据是空的（因为 memset 为 0 了）。
             * 只有当执行过下面的 recvfrom 之后，client_addr 才会被真正填入客户端的 IP。
             * 
             * UDP 的服务器其实是“被动响应者”，在不知道客户端地址之前，
             * 它是无法主动给客户端发消息的。
             */
            sendto(udp_fd, buf, sizeof(buf), 0, 
                   (struct sockaddr *)&client_addr, sizeof(client_addr));
        }

        // 场景 B：套接字可读（有 UDP 数据报到达）
        if(FD_ISSET(udp_fd, &set))
        {
            char buf[50] = {0};
            /*
             * 当收到数据时，顺便捕获了对方的地址存入 client_addr。
             * 只有在这行代码执行过后，上面的 sendto 才能真正有效工作。
             */
            recvfrom(udp_fd, buf, sizeof(buf), 0, 
                     (struct sockaddr *)&client_addr, &len);
            printf("recv buf: %s\n", buf);
        }
    }
    // 最后将文件描述符关闭
    close(udp_fd);
    return 0;
}
