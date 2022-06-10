/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "step.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	/*全局变量，便于debug-watch-window*/
	SmodeControl s1;  //1号步进电机控制曲线结构体
	SmodeControl s2;  //2号步进电机控制曲线结构体

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
		
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */   
	step_infor step1;     //步进电机1参数设置结构体
	step_infor step2;     //步进电机2参数设置结构体
	

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
	/*1号步进电机参数设置*/
	step1.direction = 1; //方向  
	step1.pulse_rev = 8; //细分数
	step1.step_id = 1;   //电机ID
	
	s1.flexible= 1;			 //S曲线圆滑度
	s1.max_speed = 500;  //最大速度限幅
	s1.min_speed = 0;    //最小速度限幅
	s1.step_speed = 0.1; //速度单次调节量（步长）

	/*2号步进电机参数设置*/
	step2.direction = 1; 
	step2.pulse_rev = 8;
	step2.step_id = 2;
	
	s2.flexible= 1;
	s2.max_speed = 500;
	s2.min_speed = 0;
	s2.step_speed = 0.1;   
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);  //产生PWM波
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
	  /*1号0->5，2号0->10*/
		s1.start_speed = 0;
		s1.current_speed = 0;
		s1.traget_speed = 5;
		s2.start_speed = 0;
		s2.current_speed = 0;
		s2.traget_speed = 10;
		while(!(s2.current_speed>9)){
			
			if(s1.current_speed<6){   //1号电机是否加速到目标速度
				step_control(&s1);      //计算当前控制速度
				step1.step_speed = s1.current_speed;
				step_run(step1.step_id, step1.direction, step1.step_speed, step1.pulse_rev);  //控制速度
			}
			step_control(&s2);
			step2.step_speed = s2.current_speed;
		  step_run(step2.step_id, step2.direction, step2.step_speed, step2.pulse_rev);
			htim1.Instance->CCR1 = 0.5 * TIM1->ARR;    //保持50%占空比，等宽脉冲波              
			htim2.Instance->CCR1 = 0.5 * TIM2->ARR;
			HAL_Delay(10);
		}
		HAL_Delay(1000); //保持1S
		/*1号5~20，2号10->40*/
		s1.start_speed = s1.current_speed; //开始调速时速度等于当前速度
		s1.traget_speed = 20;         //目标速度
		s2.start_speed = s2.current_speed;
		s2.traget_speed = 40;
		while(!(s2.current_speed>39)){
			if(s1.current_speed < 21){
				step_control(&s1);
				step1.step_speed = s1.current_speed;
				step_run(step1.step_id, step1.direction, step1.step_speed, step1.pulse_rev);
			}
			step_control(&s2);
			step2.step_speed = s2.current_speed;
			step_run(step2.step_id, step2.direction, step2.step_speed, step2.pulse_rev);
			htim1.Instance->CCR1 = 0.5 * TIM1->ARR;
			htim2.Instance->CCR1 = 0.5 * TIM2->ARR;
			HAL_Delay(10);
		}
		HAL_Delay(1000); //保持1S
		/*1号20~50，2号40->100*/
		s1.start_speed = s1.current_speed;
		s1.traget_speed = 50;
		s2.start_speed = s2.current_speed;
		s2.traget_speed = 100;
		while(!(s2.current_speed>99)){
			if(s1.current_speed < 51){
				step_control(&s1);
				step1.step_speed = s1.current_speed;
				step_run(step1.step_id, step1.direction, step1.step_speed, step1.pulse_rev);
			}
			step_control(&s2);
			step2.step_speed = s2.current_speed;
			step_run(step2.step_id, step2.direction, step2.step_speed, step2.pulse_rev);
			htim1.Instance->CCR1 = 0.5 * TIM1->ARR;
			htim2.Instance->CCR1 = 0.5 * TIM2->ARR;
			HAL_Delay(10);
		}
		HAL_Delay(1000);   //保持1s
		/*1号50~25，2号100->50*/
		s1.start_speed = s1.current_speed;
		s1.traget_speed = 25;
		s2.start_speed = s2.current_speed;
		s2.traget_speed = 50;
		while(!(s2.current_speed < 51)){
			if(s1.current_speed > 24){
				step_control(&s1);
				step1.step_speed = s1.current_speed;
				step_run(step1.step_id, step1.direction, step1.step_speed, step1.pulse_rev);
			}
			step_control(&s2);
			step2.step_speed = s2.current_speed;
			step_run(step2.step_id, step2.direction, step2.step_speed, step2.pulse_rev);
			htim1.Instance->CCR1 = 0.5 * TIM1->ARR;
			htim2.Instance->CCR1 = 0.5 * TIM2->ARR;
			HAL_Delay(10);
		}
		
		HAL_Delay(1000);
		/*降为0*/
		s1.start_speed = s1.current_speed;
		s1.traget_speed = 0;
		s2.start_speed = s2.current_speed;
		s2.traget_speed = 0;
		while(!(s2.current_speed <=0)){
				step_control(&s1);
			  step1.step_speed = s1.current_speed;
				step_control(&s2);
			  step2.step_speed = s2.current_speed;
				step_run(step1.step_id, step1.direction, step1.step_speed, step1.pulse_rev);
				step_run(step2.step_id, step2.direction, step2.step_speed, step2.pulse_rev);
				htim1.Instance->CCR1 = 0.5 * TIM1->ARR;
				htim2.Instance->CCR1 = 0.5 * TIM2->ARR;
				HAL_Delay(10);
		}
		
		/*停止*/
		htim1.Instance->CCR1 = 0;
		htim2.Instance->CCR1 = 0;
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);  //停止PWM波
	  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
		while(1){
				;
		}
	

  } 
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
