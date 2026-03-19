#include "hashmap.h"

HashMap *hashmap_create() {
	HashMap *map = calloc(1, sizeof(HashMap));
	//HashMap *map2 = malloc(sizeof(HashMap));

	// if 内存申请失败。 

	map->length = 10;
	map->seed = time(NULL);

	return map;
}
void hashmap_destory(HashMap *map) {

	for (int i = 0; i < map->length; i++) {
		KeyValueNode *cur = map->table[i];

		while (cur != NULL) {
			KeyValueNode *next = cur->next;
			free(cur);
			cur = next;
		}
	}
	free(map);
}
void hashmap_print(HashMap *map) {
	for (int i = 0; i < map->length; i++) {
		KeyValueNode *cur = map->table[i];

		while (cur != NULL) {
			printf("key=%s, value=%s\n", cur->key, cur->value);
			cur = cur->next;
		}
	}

}

/*
* murmur_hash2
* key： key。需要计算的key，
* len: 键的长度。 如果是字符串，传strlen的结果； 如果是其他的类型， sizeof
* seed： 种子值。影响生成的int值。
*/
static uint32_t hash(const void *key, int len, uint32_t seed) {
	const uint32_t m = 0x5bd1e995;
	const int r = 24;
	uint32_t h = seed ^ len;
	const unsigned char *data = (const unsigned char *)key;

	while (len >= 4) {
		uint32_t k = *(uint32_t *)data;
		k *= m;
		k ^= k >> r;
		k *= m;
		h *= m;
		h ^= k;
		data += 4;
		len -= 4;
	}

	switch (len) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

ValueType hashmap_put(HashMap *map, KeyType key, ValueType value) {
	// 1.将key，转换为一个整数。 
	uint32_t hash_num = hash(key, strlen(key), map->seed);

	// 2.对这个整数，用数组长度取余。 得到数据在数组的什么下标上
	int index = hash_num % map->length;

	// int index = hash(key, strlen(key), map->seed) % map->length;

	// 3.判断这个下标，是否有这个key。 
	// 有这个key， 用新value覆盖旧value。返回旧value。 
	// 没有这个key（整个链表都没有），创建新结点。key-value存进去 头插
	KeyValueNode *cur = map->table[index];

	while (cur != NULL) {
		//  往下走。 说明这个位置不为空。 
		//  循环判断，这个位置，是否有重复的key。 
		//  如果有重复key。 直接新值替换旧值，返回旧值； 
		//  如果循环判断完了，还是没有。 

		if (strcmp(cur->key, key) == 0) {
			// 相等。  新值替换旧值，返回旧值。
			ValueType oldvalue = cur->value;
			cur->value = value;
			return oldvalue;
		}
		cur = cur->next;
	}

	// 说明，这个位置没数据 或 这个key不存在。
	// 新建结点，存数据，插入该位置。 齐活 8

	KeyValueNode *new_node = malloc(sizeof(KeyValueNode));

	// if

	new_node->key = key;
	new_node->value = value;
	new_node->next = map->table[index];
	map->table[index] = new_node;

	map->size++;
}

ValueType hashmap_get(HashMap *map, KeyType key) {
	// 1.将key，转换为一个整数。 
	uint32_t hash_num = hash(key, strlen(key), map->seed);

	// 2.对这个整数，用数组长度取余。 得到数据在数组的什么下标上
	int index = hash_num % map->length;

	KeyValueNode *cur = map->table[index];

	while (cur != NULL) {
		if (strcmp(cur->key, key) == 0) {
			// 相等。  新值替换旧值，返回旧值。
			return cur->value;
		}
		cur = cur->next;
	}

	return NULL;
}

ValueType hashmap_remove(HashMap *map, KeyType key) {
	// 1.转int。  
	int index = hash(key, strlen(key), map->seed) % map->length;

	KeyValueNode *cur = map->table[index];

	if (cur == NULL) {
		return NULL;
	}

	if (strcmp(key, cur->key) == 0) {
		// 说明，找到了。而且是第一个。
		map->table[index] = cur->next;
		ValueType oldvalue = cur->value;
		free(cur);
		return oldvalue;
	}

	KeyValueNode *prev_node = cur;
	cur = cur->next;

	while (cur != NULL)
	{
		if (strcmp(cur->key, key) == 0) {
			// 说明找到了。
			prev_node->next = cur->next;
			ValueType oldvalue = cur->value;
			free(cur);
			return oldvalue;
		}

		prev_node = cur;
		cur = cur->next;
	}

	return NULL;
}
