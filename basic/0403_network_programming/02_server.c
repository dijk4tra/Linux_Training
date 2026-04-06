#include <my_header.h>

//第二版：引入 select 实现双向随机通信
//特点：使用 select 监听多个文件描述符，打破了固定的“一问一答”模式，实现了真正的自由收发。

/*
 *关于死循环的原理（深入理解TCP）：
 *网络编程中有一个重要法则：对端关闭连接（发送FIN包），在 select 看来，
 *属于“读事件就绪”！因为底层必须要通知应用层：“对方断开了，你可以来读一读试试看。”
 *当应用程序调用 recv 或者 read 去读一个已经断开的 Socket 时，返回值是 0。
 *如果你不判断这个 0 并且跳出循环，程序就会以为刚读完一次，接着回去调 select；
 *而 select 发现底层还是断开状态，又立刻放行... 最终演变成了不消耗时间片的疯狂死循环。
 */

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建监听套接字
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

    printf("server is listening...\n");

    /* accept(listen_fd, NULL, NULL);//不想知晓客户端的ip与端口号 */
    //从全连接队列中取出一条连接，并且返回一个用于通信的文件描述符
    //阻塞等待客户端连接，返回专门用于通信的 conn_fd
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");
    
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //将标准输入与用于通信的文件描述符conn_fd添加至位图中
        FD_SET(STDIN_FILENO, &set);
        FD_SET(conn_fd, &set);
        
        /*
         * 知识点：select (I/O 多路复用)
         * 参数1: 10，代表内核需要扫描的最大文件描述符的值 + 1。（此处硬编码为10，实际开发中通常写为 max_fd + 1）
         * 参数2: &set，读事件位图，select会阻塞监控这些描述符是否可读。
         * 参数3/4: 写事件位图 / 异常事件位图（这里传NULL，不需要监听）。
         * 参数5: 超时时间，传NULL代表【永久阻塞等待】，直到有事件发生才唤醒。
         * 返回值 nready: 代表当前有几个文件描述符发生了事件。
         */

        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);
        
        //场景 A：服务器自己在终端上进行了输入
        if(FD_ISSET(STDIN_FILENO, &set)){
            char buf[50] = {0};
            //从标准输入(键盘)读取输入的数据，存放在buf中
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价(仅限于socket)
            
            //将buf中的数据通过网络传输给对端(客户端)
            send(conn_fd, buf, sizeof(buf), 0);
        }
        
        //场景 B：连接的客户端向服务器发送了数据(网卡收到了TCP报文，网络套接字可读)
        if(FD_ISSET(conn_fd, &set)){
            char buf[50] = {0};
            //从网络缓冲区中读取数据
            int cnt = recv(conn_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            printf("server recv from client buf: %s\n", buf);

            /*
             * 【此处存在一个重大Bug】
             * 现象: "任何一端断开后，对端死循环，也就是一直打印"
             * 原因分析：当客户端断开连接(调用close)时，TCP协议会发送一个 FIN 包。
             * 内核收到 FIN 包后，会认为这是一个“读事件”，并立刻唤醒 select！
             * 此时 recv 被调用去读取数据，但是因为连接已断开，recv 会返回 0 (代表EOF)。
             * 本版代码没有判断 cnt == 0 的情况。于是循环继续，select 发现对方处于断开状态，
             * 认为依然“可读(读到EOF)”，瞬间再次唤醒，recv 再返回 0，导致死循环！
             */
        }

    }

    //将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;
}

