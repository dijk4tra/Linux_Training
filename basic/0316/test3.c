#include <my_header.h>

void copy_file_by_path(char *src_path, char *dest_path){
    FILE *src_fp = fopen(src_path, "r");
    if (src_fp == NULL){
        printf("fopen src failed.");
        return;
    }

    FILE *dest_fp = fopen(dest_path, "w");
    if(dest_fp == NULL){
        printf("fopen dest failed.");
        fclose(src_fp);
        return;
    }
    
    int ch;
    while((ch = fgetc(src_fp)) != EOF){
        fgetc(dest_fp);
    }
    
    fclose(src_fp);
    fclose(dest_fp);

}

int main(int argc, char *argv[])
{   
    // 需要完成文件的复制，是外面给的路径，然后用于参数
    // ./test3_copy 111.txt 111_copy.txt
    // argv[1]  argv[2]
    
    /* copy_file_by_path("1.txt", "1_copy.txt"); */
    /* argv[1]; argv[2] */

    copy_file_by_path(argv[1], argv[2]);

    return 0;
}

