#include <my_header.h>

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

    printf("server is listening...\n");
    
    /*
     * 知识点：持久化位图 (set) 与 临时位图 (tmp)
     * 在之前的版本中，每次循环都要重新调一遍 FD_SET。
     * 现在定义了一个全局视角的 set，用来保存“服务器当前到底需要关注哪些描述符”。
     */
    int conn_fd = 0; 
    //定义读位图变量
    fd_set set;
    FD_ZERO(&set);
    //最开始（没有客户端连接时），服务器唯一要做的就是等待新客户端上门。
    //所以只需要把迎宾员 (listen_fd) 加入关注列表。
    FD_SET(listen_fd, &set);


    while(1){
        /* 
         * 知识点：select 传入传出参数的保护机制
         * select 执行完后，会把没发生事件的位清零，破坏原有的位图。
         * 所以每次调用 select 前，把持久化位图 set 拷贝给临时位图 tmp，
         * 把 tmp 丢进 select 里去“踩坑”，从而保护了 set 的完整性。
         * 这是教科书级的标准做法！
         */
        fd_set tmp; 
        tmp = set;
        
        //阻塞等待事件发生，此时只监控 tmp 中为 1 的位
        int nready = select(10, &tmp, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready = %d\n", nready);
        
        // ==================== 事件分发处理 ==================== //

        /*
         * 事件源 1：listen_fd 就绪
         * 核心原理：当有客户端完成三次握手，进入全连接队列时，
         * 内核会认为 listen_fd "可读"。
         */
        if(FD_ISSET(listen_fd, &tmp)){
            //此时调用 accept 绝对不会阻塞，因为一定有连接在队列里等着被取走    
            conn_fd = accept(listen_fd, NULL, NULL);
            ERROR_CHECK(conn_fd, -1, "accept");
            //这样连接建立了，后续需要通过文件描述符conn_fd进行收发数据
            //所以把它加入持久化位图 set
            FD_SET(conn_fd, &set);
            //同时也可以让服务器在终端输入数据，将数据发给客户端
            FD_SET(STDIN_FILENO, &set);
            
            /*
             * 【此处是这版代码的“特色设计”，也是只能服务单个客户端的原因】
             * 主动将 listen_fd 从 set 中踢出去了！
             * 意味着：在当前这个客户端没断开之前，收发室大爷(select)不再关注大门(listen_fd)了。
             * 即使外面有新的客户端排队完成了握手，服务器也视而不见，不调用 accept 取出它们。
             */
            //不想监听其他的新的连接，那么就可以将listen_fd从set中清除
            FD_CLR(listen_fd, &set);
        }

        /*
         * 事件源 2：服务器本地终端键盘输入
         */
        if(FD_ISSET(STDIN_FILENO, &tmp)){
            //通过键盘输入数据，然后存放在buf中
            char buf[50] = {0};
            read(STDIN_FILENO, buf, sizeof(buf));
            //这里也可以使用recv(),recv函数前三个参数和read相同
            //第四个参数flag设置为0时,recv和read等价(仅限于socket)

            //需要将buf中的数据传输给对端(客户端)
            send(conn_fd, buf, sizeof(buf), 0);
        }
        
        /*
         * 事件源 3：连接的客户端发来数据或断开
         */
        if(FD_ISSET(conn_fd, &tmp)){
            char buf[50] = {0};
            int cnt = recv(conn_fd, buf, sizeof(buf), 0);
            printf("recv cnt: %d\n", cnt);
            if(0 == cnt){
                printf("客户端已关闭\n");
                //客户端已经关闭了，不再监听该套接字
                FD_CLR(conn_fd, &set);
                //没客户端接收数据了，键盘输入也暂时不用监听了
                FD_CLR(STDIN_FILENO, &set);
                //此时服务器空闲下来了,重新把迎宾员listen_fd加入set
                //继续关注大门,等待下一个连接
                FD_SET(listen_fd, &set);

                //彻底释放旧连接占用的系统资源（挥手告别）
                close(conn_fd);
                
                //结束本次事件处理，重新回到 select 阻塞等待
                continue;//可以结束本次循环，后面的打印printf
                         //就不执行了，原因是：已经断开了，
                         //没有数据了
            }
            //如果 cnt > 0，正常打印收到的数据
            printf("server recv from client buf: %s\n", buf);
        }

    }

    //将所有的文件描述符关闭
    close(listen_fd);
    close(conn_fd);;
    return 0;
}

/*
 *在这一版中，为了避免 conn_fd 这个 int 变量被新连接覆盖，
 *采取了一个防守策略: 当得到一个 conn_fd 后，就干脆屏蔽掉
 *listen_fd, 也就是FD_CLR(listen_fd, &set);
 *
 *这就好比餐厅里只有一个服务员(conn_fd 变量只有一个槽位):
 *1.大门（listen_fd）打开，1号客人进来，服务员去接待
 *2.为了专心接待1号客人，老板直接把大门锁了（FD_CLR）
 *3.这时2号客人在门外排队(三次握手完成，在全连接队列里)，
 *  但是大门关着，进不来
 *4.直到1号客人吃完饭买单走人(cnt == 0)
 *5.此时老板重新把大门打开,FD_SET(listen_fd, &set)
 *6.然后2号客人才能进来被接待
 *
 *这种写法使得服务器变成了一个串行服务器(顺序执行)，而不是并发服务器。
 *
 *如何走向真正的并发？
 *既然问题出在“conn_fd 只有一个槽位，存不下那么多客人”，解决方案自然
 *是用一个数组把所有连进来的 conn_fd 都存起来，然后把它们全部存到位图 
 *set 中，让 select 同时盯着成百上千个客人，谁进来了，就去读谁的数据。
 *
 */
