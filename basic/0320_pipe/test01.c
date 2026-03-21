#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // Linux中万物皆文件。
    // read 读文件。
    // int fd = open("1.txt", O_RDONLY);

    // STDIN_FILENO
    char buf[100] = {0};
    ssize_t ret = read(0, buf, sizeof(buf));

    // 可以使用read 来读取键盘输入。
    printf("read_count = %d\n", ret);
    printf("input_msg = %s\n", buf);
    
    return 0;
}

