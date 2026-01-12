
#include "world.h"
#include "map.h"
#include "render.h"
#include "entity.h"
#include "config.h"
#include "player.h"
#include <math.h>
#include<stdio.h>
#include<stdlib.h>

#define PLAYER_SPEED 2.0f

typedef struct {
    float player_start_x;
    float player_start_y;
    // 为每个关卡定义多个敌人起始位置和方向
    struct {
        float x;
        float y;
        float dx;
        float dy;
    } enemy_starts[3];  // 最多支持3个敌人
    int enemy_count;    // 该关卡的敌人数量
    const char* map_file;
} LevelConfig;

static LevelConfig level_configs[] = {
    // 关卡1: 1个敌人
    {
        .player_start_x = 2 * TILE_SIZE,   // 80
        .player_start_y = 7 * TILE_SIZE,   // 280
        .enemy_starts = {
            {15 * TILE_SIZE, 7 * TILE_SIZE, 0.0f, 1.0f}, // 敌人1
            {0, 0, 0, 0}, // 空位
            {0, 0, 0, 0}  // 空位
        },
        .enemy_count = 1,
        .map_file = "map.txt"
    },
    // 关卡2: 2个敌人
    {
        .player_start_x = 2 * TILE_SIZE,   // 80
        .player_start_y = 12 * TILE_SIZE,  // 480
        .enemy_starts = {
            {17 * TILE_SIZE, 2 * TILE_SIZE, 0.0f, 1.0f},  // 敌人1
            {5 * TILE_SIZE, 7 * TILE_SIZE, 1.0f, 0.0f},   // 敌人2 - 新增，水平移动
            {0, 0, 0, 0}  // 空位
        },
        .enemy_count = 2,
        .map_file = "map2.txt"
    },
    // 关卡3: 3个敌人
    {
        .player_start_x = 3 * TILE_SIZE,   // 120
        .player_start_y = 2 * TILE_SIZE,   // 80
        .enemy_starts = {
            {17 * TILE_SIZE, 12 * TILE_SIZE, 0.0f, -1.0f}, // 敌人1 - 向上移动
            {5 * TILE_SIZE, 5 * TILE_SIZE, -1.0f, 0.0f},   // 敌人2 - 新增，向左移动
            {10 * TILE_SIZE, 10 * TILE_SIZE, 0.0f, 1.0f}   // 敌人3 - 新增，向下移动
        },
        .enemy_count = 3,
        .map_file = "map3.txt"
    }
};

void world_init(World* world)
{
    world->entities = NULL;    // 初始时没有实体
    world->current_level = 0;  // 默认第一关
    // 关键：清空概率图
    memset(world->belief, 0, sizeof(world->belief));
    // 加载第一关地图
    if (!map_load_from_file(level_configs[0].map_file)) {
        printf("Warning: Failed to load map for level 1\n");
    }

    // 创建玩家实体（使用第一关配置）
    Entity* player = entity_create(
        ENTITY_PLAYER,
        level_configs[0].player_start_x,
        level_configs[0].player_start_y,
        PLAYER_W,
        PLAYER_H
    );
    player->hidden = 0;
    player->win = 0;
    entity_add(&world->entities, player);

    // 创建敌人实体（第一关只有1个敌人）
    Entity* enemy = entity_create(
        ENTITY_ENEMY,
        level_configs[0].enemy_starts[0].x,
        level_configs[0].enemy_starts[0].y,
        20,
        20
    );
    enemy->start_x = enemy->x;
    enemy->start_y = enemy->y;
    enemy->search_timer = 0;
    enemy->target_x = 0;
    enemy->target_y = 0;
    enemy->dx = level_configs[0].enemy_starts[0].dx;
    enemy->dy = level_configs[0].enemy_starts[0].dy;
    enemy->state = STATE_PATROL;
    enemy->stuck_timer = 0;
    enemy->action_pause_timer = 0;
    entity_add(&world->entities, enemy);
}

void world_load_level(World* world, int level)
{
    if (level < 0 || level >= 3) {
        level = 0;  // 默认第一关
    }

    // 清理当前世界
    world_cleanup(world);

    // 设置当前关卡
    world->current_level = level;

    // 加载对应关卡的地图
    if (!map_load_from_file(level_configs[level].map_file)) {
        printf("Warning: Failed to load map for level %d\n", level + 1);
        // 尝试加载默认地图
        map_load_from_file("map.txt");
    }

    // 创建玩家实体
    Entity* player = entity_create(
        ENTITY_PLAYER,
        level_configs[level].player_start_x,
        level_configs[level].player_start_y,
        PLAYER_W,
        PLAYER_H
    );
    player->hidden = 0;
    player->win = 0;
    player->dead = 0;
    entity_add(&world->entities, player);

    // 创建敌人实体
    for (int i = 0; i < level_configs[level].enemy_count; i++) {
        Entity* enemy = entity_create(
            ENTITY_ENEMY,
            level_configs[level].enemy_starts[i].x,
            level_configs[level].enemy_starts[i].y,
            20,
            20
        );
        enemy->start_x = enemy->x;
        enemy->start_y = enemy->y;
        enemy->search_timer = 0;
        enemy->target_x = 0;
        enemy->target_y = 0;
        enemy->dx = level_configs[level].enemy_starts[i].dx;
        enemy->dy = level_configs[level].enemy_starts[i].dy;
        enemy->state = STATE_PATROL;
        enemy->stuck_timer = 0;
        enemy->action_pause_timer = 0;
        entity_add(&world->entities, enemy);
    }
}

void world_update(World* world)
{
    
    // 1. 全局概率演化：模拟玩家"气味"或痕迹随时间向四周扩散
    update_belief_diffusion(world);
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++)
        {
            world->belief[y][x] *= 0.995f;
        }
    }
    // 假设链表第一个节点是玩家实体
    Entity* player = world->entities;
    Entity* e = world->entities;
    while (player && player->type != ENTITY_PLAYER) {
        player = player->next;
    }

    if (player && !player->dead && !player->win) {
        // 只在玩家没有死亡/胜利时注入热度
        int tile_x = (int)(player->x / TILE_SIZE);
        int tile_y = (int)(player->y / TILE_SIZE);

        if (tile_x >= 0 && tile_x < MAP_W && tile_y >= 0 && tile_y < MAP_H) {
            // 如果玩家不在掩体中，注入更多热度
            float injection = player->hidden ? 0.3f : 0.7f;
            world->belief[tile_y][tile_x] += injection;

            // 限制热度最大值
            if (world->belief[tile_y][tile_x] > 1.0f) {
                world->belief[tile_y][tile_x] = 1.0f;
            }
        }
    }
    while (e != NULL)
    {
        Entity* next_node = e->next;// 关键：先保存下一个节点的指针，防止循环内部逻辑干扰遍历
        if (e->x < 0) e->x = 0;
        if (e->y < 0) e->y = 0;
        if (e->x + e->w > MAP_W * TILE_SIZE)
            e->x = MAP_W * TILE_SIZE - e->w;
        if (e->y + e->h > MAP_H * TILE_SIZE)
            e->y = MAP_H * TILE_SIZE - e->h;

        // 仅处理敌人类型的 AI 逻辑
        if (e->type == ENTITY_ENEMY)
        {
            if (e->action_pause_timer > 0) // 只要暂停计时器在倒计时，AI 就原地不动，不执行下面的 switch 逻辑
            {
                e->action_pause_timer--;
                e = e->next;
                continue;
            }
            else
            {
                switch (e->state)
                {
                case STATE_PATROL: // --- 巡逻模式 ---
                {
                    // 计算下一帧的垂直位置，进行预判移动
                    float next_y = e->y + e->dy;

                    // 碰撞检测：如果撞墙或到达屏幕边界，则反转 Y 轴速度
                    if (map_is_blocked(e->x, next_y) || next_y < 0 || next_y >(600 - e->h))
                    {
                        e->dy *= -1;
                    }
                    else
                    {
                        e->y = next_y; // 路径安全，执行移动
                    }
                    // 设置巡逻时的视野方向（垂直方向）
                    if (e->dy > 0) {
                        e->dx = 0; e->dy = 1.0f; // 向下
                    }
                    else {
                        e->dx = 0; e->dy = -1.0f; // 向上
                    }

                    // 感知逻辑：检测玩家是否出现在 AI 视野范围内
                    if (player && !player->hidden && !player->dead) {
                        if (is_player_in_view(e, player, world)) {
                            e->state = STATE_CHASE; // 发现玩家，切换至追逐
                            printf("AI: 发现目标！开始追击。\n");
                        }
                    }
                    break;
                }

                case STATE_CHASE: // --- 追逐模式 ---
                {
                    // 闭环关键：如果玩家躲藏，AI 失去直接视野，转入搜索
                    if (player->hidden)
                    {
                        // 将玩家最后出现的位置坐标转换为网格索引
                        int tx = (int)(player->x / TILE_SIZE);
                        int ty = (int)(player->y / TILE_SIZE);

                        // 在概率图中注入 1.0，标记为"最后目击点"
                        if (tx >= 0 && tx < MAP_W && ty >= 0 && ty < MAP_H) {
                            world->belief[ty][tx] = 1.0f;
                        }

                        e->state = STATE_SEARCH; // 切换状态
                        e->search_timer = 300;   // 设置搜索持续时间（约5秒）
                        e->action_pause_timer = 30; // 停顿 0.5 秒
                        e->target_x = player->x; // 记录初始搜索目标点
                        e->target_y = player->y;
                        e->dx = 0;
                        e->dy = 0;
                        printf("AI: 目标消失，开始搜索...\n");
                        break;
                    }

                    // 追逐移动：向玩家当前坐标靠近
                    float speed = 1.2f;
                    float nx = e->x, ny = e->y;
                    // 计算到玩家的方向
                    float dx_to_player = player->x - e->x;
                    float dy_to_player = player->y - e->y;
                    float dist_to_player = sqrtf(dx_to_player * dx_to_player + dy_to_player * dy_to_player);
                    if (dist_to_player > 0)
                    {
                        e->dx = dx_to_player / dist_to_player;
                        e->dy = dy_to_player / dist_to_player;
                    }
                    if (player->x > e->x) nx += speed; else if (player->x < e->x) nx -= speed;
                    if (player->y > e->y) ny += speed; else if (player->y < e->y) ny -= speed;

                    // 分轴碰撞检测，确保 AI 不会卡死在墙角
                    if (!map_is_blocked(nx, e->y)) e->x = nx;
                    if (!map_is_blocked(e->x, ny)) e->y = ny;
                    break;
                }

                case STATE_SEARCH:
                {
                    // 1. 视觉重连检测
                    if (!player->hidden && is_player_in_view(e, player, world)) {
                        e->state = STATE_CHASE;
                        memset(world->belief, 0, sizeof(world->belief));
                        printf("AI: 再次锁定目标！\n");
                        break;
                    }


                    // --- C. 计时器逻辑 (只在 SEARCH 状态下减量) ---
                    if (e->search_timer > 0) {
                        e->search_timer--;
                    }
                    else {
                        e->state = STATE_RETURN; // 真正的时间到了才返回
                        printf("AI: 搜索无果，返回岗位。\n");
                        break;
                    }
                    update_belief_from_view(e, world);

                    // 2. 定期决策：寻找最高热度
                    if (e->search_timer % 60 == 0)
                    {
                        float max_b = 0.005f; // 稍微提高一点阈值，忽略背景噪声
                        // 默认目标保持在当前格中心，防止坐标归零
                        int target_tx = (int)(e->x / TILE_SIZE);
                        int target_ty = (int)(e->y / TILE_SIZE);

                        for (int i = -7; i <= 7; i++) {
                            for (int j = -7; j <= 7; j++) {
                                int ty = (int)(e->y / TILE_SIZE) + i;
                                int tx = (int)(e->x / TILE_SIZE) + j;
                                if (tx >= 0 && tx < MAP_W && ty >= 0 && ty < MAP_H) {
                                    float score = world->belief[ty][tx] / (1.0f + sqrtf(i * i + j * j) * 0.1f);
                                    if (score > max_b) {
                                        max_b = score; target_tx = tx; target_ty = ty;
                                    }
                                }
                            }
                        }
                        // 锁定格子中心
                        e->target_x = target_tx * TILE_SIZE + 16;
                        e->target_y = target_ty * TILE_SIZE + 16;
                    }

                    // 3. 物理移动逻辑
                    float dist_to_target = sqrtf(powf(e->target_x - e->x, 2) + powf(e->target_y - e->y, 2));

                    // 防抖：如果离目标点已经很近了，就减速，避免冲过头
                    if (dist_to_target > 5.0f) {
                        float d_dx = (e->target_x > e->x) ? 1.0f : -1.0f;
                        float d_dy = (e->target_y > e->y) ? 1.0f : -1.0f;
                        e->dx = e->dx * 0.8f + d_dx * 0.2f;
                        e->dy = e->dy * 0.8f + d_dy * 0.2f;

                        float next_x = e->x + e->dx * 0.7f;
                        float next_y = e->y + e->dy * 0.7f;

                        // 增加搜索时的碰撞检测，防止 AI 贴着墙走时卡住
                        if (!map_is_blocked(next_x, e->y)) e->x = next_x;
                        if (!map_is_blocked(e->x, next_y)) e->y = next_y;
                    }

                    if (e->search_timer <= 0) e->state = STATE_RETURN;
                    break;
                }

                case STATE_RETURN: // --- 回归模式 ---
                {
                    float speed = 1.0f; // 回归速度

         // 计算到目标的距离
                    float dist_x = e->start_x - e->x;
                    float dist_y = e->start_y - e->y;
                    float dist = sqrtf(dist_x * dist_x + dist_y * dist_y);

                    if (dist > speed) {
                        // 使用向量归一化，确保斜向移动速度也是 1.0
                       // e->x += (dist_x / dist) * speed;
                       // e->y += (dist_y / dist) * speed;

                        // 更新方向，让 AI 看起来是面朝家跑
                       // e->dy = (dist_y > 0) ? 1.0f : -1.0f;
                        e->dx = dist_x / dist;
                        e->dy = dist_y / dist;

                        e->x += e->dx * speed;
                        e->y += e->dy * speed;
                    }
                    else {
                        // 距离已经非常近了，直接归位
                        e->x = e->start_x;
                        e->y = e->start_y;
                        e->state = STATE_PATROL;
                        // 恢复关卡配置中的初始移动方向
                        for (int i = 0; i < level_configs[world->current_level].enemy_count; i++) {
                            if (e->x == level_configs[world->current_level].enemy_starts[i].x &&
                                e->y == level_configs[world->current_level].enemy_starts[i].y) {
                                e->dx = level_configs[world->current_level].enemy_starts[i].dx;
                                e->dy = level_configs[world->current_level].enemy_starts[i].dy;
                                break;
                            }
                        }
                        printf("AI: 撤退完成，恢复巡逻。\n");
                    }
                    break;
                }
                }
            } // 结束 switch
        }
        e = next_node; // 处理链表中的下一个实体
    }
}


void world_cleanup(World* world)
{
    entity_free_all(world->entities);
    world->entities = NULL;
}