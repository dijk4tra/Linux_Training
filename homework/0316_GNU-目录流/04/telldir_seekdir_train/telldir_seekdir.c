#include <my_header.h>

int main(int argc, char *argv[]) {
    // 1. 参数校验：确保用户输入了目录路径（程序名 + 路径，共2个参数）
    ARGS_CHECK(argc, 2);

    // 2. 打开目录流：argv[1] 是用户从命令行传入的目录路径
    DIR *dirp = opendir(argv[1]);
    
    // 检查目录流指针是否成功获取（NULL 表示打开失败，如目录不存在或权限不足）
    ERROR_CHECK(dirp, NULL, "opendir");

    struct dirent *pdirent;
    long loc = 0; // 用于保存 telldir 返回的目录流偏移位置（类似“书签”）

    printf("--- 开始第一次完整遍历 ---\n");
    // 3. 循环读取目录项，直到末尾（readdir 返回 NULL）
    while ((pdirent = readdir(dirp)) != NULL) {
        // 打印当前项的详细信息：inode号、结构体长度、文件类型、文件名
        printf("inode num = %lu, reclen = %hu, type = %u, name = %s\n",
               pdirent->d_ino, 
               pdirent->d_reclen,
               pdirent->d_type, 
               pdirent->d_name);

        // 4. 设置断点位置：如果当前项的文件名是 "file1"
        if (strcmp("file1", pdirent->d_name) == 0) {
            // telldir 返回当前目录流指针相对于起始位置的偏移量
            // 注意：此时指针其实指向的是 file1 的下一个条目
            loc = telldir(dirp);
            printf("[标记成功] 已记录文件 'file1' 之后的位置偏移量: %ld\n", loc);
        }
    }

    printf("--------------------------\n");

    // 5. 恢复指针位置：利用之前存下的 loc，让指针“跳”回去
    // 如果目录里没有 file1，loc 默认为 0，则跳回目录开头
    seekdir(dirp, loc);
    printf("执行 seekdir 成功，指针已回到标记位置。\n");

    // 6. 验证指针：再次读取。此时读到的应是 file1 后面的那一个条目
    pdirent = readdir(dirp);
    if (pdirent != NULL) {
        printf("重新定位后读到的第一个条目是: %s\n", pdirent->d_name);
    } else {
        printf("标记位置之后没有更多文件了。\n");
    }

    // 7. 释放资源：关闭目录流，防止内存泄漏
    closedir(dirp);

    return 0;
}
