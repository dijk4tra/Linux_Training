#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    printf("open before.\n");
    // 可以直接以读写模式开文件，这时候不会卡。
    int fd = open("1.pipe", O_RDWR);
    printf("open after.\n");

    // 管道是一个半双工的。 但是我们将其当做单工使用。
    // 单工  半双工 双工
    // 单工：信息的传递是单向的，不能来回发消息。
    // 半双工： 信息传递可以双向，但是不能同时。
    // 双工： 信息可以双向，可以同时发送。
    // 一根管子，既可以用来读，也可以用来写。

    // read write: 一定不能先read，再write.因为读空阻，所以，会出现程序卡死
    // write read： 数据会先存储到管道中。 read，就能读出来数据
    // read
    // write

    char msg[] = "mssssssssg from test8888888";
    write(fd, msg, sizeof(msg));

    char buf[100] = {0};
    ssize_t ret = read(fd, buf, sizeof(buf));
    printf("read msg from pipe = %s\n", buf);
    printf("ret = %ld\n", ret);

    close(fd);
    
    return 0;
}

