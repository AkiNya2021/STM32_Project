//
// Created by admin on 2022/7/10.
//

#include "DSP.h"

const arm_cfft_instance_f32 *cfft_instance_f32_get(uint16_t len) {
    switch (len) {
        case 32:
            return &arm_cfft_sR_f32_len32;
        case 64:
            return &arm_cfft_sR_f32_len64;
        case 128:
            return &arm_cfft_sR_f32_len128;
        case 256:
            return &arm_cfft_sR_f32_len256;
        case 512:
            return &arm_cfft_sR_f32_len512;
        case 1024:
            return &arm_cfft_sR_f32_len1024;
        case 2048:
            return &arm_cfft_sR_f32_len2048;
        case 4096:
            return &arm_cfft_sR_f32_len4096;
        default:
            return &arm_cfft_sR_f32_len1024;
    }
}

void Get_BaseWave(wave *basewave, uint16_t fftlen, float32_t fs, const float32_t *samples, float32_t *fft_inputbuf, float32_t *fft_outputbuf) {
    float32_t k;
    uint16_t m;
    for (uint16_t i = 0; i < fftlen; i++) {
        fft_inputbuf[2 * i] = samples[i];
        fft_inputbuf[2 * i + 1] = 0;
    }
    arm_cfft_f32(cfft_instance_f32_get(fftlen), fft_inputbuf, 0, 1);
    arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, fftlen);
    m = 1;
    //0 直流 1 2 3 4 5
    for (uint16_t i = 2; i < fftlen / 2; i++) {
        if (fft_outputbuf[m] < fft_outputbuf[i]) {
            m = i;
        }
    }
    if (fft_outputbuf[m + 1] >= fft_outputbuf[m - 1]) {
        k = fft_outputbuf[m + 1] / (fft_outputbuf[m] + fft_outputbuf[m + 1]);
    } else {
        k = -fft_outputbuf[m - 1] / (fft_outputbuf[m] + fft_outputbuf[m - 1]);
    }
    basewave->amp = (float32_t) (PI * k * fft_outputbuf[m]) * (arm_sin_f32((float32_t) 3.1415926 * k)) / (float32_t) fftlen * 2;
    basewave->freq = (k + (float32_t) m) * fs / (float32_t) fftlen;
}
