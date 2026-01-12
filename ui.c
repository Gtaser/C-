#pragma once
#include <SDL.h>
#include "ui.h"
SDL_Rect btn_start = { 320, 280, 160, 40 };
SDL_Rect btn_level = { 320, 340, 160, 40 };
SDL_Rect btn_exit = { 320, 400, 160, 40 };
SDL_Rect lvl1 = { 150, 250, 100, 100 };
SDL_Rect lvl2 = { 350, 250, 100, 100 };
SDL_Rect lvl3 = { 550, 250, 100, 100 };
/* 画一个小方块 */
static void draw_block(SDL_Renderer* r, int x, int y, int s)
{
    SDL_Rect rect = { x, y, s, s };
    SDL_RenderFillRect(r, &rect);
}

/* 数字 1 */
static void draw_1(SDL_Renderer* r, int x, int y, int s)
{
    draw_block(r, x + s, y, s);
    for (int i = 0; i < 5; i++) draw_block(r, x + 2 * s, y + i * s, s);
    for (int i = 0; i < 5; i++) draw_block(r, x + i * s, y + 4 * s, s);
}

/* 数字 2 */
static void draw_2(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) {
        draw_block(r, x + i * s, y, s);         // 顶
        draw_block(r, x + i * s, y + 2 * s, s); // 中
        draw_block(r, x + i * s, y + 4 * s, s); // 底
    }
    draw_block(r, x + 4 * s, y + s, s); // 右上
    draw_block(r, x, y + 3 * s, s);     // 左下
}

/* 数字 3 */
static void draw_3(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) {
        draw_block(r, x + i * s, y, s);         // 顶
        draw_block(r, x + i * s, y + 2 * s, s); // 中
        draw_block(r, x + i * s, y + 4 * s, s); // 底
    }
    draw_block(r, x + 4 * s, y + s, s);
    draw_block(r, x + 4 * s, y + 3 * s, s);
}
/* 字母 Y */
static void draw_Y(SDL_Renderer* r, int x, int y, int s)
{
    draw_block(r, x, y, s);
    draw_block(r, x + 4*s, y, s);
    draw_block(r, x + 2*s, y + s, s);
    draw_block(r, x + 2*s, y + 2*s, s);
    draw_block(r, x + 2*s, y + 3*s, s);
    draw_block(r, x + 2*s, y + 4*s, s);
}
/* 字母 S */
static void draw_S(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) draw_block(r, x + i * s, y, s);         // 顶
    for (int i = 0; i < 3; i++) draw_block(r, x, y + i * s, s);         // 左上
    for (int i = 0; i < 5; i++) draw_block(r, x + i * s, y + 2 * s, s); // 中
    for (int i = 2; i < 5; i++) draw_block(r, x + 4 * s, y + i * s, s); // 右下
    for (int i = 0; i < 5; i++) draw_block(r, x + i * s, y + 4 * s, s); // 底
}

/* 字母 T */
static void draw_T(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) draw_block(r, x + i * s, y, s);         // 顶
    for (int i = 0; i < 5; i++) draw_block(r, x + 2 * s, y + i * s, s); // 柱
}

/* 字母 L */
static void draw_L(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) draw_block(r, x, y + i * s, s);         // 柱
    for (int i = 1; i < 5; i++) draw_block(r, x + i * s, y + 4 * s, s); // 底
}

/* 字母 H */
static void draw_H(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) {
        draw_block(r, x, y + i * s, s);
        draw_block(r, x + 4 * s, y + i * s, s);
    }
    for (int i = 1; i < 4; i++) draw_block(r, x + i * s, y + 2 * s, s);
}
/* 字母 R */
static void draw_R(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) draw_block(r, x, y + i * s, s);         // 柱
    for (int i = 0; i < 4; i++) draw_block(r, x + i * s, y, s);         // 顶
    draw_block(r, x + 4 * s, y + s, s);                                 // 右上
    for (int i = 0; i < 4; i++) draw_block(r, x + i * s, y + 2 * s, s); // 中
    draw_block(r, x + 2 * s, y + 3 * s, s);                             // 腿1
    draw_block(r, x + 4 * s, y + 4 * s, s);                             // 腿2
}

/* 字母 V */
static void draw_V(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 4; i++) {
        draw_block(r, x, y + i * s, s);         // 左
        draw_block(r, x + 4 * s, y + i * s, s); // 右
    }
    draw_block(r, x + 2 * s, y + 4 * s, s);     // 底尖
}

/* 字母 X */
static void draw_X(SDL_Renderer* r, int x, int y, int s)
{
    draw_block(r, x, y, s);
    draw_block(r, x + 4 * s, y, s);
    draw_block(r, x + s, y + s, s);
    draw_block(r, x + 3 * s, y + s, s);
    draw_block(r, x + 2 * s, y + 2 * s, s);
    draw_block(r, x + s, y + 3 * s, s);
    draw_block(r, x + 3 * s, y + 3 * s, s);
    draw_block(r, x, y + 4 * s, s);
    draw_block(r, x + 4 * s, y + 4 * s, s);
}
/* 字母 G */
static void draw_G(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) {
        draw_block(r, x + i * s, y, s);         // 顶
        draw_block(r, x, y + i * s, s);         // 左
        draw_block(r, x + i * s, y + 4 * s, s); // 底
    }
    draw_block(r, x + 4 * s, y + 2 * s, s);
    draw_block(r, x + 4 * s, y + 3 * s, s);
    draw_block(r, x + 3 * s, y + 2 * s, s);
}

/* 字母 M */
static void draw_M(SDL_Renderer* r, int x, int y, int s) {
    for (int i = 0; i < 5; i++) {
        draw_block(r, x, y + i * s, s);
        draw_block(r, x + 4 * s, y + i * s, s);
    }
    draw_block(r, x + s, y + s, s);
    draw_block(r, x + 2 * s, y + 2 * s, s);
    draw_block(r, x + 3 * s, y + s, s);
}
/* 字母 O */
static void draw_O(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++)
    {
        draw_block(r, x + i*s, y, s);
        draw_block(r, x + i*s, y + 4*s, s);
    }
    draw_block(r, x, y + s, s);
    draw_block(r, x, y + 2*s, s);
    draw_block(r, x, y + 3*s, s);
    draw_block(r, x + 4*s, y + s, s);
    draw_block(r, x + 4*s, y + 2*s, s);
    draw_block(r, x + 4*s, y + 3*s, s);
}

/* 字母 U */
static void draw_U(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 4; i++)
    {
        draw_block(r, x, y + i*s, s);
        draw_block(r, x + 4*s, y + i*s, s);
    }
    for (int i = 1; i < 4; i++)
        draw_block(r, x + i*s, y + 4*s, s);
}

/* 字母 W */
static void draw_W(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++)
    {
        draw_block(r, x, y + i*s, s);
        draw_block(r, x + 4*s, y + i*s, s);
    }
    draw_block(r, x + 2*s, y + 3*s, s);
    draw_block(r, x + s, y + 4*s, s);
    draw_block(r, x + 3*s, y + 4*s, s);
}

/* 字母 I */
static void draw_I(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++)
        draw_block(r, x + 2*s, y + i*s, s);
}

/* 字母 N */
static void draw_N(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++)
    {
        draw_block(r, x, y + i*s, s);
        draw_block(r, x + 4*s, y + i*s, s);
        draw_block(r, x + i*s, y + i*s, s);
    }
}

/* 主渲染函数 */
void render_win_screen(SDL_Renderer* renderer)
{
    // 黑屏
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 白色文字
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int s = 10;              // 像素块大小
    int gap = s * 2;         // 字母间距
    int word_w = 6 * 5 * s + 5 * gap; // YOU WIN 总宽度
    int start_x = (800 - word_w) / 2;
    int y = 260;

    int x = start_x;
    draw_Y(renderer, x, y, s); x += 5*s + gap;
    draw_O(renderer, x, y, s); x += 5*s + gap;
    draw_U(renderer, x, y, s); x += 5*s + gap * 2;

    draw_W(renderer, x, y, s); x += 5*s + gap;
    draw_I(renderer, x, y, s); x += 5*s + gap;
    draw_N(renderer, x, y, s);

    SDL_RenderPresent(renderer);
}

static void draw_D(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) {
        draw_block(r, x, y + i * s, s);
        if (i == 0 || i == 4) {
            for (int j = 1; j < 4; j++) draw_block(r, x + j * s, y + i * s, s);
        }
        else {
            draw_block(r, x + 4 * s, y + i * s, s);
        }
    }
}

/* 字母 E */
static void draw_E(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) {
        draw_block(r, x, y + i * s, s);
        if (i == 0 || i == 2 || i == 4) {
            for (int j = 1; j < 5; j++) draw_block(r, x + j * s, y + i * s, s);
        }
    }
}

/* 字母 A */
static void draw_A(SDL_Renderer* r, int x, int y, int s)
{
    for (int i = 0; i < 5; i++) {
        draw_block(r, x, y + i * s, s);
        draw_block(r, x + 4 * s, y + i * s, s);
        if (i == 0 || i == 2) {
            for (int j = 1; j < 4; j++) draw_block(r, x + j * s, y + i * s, s);
        }
    }
}

/* 死亡界面主渲染函数 */
void render_dead_screen(SDL_Renderer* renderer)
{
    // 1. 背景变暗（深红色调）
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. 红色文字
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int s = 10;
    int gap = s * 2;
    int start_x = 100;
    int y = 260;

    int x = start_x;
    // 绘制 YOU
    draw_Y(renderer, x, y, s); x += 5 * s + gap;
    draw_O(renderer, x, y, s); x += 5 * s + gap;
    draw_U(renderer, x, y, s); x += 5 * s + gap * 3;

    // 绘制 DEAD
    draw_D(renderer, x, y, s); x += 5 * s + gap;
    draw_E(renderer, x, y, s); x += 5 * s + gap;
    draw_A(renderer, x, y, s); x += 5 * s + gap;
    draw_D(renderer, x, y, s);

    SDL_RenderPresent(renderer);
}

void render_main_menu(SDL_Renderer* renderer, int mx, int my)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    int s_title = 12; // 游戏标题像素大小
    int s_btn = 5;    // 按钮像素大小
    int gap = 15;

    // 2. 绘制游戏名称: "STEALTH" (居中)
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // 标题青色
    int title_x = 180;
    int title_y = 100;
    draw_S(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_T(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_E(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_A(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_L(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_T(renderer, title_x, title_y, s_title); title_x += 5 * s_title + gap;
    draw_H(renderer, title_x, title_y, s_title);

    SDL_Point mouse = { mx, my };

    // 3. 按钮：START (开始游戏)
    if (SDL_PointInRect(&mouse, &btn_start)) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255);
    // 简写绘制 START
    int bx = 330, by = 310;
    draw_S(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_T(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_A(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_R(renderer, bx, by, s_btn); bx += 6 * s_btn; // 需要补一个R
    draw_T(renderer, bx, by, s_btn);

    // 4. 按钮：LEVELS (选择关卡)
    bx = 330; by = 370;
    if (SDL_PointInRect(&mouse, &btn_level)) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
    draw_L(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_E(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_V(renderer, bx, by, s_btn); bx += 6 * s_btn; // 需要补一个V
    draw_E(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_L(renderer, bx, by, s_btn);

    // 5. 按钮：EXIT (退出)
    bx = 330; by = 430;
    if (SDL_PointInRect(&mouse, &btn_exit)) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else SDL_SetRenderDrawColor(renderer, 231, 76, 60, 255);
    draw_E(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_X(renderer, bx, by, s_btn); bx += 6 * s_btn; // 需要补一个X
    draw_I(renderer, bx, by, s_btn); bx += 6 * s_btn;
    draw_T(renderer, bx, by, s_btn);

    SDL_RenderPresent(renderer);
}


void render_level_select(SDL_Renderer* renderer, int mx, int my) 
{
    // 背景深蓝色
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);

    SDL_Point mouse = { mx, my };

    // 关卡方块定义 (使用外部声明的变量)
    SDL_Rect lvls[] = { lvl1, lvl2, lvl3 };

    for (int i = 0; i < 3; i++) {
        // 1. 画背景方块
        if (SDL_PointInRect(&mouse, &lvls[i]))
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 悬停变白
        else
            SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255); // 默认灰色

        SDL_RenderFillRect(renderer, &lvls[i]);

        // 2. 画数字 (黑色，放在方块中心)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        int num_s = 8; // 数字的像素大小
        int num_x = lvls[i].x + (lvls[i].w - 5 * num_s) / 2;
        int num_y = lvls[i].y + (lvls[i].h - 5 * num_s) / 2;

        if (i == 0) draw_1(renderer, num_x, num_y, num_s);
        else if (i == 1) draw_2(renderer, num_x, num_y, num_s);
        else if (i == 2) draw_3(renderer, num_x, num_y, num_s);
    }

    SDL_RenderPresent(renderer);
}

void render_all_completed_screen(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
    SDL_RenderClear(renderer);

    // 绘制"ALL LEVELS COMPLETED!"
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int s = 8;
    int gap = s * 2;
    int start_x = 100;
    int y = 260;

    // 这里可以添加绘制"ALL COMPLETE"的文字
    // 需要实现对应的字母绘制函数
    // ...

    SDL_RenderPresent(renderer);
}
