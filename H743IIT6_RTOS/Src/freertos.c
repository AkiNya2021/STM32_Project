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
#define spi_buffer_size 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
float ADC1_Fs = 100.0f * 1000.0f;
float ADC2_Fs = 100.0f * 1000.0f;
uint32_t TIM2_period;
uint32_t TIM4_period;
bool ADC1_ConvCplt = false;
bool ADC2_ConvCplt = false;
__attribute__((section(".d3"))) uint16_t ADC1Value[ADC_NUM]={0};
__attribute__((section(".d3"))) uint16_t ADC2Value[ADC_NUM]={0};
float ADC1Sample[ADC_NUM];
float ADC2Sample[ADC_NUM];

//__attribute__((section(".d3")))
uint16_t spi_buffer_rx[spi_buffer_size];
//__attribute__((section(".d3")))
uint16_t spi_buffer_tx[spi_buffer_size];
uint16_t spi_packet_length=spi_buffer_size;
bool spi_packet_Cplt = false;

/* USER CODE END Variables */
/* Definitions for lvglTask */
osThreadId_t lvglTaskHandle;
const osThreadAttr_t lvglTask_attributes = {
  .name = "lvglTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
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

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    if(hspi ->Instance == SPI2||hspi ->Instance == SPI3){
        spi_packet_Cplt = true;
    }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1){
        ADC1_ConvCplt = true;
    }
    if(hadc->Instance==ADC2){
        ADC2_ConvCplt = true;
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
//    lv_demo_benchmark();
    lv_demo_widgets();
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
  TIM2_period=(uint32_t) (10000000.0/ADC1_Fs);
  TIM4_period=(uint16_t) (10000000.0/ADC1_Fs);
  __HAL_TIM_SET_AUTORELOAD(&htim2, TIM2_period - 1);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, TIM2_period/2);
  HAL_TIM_Base_Start(&htim2);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC1Value, ADC_NUM);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

  __HAL_TIM_SET_AUTORELOAD(&htim4, TIM4_period - 1);
  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4, TIM4_period/2);
  HAL_TIM_Base_Start(&htim4);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *) ADC2Value, ADC_NUM);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    /* Infinite loop */
    for (;;) {
        if (ADC1_ConvCplt==1) {
            //ADC采样数据转换实际电压幅度
            SCB_InvalidateDCache_by_Addr((uint32_t*)ADC1Value, sizeof(ADC1Value));
            for (int i = 0; i < ADC_NUM; i++) {
                ADC1Sample[i] = (float) ADC1Value[i] / 65535.0f * 6.6f - 3.3f;
                printf("wave:%.3f\r\n",ADC1Sample[i]);
            }
            ADC1_ConvCplt=0;
            HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC1Value, ADC_NUM);
        }
        if (ADC2_ConvCplt==1) {
            //ADC采样数据转换实际电压幅度
            SCB_InvalidateDCache_by_Addr((uint32_t*)ADC2Value, sizeof(ADC2Value));
            for (int i = 0; i < ADC_NUM; i++) {
                ADC2Sample[i] = (float) ADC2Value[i] / 65535.0f * 6.6f - 3.3f;
                printf("wave:%.3f\r\n",ADC2Sample[i]);
            }
            ADC2_ConvCplt=0;
            HAL_ADC_Start_DMA(&hadc2, (uint32_t *) ADC2Value, ADC_NUM);
        }
        osDelay(1);
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
  //测试数据
//  for (int i = 0; i < spi_packet_length; ++i) {
//      spi_buffer_tx[i]= (uint16_t)((arm_sin_f32(1.0*i/spi_packet_length*2*PI)+1)/2*0xFFFF);
//  }

  uint16_t adc_select;
  uint64_t sample_freq;
  uint16_t sample_num;
  uint16_t cycle_times;
  uint16_t cycle_points;

  adc_select=0xADC0;
  sample_freq=10000000;
  cycle_times=1;
  cycle_points=20;
  sample_num=cycle_points*cycle_times;

  spi_packet_Cplt=false;
  spi_buffer_tx[0]=0xABCD;
  spi_buffer_tx[1]=0xFADC;
  spi_buffer_tx[2]=0xAF00;
  spi_buffer_tx[3]=adc_select; //选择ADC
  spi_buffer_tx[4]=0xAF01;
  spi_buffer_tx[5]=sample_freq / 0xFFFF;      //设置采样频率
  spi_buffer_tx[6]=sample_freq % (0xFFFF+1);
  spi_buffer_tx[7]=0xAF02;
  spi_buffer_tx[8]=sample_num;                //设置采样点数
  spi_buffer_tx[9]=0xCDEF;
  spi_buffer_tx[spi_buffer_size-1]=0xDCAB;
  //开启SPI传输
  HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)spi_buffer_tx,(uint8_t *)spi_buffer_rx, spi_packet_length);
  /* Infinite loop */
  for(;;)
  {
      if(spi_packet_Cplt){
          //打印接收数据
          for (int i = 0; i < spi_packet_length; ++i) {
              //printf("d:%d\r\n",spi_buffer_rx[i]);
              printf("[%d]txd:0x%04x rxd:0x%04x\r\n",i,spi_buffer_tx[i],spi_buffer_rx[i]);
          }
//          for (int i = 2; i < spi_packet_size-1; ++i) {
//              //        printf("[%llu][%d]txd:0x%04x rxd:0x%04x\r\n",scan_freq,i,spi_txd[i],spi_rxd[i]);
//              if(spi_rxd[i-2]==0x0000&&spi_rxd[i-1]==0xABCD) wave_data_flg=1;
//              if(wave_data_flg){
//                  *((uint16_t*)&wave_data[k])=spi_rxd[i];
//                  k++;
//              }
//              if(spi_rxd[i+1]==0xDCBA&&spi_rxd[i+2]==0x0000) wave_data_flg=0;
//          }
          HAL_Delay(1500);
          //再次发送
          HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)spi_buffer_tx,(uint8_t *)spi_buffer_rx, spi_packet_length);
          spi_packet_Cplt=!spi_packet_Cplt;
      }
    osDelay(1);
  }
  /* USER CODE END spiTask_cb */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

