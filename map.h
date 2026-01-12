#include "config.h"
typedef struct
{
	int blocked;   //是否阻挡移动
   	int cover;       //是否是掩体
	int exit;         //是否是出口
}TileDef;
extern int g_map_id[MAP_H][MAP_W];//地图本体
extern TileDef g_tile_defs[];//规则行为
extern const int g_tile_def_count;
int map_load_from_file(const char* filename);  //加载地图成功返回1，失败返回0
int map_is_blocked(float world_x, float world_y);
int map_is_cover(float world_x, float world_y);
int map_is_exit(float world_x, float world_y);

