/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "memory.h"
#include "DSP.h"
#include "adc.h"
#include "tim.h"
#include "spi.h"
#include "ui.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_NUM 1024
#define FFT_LENGTH ADC_NUM
#define FFT_LENGTH_MAX 4096
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for lvglTask */
osThreadId_t lvglTaskHandle;
const osThreadAttr_t lvglTask_attributes = {
  .name = "lvglTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for spiTask */
osThreadId_t spiTaskHandle;
const osThreadAttr_t spiTask_attributes = {
  .name = "spiTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

#define spi_buffer_size 1024
//__attribute__((section(".d3")))
uint16_t spi_buffer_rx[spi_buffer_size];
//__attribute__((section(".d3")))
uint16_t spi_buffer_tx[spi_buffer_size];
uint16_t spi_packet_length=spi_buffer_size;
bool spi_packet_Cplt=false;
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if(hspi ->Instance == SPI2||hspi ->Instance == SPI3){
        spi_packet_Cplt = true;
    }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC3) {
        //ADC_ConvCplt=1;
    }
}


/* USER CODE END FunctionPrototypes */

void lvglTask_cb(void *argument);
void adcTask_cb(void *argument);
void spiTask_cb(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void vApplicationTickHook(void) {
    /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
    lv_tick_inc(1);
}
/* USER CODE END 3 */

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

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of lvglTask */
  lvglTaskHandle = osThreadNew(lvglTask_cb, NULL, &lvglTask_attributes);

  /* creation of adcTask */
//  adcTaskHandle = osThreadNew(adcTask_cb, NULL, &adcTask_attributes);

  /* creation of spiTask */
  spiTaskHandle = osThreadNew(spiTask_cb, NULL, &spiTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_lvglTask_cb */
/**
  * @brief  Function implementing the lvglTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_lvglTask_cb */
void lvglTask_cb(void *argument)
{
  /* USER CODE BEGIN lvglTask_cb */
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
      lv_demo_benchmark();
//    lv_demo_widgets();
    /* Infinite loop */
    for (;;) {
        lv_timer_handler();
        osDelay(1);
    }
  /* USER CODE END lvglTask_cb */
}

/* USER CODE BEGIN Header_adcTask_cb */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adcTask_cb */
void adcTask_cb(void *argument)
{
  /* USER CODE BEGIN adcTask_cb */

    /* Infinite loop */
    for (;;) {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
    }
  /* USER CODE END adcTask_cb */
}

/* USER CODE BEGIN Header_spiTask_cb */
/**
* @brief Function implementing the spiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_spiTask_cb */
void spiTask_cb(void *argument)
{
  /* USER CODE BEGIN spiTask_cb */

  for (int i = 0; i < spi_packet_length; ++i) {
      spi_buffer_tx[i]= (uint16_t)((arm_sin_f32(1.0*i/spi_packet_length*2*PI)+1)/2*0xFFFF);
  }
  if(HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)spi_buffer_tx,(uint8_t *)spi_buffer_rx, spi_packet_length) != HAL_OK)
  {
      printf("spi faild\r\n");
  }
  /* Infinite loop */
  for(;;)
  {

      if(spi_packet_Cplt){
          for (int i = 0; i < spi_packet_length; ++i) {
              printf("d:%d\r\n",spi_buffer_rx[i]);
          }
//          printf("ok\r\n");
          if(HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)spi_buffer_tx,(uint8_t *)spi_buffer_rx, spi_packet_length) != HAL_OK)
          {
              printf("spi faild\r\n");
          }
          spi_packet_Cplt=!spi_packet_Cplt;
      }
    osDelay(1);
  }
  /* USER CODE END spiTask_cb */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

