#include <my_header.h>

//第五版：I/O 多路复用并发服务器
//特点：
//1.引入了文件描述符数组 arr[1024]，用于动态管理所有已连接的客户端。
//2.listen_fd 永远处于被监听状态，可以随时接纳新客户端的连接。
//3.真正实现了单个线程同时处理多客户端数据的收发（不阻塞、不串行）。

int main(int argc, char *argv[])
{
    char *ip = "192.168.193.128";
    char *port = "12345";
    
    //创建监听套接字
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
    
    /*
     * 【核心知识点：客户端文件描述符管理数组】
     * 为什么是 1024？在 Linux 缺省配置下，一个进程最多能打开 1024 个文件描述符。
     * 这里的 arr 数组就相当于“酒店的客人名册”。
     * 初始化为 0，代表这个房间(槽位)是空的，没人住。
     */
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
    //服务器一启动，就随时准备好听键盘输入
    FD_SET(STDIN_FILENO, &set);

    while(1){
        //tmp/set 双位图技巧，保护持久化位图
        fd_set tmp;
        tmp = set;
        
        //阻塞等待事件，nready 会告诉我们有几个 FD 发生了事件
        int nready = select(10, &tmp, NULL, NULL, NULL);
        ERROR_CHECK(nready, -1, "select");
        printf("nready: %d\n", nready);
        
        // ==================== 事件 1：处理新连接 ====================
        /*
         * 如果 listen_fd 就绪，说明外面有人排队，我们需要接客！
         * 调用accept，从全连接队列中取出一条连接。
         * 与04版最大的不同：这里不再把 listen_fd 从 set 中踢出！
         * 大门永远敞开，随时欢迎新客人。
         */
        if(FD_ISSET(listen_fd, &tmp)){
            conn_fd = accept(listen_fd, NULL, NULL);
            ERROR_CHECK(conn_fd, -1, "accept");
            
            //将新客人的专属套接字加入监听名册
            FD_SET(conn_fd, &set);

            /*
             *【错误修正】
             *允许服务器通过终端输入数据(注意:这个位置存在缺陷)
             *FD_SET(STDIN_FILENO, &set); 
             *这段代码被错误的放在了if(FD_ISSET(listen_fd, &tmp))里面
             *这意味着：服务器刚启动时，敲键盘是没有反应的，必须等第一个
             *客户端连进来，服务器才开始监听键盘。
             */
            
            //在这里使用数组的目的就是因为，如果多个客户端连接上来
            //之后，conn_fd就会发生更改，如果不进行处理，那么之前
            //已连接的conn_fd就无法找到

            /*
             * 【核心逻辑：分配槽位】
             * 遍历名册数组，找出一个值为 0 的空位，把新的 conn_fd 存进去。
             * 这样哪怕有 1000 个客户端连进来，也不会互相覆盖了！
             */
            for(int idx=0; idx<1024; idx++){
                if(0 == arr[idx]){
                    arr[idx] = conn_fd;
                    break; // 找到空位存好后，退出循环
                }
            }
        }
        
        // ==================== 事件 2：处理已连接的客户端业务 ====================
        /*
         * 接下来怎么知道是哪个客户端发了消息呢？
         * 笨方法：把名册(arr)从头到尾查一遍！
         */
        for(int idx=0; idx<1024; idx++){
            //值为 0 说明这个槽位没存客户端，直接跳过，查下一个
            if(0 == arr[idx]){
                continue;
            }
            
            //----------------------
            //场景 A：服务器自己在终端上进行了输入
            if(FD_ISSET(STDIN_FILENO, &tmp)){
                //通过键盘输入数据，然后存放在buf中
                char buf[50] = {0};
                read(STDIN_FILENO, buf, sizeof(buf));
                /*
                 * 【广播效果】
                 * 因为这段代码包在了 arr 的遍历循环里，
                 * 当在服务器敲下回车时，循环会针对每一个连着的 arr[idx] 执行 send！
                 * 这实现了一个特别实用的功能：“服务器向所有已连接客户端群发广播”。
                 */
                //将buf中的数据传输给对端(客户端)
                send(arr[idx], buf, sizeof(buf), 0);
            }

            //----------------------
            //场景 B：第 idx 个客户端向服务器发来了数据
            if(FD_ISSET(arr[idx], &tmp)){
                char buf[50] = {0};
                int cnt = recv(arr[idx], buf, sizeof(buf), 0);
                printf("recv cnt: %d\n", cnt);

                if(0 == cnt){
                    printf("客户端关闭了\n");
                    //1.在 select 的监听位图中，取消关注这个断开的客户端
                    FD_CLR(arr[idx], &set);
                    
                    /*
                     *【错误修正】
                     * 这里不能取消对标准输入的监听!
                     * FD_CLR(STDIN_FILENO, &set);
                     * 假设现在有 A、B 两个客户端连着。A 突然断开了，但
                     * 代码把 STDIN 移除了。结果就是：虽然 B 还在连着，
                     * 但服务器再也不能通过键盘给 B 发送消息了，除非新 
                     * 客户端 C 此时连进来重新触发 FD_SET，把STDIN_FILENO
                     * 重新加入监听位图 set 中.
                     */
                    
                    //2.释放资源(极其关键)
                    //将本连接的文件描述符关掉，并把 arr 里的记录抹除
                    close(arr[idx]);
                    arr[idx] = 0; //特别注意:不能遗漏
                    
                    continue;//可以结束本次循环，后面的打印printf
                             //就不执行了，原因是：连接已经断开
                }
                printf("server recv from client buf : %s\n", buf);
            }
        }

    }

    //关闭文件描述符
    close(listen_fd);
    return 0;


}

