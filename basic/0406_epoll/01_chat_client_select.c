#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    char *ip = "192.168.193.128";
    char *port = "12333";

    //创建文件描述符
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(client_fd, -1, "socket");

    //绑定服务器的ip与端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    //调用connect函数进行三次握手
    int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");

    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //将标准输入与用于通信的文件描述符listen_fd添加至位图中
        FD_SET(STDIN_FILENO, &set);
        FD_SET(client_fd, &set);
        
        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready: %d\n", nready);

        //客户端在自己的终端上进行了输入
        if(FD_ISSET(STDIN_FILENO, &set)){
            //通过键盘输入数据, 然后存放在buf中
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));

            //需要将buf中的数据传输给对端(服务器)
            send(client_fd, buf, sizeof(buf), 0);
        }

        //连接的服务器端向该客户端发送数据
        if(FD_ISSET(client_fd, &set)){
            char buf[50] = {0};
            int cnt = recv(client_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            if(0 == cnt){
                printf("服务器已经关闭\n");
                break;
            }
            printf("client recv from server buf: %s\n", buf);
        }
    }
    
    //关闭文件描述符(意味着执行了四次挥手)
    close(client_fd);
    return 0;
}

  
