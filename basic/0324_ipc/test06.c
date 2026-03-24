#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 父子进程之间，想实现有来有往的通信.也就是双向的通信 
    // 现在需要两条管道

    int arr[2] = {0};
    int arr2[2] = {0};

    pipe(arr);
    pipe(arr2);

    printf("%d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    
    return 0;
}

