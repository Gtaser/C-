#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include<stdio.h>
#include<stdlib.h>
#include "config.h"


 int g_map_id[MAP_H][MAP_W] = { 0 };
TileDef g_tile_defs[] =
{
	{0,0,0},//空地0
	{1,0,0},//墙面1
	{0,1,0},//掩体2
	{0,0,1} //出口3
};
 const int g_tile_def_count=4;
 void world_to_tile(float x, float y, int* tx, int* ty) 
{
	*tx = (int)(x / TILE_SIZE);
	*ty = (int)(y / TILE_SIZE);
}

int map_load_from_file(const char* filename)
{
	FILE* f = fopen(filename, "r");
	if (!f) {
		printf("错误: 无法打开地图文件 %s\n", filename);
		return 0;
	}

	// 重置地图数组（防止读取不足时保留旧数据）
	memset(g_map_id, 0, sizeof(g_map_id));

	// 只使用一种读取方式：fscanf 会自动跳过空格和换行符
	for (int y = 0; y < MAP_H; y++) {
		for (int x = 0; x < MAP_W; x++) {
			if (fscanf(f, "%d", &g_map_id[y][x]) != 1) {
				// 如果没读够指定的数量
				printf("错误: 地图文件 %s 数据不足或格式错误 (y:%d, x:%d)\n", filename, y, x);
				fclose(f);
				return 0;
			}
		}
	}

	fclose(f);
	printf("地图 %s 加载成功，尺寸: %dx%d\n", filename, MAP_W, MAP_H);
	return 1;
}
int map_load_for_level(int level)
{
	char filename[20];
	sprintf(filename, "map%d.txt", level + 1); // level从0开始，文件从1开始

	return map_load_from_file(filename);
}


int map_is_blocked(float world_x, float world_y)
{
	// 定义 AI 的逻辑碰撞体积大小（略小于 TILE_SIZE，方便过门）
	float size = 16.0f;

	// 我们需要检测这个矩形的四个角是否入墙
	// 点 1: 左上 (world_x, world_y)
	// 点 2: 右上 (world_x + size, world_y)
	// 点 3: 左下 (world_x, world_y + size)
	// 点 4: 右下 (world_x + size, world_y + size)

	float check_points[4][2] = {
		{world_x, world_y},
		{world_x + size, world_y},
		{world_x, world_y + size},
		{world_x + size, world_y + size}
	};

	for (int i = 0; i < 4; i++) {
		int tx, ty;
		// 使用你现有的坐标转换函数
		world_to_tile(check_points[i][0], check_points[i][1], &tx, &ty);

		// 1. 越界检查
		if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H)
			return 1;

		// 2. 磁砖检查
		if (g_tile_defs[g_map_id[ty][tx]].blocked) return 1;
	}

	return 0; // 四个角都安全，才返回不堵塞
}

int map_is_cover(float world_x, float world_y) 
{
	int tx, ty;
	world_to_tile(world_x, world_y, &tx, &ty);

	if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H)
		return 0;

	int id = g_map_id[ty][tx];
	return g_tile_defs[id].cover;
}

int map_is_exit(float world_x, float world_y)
{
	int tx, ty;
	world_to_tile(world_x, world_y, &tx, &ty);
	if (tx < 0 || tx >= MAP_W || ty < 0 || ty >= MAP_H)
		return 0;
	int id = g_map_id[ty][tx];
	return g_tile_defs[id].exit;

}
