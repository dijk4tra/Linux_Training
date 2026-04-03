#include <my_header.h>

int main(int argc, char *argv[]){                                  
    
    // 0x75767778 是一个 32 位（4字节）的十六进制整数
    // 它可以拆分为 4 个字节：0x75 (高位字节 MSB), 0x76, 0x77, 0x78 (低位字节 LSB)
    int num = 0x75767778; //16进制

    // &num 获取整型变量的首地址（也就是最低的内存地址）
    // (char *) 将 int 类型的指针强制转换为 char 类型的指针
    // 这样 ptmp 就只指向了这 4 个字节中的第一个字节（地址最低的那个字节）
    char *ptmp = (char *)&num;

    //0x78 = 7*16 + 8 = 112 + 8 = 120 对应ASCII字符 x
    //0x75 = 7*16 + 5 = 112 + 5 = 117 对应ASCII字符 u
    
    // 如果打印出 'x' (0x78)：说明低位字节存放在了低地址处，这是【小端序 (Little-Endian)】
    // 修正你的疑问：如果打印出 'u' (0x75)：说明高位字节存放在了低地址处，这是【大端序 (Big-Endian)】
    printf("ptmp: %c\n", *ptmp);

    // htonl = Host TO Network Long
    // 将主机字节序转换为网络字节序。网络字节序强制规定使用【大端序】
    int a = htonl(num);

    // 同样的方法，用 char 指针探测转换后的第一个字节（低地址处）
    char *pa = (char *)&a;
    // 因为 a 已经是网络字节序（大端序），所以低地址一定会存放高位字节 0x75 ('u')
    // 无论你在什么架构的电脑上运行，这里都会打印出 'u'
    printf("pa: %c\n", *pa);
    
    return 0;
}

