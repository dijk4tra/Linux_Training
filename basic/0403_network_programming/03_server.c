#include <asm-generic/socket.h>
#include <my_header.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
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

    printf("sever is listening...\n");

    /* accept(listen_fd, NULL, NULL);//不想知晓客户端的ip与端口号 */
    //从全连接队列中取出一条连接，并且返回一个用于通信的文件描述符
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");
    
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //将标准输入与用于通信的文件描述符conn_fd添加至位图中
        FD_SET(STDIN_FILENO, &set);
        FD_SET(conn_fd, &set);

        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);
        
        //服务器自己在终端上进行了输入
        if(FD_ISSET(STDIN_FILENO, &set)){
            //通过键盘输入数据，然后存放在buf中
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价

            //需要将buf中的数据传输给对端(客户端)
            send(conn_fd, buf, sizeof(buf), 0);
        }
        
        //连接的客户端向服务器发送了数据
        if(FD_ISSET(conn_fd, &set)){
            char buf[50] = {0};
            int cnt = recv(conn_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            if(0 == cnt){
                printf("客户端已关闭\n");
                break;
            }
            printf("server recv from client buf: %s\n", buf);
        }

    }

    //将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;
}

