#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    // 将传入的参数，进行加法计算，然后输出。
    // ./test6_calculate  3 4 
    
    int num1, num2;
    sscanf(argv[1], "%d", &num1);
    sscanf(argv[2], "%d", &num2);

    printf("input num1 = %d, num2 = %d, sum = %d\n", num1, num2, (num1 + num2));
    
    return 0;
}

