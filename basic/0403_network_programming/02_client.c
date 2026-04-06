#include <my_header.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建客户端专用的文件描述符
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(client_fd, -1, "socket");

    //绑定服务器的ip与端口号
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));//初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));

    //客户端调用connect函数主动发起三次握手连接服务端
    int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //每次循环前，必须重新将需要监听的FD加入位图
        //将标准输入与用于通信的文件描述符listen_fd添加至位图中
        FD_SET(STDIN_FILENO, &set); //监听键盘输入
        FD_SET(client_fd, &set);    //监听来自服务器的网络数据
        
        //阻塞等待，直到有键盘输入，或者服务器发来数据
        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);

        //客户端自己在终端上进行了输入
        if(FD_ISSET(STDIN_FILENO, &set)){
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价(仅限socket)

            //将buf中的数据传输给对端(服务器端)
            send(client_fd, buf, sizeof(buf), 0);
        }

        //连接的服务器向客户端发送了数据
        if(FD_ISSET(client_fd, &set)){
            char buf[50] = {0};
            int cnt = recv(client_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            printf("server recv from client buf: %s\n", buf);

            /*
             * 同样的 Bug 存在于客户端：
             * 如果服务器突然 ctrl+c 终止，服务器会发送 FIN 断开连接。
             * 客户端的 select 会被无限次唤醒，recv 会无限次返回 0。
             * 终端屏幕上就会出现疯狂刷屏的 "nready = 1" 和 "recv cnt: 0" 。
             */

        }
    }

    //关闭文件描述符(意味着执行了四次挥手)
    close(client_fd);
    return 0;
}

