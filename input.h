#ifndef INPUT_H
#define INPUT_H
typedef struct  
{
	int up;
	int down;
	int left;
	int right;
}InputState;
void input_update(InputState* input, int* runing);
#endif
 