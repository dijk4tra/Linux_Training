#include <my_header.h>

int main(int argc, char *argv[])
{
    /*==============================================================
     * 一、服务器的 IP 和端口
     *
     * 客户端不是绑定自己，而是“要去连接谁”
     * 所以这里填写的是“服务器地址”
     *==============================================================*/
    char *server_ip = "192.168.193.128";
    char *server_port = "12345";

    /*==============================================================
     * 二、创建客户端套接字
     *
     * socket(AF_INET, SOCK_STREAM, 0)
     * - AF_INET     : IPv4
     * - SOCK_STREAM : TCP
     * - 0           : 自动选择协议（此处通常为 TCP）
     *
     * 返回值：
     * - 成功：socket 文件描述符
     * - 失败：-1
     *==============================================================*/
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sock_fd, -1, "socket");

    /*==============================================================
     * 三、准备服务器地址结构体
     *
     * 客户端调用 connect 时，需要知道服务器的地址
     *==============================================================*/
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    // 1. IPv4
    server_addr.sin_family = AF_INET;

    // 2. 服务器 IP：字符串 -> 网络字节序二进制地址
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // 3. 服务器端口：字符串 -> 整数 -> 网络字节序
    server_addr.sin_port = htons(atoi(server_port));

    /*==============================================================
     * 四、connect：主动发起 TCP 连接
     *
     * connect 的作用：
     * - 客户端主动连接服务器
     *
     * 参数说明：
     * 1. sock_fd
     *    - 客户端 socket
     *
     * 2. (struct sockaddr *)&server_addr
     *    - 服务器地址
     *
     * 3. sizeof(server_addr)
     *    - 地址长度
     *
     * 为什么要强制类型转换？
     * - connect 的参数要求是 struct sockaddr *
     * - 但我们实际使用的是 struct sockaddr_in *
     * - 因此需要显式转换为通用地址结构体指针
     *
     * 返回值：
     * - 成功：0
     * - 失败：-1
     *
     * 与三次握手的关系：
     * - 客户端调用 connect 后，内核开始执行三次握手
     *   第一次：客户端发送 SYN
     *   第二次：服务器回复 SYN + ACK
     *   第三次：客户端再回复 ACK
     * - 当 connect 返回成功时，通常表示 TCP 连接已建立
     *==============================================================*/
    int ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "connect");

    /*==============================================================
     * 五、客户端发送数据给服务器
     *
     * 这里发送 "hello" 共 5 个字节，不发送字符串结束符 '\0'
     *==============================================================*/
    int cnt = send(sock_fd, "hello", 5, 0);
    ERROR_CHECK(cnt, -1, "send");
    printf("send cnt = %d\n", cnt);

    /*==============================================================
     * 六、接收服务器回复的数据
     *
     * 这里用 sizeof(buf) - 1 接收，
     * 是为了给字符串结束符 '\0' 留空间，方便后面按字符串打印
     *==============================================================*/
    char buf[100] = {0};
    cnt = recv(sock_fd, buf, sizeof(buf) - 1, 0);
    ERROR_CHECK(cnt, -1, "recv");

    if (cnt == 0) {
        printf("server has closed the connection\n");
    } else {
        buf[cnt] = '\0';
        printf("recv cnt = %d\n", cnt);
        printf("client recv: %s\n", buf);
    }

    /*==============================================================
     * 七、关闭套接字
     *
     * close(sock_fd)：
     * - 关闭客户端 socket
     * - 若条件满足，会由内核发起 TCP 关闭流程
     *
     * 与四次挥手的关系：
     * - close 是应用层发起“关闭连接”的动作
     * - 后续四次挥手中的 FIN / ACK 交换由内核协议栈完成
     *==============================================================*/
    close(sock_fd);

    return 0;
}
