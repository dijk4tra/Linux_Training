#include <my_header.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    /*
     * 知识点 1：创建 UDP 套接字
     * SOCK_DGRAM: Datagram（数据报），指定底层使用 UDP 协议。
     * UDP 是无连接的，它不会像 TCP 那样在内核中维护复杂的发送/接收窗口和状态机。
     */
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(fd, -1, "socket");

    struct sockaddr_in addr, client_addr;;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    
    // len 必须初始化为结构体的大小，因为在 recvfrom 中它是传入传出参数
    socklen_t len = sizeof(client_addr);

    /*
     * 知识点 2：绑定 (Bind)
     * UDP 服务器同样需要绑定 IP 和端口，这样操作系统才知道
     * 把发往 12345 端口的 UDP 数据报交给当前这个进程。
     */
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    /*
     * 【重要区别】
     * 这里没有 listen()，也没有 accept()！
     * UDP 服务器不需要等待连接建立，绑定完端口后，直接就可以开始接收数据了。
     */

    //---- 数据的收发
    char buf[50] = {0};
    
    /*
     * 知识点 3：recvfrom (接收数据报并捕获发送方地址)
     * 因为没有 TCP 的专属连接(conn_fd)，UDP 套接字可能会收到来自四面八方的数据。
     * 为了知道这条数据是谁发的（以便后续回信），必须使用 recvfrom。
     * 参数 5 (client_addr)：传出参数，内核会把发信人的 IP 和端口填入这里。
     * 参数 6 (len)：传入传出参数，标明结构体长度。
     */
    recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &len);
    printf("recv from client : %s\n", buf);

    /*
     * 知识点 4：sendto (指定目的地发送数据)
     * 既然刚通过 recvfrom 获取了客户端的地址 (client_addr)，
     * 我们就可以利用这个地址，原路给客户端发回一个响应。
     */
    sendto(fd, "nihao", 5, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

    close(fd);
    return 0;
}
