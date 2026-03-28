#include <my_header.h>

/* 
 *使用sigaction注册2号信号的处理函数
 * 1)允许自动重启低速系统调用;
 * 2)在处理2号信号时阻塞3号信号,不会被3号信号打断;
 * 3)在2号信号的信号处理函数中,使用sigpending函数判断一下有没有3号信号处于未决状态;
 */

void func(int num){
    printf("\n[进入函数] 正在处理信号 : %d\n", num);
    printf("--- sleep 开始 (10秒)，此时信号 3 会被屏蔽 ---\n");
    
    // 模拟一段耗时操作
    // 在这 10 秒内，即使你按了 Ctrl+\ (信号3)，程序也不会立刻处理
    sleep(10); 
        
    sigset_t set;
    sigemptyset(&set); //初始化/清空信号集
    
    // sigpending 函数的作用：
    // 获取当前进程中由于被阻塞（Masked）而无法递送的信号集。
    // 如果某个信号产生了但被屏蔽了，它就会被放入 set 中。
    sigpending(&set);
    
    // 使用sigismember检测3号信号 (SIGQUIT) 是否在当前的未决信号集中
    if(sigismember(&set, 3)){
        printf("检测结果：3号信号(SIGQUIT)目前正处于【未决信号集】中(已被屏蔽)\n");
    }else{
        printf("检测结果：3号信号不在未决信号集中\n");
    }

    printf("--- sleep 结束 ---\n");
    printf("[退出函数] 信号 %d 处理完毕\n", num);
}

int main(int argc, char *argv[]){                                  
    
    struct sigaction act, oldact;
    //初始化结构体
    memset(&act, 0, sizeof(act));
    //设置信号处理函数
    act.sa_handler = func;
    //设置标志位为 SA_RESTART,作用是自动重启被打断的系统调用
    act.sa_flags = SA_RESTART;
    /* 
     * 3. 设置 sa_mask（信号屏蔽掩码）
     * 
     * 原理：
     * 当信号 2 (SIGINT) 被触发并进入 func 函数时，
     * 内核会自动将 act.sa_mask 中包含的信号加入到系统的“信号屏蔽字”中。
     * 
     * 效果：
     * 在 func 运行期间，如果你发送了信号 3 (SIGQUIT)，
     * 该信号会被阻塞（Pending 状态），不会打断 func 的执行。
     * 当 func 执行结束返回后，内核会自动解除屏蔽，刚才“攒着”的信号 3 会被立刻递送。
     */
    sigemptyset(&act.sa_mask);  //清空信号集,防止有随机值
    sigaddset(&act.sa_mask, 3); //将3号信号(SIGQUIT,快捷键Ctrl+\)加入该集合   

    // 注册信号2(SIGINT / Ctrl+C)
    // 注意: sa_mask只在处理信号2的过程中生效！
    sigaction(2, &act, &oldact);
    
    printf("进程已启动 (PID: %d)。\n", getpid());
    printf("请输入一些文字(此时按下 Ctrl+C 观察 read 是否会被强制结束)：\n");
    printf("1. 按下 Ctrl+C (信号2)，进入函数。\n");
    printf("2. 在 10秒内，按下 Ctrl+\\ (信号3)。\n");
    printf("3. 观察现象：信号 3 不会立刻被触发，而是等 func 结束后才打印 '退出确认'。\n");
    
    
    // 这是一个阻塞式的低速系统调用
    // STDIN_FILENO 是标准输入（键盘）的文件描述符
    char buf[100] = {0};
    ssize_t bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
    
    if (bytes_read == -1) {
        // 如果没有 SA_RESTART，Ctrl+C 会导致代码运行到这里
        perror("read 失败");
    } else {
        printf("read 成功结束，读取到了: %s", buf);
    }

    printf("read over\n");

    return 0;
}

