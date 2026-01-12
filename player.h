#ifndef PLAYER1_H
#define PLAYER1_H
#include "entity.h"

void entity_update_player(
	Entity* p,
	Entity* entities,// 之后我会把整个世界的实体链表传进来
	const int up, 
	const int down,
	const int left,
	const int right
	);
#endif
