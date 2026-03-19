#ifndef HASHMAP_H
#define HASHMAP_H

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
#include <stdint.h>   // 用于定义固定大小的整数类型，如 uint32_t
#include <time.h>     // 用于获取时间，作为哈希种子的生成依据

// 宏定义：哈希表底层数组的初始容量
// 当存入的数据变多时，理想情况下应该进行扩容（这里暂定为固定大小 10）
#define HASHMAP_INIT_CAPACITY 10

// 类型别名定义：提高代码的可读性和可维护性
// 如果以后想把 Key 或 Value 改成 int 类型，只需要在这里修改即可，不用改动全局代码
typedef char *KeyType;
typedef char *ValueType;

// 定义哈希表中“链表结点”的结构体
// 因为哈希冲突时会产生多个数据映射到同一个数组下标，我们需要用链表把它们串起来
typedef struct node_s {
	KeyType key;			// 键（用于查找）
	ValueType value;		// 值（实际存储的数据）
	struct node_s *next;    // 指向下一个结点的指针（解决哈希冲突）
}KeyValueNode;

// 定义哈希表本身的结构体
// 哈希表：数组 + 链表
typedef struct {
	// 以下两种数组要会区分和使用
	// KeyValueNode table1[10];   // 结构体数组（不推荐）：无论是否有数据，都会占用10个结点的完整内存
	// KeyValueNode *table2[10];  // 指针数组，装指针的数组（推荐写法）：只存了 10 个指针，没有数据时指针全是 NULL，更加节省内存

	// Node是装数据的，如果在HashMap中，上来就有一个数组，表明有十个数据
	// 如果是用指针数组的语法，指针可以全是NULL，这时候表明哈希表中没有数据
	KeyValueNode *table[HASHMAP_INIT_CAPACITY];
	int length;		 // 哈希表底层数组的长度（当前为10）
	int size;		 // 哈希表中实际存储的键值对元素个数，一个键值对算一个数据
	uint32_t seed;   // 随机种子，用于让哈希函数的计算结果更加随机，减少冲突
}HashMap;

// 函数声明
// 基本操作：哈希表的创建、销毁、打印
HashMap *hashmap_create();             // 创建并初始化哈希表
void hashmap_destory(HashMap *map);    // 销毁哈希表，释放内存
void hashmap_print(HashMap *map);      // 打印哈希表的内容

// 增删改查
// 增和改的逻辑是写在一起的，对使用会友好一点
// 先判断是否存在这个键
// if(hash_contains(map, "key")){
//     hashmap_update(.....);
// }else{
//     hashmap_add(.....);
// }

// 向哈希表中存入键值对。如果 key 已经存在，则更新 value，并返回被覆盖的旧值；如果是新 key，则返回 NULL
ValueType hashmap_put(HashMap *map, KeyType key, ValueType value);

// 根据 key 从哈希表中移除对应的键值对。如果存在，返回被删除的 value；如果不存在，返回 NULL
ValueType hashmap_remove(HashMap *map, KeyType key);

// 根据 key 查找对应的 value。如果不存在，返回 NULL
ValueType hashmap_get(HashMap *map, KeyType key);

#endif // !HASHMAP_H