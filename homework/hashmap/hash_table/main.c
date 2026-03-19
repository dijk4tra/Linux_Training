#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "hashmap.h"


void test1() {
	// 哈希表： 根据键来找值。 
	HashMap *map = hashmap_create();

	hashmap_put(map, "zhangsan", "hubei");
	hashmap_put(map, "lisi", "hunan");
	hashmap_put(map, "wangwu", "beijing");
	hashmap_put(map, "sihong", "C9");
	hashmap_put(map, "qiyue", "shanhujian");

	ValueType value = hashmap_get(map, "zhangsan");
	printf("value = %s\n", value);
	printf("value2 = %s\n", hashmap_get(map, "lisi"));
	printf("value3 = %s\n", hashmap_get(map, "sihong"));

	// O(1)
}


int main(void) {

	// 哈希表： 就是一个存储键值对数据的数据结构。 
	// 期望通过键值对这样一种结构，快速的根据键找到值。 

	// 举例： 现在，想根据这个人的手机号，找到这个人的 名字、家庭住址、年龄。 

	// 数组： 遍历。O(n)
	// 链表: 遍历。 O(n)
	// 栈
	// 队列

	// 哈希表： 根据键找值。 期望达到时间复杂度是 O(1) 的速度。 
	// 它为什么能到O(1) 


	test1();

	return 0;
}