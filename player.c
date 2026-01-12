#include"player.h"
#include"map.h"
#include "entity.h"
#include "config.h"
#include<stdio.h>
void entity_update_player(Entity* p, Entity* entities, const int up, const int down, const int left, const int right)
{
	if (p->dead)
	{
		return;
	}
	const float speed = 2.0f;
	float nx = p->x;
	float ny = p->y;//预测下一步位置
	if (up)
		ny -= speed;//玩家坐标移动后面相同
	if (down)
		ny += speed;
	if (left)
		nx -= speed;
	if (right)
		nx += speed;
	if (!map_is_blocked(nx, ny))
	{
		p->x = nx;
		p->y = ny;
	}

	if (p->hide_cooldown > 0) {
		p->hide_cooldown--; // 逐帧减少惩罚时间
	}

	// --- 3. 自动隐藏逻辑（不使用 Shift）---
	// 逻辑：如果在掩体内 且 不在被识破的惩罚期内，则自动隐藏
	if (map_is_cover(p->x, p->y) && p->hide_cooldown <= 0) {
		p->hidden = 1;
	}
	else {
		p->hidden = 0; // 不在掩体 或 惩罚期未满，强制暴露
	}

	float cx = p->x + p->w/ 2;
	float cy = p->y + p->h/ 2;

	if (map_is_exit(cx,cy))
	{
		p->win =1;
	}
	// --- 碰撞 AI 判定 ---
	  // 只有没躲起来时才会被抓
	if (p->hidden == 0) 
	{
		Entity* e = entities;
		while (e) {
			if (e->type == ENTITY_ENEMY) {
				// AABB 矩形碰撞判定
				if (p->x < e->x + e->w &&
					p->x + p->w > e->x &&
					p->y < e->y + e->h &&
					p->y + p->h > e->y)
				{
					p->dead = 1; // 标记死亡
					printf("Player Caught!\n");
				}
			}
			e = e->next;
		}
	}

}
