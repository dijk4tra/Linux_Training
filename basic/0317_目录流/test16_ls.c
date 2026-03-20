#include <my_header.h>
#include <bits/types/stack_t.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 我们可以使用目录流，获取目录中的子项
    // 可以提供：inode name type
    
    ARGS_CHECK(argc, 2);
    DIR *dirp = opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir failed");
    
    // 切换工作目录为参数传入的目录，不然输入上级路径时会报错
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret, -1, "chdir failed");
    // 绝对路径和相对路径都要掌握，这里是相对路径写法

    struct dirent *dirent_p = NULL;
    struct stat *stat_p = (struct stat *)calloc(1, sizeof(struct stat));
    while((dirent_p = readdir(dirp)) != NULL){
        // printf("%s------\n", dirent_p->d_name);
        // 可以将获取到的name，塞进stat，用于获取详细信息
        int ret = stat(dirent_p->d_name, stat_p);
        ERROR_CHECK(ret, -1, "stat failed");

        // stat_p中有绝大多数信息，就是缺名字
        // dirent结构体中有名字

        printf("%o %ld %d %d %ld %ld %s\n",
               stat_p->st_mode, stat_p->st_nlink,
               stat_p->st_uid, stat_p->st_gid,
               stat_p->st_size, stat_p->st_mtime,
               dirent_p->d_name
               );
    }
    
    closedir(dirp);

    return 0;
}

