#include <my_header.h>

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";
    
    //创建文件描述符
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listen_fd, -1, "socket");

    //设置端口重用
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //绑定服务器ip与端口号
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr)); //初始化addr

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip); //本机字节序转换为网络字节序
    addr.sin_port = htons(atoi(port));
    
    int ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    //监听
    ret = listen(listen_fd, 10);
    ERROR_CHECK(ret, -1, "listen");

    printf("server is listening...\n");

    int arr[1024];
    for(int idx=0; idx<1024; idx++){
        arr[idx] = 0;
    }
    
    int conn_fd = 0;
    //定位读位图变量
    fd_set set;
    FD_ZERO(&set);
    //最开始只需要将listen_fd监听起来，
    //只有这个文件描述符就绪(可读),才可以取连接
    FD_SET(listen_fd, &set);
    
    while(1){
        fd_set tmp;//临时的位图，它的作用就是在select之前，设置
                   //需要监听的文件描述符，因为在select中
                   //位图是一个传入传出参数，有可能会改了之前的
                   //位图中的值
        tmp = set;
        
        int nready = select(10, &tmp, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select");
        printf("nready: %d\n", nready);

        //listen_fd就绪（可读），那么就可以从全连接队列中
        //取出一条连接，也就是需要调用accept
        if(FD_ISSET(listen_fd, &tmp)){
            conn_fd = accept(listen_fd, NULL, NULL);
            ERROR_CHECK(conn_fd, -1, "accept");

            //这样连接就出来了，后续需要通过文件描述符conn_fd
            //进行收发数据，但是在收发之前，需要监听
            FD_SET(conn_fd, &set);
            //同时也可以让服务器在终端输入数据，将数据发给客户端
            FD_SET(STDIN_FILENO, &set);
            //不想监听其他的新的连接，那么就可以将listen_fd从set
            //中清除
            /* FD_CLR(listen_fd, &set); */

            //在这里使用数组的目的就是因为，如果多个客户端连接上来
            //之后，conn_fd就会发生更改，如果后续不处理，那么老的
            //已连接的conn_fd就无法找到
            for(int idx=0; idx<1024; idx++){
                if(0 == arr[idx]){
                    arr[idx] = conn_fd;
                    break;
                }
            }
        }

        for(int idx=0; idx<1024; idx++){
            if(0 == arr[idx]){
                continue;
            }
            
            //----------------------
            //服务器自己在终端上进行了输入
            if(FD_ISSET(STDIN_FILENO, &tmp)){
                //通过键盘输入数据，然后存放在buf中
                char buf[50] = {0};
                read(STDIN_FILENO, buf, sizeof(buf));
                
                //需要将buf中的数据传输给对端(客户端)
                send(arr[idx], buf, sizeof(buf), 0);
            }

            //说明连接的客户端发数据给服务器端
            if(FD_ISSET(arr[idx], &tmp)){
                char buf[50] = {0};
                int cnt = recv(arr[idx], buf, sizeof(buf), 0);
                printf("recv cnt: %d\n", cnt);

                if(0 == cnt){
                    printf("客户端关闭了\n");
                    //如果客户端已经关闭了，需要将conn_fd不再监听
                    FD_CLR(arr[idx], &set);
                    //还需要将服务器在终端输入的操作做处理，不让其
                    //发给已经断开的连接
                    FD_CLR(STDIN_FILENO, &set);
                    //这个连接已经断开了，之前又没有监听listen_fd，
                    //那就说明服务器不能接受第二个连接,这样写的
                    //目的是可以让本连接断开之后，可以继续连接
                    //到服务器上，或者其他连接连接到服务器
                    /* FD_SET(listen_fd, &set); */

                    //将本连接的文件描述符关掉，释放文件描述符资源
                    close(arr[idx]);
                    continue;//可以结束本次循环，后面的打印printf
                             //就不执行了，原因是：已经断开了，
                             //没有数据了
                }
                printf("server recv from client buf : %s\n", buf);
            }
        }

    }

    //最后将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;


}

