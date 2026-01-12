//管理一切动态对象
#ifndef ENTITY_H
#define ENTITY_H

typedef enum
{
	ENTITY_PLAYER,   //玩家实体
    ENTITY_ENEMY      //敌人实体
}EntityType;
typedef enum
{
	STATE_PATROL, //巡逻
	STATE_CHASE,//追逐
	STATE_RETURN, //平滑回归
	STATE_SEARCH  //搜索
}AIState;

typedef struct Entity
{
	float x, y;    //世界坐标
	int w, h;      //碰撞盒大小
	EntityType type;//类型（玩家/敌人）
	AIState state;
	struct Entity* next;    //链表指针
	int hidden;      //玩家专用：是否在掩体中
	int win;         //玩家专用：是否胜利
	int dead ; //0为活着，1为被抓
	float start_x, start_y;
	int search_timer;   // 搜索剩余时间（帧数）
	float target_x;     // 玩家最后出现的 X 坐标（消失点）
	float target_y;     // 玩家最后出现的 Y 坐标
	float dx, dy;       // 当前的移动方向速度（用于平滑游走）
	int hide_cooldown; // 识破后的禁隐时间
	int stuck_timer;         // 累计卡在墙上的时间
	float last_positions_x[5];
	float last_positions_y[5];
	int pos_index;
	int action_pause_timer; // 新增：用于动作暂停的帧数计时
}Entity;
//要创建的实体
Entity* entity_create(EntityType type,   //实体类型
	float x,                             //初始x坐标
	float y,                              //初始y坐标
	int w,                                //宽度
	int h                                 //高度
);
//把实体加入链表
void entity_add(Entity** list,             //实体链表头指针
	Entity* e                             //要加入的实体
);
//释放实体链表
void entity_free_all(Entity* list);      //链表头    

#endif // !ENTITY_H
