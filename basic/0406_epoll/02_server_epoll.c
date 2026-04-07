#include <my_header.h>

//客户端连接状态结构体
typedef struct conn{
    int fd;     //文件描述符
    int alive;  //连接是否存活: 1存活, 0死亡
}conn_t;

void add_epoll_fd(int epfd, int fd){
    struct epoll_event evt;
    evt.events = EPOLLIN; //读事件
    evt.data.fd = fd;     //监听的文件描述符
    
    //将fd放在红黑树上进行监听
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
    ERROR_CHECK(ret, -1, "add_epoll_fd");
}

void del_epoll_fd(int epfd, int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;//读事件
    evt.data.fd = fd;//监听的文件描述符

    //将fd放在红黑树上进行监听
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
    
    // 存放群聊客户端的数组
    conn_t con_arr[100] = {0};
    int size = 0;
    
    //使用函数epoll_create()创建 epoll 句柄
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
        struct epoll_event lst[100]; // 准备接收就绪事件的数组
        //nready就是就绪文件描述符的数目
        //lst就是存储就绪文件描述符的数据结构
        //(用户态的时候就是自己定义的数组)
        int nready = epoll_wait(epfd, lst, 100, -1);
        ERROR_CHECK(nready, -1, "epoll_wait");
        printf("nready: %d\n", nready);

        //遍历就绪的文件描述符
        for(int idx=0; idx<nready; ++idx){
            //拿到自定义数组中的文件描述符（也就是就绪的文件描述符）
            int fd = lst[idx].data.fd;

            //1.如果是 listen_fd 发生读事件，说明有新客户端连接
            if(fd == listen_fd){
                conn_fd = accept(listen_fd, NULL, NULL);
                ERROR_CHECK(conn_fd, -1, "accept");
                printf("新客户端加入, fd = %d\n", conn_fd);
                
                //记录新客户端的信息
                con_arr[size].fd = conn_fd;
                con_arr[size].alive = 1;
                ++size;

                //将新客户端 fd 加入 epoll 监听
                add_epoll_fd(epfd, conn_fd);

            }
            //2.如果是其他 fd，说明有老客户端发来消息，或者是断开连接
            else{
                char buf[50] = {0};
                int cnt = recv(fd, buf, sizeof(buf), 0);
                if(0 == cnt)
                {
                    printf("客户端 fd = %d 已断开连接\n", fd);
                    
                    //从 epoll 树上删除该 fd 并关闭
                    del_epoll_fd(epfd, fd);
                    close(fd);

                    //遍历数组，将该客户端标记为死亡（alive = 0）
                    for(int i=0; i<size; i++){
                        if(con_arr[i].fd == fd){
                            con_arr[i].alive = 0;
                            break;
                        }
                    }
                }
                else{
                    //转发消息给其它群聊客户端
                    for(int pos=0; pos<size; ++pos){
                        //仅转发给除自己外，且仍在连接状态的客户端
                        if(0 == con_arr[pos].alive || con_arr[pos].fd == fd){
                            continue;
                        }
                        //最好发送实际接收到的字节数 cnt，而不是整个 sizeof(buf)
                        send(con_arr[pos].fd, buf, cnt, 0);
                    }
                }
            }
        }
    }
    
    //关闭文件描述符
    close(listen_fd);
    return 0;
}

