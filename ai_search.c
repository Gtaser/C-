#include"entity.h"
#include"world.h"
#include"config.h"
#include<string.h>
#include"world.h"
#include<math.h>
#include"map.h"
void update_belief_diffusion(World* world)
{
    if (!world || !world->belief || !g_map_id) return;

    static float temp[MAP_H][MAP_W];
    memset(temp, 0, sizeof(temp));

    for (int y = 1; y < MAP_H - 1; y++) {
        for (int x = 1; x < MAP_W - 1; x++) {

            if (g_map_id[y][x] != 0) continue;

            float current = world->belief[y][x];
            if (current < 0.01f) continue;

            temp[y][x] += current * 0.8f;
            temp[y + 1][x] += current * 0.04f;
            temp[y - 1][x] += current * 0.04f;
            temp[y][x + 1] += current * 0.04f;
            temp[y][x - 1] += current * 0.04f;
        }
    }

    memcpy(world->belief, temp, sizeof(temp));
}

// 2. 更新逻辑：AI 视野内的区域概率清零
// 在渲染或逻辑循环中调用
void update_belief_from_view(Entity* e, World* world) 

{
    float view_dist = 100.0f;
    // 获取当前 AI 的朝向弧度
    float angle = atan2f(e->dy, e->dx);

    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            float dx = (x * TILE_SIZE) - e->x;
            float dy = (y * TILE_SIZE) - e->y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < view_dist) {
                float cell_angle = atan2f(dy, dx);
                // 如果格子在视野扇形内（约 60 度）
                if (fabsf(cell_angle - angle) < 0.5f) {
                    world->belief[y][x] *= 0.2f; // 快速降低概率
                }
            }
        }
    }
}
int is_player_in_view(Entity* e, Entity* player, World* world) {
    if (player->hidden || player->dead) return 0; // 玩家藏起来了或死了就看不见

    float dx = player->x - e->x;
    float dy = player->y - e->y;
    float dist = sqrtf(dx * dx + dy * dy);

    // 1. 距离判定（例如 150 像素内能看见）
    if (dist < 150.0f) {
        // 2. 角度判定（计算 AI 朝向与玩家位置的夹角）
        float angle_to_player = atan2f(dy, dx);
        float ai_angle;//= atan2f(e->dy, e->dx); // 使用 AI 的移动向量作为朝向
        if (e->state == STATE_CHASE && e->dx != 0 && e->dy != 0) {
            // 追逐状态：使用指向玩家的方向
            ai_angle = atan2f(e->dy, e->dx);
        }
        else {
            // 其他状态：使用移动方向
            ai_angle = atan2f(e->dy, e->dx);
        }

        float diff = fabsf(angle_to_player - ai_angle);
        // 处理 360 度跨越问题
        if (diff > 3.14159f) diff = 2.0f * 3.14159f - diff;

        // 如果夹角小于 30 度（弧度约 0.5），则视为在视野内
        if (diff < 0.5f) {
            return 1;
        }
    }
    return 0;
}