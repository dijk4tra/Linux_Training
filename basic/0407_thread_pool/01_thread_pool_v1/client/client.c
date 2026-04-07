#include <my_header.h>

int main(int argc, char *argv[]){                                  
   
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建文件描述符
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(client_fd, -1, "socket");

    //绑定服务器的ip和端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    
    //调用connect函数进行三次握手
    int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    //客户端的代码走到这里就说明三次握手已经建立成功了，
    //已经可以与服务器传输数据
    
    //接收服务器发送过来的数据
    char buf[100] = {0};
    int cnt = recv(client_fd, buf, sizeof(buf), 0);
    printf("recv cnt: %d\n", cnt);
    printf("client buf: %s\n", buf);
    
    close(client_fd);
    return 0;
}

