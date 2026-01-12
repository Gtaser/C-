//SDL初始化 主循环 定义游戏常量 绘图函数，定义游戏元素
#include<stdio.h>
#include<SDL.h>
#include"input.h"               //输入模块
#include"render.h"              //渲染模块
#include"player.h"              //玩家模块
#include"map.h"
#include"world.h"
#include "ui.h"
#include "config.h"
#undef main
typedef enum 
{
	ST_MENU, 
	ST_LEVELS, 
	ST_PLAYING,
} GameState;

int main()
{ /* 初始化 SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return 1;
	}
	SDL_Window* window = SDL_CreateWindow(
		//窗口设置  
		"STEALTH",    //窗口标题
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN);
	if (!window)
	{
		SDL_Log("CreateWindow failed: %s", SDL_GetError());
		return 1;
	}
	                                             //关联窗口 默认驱动 硬件加速 
	SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!render)
	{
		SDL_Log("CreateRenderer failed: %s", SDL_GetError());

		/* 回退方案：用软件渲染器 */
		render = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_SOFTWARE
		);
		if (!render)
		{
			SDL_Log("Software renderer also failed: %s", SDL_GetError());
			return 1;
		}
	}
	if (!map_load_from_file("map.txt"))
	{
		
		SDL_Log("无法加载地图文件 map.txt");
		SDL_Delay(3000);
		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
		int running = 1;       //程序状态，控制主程序
		GameState state = ST_MENU; // 默认开始界面
		InputState input = { 0 };//当前帧的输入快照
		World world;
		world_init(&world);
		int current_level = 0; // 当前关卡 (0=第一关, 1=第二关, 2=第三关)
		
		Entity* player_entity = world.entities; // 第一个实体就是玩家
		const char* level_files[3] = { "map.txt", "map2.txt", "map3.txt" };
		while (running)
		{
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			SDL_Point m_ptr = { mx, my };

			SDL_Event ev;
			while (SDL_PollEvent(&ev)) {
				if (ev.type == SDL_QUIT)
					running = 0;

				// 状态机点击判定
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					if (state == ST_MENU) 
					{
						if (SDL_PointInRect(&m_ptr, &btn_start))
						{
							current_level = 0;
							world_load_level(&world, current_level);
							player_entity = world.entities;
							state = ST_PLAYING;
						}
						else if (SDL_PointInRect(&m_ptr, &btn_level)) 
							state = ST_LEVELS;
						else if (SDL_PointInRect(&m_ptr, &btn_exit)) 
							running = 0;
					}
					
				
				  else if (state == ST_LEVELS) 
				  {
					// 点击左上角返回菜单
					if (mx < 50 && my < 50) 
					{
						state = ST_MENU;
					}
					// 点击关卡1
					else if (SDL_PointInRect(&m_ptr, &lvl1)) 
					{
						current_level = 0;
						world_load_level(&world, current_level);
						player_entity = world.entities;
						state = ST_PLAYING;
					}
					// 点击关卡2
					else if (SDL_PointInRect(&m_ptr, &lvl2)) 
					{
						current_level = 1;
						world_load_level(&world, current_level);
						player_entity = world.entities;
						state = ST_PLAYING;
					}
					// 点击关卡3
					else if (SDL_PointInRect(&m_ptr, &lvl3)) 
					{
						current_level = 2;
						world_load_level(&world, current_level);
						player_entity = world.entities;
						state = ST_PLAYING;
					}
				  }
				  else if (state == ST_PLAYING)
					{
						// 如果玩家死亡或胜利，点击屏幕继续
						if (player_entity && (player_entity->dead || player_entity->win))
						{
							if (player_entity->dead)
							{
								// 死亡：重新开始当前关卡
								world_load_level(&world, current_level);
								player_entity = world.entities;
							}
							else if (player_entity->win)
							{
								// 胜利：进入下一关
								current_level++;
								if (current_level >= 3)
								{
									// 所有关卡完成，返回主菜单
									current_level = 0;
									state = ST_MENU;
								}
								else {
									// 加载下一关
									world_load_level(&world, current_level);
									player_entity = world.entities;
								}
							}
						}
				  }
				}
			}
		
			
		


			// 核心分支逻辑
			if (state == ST_MENU)
			{
				render_main_menu(render, mx, my);
			}
			else if (state == ST_LEVELS)
			{
				render_level_select(render, mx, my);
			}
			else if (state == ST_PLAYING) 
			{
				input_update(&input, &running);

				if (player_entity && !player_entity->win && !player_entity->dead) 
				{
					entity_update_player(player_entity, world.entities, input.up, input.down, input.left, input.right);
					world_update(&world);
					render_world(render, &world);
				}
				else if (player_entity->dead) 
				{
					render_dead_screen(render);
					SDL_RenderPresent(render);
				}
				else {
					render_win_screen(render);
					SDL_RenderPresent(render);
					// 等待点击（在事件处理中处理
				}
			}
			SDL_Delay(16);
		}

		
		world_cleanup(&world);
		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(window);
		SDL_Quit();      //资源释放
		return 0;
	
}