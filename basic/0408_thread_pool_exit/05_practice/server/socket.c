#include <my_header.h>
#include "socket.h"

// 封装套接字的创建、绑定、监听流程
void init_socket(int *fd, char *ip, char *port){
    
    //1.创建用于监听的TCP套接字
    *fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(*fd, -1, "socket");

    //2.设置端口复用
    int opt = 1;
    setsockopt(*fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    //3.填充服务器的网络地址结构体
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    //4.将套接字与指定的IP和端口号绑定
    int ret = bind(*fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //5.将套接字设置为被动监听状态
    ret = listen(*fd, 100);
    ERROR_CHECK(ret, -1, "listen");

}
