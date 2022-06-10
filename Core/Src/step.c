#include "step.h"
#include "tim.h"
#include "gpio.h"
#include "math.h"


/*@description: ������ٿ��ƺ���
*@params: ���ID������0/1�������ת�٣�r/min����������ϸ����
*@return: None
*@date: 2022.5
*/
void  step_run(char step_id, char dir, float step_speed, int pulse_rev){
	
	float f = 3.333*pulse_rev*step_speed;   //��ת�١�ϸ�����õ�Ƶ��
	int arr = 1000000 / f;                  //Ԥ��Ƶϵ��Ϊ72��������Ƶ�ʷ��ƼĴ���ARRֵ
	if(step_id == 1){
		__HAL_TIM_SET_AUTORELOAD(&htim1,arr); //���üĴ���ֵ
		if(dir == 1){
			HAL_GPIO_WritePin(STEP1_DIR_PIN1_GPIO_Port, STEP1_DIR_PIN1_Pin, GPIO_PIN_SET); //����IO����
		}else{
			HAL_GPIO_WritePin(STEP1_DIR_PIN1_GPIO_Port, STEP1_DIR_PIN1_Pin, GPIO_PIN_RESET);
		}
	}else if(step_id == 2){
		__HAL_TIM_SET_AUTORELOAD(&htim2,arr);
		if(dir == 1){
			HAL_GPIO_WritePin(STEP2_DIR_PIN1_GPIO_Port, STEP2_DIR_PIN1_Pin, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(STEP2_DIR_PIN1_GPIO_Port, STEP2_DIR_PIN1_Pin, GPIO_PIN_RESET);
		}
		
	}else{
		__HAL_TIM_SET_AUTORELOAD(&htim1,arr);  //Ĭ�����ò������1
	}
}

/*@description: S���߿��ƺ���
*@params: ���ƶ���ṹ��
*@return: None
*@date: 2022.5
*/
void sControl(SmodeControl *spta){
	float power=0.0;
	float speed=0.0;
	/*�������� y = 1/( 1 + e^-x) */
	power=(2*((float)spta->atimes)-((float)spta->stimes))/((float)spta->stimes);
  power=(0.0-spta->flexible)*power;
  
  speed=1+expf(power);
  speed=(spta->traget_speed-spta->start_speed)/speed;
  spta->current_speed=speed+spta->start_speed;
  
  if(spta->current_speed>spta->max_speed){
     spta->current_speed=spta->max_speed;
  }
  
  if(spta->current_speed<spta->min_speed){
     spta->current_speed=spta->min_speed;
  }
}

/*@description: ���ٿ��ƺ���
*@params: ���ƶ���ṹ��
*@return: None
*@date: 2022.5
*/
void step_control(SmodeControl *smode){
	float tmp = 0;
	/*�޷�*/
	if(smode->traget_speed > smode->max_speed){
		smode->traget_speed = smode->max_speed;
	}
	/*�޷�*/
	if(smode->traget_speed < smode->min_speed){
		smode->traget_speed = smode->min_speed;
	}
	
	if((fabs(smode->current_speed - smode->start_speed) < smode->step_speed && smode->stimes == 0)){
		if(smode->start_speed < smode->min_speed){
			smode->start_speed = smode->min_speed;
		}
		
		tmp = fabs(smode->traget_speed - smode->start_speed);
		tmp = tmp / smode->step_speed;
		smode->stimes = (uint32_t) tmp + 1;
		smode->atimes = 0;
	}
		
	if(smode->atimes < smode->stimes){
			sControl(smode);//�����ٶȿ��ƺ���
			smode->atimes++;
	}else{
			smode->current_speed = smode->traget_speed;
			smode->stimes = 0;
			smode->atimes = 0;
	}

}





