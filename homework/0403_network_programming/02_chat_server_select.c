#include <my_header.h>

typedef struct conn{
    int fd; //文件描述符
    int alive; //连接是否存活: 1存活,2死亡
}conn_t;

int main(int argc, char *argv[]){                                  
    
    char *ip = "192.168.193.128";
    char *port = "12333";
    
    //创建用于监听的文件描述符
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");
    
    //端口复用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    //绑定服务器的ip和端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    
    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //监听客户端
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    //存放群聊客户端的个数的数组
    conn_t con_arr[100] = {0};
    int size = 0;
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(listen_fd, &set);

    while(1){
        fd_set tmp;
        tmp = set;
        int nready = select(100, &tmp, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready: %d\n", nready);

        if(FD_ISSET(listen_fd, &tmp)){
            int conn_fd = accept(listen_fd, NULL, NULL);
            ERROR_CHECK(conn_fd, -1, "accept");

            con_arr[size].fd = conn_fd;
            con_arr[size].alive = 1;
            ++size;

            //需要将conn_fd放在监听集合中进行监听
            FD_SET(conn_fd, &set);
        }

        //需要遍历size
        for(int idx=0; idx<size; idx++){
            //从数组中取值
            int fd = con_arr[idx].fd;

            //表明有数据从老的连接上发过来
            if(FD_ISSET(fd, &tmp)){
                char buf[50] = {0};
                //recv返回的是实际接收到的数据的个数
                int cnt = recv(fd, buf, sizeof(buf), 0);
                if(0 == cnt){
                    printf("客户端fd = %d已断开连接", fd);
                    con_arr[idx].fd = 0;
                    con_arr[idx].alive = 0;

                    //客户端断开连接后就不需要再进行监听
                    //需要将其从set中清除
                    FD_CLR(fd, &set);
                    close(fd);
                    
                    continue;
                }

                //转发消息给其它客户端
                for(int pos = 0; pos<size; pos++){
                    //仅转发给除自己外仍在连接状态中的客户端
                    if(0 == con_arr[pos].alive || pos == idx){
                        continue;
                    }
                    send(con_arr[pos].fd, buf, sizeof(buf), 0);
                }
            }
        }
    }
    
    close(listen_fd);

    return 0;
}

