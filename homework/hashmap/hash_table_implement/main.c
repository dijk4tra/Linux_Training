#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "hashmap.h"


void test1() {
    HashMap *map = hashmap_create(); // 创建哈希表

    // 存入多组人员年龄数据
    hashmap_put(map, "zhangsan", "28");
    hashmap_put(map, "lisi", "20");
    hashmap_put(map, "wangwu", "16");
    hashmap_put(map, "sihong", "17");
    hashmap_put(map, "qiyue", "18");
    hashmap_put(map, "liwang", "18");
    hashmap_put(map, "lirui", "16");

    // 验证能否正确打印存入的数据
    printf("zhangsan = %s\n", hashmap_get(map, "zhangsan")); // 预期输出 28
    printf("lisi = %s\n", hashmap_get(map, "lisi"));         // 预期输出 20
    printf("qiyue = %s\n", hashmap_get(map, "qiyue"));       // 预期输出 18
    printf("liwang = %s\n", hashmap_get(map, "liwang"));     // 预期输出 18
    printf("lirui = %s\n", hashmap_get(map, "lirui"));       // 预期输出 16
}


void test2() {
    HashMap *map = hashmap_create();

    // 存入数据
    hashmap_put(map, "zhangsan", "28");
    hashmap_put(map, "lisi", "20");
    hashmap_put(map, "wangwu", "16");
    hashmap_put(map, "sihong", "17");
    hashmap_put(map, "qiyue", "18");
    hashmap_put(map, "liwang", "18");
    hashmap_put(map, "lirui", "16");

    // 测试查询不存在的键
    printf("kongling = %s\n", hashmap_get(map, "kongling"));

    // 判断返回值是否为 NULL，这是标准的判断 key 不存在的方式
    printf("kongling = %d\n", hashmap_get(map, "kongling") == NULL); // 预期输出 1 (true)

    // 测试删除前的状态
    printf("lisi = %s\n", hashmap_get(map, "lisi")); // 预期输出 20

    // 删除键为 "lisi" 的元素
    hashmap_remove(map, "lisi");

    // 再次查询已被删除的键，预期应该返回 NULL 
    printf("lisi = %s\n", hashmap_get(map, "lisi"));
}

int main(void) {

    // test1();
    test2();

    return 0;
}