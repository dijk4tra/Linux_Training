#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/*
模拟网盘服务器记录配置信息。
编写一个程序，将服务器的 IP 地址（字符串）和端口号（整数）格式化写入 `config.txt` 文件，然后再从该文件中读取并打印出来。
要求：
    1. 使用 `fprintf()` 将数据格式化写入文件。
    2. 使用 `fscanf()` 从文件中还原数据。
    3. 输出读取到的配置信息，格式为："加载配置: IP=X, Port=Y"。
*/

int main(void) {
    // 定义要写入的数据
    char write_ip[] = "192.168.1.100";
    int write_port = 8080;

    // 定义用于接收读取数据的变量
    char read_ip[50];
    int read_port;

    // 1.使用 `fprintf()` 将数据格式化写入文件
    FILE *fp_write = fopen("config.txt", "w");
    if (fp_write == NULL) {
        printf("fopen config.txt failed.");
        return 1;
    }

    // 使用 fprintf 将数据格式化写入文件
    // 格式为 "IP=xxx Port=xxx" 
    fprintf(fp_write, "IP=%s Port=%d", write_ip, write_port);

    fclose(fp_write);
    printf("配置信息已成功写入 config.txt\n");

    // 2.使用 `fscanf()` 从文件中还原数据
    FILE *fp_read = fopen("config.txt", "r");
    if (fp_read == NULL) {
        printf("fopen config.txt failed.");
        return 1;
    }
    // 使用 fscanf 还原数据
    // 注意：格式字符串必须与写入时的格式 "IP=%s Port=%d" 严格匹配
    // 这样 fscanf 就能跳过 "IP=" 这个前缀，直接把后面的值读入变量
    fscanf(fp_read, "IP=%s Port=%d", read_ip, &read_port);
    fclose(fp_read);
    
    // 3.输出读取到的配置信息，格式为："加载配置: IP=X, Port=Y"
    printf("加载配置: IP=%s, Port=%d\n", read_ip, read_port);

    return 0;
}