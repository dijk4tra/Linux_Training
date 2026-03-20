#include <my_header.h>
#include <sys/mman.h>
// 单次最大拷贝大小：16MB
#define ONCE_MAX_COPY_SIZE (1024 * 1024 * 16)

int main(int argc, char *argv[]){                                  
    
    int src_fd = open(argv[1], O_RDONLY);
    int dest_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    ERROR_CHECK(src_fd, -1, "open src failed");
    ERROR_CHECK(dest_fd, -1, "open dest failed");
    
    // 获取源文件信息
    struct stat stat_buf;
    fstat(src_fd, &stat_buf);
    // 源文件总大小(单位：字节)
    long src_total_size = stat_buf.st_size;
    // 将目标文件扩展到和源文件一样大(为mmap做准备)
    ftruncate(dest_fd, src_total_size);
    // 已拷贝的字节数
    long copied_size = 0;
    // 循环拷贝，直到全部完成
    while(copied_size < src_total_size){

        // 当前这一轮要拷贝的大小
        // 如果剩余大小 > 16MB，就拷16MB，否则拷剩余部分
        int cur_copy_size = (src_total_size - copied_size) > ONCE_MAX_COPY_SIZE
                            ? ONCE_MAX_COPY_SIZE 
                            : (src_total_size - copied_size);

        // ===================== 映射源文件 =====================
        // 从文件 offset = copied_size 开始映射 cur_copy_size 大小
        // PROT_READ：只读权限
        // MAP_SHARED：修改会同步到文件
        char *src_p = (char *)mmap(NULL, cur_copy_size, PROT_READ, MAP_SHARED, src_fd, copied_size);
        ERROR_CHECK(src_p, MAP_FAILED, "mmap src failed");

        // ===================== 映射目标文件 =====================
        // PROT_READ | PROT_WRITE：可读可写
        char *dest_p = (char *)mmap(NULL, cur_copy_size, PROT_READ | PROT_WRITE, MAP_SHARED, dest_fd, copied_size);
        ERROR_CHECK(dest_p, MAP_FAILED, "mmap dest failed");
        
        // ===================== 数据拷贝 =====================
        // 直接内存拷贝（比read/write更高效）
        memcpy(dest_p, src_p, cur_copy_size);

        // ===================== 解除映射 =====================
        munmap(src_p, cur_copy_size);
        munmap(dest_p, cur_copy_size);
        
        // 更新已拷贝大小
        copied_size += cur_copy_size;
    }
    
    // 关闭文件描述符
    close(src_fd);
    close(dest_fd);

    return 0;
}

