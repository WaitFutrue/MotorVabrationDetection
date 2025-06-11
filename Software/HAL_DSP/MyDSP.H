#ifndef MYDSP_H_
#define MYDSP_H_

#include "main.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#include "ADXL345.h"
#include "OLED.h"
#include "stdio.h"


//ADXL345的采样率为3200HZ,即为每秒采集3200个数据点
//本设计中使用1024点FFT变换，即每次采集1024个数据点进行频域分析
//采集了1024/3200秒的数据，约等于0.32秒
#define LENGTH_SAMPLE 2048 //采样点数，必须是2的整数次幂,经典值1024，可用2048
#define RATE_SAMPLE 3200 //采样率，单位为Hz
#define FFT_POINTS (LENGTH_SAMPLE/2+1) //FFT变换的点数，N/2+1
#define FREQUENCY_STEP (RATE_SAMPLE / LENGTH_SAMPLE) //频率步长，单位为Hz

extern float32_t hanning_window[];// 定义汉宁窗数组


void MyDSP_Init(void);
void arm_fir_f32_lp(void);
void arm_hanning_f32(float32_t * pDst,uint32_t blockSize);
void MyFFT_Process(void);
void MyDSP_Process(void);
//void test(void);
void arm_rfft_fast_f32_app(void);

#endif /* MYDSP_H_ */
