#ifndef _STEP__H_
#define _STEP__H_
#include "tim.h"

typedef struct {

	char direction;  //方向
	int pulse_rev;   //细分数
	float step_speed; //转数
	char step_id;    //电机ID
}step_infor;  

typedef struct{
	float start_speed;     //开始速度
	float current_speed;   //当前速度
	float traget_speed;    //目标速度
	float step_speed;      //速度调节分辨率
	float max_speed;       //最大速度
	float min_speed;       //最小速度
	uint32_t atimes;       //调节次数
	uint32_t stimes;       //调阶总次数 
	float flexible;        //曲线曲度
}SmodeControl;

void step_run(char step_id, char dir, float step_speed, int pulse_rev);  //电机调速控制函数
void sControl(SmodeControl *spta);                                       //S曲线控制函数
void step_control(SmodeControl *smode);                                  //变速控制函数

#endif
