#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    FILE *fp = fopen("222.txt", "w");
    ERROR_CHECK(fp, NULL, "fopen failed");

    fputs("hello\n", fp);
    fflush(fp);
    // 有缓冲区文件流是一个满缓冲区。缓冲区满时，才会输出到内核
    // printf标准输出，是一个行缓冲区

    write(3, "world", 5);
    // 这里可以证明在Linux中fopen在底层是由系统调用open实现的
    fclose(fp);
    
    return 0;
}

