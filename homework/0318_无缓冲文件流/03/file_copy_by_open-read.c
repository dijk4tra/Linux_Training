#include <my_header.h>

/*
 * 函数功能：
 *     使用无缓冲文件IO复制文件，并且可以指定每次读写的字节数，
 *     以便测试不同缓冲区大小对复制效率的影响。
 *
 * 参数：
 *     src_path   - 源文件路径
 *     dest_path  - 目标文件路径
 *     buf        - 用户提供的缓冲区
 *     buf_count  - 每次读取的字节数
 *
 * 使用函数：
 *     open()/read()/write()/close()
 */
void copy_file_by_path(const char *src_path,
                       const char *dest_path,
                       char *buf,
                       int buf_count){
    // 以只读方式打开源文件
    int src_fd = open(src_path, O_RDONLY);
    if(src_fd == -1){
        perror("open src failed");
        return;
    }
    // 打开目标文件，只写；若文件不存在则创建；若文件存在则清空原有内容
    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(dest_fd == -1){
        perror("open dest failed");
        close(src_fd);
        return;
    }

    ssize_t count = 0;    
    // 循环读取源文件内容
    // read返回值：
    // > 0 : 实际读到的字节数
    // = 0 ：到达文件末尾
    // < 0 : 读取失败
    while((count = read(src_fd, buf, buf_count)) > 0){
        // 将本次读到的count个字节写入目标文件
        // 注意：因为最后一次读取到的字节数可能小于buf_count
        // 所以write中写入字符数应该写count，即每次实际读到的字符数
        // 不能又写buf_count，不然这会把buf中的废数据也写入
        ssize_t write_bytes = write(dest_fd, buf, count);
        if(write_bytes == -1){
            perror("write file failed");
            close(src_fd);
            close(dest_fd);
            return;
        }
    }
    // 如果read返回-1，说明读取出错
    if(count == -1){
        perror("read file failed");
    }
    
    // 关闭文件标识符，释放内核资源
    close(src_fd);
    close(dest_fd);
}


/*
 * 函数功能：
 *     获取当前时间的毫秒值，用于统计程序运行耗时
 *
 * 返回值：
 *     当前时间对应的毫秒数
 */
long long current_time_millis(void){
    struct timespec ts;
    // 获取当前系统时间
    clock_gettime(CLOCK_REALTIME, &ts);
    // 将“秒+纳秒”转换为毫秒
    return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}


int main(int argc, char *argv[]){                                  
    // 参数数量检查，argv[1]是源文件路径，argv[2]是目标文件路径
    ARGS_CHECK(argc, 3);
    // 申请一块足够大的缓冲区
    // 因为后面要测试 1000 和 4096 两种块大小，所有申请的缓冲区大于4096即可
    char *buf = (char *)malloc(10000);
    ERROR_CHECK(buf, NULL, "malloc failed");

    // 第一次测试开始时间
    long long time1 = current_time_millis();

    // 使用 1000 字节作为每次读写的大小进行复制
    copy_file_by_path(argv[1], argv[2], buf, 1000);

    // 第一次测试结束时间 / 第二次测试开始前时间
    long long time2 = current_time_millis();

    // 使用 4096 字节作为每次读写的大小进行复制
    copy_file_by_path(argv[1], argv[2], buf, 4096);

    // 第二次测试结束时间
    long long time3 = current_time_millis();

    // 输出两次实验的耗时
    printf("time2 - time1 = %lld\n", time2 - time1);
    printf("time3 - time2 = %lld\n", time3 - time2);

    // 释放堆内存
    free(buf);
    
    return 0;
}

