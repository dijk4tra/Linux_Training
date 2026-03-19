#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "dynamic_hashmap.h" 

void test1() {
    printf("--- 开始 Test 1 ---\n");
    // 注意：类型已修改为 DynamicHashMap
    DynamicHashMap *map = hashmap_create();

    // 存入多组人员年龄数据
    // 初始容量是 10，这里存入 7 个数据，7/10 = 0.7，暂时不会触发扩容 (阈值 0.75)
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

    // 记得销毁释放内存，养成良好习惯
    hashmap_destroy(map);
}

void test2() {
    printf("\n--- 开始 Test 2 ---\n");
    DynamicHashMap *map = hashmap_create();

    // 存入数据
    hashmap_put(map, "zhangsan", "28");
    hashmap_put(map, "lisi", "20");
    hashmap_put(map, "wangwu", "16");
    hashmap_put(map, "sihong", "17");
    hashmap_put(map, "qiyue", "18");
    hashmap_put(map, "liwang", "18");
    hashmap_put(map, "lirui", "16");

    // 此时再插入一个元素，达到 8 个，8/10 = 0.8 > 0.75，将触发自动扩容机制！
    hashmap_put(map, "kuorong", "99");
    printf("测试扩容后获取元素 kuorong = %s\n", hashmap_get(map, "kuorong"));

    // 测试查询不存在的键
    printf("kongling = %s\n", hashmap_get(map, "kongling")); // 预期输出 (null)

    // 判断返回值是否为 NULL，这是标准的判断 key 不存在的方式
    printf("kongling 存在状态(0存在, 1不存在) = %d\n", hashmap_get(map, "kongling") == NULL); // 预期输出 1 (true)

    // 测试删除前的状态
    printf("lisi (删除前) = %s\n", hashmap_get(map, "lisi")); // 预期输出 20

    // 删除键为 "lisi" 的元素
    hashmap_remove(map, "lisi");

    // 再次查询已被删除的键，预期应该返回 NULL 
    printf("lisi (删除后) = %s\n", hashmap_get(map, "lisi")); // 预期输出 (null)

    hashmap_destroy(map);
}

int main(void) {
    test1();
    test2();

    return 0;
}