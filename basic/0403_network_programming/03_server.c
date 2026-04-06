#include <my_header.h>

//第三版：处理对端断开，但暴露了单任务架构的缺陷
//特点：加入了对 recv == 0 的判断，解决了死循环问题。
//但服务器只能服务一个客户端，且该客户端断开后，服务器程序也会随之终止。

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";

    //创建套接字
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
    
    /* 
     * 【此处是导致本版代码无法服务多个客户端的根本原因】
     * 架构缺陷：accept 被放在了 while(1) 循环的外部！
     * 这意味着，服务器的生命周期内，只能从全连接队列里取出一个连接（创建一个conn_fd）。
     * 即使有第二个客户端连上来（底层握手已完成，存在队列里），由于代码再也不会执行accept，
     * 服务器就不会为第二个客户端生成专门的通信套接字，因此无法与第二个客户端通信。
     */

    /* accept(listen_fd, NULL, NULL);//不想知晓客户端的ip与端口号 */
    //从全连接队列中取出一条连接，并且返回一个用于通信的文件描述符
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &len);
    ERROR_CHECK(conn_fd, -1, "accept");
    
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);

    while(1){
        //将标准输入与用于通信的文件描述符conn_fd添加至位图中
        //每次循环重新设置要监听的文件描述符
        FD_SET(STDIN_FILENO, &set);
        FD_SET(conn_fd, &set);

        int nready = select(10, &set, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);
        
        //场景 A：服务器端从键盘输入数据
        if(FD_ISSET(STDIN_FILENO, &set)){
            //通过键盘输入数据，然后存放在buf中
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价

            //需要将buf中的数据传输给对端(客户端)
            send(conn_fd, buf, sizeof(buf), 0);
        }
        
        //场景 B：套接字可读(可能是对方发来数据，也可能是对方断开连接)
        if(FD_ISSET(conn_fd, &set)){
            char buf[50] = {0};
            /*
             * 知识点：recv 的返回值 (极其重要)
             * > 0: 成功接收到的字节数。
             * = 0: 对端正常关闭了连接（接收到了 TCP 的 FIN 报文），也就是 End-Of-File (EOF)。
             * < 0: 发生了异常或错误（例如非阻塞模式下没有数据可读）。
             */
            int cnt = recv(conn_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            if(0 == cnt){
                printf("客户端已关闭\n");
                break;
                /*
                 * 【此处是导致服务器挂掉的直接原因】
                 * 遇到客户端断开，直接 break 跳出了 while(1) 主循环！
                 * 跳出循环后，代码直接执行到最后的 close(listen_fd) 和 return 0，
                 * 导致整个服务器进程结束。
                 * 正常的服务器逻辑应该是：关闭与这个断开客户端的 conn_fd，
                 * 然后继续留在循环里，等待并处理其他客户端的连接或消息。
                 */
            }
            printf("server recv from client buf: %s\n", buf);
        }

    }

    //将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;
}

/*
 *原理补充：TCP全连接队列（Accept Queue）
 *1.当客户端调用 connect() 时，客户端与服务器的系统内核开始进行三次握手。
 *2.握手完成后，内核会将这个连接放入一个叫做全连接队列（Accept Queue）的
 *  结构中。这就好比客人进了餐厅，坐在了等候区。
 *3.应用层的程序必须要调用 accept() 函数，才能把连接从队列里取出来，并
 *  获得一个用于通信的 conn_fd。这相当于服务员把客人领到桌子旁开始服务。
 *4.在03版的代码中，accept() 只在程序启动时执行了一次。所以第二个客户端
 *  连上来时，虽然内核帮它完成了握手（客户端的 connect 会返回成功），
 *  但它永远坐在“等候区”，服务器端没有新的 conn_fd 去收发它的数据。
 *
 *如何解决这个问题？
 *  既然必须要调 accept 才能处理新客户端，而我们的程序大部分时间都阻塞在
 *select 上等数据，那我们该什么时候调用 accept 呢？
 *  答案是：把迎宾员（listen_fd）也交给收发室大爷（select）去监听!
 *  当有新客户端连上来时，listen_fd 就会发生“可读事件”。此时 select 被唤醒，
 *我们再去调用 accept，这才是异步事件驱动架构(Event-Driven Architecture)的雏形。
 *
 */

