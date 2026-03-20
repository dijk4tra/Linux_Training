#include <my_header.h>

int main(int argc, char *argv[])
{
    DIR *dir;                    // 目录流指针
    struct dirent *entry;        // 指向读取到的目录项
    const char *path;            // 要打开的目录路径

    /*
     * 如果命令行传入目录参数，就使用用户指定目录
     * 否则默认查看当前目录 "."
     */
    if (argc < 2)
    {
        path = ".";
    }
    else
    {
        path = argv[1];
    }

    // 打开目录
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir 打开目录失败");
        return 1;
    }

    printf("当前列出目录：%s\n", path);
    printf("--------------------------------------------------\n");

    /*
     * readdir 每调用一次，就读取一个目录项
     * 直到返回 NULL 表示读完
     */
    while ((entry = readdir(dir)) != NULL)
    {
        printf("文件名 d_name      : %s\n", entry->d_name);
        printf("inode号 d_ino      : %lu\n", (unsigned long)entry->d_ino);
        printf("偏移量 d_off       : %ld\n", (long)entry->d_off);
        printf("记录长度 d_reclen  : %hu\n", entry->d_reclen);
        printf("文件类型 d_type    : %u\n", entry->d_type);
        printf("--------------------------------------------------\n");
    }

    // 关闭目录
    if (closedir(dir) == -1)
    {
        perror("closedir 关闭目录失败");
        return 1;
    }

    return 0;
}
