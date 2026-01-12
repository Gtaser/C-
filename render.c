#include "render.h"
#include"map.h"
#include<SDL.h>
#include "entity.h"
#include"world.h"
#include "config.h"
/*
 * 渲染一个“墙 tile”
 * 墙不是一个颜色，而是由多个小色块组成
 */
static void render_wall(SDL_Renderer* r, int x, int y) 
{
    const int sub = 8;                      // 小色块大小
    const int cnt = TILE_SIZE / sub;        // 每行/列数量

    for (int j = 0; j < cnt; j++) {
        for (int i = 0; i < cnt; i++) {

            SDL_Rect b = {
                x + i * sub,
                y + j * sub,
                sub,
                sub
            };

            /* 简单棋盘式颜色变化 */
            if ((i + j) % 2 == 0)
                SDL_SetRenderDrawColor(r, 90, 90, 90, 255);
            else
                SDL_SetRenderDrawColor(r, 60, 60, 60, 255);

            SDL_RenderFillRect(r, &b);
        }
    }
}
static void render_belief_map(SDL_Renderer* r, World* world)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            float b = world->belief[y][x];
            if (b > 0.005f) {
                SDL_Rect rect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                // 颜色映射：根据热度从黄到红
                int alpha = (int)(b * 255.0f * 2.0f);
                if (alpha > 160) alpha = 160; // 避免完全遮挡
                int base_color = 180; 
                int red = (int)(b * 100.0f);        // 0-100 (很少的红色)
                int green = base_color + (int)(b * 75.0f);  // 180-255
                int blue = base_color - (int)(b * 100.0f);  // 180-80

                // 限制范围
                if (red > 100) red = 100;
                if (green > 255) green = 255;
                if (blue < 80) blue = 80;

                SDL_SetRenderDrawColor(r, red, green, blue, alpha);
                SDL_RenderFillRect(r, &rect);

                // 微妙的边框（可选）
                if (b > 0.4f) {
                    SDL_SetRenderDrawColor(r, 255, 255, 220, alpha / 4);
                    SDL_RenderDrawRect(r, &rect);
                }
                
                
            }
        }
    }
}
static void render_exit(SDL_Renderer* r, int x, int y)
{
    static Uint32 start_time = 0;
    if (start_time == 0) {
        start_time = SDL_GetTicks();
    }

    Uint32 current_time = SDL_GetTicks();
    float elapsed_seconds = (current_time - start_time) / 1000.0f;

    // 多重闪烁频率
    float pulse1 = (sinf(elapsed_seconds * 2.5f) + 1.0f) / 2.0f;
    float pulse2 = (sinf(elapsed_seconds * 4.0f) + 1.0f) / 2.0f;
    float pulse3 = (sinf(elapsed_seconds * 1.5f) + 1.0f) / 2.0f;

    // 组合多个脉冲
    float combined_pulse = (pulse1 + pulse2 + pulse3) / 3.0f;

    // 主矩形
    SDL_Rect exit_rect = {
        x * TILE_SIZE,
        y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    // 基础颜色（橙色到金黄色的渐变）
    int base_red = 255;
    int base_green = 165 + (int)(50.0f * combined_pulse);
    int base_blue = 0;

    SDL_SetRenderDrawColor(r, base_red, base_green, base_blue, 255);
    SDL_RenderFillRect(r, &exit_rect);

    // 外发光边框
    int border_alpha = (int)(100 + 155 * pulse1);
    SDL_SetRenderDrawColor(r, 255, 255, 100, border_alpha);
    for (int i = 0; i < 3; i++) {
        SDL_Rect border_rect = {
            x * TILE_SIZE - i,
            y * TILE_SIZE - i,
            TILE_SIZE + 2 * i,
            TILE_SIZE + 2 * i
        };
        SDL_RenderDrawRect(r, &border_rect);
    }

    // 内部高光
    SDL_Rect inner_rect = {
        x * TILE_SIZE + 3,
        y * TILE_SIZE + 3,
        TILE_SIZE - 6,
        TILE_SIZE - 6
    };
    int inner_alpha = (int)(80 + 175 * pulse2);
    SDL_SetRenderDrawColor(r, 255, 255, 220, inner_alpha);
    SDL_RenderFillRect(r, &inner_rect);

    // 中心亮点
    SDL_Rect center_rect = {
        x * TILE_SIZE + 8,
        y * TILE_SIZE + 8,
        TILE_SIZE - 16,
        TILE_SIZE - 16
    };

    int center_alpha = (int)(100 + 155 * pulse3);
    SDL_SetRenderDrawColor(r, 255, 255, 255, center_alpha);
    SDL_RenderFillRect(r, &center_rect);

    // 添加"EXIT"文字提示（简单版本）
    if (pulse1 > 0.7f) { // 当脉冲较强时显示
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        // 绘制一个简单的E字母表示出口
        for (int i = 0; i < 3; i++) {
            // 横线
            SDL_RenderDrawLine(r,
                x * TILE_SIZE + 10,
                y * TILE_SIZE + 10 + i * 8,
                x * TILE_SIZE + TILE_SIZE - 10,
                y * TILE_SIZE + 10 + i * 8);
            // 竖线
            SDL_RenderDrawLine(r,
                x * TILE_SIZE + 10,
                y * TILE_SIZE + 10,
                x * TILE_SIZE + 10,
                y * TILE_SIZE + TILE_SIZE - 10);
        }
    }
}

/*
 * 渲染整张地图
 * 根据 tile 的“规则”决定怎么画
 */
 void render_map(SDL_Renderer* r) 
 {
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {

            int id = g_map_id[y][x];

            if (g_tile_defs[id].blocked) {
                render_wall(r, x * TILE_SIZE, y * TILE_SIZE);
            }
            else if (g_tile_defs[id].cover) {
                SDL_Rect rect = 
                {
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };
                SDL_SetRenderDrawColor(r, 0, 120, 0, 255);
                SDL_RenderFillRect(r, &rect);
            }
            else if (g_tile_defs[id].exit) {
                render_exit(r, x, y); // 渲染出口
            }
        }
    }
}

 void render_world(SDL_Renderer* r, World* world)
 {
     // 1. 背景清屏
     SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
     SDL_RenderClear(r);

     // 2. 渲染地图
     render_map(r);

     // 3. 渲染热度图 (插在地图和实体之间)
     render_belief_map(r, world);

     // 4. 遍历渲染所有实体
     Entity* p = world->entities;
     while (p != NULL)
     {
         SDL_Rect rect = { (int)p->x, (int)p->y, p->w, p->h };

         // --- A. 决定实体颜色 ---
         if (p->type == ENTITY_PLAYER) {
             if (p->hidden) SDL_SetRenderDrawColor(r, 0, 80, 160, 255);
             else SDL_SetRenderDrawColor(r, 0, 150, 255, 255);
         }
         else if (p->type == ENTITY_ENEMY) {
             if (p->state == STATE_CHASE) SDL_SetRenderDrawColor(r, 255, 120, 0, 255);
             else if (p->state == STATE_SEARCH) SDL_SetRenderDrawColor(r, 200, 0, 255, 255);
             else if (p->state == STATE_RETURN) SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
             else SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
         }
         SDL_RenderFillRect(r, &rect);

         // --- B. 渲染敌人视野 ---
         if (p->type == ENTITY_ENEMY)
         {
             float view_distance = 75.0f;
             float view_angle = 35.0f;
             float base_dir;// = (p->dy == 1);// (M_PI / 2.0f) : (-M_PI / 2.0f);
             if (p->state == STATE_CHASE) {
                 // 追逐状态：视野指向移动方向（已经指向玩家）
                 if (p->dx == 0 && p->dy == 0) {
                     base_dir = M_PI / 2.0f; // 默认向下
                 }
                 else {
                     base_dir = atan2f(p->dy, p->dx);
                 }
             }
             else {
                 // 其他状态：使用移动方向
                 base_dir = atan2f(p->dy, p->dx);
             }
             SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

             if (p->state == STATE_SEARCH)
             {
                 SDL_SetRenderDrawColor(r, 200, 0, 255, 60); // 搜索视野：紫色
                 float wobble = sinf(SDL_GetTicks() * 0.01f) * 0.5f;
                 base_dir += wobble;
             }
             else if (p->state == STATE_CHASE) {
                 SDL_SetRenderDrawColor(r, 255, 100, 0, 80); // 追逐视野：橙色
             }
             else {
                 SDL_SetRenderDrawColor(r, 255, 255, 150, 40); // 普通视野：黄色
             }

             float cx = p->x + p->w / 2.0f;
             float cy = p->y + p->h / 2.0f;

             // 绘制射线（带墙体碰撞截断）
             for (float i = -view_angle; i <= view_angle; i += 1.0f) {
                 float angle_rad = base_dir + (i * M_PI / 180.0f);
                 float x_end = cx + view_distance * cosf(angle_rad);
                 float y_end = cy + view_distance * sinf(angle_rad);

                 for (float step = 0; step < view_distance; step += 2.0f) {
                     float check_x = cx + step * cosf(angle_rad);
                     float check_y = cy + step * sinf(angle_rad);
                     if (map_is_blocked(check_x, check_y)) {
                         x_end = check_x;
                         y_end = check_y;
                         break;
                     }
                 }
                 SDL_RenderDrawLine(r, (int)cx, (int)cy, (int)x_end, (int)y_end);
             }
         }
         p = p->next;
     }

     // 5. 提交显示
     SDL_RenderPresent(r);
     }
 
    


