#pragma once
#pragma once

// 地图设置
#define MAP_W 20
#define MAP_H 15
#define TILE_SIZE 40
#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))
// 玩家实体设置
#define PLAYER_W 20
#define PLAYER_H 20
#define PLAYER_START_X 40.0f
#define PLAYER_START_Y 200.0f
#define PLAYER_SPEED 2.0f

// SDL 窗口大小
#define WINDOW_W 800
#define WINDOW_H 600
// 关卡配置
#define MAX_LEVELS 3
