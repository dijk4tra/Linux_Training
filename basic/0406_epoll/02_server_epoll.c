#include <my_header.h>

void add_epoll_fd(int epfd, int fd){
    struct epoll_event evt;
    evt.events = EPOLLIN; //读事件
    evt.data.fd = fd;     //监听的文件描述符
    
    //将listen_fd放在红黑树上进行监听
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
    ERROR_CHECK(ret, -1, "add_epoll_fd");
}

void del_epoll_fd(int epfd, int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;//读事件
    evt.data.fd = fd;//监听的文件描述符

    //将listen_fd放在红黑树上进行监听
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt);
    ERROR_CHECK(ret, -1, "del_epoll_fd");
}


int main(int argc, char *argv[]){                                  
    
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建文件描述符
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");

    //设置端口重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    //绑定服务器的ip与端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);//本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));

    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    printf("server is listening\n");
    
    //返回一个文件描述符对象，其中存储有：红黑树与就绪链表(双向链表)
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    
    struct epoll_event evt;
    evt.events = EPOLLIN; //读事件
    evt.data.fd = listen_fd; //监听的文件描述符

    //将listen_fd放在红黑树上进行监听
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &evt);
    ERROR_CHECK(ret, -1, "epoll_ctl add");

    int conn_fd = 0;

    while(1){
        struct epoll_event lst[10];
        //nready就是就绪文件描述符的数目
        //lst就是存储就绪文件描述符的数据结构
        //(用户态的时候就是自己定义的数组)
        int nready = epoll_wait(epfd, lst, 10, -1);
        ERROR_CHECK(nready, -1, "epoll_wait");
        printf("nready: %d\n", nready);

        //遍历就绪的文件描述符
        for(int idx=0; idx<nready; ++idx){
            //拿到自定义数组中的文件描述符（也就是就绪的文件描述符）
            int fd = lst[idx].data.fd;

            //有新的连接请求上来
            if(fd == listen_fd){
                conn_fd = accept(listen_fd, NULL, NULL);
                ERROR_CHECK(conn_fd, -1, "accept");

                add_epoll_fd(epfd, conn_fd);
                add_epoll_fd(epfd, STDIN_FILENO);
                del_epoll_fd(epfd, listen_fd);

            }else if(fd == STDIN_FILENO){
                //通过键盘输入数据，然后存放在buf中
                char buf[50] = {0};
                read(STDIN_FILENO, buf, sizeof(buf));

                //需要将buf中的数据传输给对端（客户端）
                send(conn_fd, buf, sizeof(buf), 0);

            }else{ //就是老的连接上有数据发过来，也就是conn_fd就绪了
                char buf[50] = {0};
                int cnt = recv(fd, buf, sizeof(buf), 0);
                if(0 == cnt)
                {
                    printf("客户端关闭了fd = %d\n", fd);
                    //将conn_fd以及STDIN_FILENO从红黑树上删除
                    del_epoll_fd(epfd, fd);
                    del_epoll_fd(epfd, STDIN_FILENO);
                    //继续监听listen_fd
                    add_epoll_fd(epfd, listen_fd);
                    close(fd);
                    break;
                }
                printf("server recv from client buf: %s\n", buf);

            }
        }
    }
    
    //最后就应该将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);
    return 0;
}

