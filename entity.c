#include "entity.h"
#include<stdlib.h>
Entity* entity_create(EntityType type, float x, float y, int w, int h)
{
	Entity* p = (Entity*)malloc(sizeof(Entity));
	if (!p) return NULL;

	memset(p, 0, sizeof(Entity));
	p->type = type;   //设置实体类型
	p->x = x;           // 设置 x 坐标
	p->y = y;             // 设置 y 坐标
	p->w = w;                 // 设置宽度
	p->h = h;                 // 设置高度
	p->next = NULL;  // 新实体默认没有下一个
	p ->hidden = 0;
	
	p->win = 0;
	p->dead = 0;
	return p;
}

void entity_add(Entity** list, Entity* e)
{
	if (*list == NULL) {
		// 如果链表为空，新实体就是头节点
		*list = e;
	}
	else {
		// 否则找到链表尾部
		Entity* current = *list;
		while (current->next != NULL) {
			current = current->next;
		}
		// 在尾部添加新实体
		current->next = e;
	}
	// 确保新实体的next为NULL（链表结束）
	e->next = NULL;                // 链表头更新为新实体
}

void entity_free_all(Entity* list)
{
	while (list) {             // 遍历整个链表
		Entity* next = list->next; // 记录下一个实体
		free(list);            // 释放当前实体
		list = next;           // 移动到下一个
	}
}
