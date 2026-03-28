#include <my_header.h>

/*
 *通过sigprocmask阻塞2号信号，睡眠5秒后，解除阻塞，2号信号得到执行；
 *在睡眠后，解除阻塞之前，通过sigpending检测是否有信号挂起。
 */

void func(int num){
    printf("\n[进入函数] 正在处理信号: %d\n", num);
    printf("[退出函数] 信号 %d 处理完毕\n", num);
}


int main(int argc, char *argv[]){                                  
   
    // 1. 注册 2 号信号 (SIGINT, 按 Ctrl+C 触发) 的处理动作
    // signal 是简化版的信号注册函数
    signal(2, func);
    
    // 2. 准备信号集
    sigset_t set, oldset;
    sigemptyset(&set);   // 清空信号集
    sigaddset(&set, 2);  // 将 2 号信号添加到信号集中
    
    // 全局屏蔽2号信号(sigprocmask函数本身不是阻塞函数)
    /*
     * 3. 设置信号屏蔽 (sigprocmask)
     * SIG_BLOCK: 将 set 中的信号添加到进程当前的屏蔽掩码中。
     * 运行此行后，即便按下 Ctrl+C，信号也会被阻塞在“未决”状态，不会触发 func。
     * oldset: 用于保存修改之前的屏蔽状态（方便以后恢复）。
     */
    printf("--- 信号屏蔽已开启，现在按 Ctrl+C 将不会立即响应 ---\n");
    sigprocmask(SIG_BLOCK, &set, &oldset);

    // 睡5秒
    printf("程序开始休眠5秒...\n");
    sleep(5);
    printf("休眠结束.\n");
    
    // 在信号屏蔽解除前检测未决信号集
    sleep(5);

    sigset_t pending_set;
    sigemptyset(&pending_set);
    // sigpending 函数的作用：
    // 获取当前进程中由于被阻塞（Masked）而无法递送的信号集。
    // 如果某个信号产生了但被屏蔽了，它就会被放入 set 中。
    sigpending(&pending_set);
    // NSIG 是系统定义的信号最大值
    bool is_pending_set_empty = true;
    for (int i = 1; i < NSIG; ++i) {
        if (sigismember(&pending_set, i)) {
            printf("检测结果：%d号信号(SIGQUIT)目前正处于[未决信号集]中(已被屏蔽)\n", i);
            is_pending_set_empty = false;
        }
    }

    if(is_pending_set_empty){
        printf("检测结果: 没有信号处于[未决信号集]中");
    }

    // 解除对2号信号的屏蔽
    /*
     * 4. 解除信号屏蔽
     * SIG_UNBLOCK: 从进程当前的屏蔽掩码中移除 set 中的信号。
     * 如果在休眠的 5 秒内你按下了 Ctrl+C，
     * 信号会被解除屏蔽并立即“递送”，导致 func 被调用。
     */
    printf("--- 信号屏蔽已解除 ---\n");
    sigprocmask(SIG_UNBLOCK, &set, &oldset);

    printf("主程序进入死循环，现在按 Ctrl+C 将直接触发信号处理函数...\n");
    printf("while begin\n");
    while(1){
        pause(); // 挂起等待信号
    }

    return 0;
}

