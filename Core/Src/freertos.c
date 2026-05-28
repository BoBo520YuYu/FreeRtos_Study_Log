/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ButterTask */
osThreadId_t ButterTaskHandle;
const osThreadAttr_t ButterTask_attributes = {
  .name = "ButterTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Data_Task */
osThreadId_t Data_TaskHandle;
const osThreadAttr_t Data_Task_attributes = {
  .name = "Data_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ButQueue */
osMessageQueueId_t ButQueueHandle;
const osMessageQueueAttr_t ButQueue_attributes = {
  .name = "ButQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartButterTask(void *argument);
void StartData_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ButQueue */
  ButQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &ButQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ButterTask */
  ButterTaskHandle = osThreadNew(StartButterTask, NULL, &ButterTask_attributes);

  /* creation of Data_Task */
  Data_TaskHandle = osThreadNew(StartData_Task, NULL, &Data_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartButterTask */
/**
  * @brief  Function implementing the ButterTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartButterTask */
void StartButterTask(void *argument)
{
  uint16_t But_count = 0;
  
  /* USER CODE BEGIN StartButterTask */
  /* Infinite loop */
  for(;;)
  {
    if(HAL_GPIO_ReadPin(Butter_GPIO_Port, Butter_Pin) == GPIO_PIN_RESET)
    {
      osDelay(20);  // 延时20ms，等待按钮稳定
      if(HAL_GPIO_ReadPin(Butter_GPIO_Port, Butter_Pin) == GPIO_PIN_RESET)
      {
        But_count++; 
        osMessageQueuePut(ButQueueHandle, &But_count, 0, osWaitForever);
      }
      while(HAL_GPIO_ReadPin(Butter_GPIO_Port, Butter_Pin) == GPIO_PIN_RESET)
      {
        osDelay(10);   //等待按钮释放
      }
    }
    else
    {
      osDelay(10);
    }
  }
  /* USER CODE END StartButterTask */
}

/* USER CODE BEGIN Header_StartData_Task */
/**
* @brief Function implementing the Data_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartData_Task */
void StartData_Task(void *argument)
{
  /* USER CODE BEGIN StartData_Task */
  uint16_t Data_count = 0;
  uint16_t butter_count = 0;
  /* Infinite loop */
  for(;;)
  {
    osMessageQueueGet(ButQueueHandle,&butter_count,0,osWaitForever);
    Data_count++;
    osDelay(1000);
    printf("butter_count: %d, Data_count: %d\r\n",butter_count,Data_count);
  }
  /* USER CODE END StartData_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */