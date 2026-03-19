#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

// 哈希表中，基本结构是什么？
// 数组+链表
typedef char *KeyType;
typedef char *ValueType;

typedef struct node_s {
	KeyType key;
	ValueType value;
	struct node_s *next;
}KeyValueNode;

typedef struct {
	// KeyValueNode 到底是什么东西？  装数据的。  
	//KeyValueNode table[10];
	KeyValueNode *table[10];
	int length; // 数组的长度
	int size; // 键值对的个数。 
	uint32_t seed;
}HashMap;

// 函数声明。 
HashMap *hashmap_create();
void hashmap_destory(HashMap *map);
void hashmap_print(HashMap *map);

// 增删改查。  增、改在一个函数。 
// NULL 表明是新增； 其他值表明是修改。 
ValueType hashmap_put(HashMap *map, KeyType key, ValueType value);

ValueType hashmap_get(HashMap *map, KeyType key);
ValueType hashmap_remove(HashMap *map, KeyType key);

#endif // !HASHMAP_H
