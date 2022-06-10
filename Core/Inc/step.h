#ifndef _STEP__H_
#define _STEP__H_
#include "tim.h"

typedef struct {

	char direction;  //����
	int pulse_rev;   //ϸ����
	float step_speed; //ת��
	char step_id;    //���ID
}step_infor;  

typedef struct{
	float start_speed;     //��ʼ�ٶ�
	float current_speed;   //��ǰ�ٶ�
	float traget_speed;    //Ŀ���ٶ�
	float step_speed;      //�ٶȵ��ڷֱ���
	float max_speed;       //����ٶ�
	float min_speed;       //��С�ٶ�
	uint32_t atimes;       //���ڴ���
	uint32_t stimes;       //�����ܴ��� 
	float flexible;        //��������
}SmodeControl;

void step_run(char step_id, char dir, float step_speed, int pulse_rev);  //������ٿ��ƺ���
void sControl(SmodeControl *spta);                                       //S���߿��ƺ���
void step_control(SmodeControl *smode);                                  //���ٿ��ƺ���

#endif
