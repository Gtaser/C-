#pragma once
#ifndef WORLD_H
#define WORLD_H
#include"entity.h"
#include"config.h"
typedef struct 
{
	Entity* entities;//所有实体链表
	float belief[MAP_H][MAP_W];// 概率分布图
	int current_level;       // 当前关卡
    int level_completed;           // 当前关卡是否已完成
}World;

void world_init(World* world);  // 修改，传入关卡

void world_update(World* world);
void world_cleanup(World* world);
void world_load_level(World* world,int level);  // 新增：切换到下一关

#endif // !WORLD_H
