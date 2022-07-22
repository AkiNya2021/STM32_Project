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
#include "DSP.h"
#include "adc.h"
#include "tim.h"
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
wave base_wave;
wave wave_info[5];
uint8_t ADC_ConvCplt = 0;
uint8_t LVGL_DrawCplt = 1;
uint8_t VGA_Open = 0;
uint16_t adc_num=0;
//uint16_t ADCxValue[ADC_NUM];
__attribute__((section(".d3"))) uint16_t ADCxValue[ADC_NUM]={0};
float32_t ADCxSample[ADC_NUM];
float32_t fft_inputbuf[FFT_LENGTH_MAX * 2];// FFT
float32_t fft_outputbuf[FFT_LENGTH_MAX];   // FFT
float32_t wave_vpp[ADC_NUM/2];
float32_t wave_freq[ADC_NUM/2];

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
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC3) {
        ADC_ConvCplt=1;
    }
}
lv_chart_series_t *wave1;
lv_chart_cursor_t * cursor1;
lv_chart_cursor_t * cursor2;
lv_chart_cursor_t * cursor3;
lv_chart_cursor_t * cursor4;
lv_chart_cursor_t * cursor5;

static void event_cursor_chart_draw(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
        if(!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_CURSOR)) return;
        if(dsc->p1 == NULL || dsc->p2 == NULL || dsc->p1->y != dsc->p2->y) return;
        printf("x[%lu]y[%lu]id[%lu]\r\n",dsc->p2->x,dsc->p1->y,dsc->value);
        if(dsc->id==32||dsc->id==64||dsc->id==96||dsc->id==128||dsc->id==160){
         char buf[16];
         lv_snprintf(buf, sizeof(buf), "%d",dsc->value);

         lv_point_t size;
         lv_txt_get_size(&size, buf, LV_FONT_DEFAULT, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

         lv_area_t a;

         a.y2 = dsc->p1->y - 5;
         a.y1 = a.y2 - size.y - 10;
         a.x1 = dsc->p1->x + 10;
         a.x2 = a.x1 + size.x + 10;

         lv_draw_rect_dsc_t draw_rect_dsc;
         lv_draw_rect_dsc_init(&draw_rect_dsc);
         draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
         draw_rect_dsc.radius = 3;

         lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

         lv_draw_label_dsc_t draw_label_dsc;
         lv_draw_label_dsc_init(&draw_label_dsc);
         draw_label_dsc.color = lv_color_white();
         a.x1 += 5;
         a.x2 -= 5;
         a.y1 += 5;
         a.y2 -= 5;
         lv_draw_label(dsc->draw_ctx, &draw_label_dsc, &a, buf, NULL);
        }
    }
}

static void event_chart_draw(lv_event_t *e) {
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DRAW_PART_BEGIN){
        if (dsc->part == LV_PART_ITEMS) {
            if (!dsc->p1 || !dsc->p2)
                return;
        } else if (dsc->part == LV_PART_MAIN) {
            if (dsc->line_dsc == NULL || dsc->p1 == NULL || dsc->p2 == NULL)
                return;
            /*Vertical line*/
            if (dsc->p1->x == dsc->p2->x) {
                dsc->line_dsc->color = lv_palette_lighten(LV_PALETTE_GREY, 1);
                if (dsc->id == 0 || dsc->id == 5 || dsc->id == 10) {
                    dsc->line_dsc->width = 2;
                    dsc->line_dsc->dash_gap = 0;
                    dsc->line_dsc->dash_width = 0;
                } else {
                    dsc->line_dsc->width = 1;
                    dsc->line_dsc->dash_gap = 6;
                    dsc->line_dsc->dash_width = 6;
                }
            }
            /*Horizontal line*/
            else {
                if (dsc->id == 0 || dsc->id == 4 || dsc->id == 8) {
                    dsc->line_dsc->width = 2;
                    dsc->line_dsc->dash_gap = 0;
                    dsc->line_dsc->dash_width = 0;
                } else {
                    dsc->line_dsc->width = 2;
                    dsc->line_dsc->dash_gap = 6;
                    dsc->line_dsc->dash_width = 6;
                }
                dsc->line_dsc->color = lv_palette_lighten(LV_PALETTE_GREY, 1);
                //      if(dsc->id == 1  || dsc->id == 3) {
                //        dsc->line_dsc->color  = lv_palette_main(LV_PALETTE_GREEN);
                //      } else {
                //
                //      }
            }
        }
    }

}

/* USER CODE END FunctionPrototypes */

void lvglTask_cb(void *argument);
void adcTask_cb(void *argument);

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
  adcTaskHandle = osThreadNew(adcTask_cb, NULL, &adcTask_attributes);

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
    //  lv_demo_benchmark();
    //  lv_demo_widgets();
    ui_init();


    lv_chart_set_div_line_count(ui_WaveChart, 8 + 1, 10 + 1);
    lv_obj_add_event_cb(ui_WaveChart, event_chart_draw, LV_EVENT_DRAW_PART_BEGIN, NULL);
    lv_obj_add_event_cb(ui_WaveChart, event_cursor_chart_draw, LV_EVENT_DRAW_PART_END, NULL);

    lv_chart_set_axis_tick(ui_WaveChart, LV_CHART_AXIS_PRIMARY_X,  10, 5, 6, 5, true, 100);
    lv_chart_set_axis_tick(ui_WaveChart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 9, 5, true, 100);
    lv_obj_set_style_size(ui_WaveChart, 0, LV_PART_INDICATOR);
    lv_chart_set_update_mode(ui_WaveChart, LV_CHART_UPDATE_MODE_SHIFT);

    lv_chart_set_point_count(ui_WaveChart, 512);
    wave1 = lv_chart_add_series(ui_WaveChart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_type(ui_WaveChart, LV_CHART_TYPE_SCATTER);
    cursor1 = lv_chart_add_cursor(ui_WaveChart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    cursor2 = lv_chart_add_cursor(ui_WaveChart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    cursor3 = lv_chart_add_cursor(ui_WaveChart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    cursor4 = lv_chart_add_cursor(ui_WaveChart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    cursor5 = lv_chart_add_cursor(ui_WaveChart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);
    /* Infinite loop */
    for (;;) {
        lv_timer_handler();
        if (!LVGL_DrawCplt) {
            char text_tmp[30]={0};
            sprintf(text_tmp,"%.3fmA",wave_info[0].amp);
            lv_label_set_text(ui_amp1,text_tmp);
            sprintf(text_tmp,"%.3fmA",wave_info[1].amp);
            lv_label_set_text(ui_amp2,text_tmp);
            sprintf(text_tmp,"%.3fmA",wave_info[2].amp);
            lv_label_set_text(ui_amp3,text_tmp);
            sprintf(text_tmp,"%.3fmA",wave_info[3].amp);
            lv_label_set_text(ui_amp4,text_tmp);
            sprintf(text_tmp,"%.3fmA",wave_info[4].amp);
            lv_label_set_text(ui_amp5,text_tmp);


            sprintf(text_tmp,"%.3fHz",wave_info[0].freq);
            lv_label_set_text(ui_freq1,text_tmp);

            lv_chart_set_range(ui_WaveChart,LV_CHART_AXIS_PRIMARY_X,0,(lv_coord_t)(wave_freq[511]*1000));//freq max
            lv_chart_set_range(ui_WaveChart,LV_CHART_AXIS_PRIMARY_Y,0,(lv_coord_t)(wave_info[0].amp*1200));//amp max
            uint32_t i;
            wave_vpp[0]=0;
            for(i = 0; i < 512; i++) {
                if(!VGA_Open){
                    lv_chart_set_next_value2(ui_WaveChart, wave1, (lv_coord_t)(wave_freq[i]*1000), (lv_coord_t)(wave_vpp[i]*1000*1000/2.52f));
                }else{
                    lv_chart_set_next_value2(ui_WaveChart, wave1, (lv_coord_t)(wave_freq[i]*1000), (lv_coord_t)(wave_vpp[i]*1000*1000/25.02f));
                }

                //printf("d:%d,%d\r\n",(lv_coord_t)(wave_freq[i]*1000),(lv_coord_t)(wave_vpp[i]*100*1000));
            }
            lv_chart_set_cursor_point(ui_WaveChart, cursor1, NULL, 32);
            lv_chart_set_cursor_point(ui_WaveChart, cursor2, NULL, 32*2);
            lv_chart_set_cursor_point(ui_WaveChart, cursor3, NULL, 32*3);
            lv_chart_set_cursor_point(ui_WaveChart, cursor4, NULL, 32*4);
            lv_chart_set_cursor_point(ui_WaveChart, cursor5, NULL, 32*5);


            HAL_Delay(100);
            VGA_Open=0;
            LVGL_DrawCplt = 1;
        }
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
    uint8_t get_wave_freq_flg = 0;
    uint8_t get_wave_freq_times = 5;

    float32_t adc_fs;
    uint32_t period;
    adc_fs = 32.0f * 1000.0f;
    period = (uint32_t)(50000000.0f/adc_fs);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *) ADCxValue, ADC_NUM);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period/2);
    __HAL_TIM_SET_AUTORELOAD(&htim2, period-1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    /* Infinite loop */
    for (;;) {
#if(0)
        if (ADC_ConvCplt&&LVGL_DrawCplt) {
            SCB_InvalidateDCache_by_Addr((uint32_t*)ADCxValue, sizeof(ADCxValue));
            //ADC采样数据转换实际电压幅度
            for (int i = 0; i < FFT_LENGTH; i++) {
                ADCxSample[i] = (float32_t) ADCxValue[i] / 65535.0f * 6.6f - 3.3f;
                printf("wave:%.3f\r\n",ADCxSample[i]);
            }
            ADC_ConvCplt=0;
            HAL_ADC_Start_DMA(&hadc3, (uint32_t *) ADCxValue, ADC_NUM);
        }
#else
        if (ADC_ConvCplt&&LVGL_DrawCplt) {
            //ADC采样数据转换实际电压幅度
            SCB_InvalidateDCache_by_Addr((uint32_t*)ADCxValue, sizeof(ADCxValue));
            for (int i = 0; i < FFT_LENGTH; i++) {
                ADCxSample[i] = (float32_t) ADCxValue[i] / 65535.0f * 3.305f*2 - 3.305f;
//                printf("wave:%.3f\r\n",ADCxSample[i]);
            }
            if (get_wave_freq_flg == 1) {
                for (uint16_t i = 0; i < FFT_LENGTH; i++) {
//                    printf("wave:%.3f,%.3f,%.3f\r\n",ADCxSample[i],adc_fs,base_wave.freq);
                    fft_inputbuf[2 * i] = ADCxSample[i];
                    fft_inputbuf[2 * i + 1] = 0;
                }
                arm_cfft_f32(cfft_instance_f32_get(FFT_LENGTH), fft_inputbuf, 0, 1);
                arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);
                for (int i = 0; i < FFT_LENGTH/2; ++i) {
                    printf("d:%.3f,%.3f,%.3f,%d,%.3f\r\n", 1.0077*(2 * fft_outputbuf[i] / (float32_t) FFT_LENGTH * 2)+0.0026584, i * adc_fs / (float32_t) FFT_LENGTH, base_wave.freq,i,ADCxSample[i]);
                    wave_vpp[i]=1.0077*(2 * fft_outputbuf[i] / (float32_t) FFT_LENGTH * 2)+0.0026584;
                    wave_freq[i]=(float32_t)(i * adc_fs / (float32_t) FFT_LENGTH);
                }
//                printf("amp:%.3f\r\n",wave_vpp[32]);
                wave_info[0].freq=wave_freq[32];
                wave_info[1].freq=wave_freq[32*2];
                wave_info[2].freq=wave_freq[32*3];
                wave_info[3].freq=wave_freq[32*4];
                wave_info[4].freq=wave_freq[32*5];

                if(!VGA_Open){
                    if(wave_info[4].freq>800){
                        wave_info[0].amp=wave_vpp[32]/2.52f*1.0431f*1000;//Avpp to mAvpp
                        wave_info[1].amp=wave_vpp[32*2]/2.52f*1.0431f*1000;
                        wave_info[2].amp=wave_vpp[32*3]/2.52f*1.0431f*1000;
                        wave_info[3].amp=wave_vpp[32*4]/2.52f*1.0431f*1000;
                        wave_info[4].amp=wave_vpp[32*5]/2.52f*1000;
                    } else {
                        wave_info[0].amp=wave_vpp[32]/2.52f*1.0431f*1000;//Avpp to mAvpp
                        wave_info[1].amp=wave_vpp[32*2]/2.52f*1.0431f*1000;
                        wave_info[2].amp=wave_vpp[32*3]/2.52f*1.0431f*1000;
                        wave_info[3].amp=wave_vpp[32*4]/2.52f*1.0431f*1000;
                        wave_info[4].amp=wave_vpp[32*5]/2.52f*1.0431f*1000;
                    }
                }else{
                    wave_info[0].amp=wave_vpp[32]/25.02f*1.06f*1000;
                    wave_info[1].amp=wave_vpp[32*2]/25.02f*1.06f*1000;
                    wave_info[2].amp=wave_vpp[32*3]/25.02f*1.06f*1000;
                    wave_info[3].amp=wave_vpp[32*4]/25.02f*1.06f*1000;
                    wave_info[4].amp=wave_vpp[32*5]/25.02f*1.06f*1000;
                }

                adc_fs = 20.0f * 1000.0f;
                period = (uint32_t)(50000000.0f/adc_fs);
                HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period/2);
                __HAL_TIM_SET_AUTORELOAD(&htim2, period-1);
                HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

                get_wave_freq_flg = 0;
                HAL_GPIO_WritePin(VGA_GPIO_Port,VGA_Pin,GPIO_PIN_RESET);
                LVGL_DrawCplt=0;
            } else {
                if (get_wave_freq_times != 0) {
                    get_wave_freq_times--;
                    Get_BaseWave(&base_wave, FFT_LENGTH, adc_fs, ADCxSample, fft_inputbuf, fft_outputbuf);
                    printf("Freq[%d]:%.3f,%.3f,%.3f\r\n",get_wave_freq_times,base_wave.amp*2,base_wave.freq,adc_fs);
                    if (base_wave.freq < 30.0f) {
                        adc_fs = (base_wave.freq) * 1024;
                    }
                    if (base_wave.freq < 300&&base_wave.freq > 30.0f) {
                        adc_fs = (base_wave.freq) * 32;
                    }
                    if (base_wave.freq >= 300 && base_wave.freq < 500) {
                        adc_fs = (base_wave.freq) * 16;
                    }
                    if(base_wave.freq >= 500) {
                        adc_fs = (base_wave.freq) * 16;
                    }
                    period = (uint32_t)(50000000.0f/adc_fs);
                    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
                    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period/2);
                    __HAL_TIM_SET_AUTORELOAD(&htim2, period-1);
                    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

                } else {
                    get_wave_freq_times = 5;
                    printf("Freq[x]:%.3f,%.3f,%.3f\r\n",base_wave.amp*2,base_wave.freq,adc_fs);
                    if (base_wave.amp*2<0.34){
                        HAL_GPIO_WritePin(VGA_GPIO_Port,VGA_Pin,GPIO_PIN_SET);
                        VGA_Open=1;
                        HAL_Delay(100);
                    }
                    adc_fs = base_wave.freq * 32;
//                    adc_fs=32.0f * 1000.0f;
                    period = (uint32_t)(50000000.0f/adc_fs);

                    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);
                    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period/2);
                    __HAL_TIM_SET_AUTORELOAD(&htim2, period-1);
                    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

                    get_wave_freq_flg = 1;
//                    HAL_Delay(500);
                }
            }
            ADC_ConvCplt = 0;
            HAL_ADC_Start_DMA(&hadc3, (uint32_t *) ADCxValue, ADC_NUM);
        }
#endif
        osDelay(1);
    }
  /* USER CODE END adcTask_cb */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

