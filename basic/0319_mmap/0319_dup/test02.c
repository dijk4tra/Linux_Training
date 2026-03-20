#include <my_header.h>

int main(int argc, char *argv[]){

    // 文件也有缓冲区。 文件的缓冲区是满缓冲区。 
    FILE *fp = fopen("1.txt", "w");
    ERROR_CHECK(fp, NULL, "fopen failed");

    fputc('a', fp);
    fputc('b', fp);

    close(3);


    return 0;
}
