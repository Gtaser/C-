#pragma once
#include <SDL.h>
extern SDL_Rect btn_start;
extern SDL_Rect btn_level;
extern SDL_Rect btn_exit;
// ¹Ø¿¨·½¿é¾ØÐÎ
extern SDL_Rect lvl1;
extern SDL_Rect lvl2;
extern SDL_Rect lvl3;
void render_win_screen(SDL_Renderer* renderer);
void render_dead_screen(SDL_Renderer* renderer);
void render_main_menu(SDL_Renderer* renderer, int mx, int my);
void render_level_select(SDL_Renderer* renderer, int mx, int my);
void render_all_completed_screen(SDL_Renderer* renderer);