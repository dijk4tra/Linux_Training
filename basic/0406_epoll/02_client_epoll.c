#include <my_header.h>

void add_epoll_fd(int epfd, int fd){
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
    ERROR_CHECK(ret, -1, "add_epoll_fd");
}

int main(int argc, char *argv[]){                                  
    
    char *ip = "192.168.193.128";
    char *port = "12345";
    
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
    printf("成功连接到群聊服务器！\n");
    
    //创建 epoll 句柄
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    //将标准输入(键盘)和网络套接字加入 epoll 监听
    add_epoll_fd(epfd, STDIN_FILENO);
    add_epoll_fd(epfd, client_fd);

    while(1){
        struct epoll_event lst[10];    
        
        //阻塞等待事件发生
        int nready = epoll_wait(epfd, lst, 10, -1);
        ERROR_CHECK(nready, -1, "epoll_wait");
        printf("nready: %d\n", nready);
        
        for(int i=0; i<nready; i++){
            int fd = lst[i].data.fd;

            //1.客户端在自己的终端上进行了输入
            if(fd == STDIN_FILENO){
                char buf[50] = {0};
                int cnt = read(STDIN_FILENO, buf, sizeof(buf));

                if(cnt>0){
                    //将buf中的数据传输给服务器
                    send(client_fd, buf, cnt, 0);
                }
            }

            //2.连接的服务器端向该客户端广播了数据
            else if(fd == client_fd){
                char buf[50] = {0};
                int cnt = recv(client_fd, buf, sizeof(buf), 0);

                if(0 == cnt){
                    printf("\n[系统提示]: 服务器已经关闭。\n");
                    close(client_fd);
                    return 0;
                }

                printf("收到群消息: %s", buf);
            }
        }
    }
    
    //关闭文件描述符(意味着执行了四次挥手)
    close(client_fd);
    return 0;
}

