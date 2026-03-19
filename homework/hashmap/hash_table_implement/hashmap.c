#include "hashmap.h"

HashMap *hashmap_create() {

	// malloc申请空间，空间里面的数据是随机值
	// 指针数组，里面的空间都是随机值，这时候就要手动初始化
	// HashMap *map = malloc(sizeof(HashMap));
	// 所以不推荐使用malloc

	// malloc 申请的内存空间里是未知的随机垃圾值，我们需要手动把 table 数组里的指针全设为 NULL
	// calloc 会在申请内存后，自动将整块内存清零，这样 map->table 里的所有指针就默认是 NULL 了

	HashMap *map = calloc(1, sizeof(HashMap));

	if (map == NULL) {
		return NULL; // 如果内存分配失败，应该进行安全处理
	}

	map->length = HASHMAP_INIT_CAPACITY; // 设置数组长度为10
	map->size = 0;                       // 初始元素个数为0 (calloc其实已经将其置为0了，显式写出逻辑更清晰)
	map->seed = time(NULL);              // 使用当前时间戳作为哈希种子，保证每次运行程序的哈希散列度

	return map;
}

/*
* 内部静态函数：计算哈希值
* key：键的指针。如果是字符串，直接给就行；如果是int，需要对它取地址
* len：键的长度。如果是字符串，传strlen的结果； 如果是其他的类型，传sizeof的结果
* seed：种子值，影响生成的int值 
* 作用：将任意长度的输入，转换成一个毫无规律的 32 位无符号整数
*/
static uint32_t hash(const void *key, int len, uint32_t seed) {
	const uint32_t m = 0x5bd1e995;   // 魔法常数，经过严密数学测试得出的最优乘数
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


// 添加或更新键值对
ValueType hashmap_put(HashMap *map, KeyType key, ValueType value) {

	// 1.将给定的字符串 key 通过哈希算法计算，得到一个巨大的整数
	uint32_t hash_num = hash(key, strlen(key), map->seed);

	// 2.将得到的整数对数组长度（10）取余，得到一个 0~9 的下标索引
	int index = hash_num % map->length;

	// 3.去底层数组看在这个下标是否有数据，如果有数据，遍历，找是否有重复的key；如果没有数据，直接插入
	KeyValueNode *cur_node = map->table[index];

	// 遍历链表，检查是否已经存在相同的 key
	while (cur_node != NULL) {
		// 往下走。 说明这个位置不为空。 
		// 循环判断，这个位置，是否有重复的key。 
		// 如果有重复key。 直接新值替换旧值，返回旧值； 

		// 如果找到相同的 key (strcmp 返回 0 表示字符串内容完全一样)
		if (strcmp(cur_node->key, key) == 0) {
			// 相等。新值替换旧值，返回旧值。
			ValueType oldvalue = cur_node->value;
			cur_node->value = value;
			return oldvalue;
		}
		// 没找到，继续看链表里的下一个结点
		cur_node = cur_node->next;
	}

	// 走到这个位置说明：链表中不存在重复的 key（或者是该位置目前连一个结点都没有）
	// 我们需要创建一个新结点来存储这对 key-value
	KeyValueNode *new_node = calloc(1, sizeof(KeyValueNode));
	new_node->key = key;
	new_node->value = value;

	// 【头插法】将新结点插入到链表的最前端
	// 新结点的 next 指向原来的第一个结点（即使原来是 NULL 也没关系）
	new_node->next = map->table[index];
	// 数组该位置的指针指向新结点，使其成为新的“链表头”
	map->table[index] = new_node;
	// 更新哈希表中的元素总数
	map->size++;

	// 插入新元素，没有旧值被替换，返回 NULL
	return NULL;

}


// 根据键获取值
ValueType hashmap_get(HashMap *map, KeyType key) {
	// 1.计算哈希值，转换为int
	uint32_t hash_num = hash(key, strlen(key), map->seed);

	// 2.取余得到存放该 key 的数组下标
	int index = hash_num % map->length;

	// 3.拿到该下标位置的链表头指针
	KeyValueNode *cur_node = map->table[index];

	// 如果该位置为空，说明一定没有这个 key
	if (cur_node == NULL) {
		return NULL;
	}

	// 遍历链表寻找对应的 key
	while (cur_node != NULL) {
		if (strcmp(cur_node->key, key) == 0) {
			return cur_node->value;  // 找到了，返回对应的 value
		}
		cur_node = cur_node->next;   // 没找到，顺着链表继续找
	}
	// 链表遍历完了也没找到
	return NULL;
}


// 根据键删除元素
ValueType hashmap_remove(HashMap *map, KeyType key) {
	// 1.计算哈希值并取余，找到数组下标
	uint32_t hash_num = hash(key, strlen(key), map->seed);
	int index = hash_num % map->length;
	// 2.拿到链表头指针
	KeyValueNode *cur_node = map->table[index];
	// 该位置为空，说明要删除的数据根本不存在
	if (cur_node == NULL) {
		return NULL;
	}
	// 3.【特殊情况】如果链表的第一个结点就是要删除的结点
	if (strcmp(cur_node->key, key) == 0) {
		// 备份要返回的值
		ValueType old_value = cur_node->value;
		// 让数组直接指向下一个结点（相当于跳过了第一个结点）
		map->table[index] = cur_node->next;
		free(cur_node);    // 释放该结点的内存，防止内存泄漏
		map->size--;	   // 哈希表元素总数减 1
		return old_value;
	}

	// 4.【一般情况】要删除的结点不是第一个，需要在链表中间寻找
	// 在单向链表中删除中间结点，必须知道它的“前驱结点（pre_node）”
	KeyValueNode *pre_node = cur_node;
	cur_node = cur_node->next;
	while (cur_node != NULL) {
		if (strcmp(cur_node->key, key) == 0) {
			// 找到了要删除的结点
			ValueType old_value = cur_node->value;
			// 前驱结点的 next 直接跨过 cur_node，指向 cur_node 的 next
			pre_node->next = cur_node->next;
			free(cur_node);   // 释放内存
			map->size--;
			return old_value;
		}
		// 两个指针同时向后移动一步
		pre_node = cur_node;
		cur_node = cur_node->next;
	}
	// 遍历整个链表都没找到要删除的 key
	return NULL;
}