#ifndef DYNAMIC_HASHMAP_H
#define DYNAMIC_HASHMAP_H

// 头文件中的内容：
// 结构体、类型别名、函数声明、宏定义
// 哈希表的头文件中会存储哈希表相关的内容

// 哈希表中，存储的数据是键值对
// 哈希表的基本结构，是数组+链表
// 哈希表中无法存储重复的key

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// 初始哈希桶（数组）的默认容量
#define HASHMAP_INIT_CAPACITY 10
// 负载因子阈值：当 (当前元素数量 / 数组容量) > 0.75 时，触发扩容
#define LOAD_FACTOR_THRESHOLD 0.75f

// 定义键和值的类型，方便后续如果要修改为其他类型时，只需在这里改动即可
typedef char *KeyType;
typedef char *ValueType;

// 哈希表链表节点的结构体定义
typedef struct node_s {
	KeyType key;
	ValueType value;
	struct node_s *next;
}KeyValueNode;

// 动态哈希表结构体定义：数组 + 链表
typedef struct {
	// int * -->指向：int数组   int，int，int，int，int
	// int ** --> int *，int*，int*，int*，int*
	// KeyValueNode ** --> KeyValueNode *，KeyValueNode *，KeyValueNode *，KeyValueNode *，KeyValueNode *

	// buckets 是一个二级指针，即“指向指针的指针”。
	// 它可以被看作是一个动态分配的数组，数组里的每一个元素都是一个 KeyValueNode*（链表头指针）。
	// 如果某个位置没有数据，该位置的指针就是 NULL。
	KeyValueNode **buckets; // 当前哈希桶（数组）的容量大小
	int capacity;    // 当前哈希桶（数组）的容量大小
	int size;		 // 当前哈希表中实际存储的键值对总数量
	uint32_t seed;   // 哈希算法的随机种子，用于增加哈希计算的随机性，减少冲突
}DynamicHashMap;

// 函数声明
// 创建并初始化哈希表
DynamicHashMap *hashmap_create();

// 彻底销毁哈希表，释放所有内存
void hashmap_destroy(DynamicHashMap *map);

// 增删改查操作
// 存入键值对。如果 key 不存在，则新增；如果 key 已存在，则更新 value 并返回旧的 value。
ValueType hashmap_put(DynamicHashMap *map, KeyType key, ValueType value);

// 删除指定 key 的键值对。如果删除成功，返回被删除的 value；如果不存在，返回 NULL。
ValueType hashmap_remove(DynamicHashMap *map, KeyType key);

// 查找指定 key 的 value。如果存在返回对应的 value，不存在则返回 NULL。
ValueType hashmap_get(DynamicHashMap *map, KeyType key);

#endif // !DYNAMIC_HASHMAP_H
