#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    pid_t ret = fork();
    
    if (ret < 0) {
        /*
         * ret < 0 表示 fork() 失败
         * 常见原因可能是：
         * - 系统进程数达到上限
         * - 内存资源不足
         * - 用户可创建的进程数受限
         */
        perror("fork failed");
        return -1;
    }

    if(ret == 0){
        // 当前代码运行在子进程中
        printf("child process.\n");
        // 为了保证shell调用和execl调用的一致性, 将execl的第一个参数, 写为path
        /* execl("./test6_calculate", "6", "7", (void *)0); */ //错误写法
        // 在shell中直接调用目标程序: ./test6_calculate 8 9
        // 在代码中使用execl调用目标程序
        execl("./test6_calculate", "./test6_calculate", "8", "7", (void *)0);
        /*
         * exec 系列函数的作用：
         * 用“一个新程序”替换“当前进程”的内容。
         *
         * 注意：
         * execl() 调用成功后，当前这段代码后面的语句就不会再执行了，
         * 因为子进程已经变成了另一个程序（这里是 ./test6_calculate）。
         *
         * ----------------------------------------------------------------
         * execl() 函数原型（常见形式）：
         * int execl(const char *path, const char *arg, ..., NULL);
         *
         * 各参数含义：
         *
         * 1) 第一个参数 path
         *    表示“要执行的程序文件路径”
         *    这里写 "./test6_calculate"
         *    表示执行当前目录下的 test6_calculate 程序。
         *
         * 2) 从第二个参数开始
         *    是传递给新程序的命令行参数，也就是新程序的 argv[0]、argv[1]、argv[2] ...
         *
         *    特别重要：
         *    第二个参数通常会作为新程序的 argv[0]
         *    按惯例，argv[0] 一般写成程序名或程序路径本身。
         *
         *    所以这里：
         *    execl("./test6_calculate", "./test6_calculate", "8", "7", NULL);
         *
         *    对新程序来说，相当于在 shell 中执行：
         *    ./test6_calculate 8 7
         *
         *    即新程序接收到的参数大致是：
         *    argv[0] = "./test6_calculate"
         *    argv[1] = "8"
         *    argv[2] = "7"
         *
         * 3) 最后一个参数必须是 NULL
         *    用来表示参数列表结束。
         *
         * ----------------------------------------------------------------
         * 为什么下面这种写法是错误的？
         *
         * execl("./test6_calculate", "6", "7", (void *)0);
         *
         * 错误原因不是“语法一定错”，而是“参数含义不符合预期”：
         * - path = "./test6_calculate"   // 这个没问题
         * - argv[0] = "6"
         * - argv[1] = "7"
         *
         * 这样新程序看到的“程序名”会变成 "6"，
         * 也就是说它接收到的参数更像是：
         * argv[0] = "6"
         * argv[1] = "7"
         *
         * 如果你的程序内部依赖 argv[0] 作为程序名，或者你想保持和 shell 调用习惯一致，
         * 这种写法就不合适。
         *
         * 所以更规范的写法是：
         * 把第二个参数写成程序名（或程序路径）本身。
         */

        /*
         * 如果 execl() 执行失败，才会继续往下执行。
         * 比如：
         * - 文件不存在
         * - 没有执行权限
         * - 路径写错
         */
        perror("execl failed");

    }else{
        // 当前代码运行在父进程中
        printf("father process begin.\n");
        sleep(20);
        printf("father process over.\n");
    }

    return 0;
}

