#include "dynamic_hashmap.h"

// 创建并初始化一个动态哈希表
DynamicHashMap *hashmap_create() {
	// malloc申请空间，空间里面的数据是随机值
	// 指针数组，里面的空间都是随机值，这时候就要手动初始化
	// HashMap *map = malloc(sizeof(HashMap));
	// 所以不推荐使用malloc

	// 给哈希表结构分配内存
	// 使用 calloc 申请内存。calloc 会自动将申请到的内存空间全部初始化为 0
	DynamicHashMap *map = calloc(1, sizeof(DynamicHashMap));
	if (map == NULL) {
		printf("Error: malloc failed in hashmap_create\n");
		exit(1);
	}

	// 给哈希桶（指针数组）分配内存
	map->buckets = calloc(HASHMAP_INIT_CAPACITY, sizeof(KeyValueNode *));
	if (map->buckets == NULL) {
		// 如果桶分配失败，千万别忘了释放刚才成功分配的map结构体本身，防止内存泄漏
		free(map);
		printf("Error: calloc failed in hashmap_create\n");
		exit(1);
	}

	// 初始化成员变量
	map->size = 0;
	map->capacity = HASHMAP_INIT_CAPACITY;
	map->seed = time(NULL);   // 使用当前时间作为哈希种子

	return map;
}


// 销毁哈希表，释放所有占用的内存
void hashmap_destroy(DynamicHashMap *map) {
	if (map == NULL) return;
	
	// 1.遍历动态数组的每一个“桶”
	for (int i = 0; i < map->capacity; i++) {
		KeyValueNode *curr = map->buckets[i];
		// 2.如果桶里有链表，遍历并销毁链表上的每一个节点
		while (curr != NULL) {
			KeyValueNode *temp = curr->next;  // 先保存下一个节点的地址
			free(curr);						  // 安全释放当前节点
			curr = temp;					  // 移动到下一个节点
		}
	}
	// 3.释放哈希桶（动态数组）本身
	free(map->buckets);
	// 4.最后释放哈希表结构体
	free(map);
}


/*
* 内部静态函数：计算哈希值
* key：键的指针。如果是字符串，直接给就行；如果是int，需要对它取地址
* len：键的长度。如果是字符串，传strlen的结果； 如果是其他的类型，传sizeof的结果
* seed：种子值，影响生成的int值
* 作用：将任意长度的输入，转换成一个毫无规律的 32 位无符号整数
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


// 存入或更新键值对，并处理动态扩容
ValueType hashmap_put(DynamicHashMap *map, KeyType key, ValueType value) {
	if (map == NULL || key == NULL) return NULL;

	// 1.将字符串 key 通过哈希函数转换为一个无符号整数
	uint32_t hash_num = hash(key, strlen(key), map->seed);

	// 2.取模运算，将巨大的哈希值映射到当前数组容量的合法下标范围内 (0 ~ capacity-1)
	int index = hash_num % map->capacity;

	// 3.定位到该下标对应的链表头节点
	KeyValueNode *cur_node = map->buckets[index];

	// 4.遍历链表，检查 key 是否已经存在
	while (cur_node != NULL) {
		// 如果找到相同的 key (strcmp 返回 0 表示字符串内容完全一样)
		if (strcmp(cur_node->key, key) == 0) {
			// 找到了相同的 key！执行“更新”操作：新值覆盖旧值，并返回旧值
			ValueType oldvalue = cur_node->value;
			cur_node->value = value;
			return oldvalue;
		}
		cur_node = cur_node->next;
	}

	// 5. 走到这里说明 key 不存在，执行“新增”操作
	// 创建新节点并分配内存
	KeyValueNode *new_node = calloc(1, sizeof(KeyValueNode));
	if (new_node == NULL) return NULL;
	new_node->key = key;
	new_node->value = value;
	// 使用“头插法”将新节点挂到链表最前面，这比遍历到尾部插入效率更高 (O(1))
	new_node->next = map->buckets[index];
	map->buckets[index] = new_node;
	map->size++;  // 元素总数加一

	// 动态扩容逻辑：判断是否超过负载因子阈值（例如 75%）
	if ((1.0 * map->size / map->capacity) > LOAD_FACTOR_THRESHOLD) {

		// 能不能用realloc？答案是不行！
		// 比如一个键"apple"，它的哈希值计算出来正好是 11
		// 扩容前：容量是 10。它的下标是 11 % 10 = 1。所以 "apple" 被存放在了 buckets[1] 的链表里
		// 如果此时用 realloc：realloc 会把旧数组的内容原封不动地拷贝到新数组的前半部分。"apple" 依然躺在 new_buckets[1] 里
		// 错误发生：此时容量变成了 20。如果用户调用 hashmap_get(map, "apple") 去查找这个值，程序会重新计算下标：11 % 20 = 11
		// 程序会直接跑去 new_buckets[11] 找，结果发现那里是空的（NULL），于是错误地告诉你：“找不到 apple”
		// 10101010111 % 10===> 1
		// 10101010111 % 20===> 11

		// 此外，realloc在扩大内存时，新增出来的那部分空间里的值是随机的垃圾值（野指针）

		// 总结正确的扩容流程：
		// 1.用 calloc 申请一个两倍大的全新且干净的数组。
		// 2.遍历旧数组，把每一个节点拿出来，用新容量重新计算新下标。
		// 3.把节点挂到新数组的对应位置。(这个过程叫Rehash（重哈希）)
		// 4.释放掉旧数组。

		int old_capacity = map->capacity;
		int new_capacity = old_capacity * 2; // 容量翻倍

		// 申请新的、更大的数组。必须用 calloc，确保新数组所有元素为 NULL
		KeyValueNode **new_buckets = calloc(new_capacity, sizeof(KeyValueNode *));
		if (new_buckets == NULL) {
			return NULL; // 如果扩容失败，按理应该有错误处理，这里简单处理为直接返回
		}

		// 遍历旧数组，把所有老数据重新映射（Rehash）到新数组中
		for (int i = 0; i < old_capacity; i++) {
			KeyValueNode *curr = map->buckets[i]; // 取出旧桶的链表头
			while (curr != NULL) {
				KeyValueNode *next_node = curr->next; // 提前保存下一个节点，否则改变 curr->next 后链表会断裂

				// 重点：必须用新的容量来重新计算下标！(哈希值没变，但取模的基数变了)
				uint32_t h_num = hash(curr->key, strlen(curr->key), map->seed);
				int new_index = h_num % new_capacity;

				// 头插法：将当前节点插入到新数组对应的链表中
				curr->next = new_buckets[new_index];
				new_buckets[new_index] = curr;

				// 移动到旧链表的下一个节点继续处理
				curr = next_node;
			}
		}

		// 释放旧的数组空间
		free(map->buckets);

		// 更新 map 结构体的状态，指向新数组并更新容量记录
		map->buckets = new_buckets;
		map->capacity = new_capacity;
	}

	// 新增节点默认返回 NULL
	return NULL;
}


// 查找指定 key 的值
ValueType hashmap_get(DynamicHashMap *map, KeyType key) {
	if (map == NULL || key == NULL) return NULL;

	// 1.计算哈希值
	uint32_t hash_num = hash(key, strlen(key), map->seed);
	// 2.计算对应下标
	int index = hash_num % map->capacity;

	// 3.定位到具体的链表
	KeyValueNode *cur_node = map->buckets[index];

	// 4.遍历链表比对字符串
	while (cur_node != NULL) {
		if (strcmp(cur_node->key, key) == 0) {
			return cur_node->value; // 找到了，返回 value
		}
		cur_node = cur_node->next;
	}

	return NULL; // 遍历完都没找到
}


// 删除指定的键值对
ValueType hashmap_remove(DynamicHashMap *map, KeyType key) {
	if (map == NULL || key == NULL) return NULL;

	uint32_t hash_num = hash(key, strlen(key), map->seed);
	int index = hash_num % map->capacity;

	KeyValueNode *cur_node = map->buckets[index];

	if (cur_node == NULL) {
		return NULL; // 桶里没东西，说明 key 肯定不存在
	}

	// 情况一：要删除的正好是链表的第一个节点（头节点）
	if (strcmp(cur_node->key, key) == 0) {
		ValueType old_value = cur_node->value;

		// 直接让数组指向当前节点的下一个节点
		map->buckets[index] = cur_node->next;
		free(cur_node); // 释放内存
		map->size--;    // 元素总数减一

		return old_value;
	}

	// 情况二：要删除的节点在链表中间或尾部
	KeyValueNode *pre_node = cur_node; // 需要一个指针记录上一个节点（前驱节点）
	cur_node = cur_node->next;         // 当前节点后移一位

	while (cur_node != NULL) {
		if (strcmp(cur_node->key, key) == 0) {
			ValueType old_value = cur_node->value;

			// 让前一个节点直接跳过当前节点，指向下一个节点
			pre_node->next = cur_node->next;
			free(cur_node); // 释放当前节点

			map->size--;

			return old_value;
		}
		// 两个指针一起向后移动
		pre_node = cur_node;
		cur_node = cur_node->next;
	}

	return NULL; // 没找到该 key
}