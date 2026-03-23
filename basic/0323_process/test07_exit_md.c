#include <my_header.h>

void func(){
    
    /*
     * 下面三种“结束进程”的方式很像，但行为并不完全一样：
     *
     * 1) exit(EXIT_SUCCESS);
     *    - 属于标准 C 库函数。
     *    - 表示“正常终止进程”。
     *    - 会先做一系列善后工作，再结束程序：
     *        a. 调用通过 atexit() 注册的退出处理函数；
     *        b. 刷新并关闭标准 I/O 缓冲区（例如 printf 的输出缓冲）；
     *        c. 做 C 运行库层面的清理工作。
     *    - 所以如果前面有 printf("123"); 即使没有 '\n'，
     *      通常也仍然能看到输出，因为缓冲区会被刷新。
     *
     * 2) _exit(EXIT_SUCCESS);
     *    - 属于 POSIX 系统调用风格接口（常用于 Unix/Linux）。
     *    - 会“立刻”结束当前进程，不做 C 库层面的善后：
     *        a. 不调用 atexit() 注册的函数；
     *        b. 不刷新 stdio 缓冲区（printf 的内容可能来不及输出）；
     *        c. 直接把控制权交还给内核。
     *    - 常见使用场景：
     *        fork() 之后子进程执行失败，需要马上退出时；
     *        避免把父进程继承来的缓冲区再次刷新一遍。
     *
     * 3) _Exit(EXIT_SUCCESS);
     *    - 属于标准 C（C99）提供的“立即终止”函数。
     *    - 语义上和 _exit() 很接近：
     *        a. 不调用 atexit() 注册的函数；
     *        b. 不保证刷新 stdio 缓冲区；
     *        c. 立即终止进程。
     *    - 可以理解为：_Exit() 是标准 C 版本的“立刻退出”，
     *      而 _exit() 更偏 POSIX/Unix。
     *
     * 简单记忆：
     *    exit()   = 正常收尾后再退出
     *    _exit()  = 立刻退出（POSIX）
     *    _Exit()  = 立刻退出（标准 C）
     */

    // exit(EXIT_SUCCESS);
    //_exit(EXIT_SUCCESS);
    _Exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){                                  
    
    /*
     * 这里没有写 '\n'，所以 " 123 " 还留在 stdout 缓冲区里。
     *
     * - 如果 func() 里调用的是 exit()：
     *      程序结束前会刷新缓冲区，通常能看到输出： 123
     *
     * - 如果 func() 里调用的是 _exit() 或 _Exit()：
     *      程序会立即终止，stdout 缓冲区可能来不及刷新，
     *      因此有可能什么都看不到。
     *
     * 想稳定看到输出，可以改成：
     *      printf(" 123 \n");
     * 或者加上：
     *      fflush(stdout);
     */

    printf(" 123 ");
    func();

    return 0;
}

