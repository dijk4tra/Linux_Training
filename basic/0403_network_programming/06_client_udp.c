#include <my_header.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.100.128";
    char *port = "12345";

    // 创建 UDP 套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    /*
     * 配置目标服务器的地址信息
     */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);

    /*
     * 【与TCP的重要区别】
     * 客户端不需要调用 connect() 进行三次握手！
     * 直接把数据和目标地址打包，通过 sendto 发送至网络。
     */
    // 将数据 "hello" 发送给目标 addr (即服务器)
    sendto(fd, "hello", 5, 0, (struct sockaddr *)&addr, sizeof(addr));

    //---- 数据的收发
    char buf[50] = {0};
    
    /*
     * 接收服务器的响应。
     * 如果客户端不在乎到底是谁回的信，最后两个参数可以填 NULL。
     * （但在实际安全开发中，通常也会填入结构体，用来校验发信人是不是真的是我们的目标服务器）
     */
    recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
    printf("recv from server: %s\n", buf);

    close(fd);

    return 0;
}
