#include <my_header.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建文件描述符
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");

    //绑定服务器的ip与端口号
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));//初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));

    //调用connect函数进行三次握手
    int ret = connect(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //将标准输入与用于通信的文件描述符listen_fd添加至位图中
        FD_SET(STDIN_FILENO, &set);
        FD_SET(listen_fd, &set);

        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);

        //服务器自己在终端上进行了输入
        if(FD_ISSET(STDIN_FILENO, &set)){
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价

            //需要将buf中的数据传输给对端(服务器端)
            send(listen_fd, buf, sizeof(buf), 0);
        }

        //连接的服务器向客户端发送了数据
        if(FD_ISSET(listen_fd, &set)){
            char buf[50] = {0};
            int cnt = recv(listen_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            printf("server recv from client buf: %s\n", buf);
        }

    }


    //关闭文件描述符(意味着执行了四次挥手)
    close(listen_fd);
    return 0;
}

