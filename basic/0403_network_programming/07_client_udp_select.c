#include <my_header.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    int udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(udp_fd, -1, "socket");

    /*
     * 提前配置好服务器的地址 addr。
     * 因为客户端知道服务器的明确地址（IP和端口是硬编码的），
     * 所以客户端不存在“不知道发给谁”的问题，客户端可以随时先开口说话。
     */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    fd_set set;
    FD_ZERO(&set);

    while(1)
    {
        FD_SET(STDIN_FILENO, &set);
        FD_SET(udp_fd, &set);

        int nready = select(10, &set, NULL, NULL, NULL);
        printf("nready: %d\n", nready);

        // 场景 A：客户端从键盘输入
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            
            // 拿着提前填好的 addr 直接发信给服务器
            int cnt = sendto(udp_fd, buf, sizeof(buf), 0, 
                             (struct sockaddr *)&addr, sizeof(addr));
            printf("send cnt: %d\n", cnt);
        }

        // 场景 B：收到服务器的消息
        if(FD_ISSET(udp_fd, &set))
        {
            char buf[50] = {0};
            // 接收数据，由于只关心数据内容，不需要再次捕获地址，后两个参数填 NULL 即可
            int cnt = recvfrom(udp_fd, buf, sizeof(buf), 0
                               , NULL, NULL);
            printf("recv cnt: %d\n", cnt);
            printf("recv buf: %s\n", buf);
        }
    }
    close(udp_fd);

    return 0;
}
