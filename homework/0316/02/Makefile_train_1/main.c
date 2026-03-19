#include <stdio.h>
#include "calc.h"


int main()
{
    int a = 10;
    int b = 5;

    int sum;
    int diff;

    /* 调用 add.c 中的加法函数 */
    sum = add(a, b);

    /* 调用 subtract.c 中的减法函数 */
    diff = subtract(a, b);

    printf("a = %d, b = %d\n", a, b);
    printf("a + b = %d\n", sum);
    printf("a - b = %d\n", diff);

    return 0;
}
