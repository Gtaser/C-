#include<SDL.h>
#include"input.h"
#include<stdio.h>
void input_update(InputState* input, int* runing)
{
	SDL_Event e;// SDL 的事件结构，用来接收窗口/输入事件
	while (SDL_PollEvent(&e))// 循环读取所有 SDL 事件
	{
		if (e.type == SDL_QUIT)
			*runing = 0;
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	input->up = keys[SDL_SCANCODE_W];
	input->down  = keys[SDL_SCANCODE_S];
	input->left  = keys[SDL_SCANCODE_A];
	input->right = keys[SDL_SCANCODE_D];
}
