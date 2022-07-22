//
// Created by admin on 2022/7/10.
//

#ifndef H743IIT6_RTOS_DSP_H
#define H743IIT6_RTOS_DSP_H

#include "main.h"
#include "arm_math.h"
#include "arm_const_structs.h"

typedef struct {
    float32_t freq;
    float32_t amp;
}wave;

const arm_cfft_instance_f32 * cfft_instance_f32_get(uint16_t len);
void Get_BaseWave(wave* basewave, uint16_t fftlen,float32_t fs,const float32_t * samples,float32_t * fft_inputbuf, float32_t * fft_outputbuf);
void Find_max();
void Find_first_peek();

#endif//H743IIT6_RTOS_DSP_H
