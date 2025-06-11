#include "MyDSP.h"

//最大遍历
    float32_t max_mag_x = 0, max_mag_y = 0, max_mag_z = 0;
    uint32_t max_index_x = 0, max_index_y = 0, max_index_z = 0;
//ADXL345原始输入信号（相当于ADXL345输出序列被矩形窗被截断）
static float32_t Original_input_x[LENGTH_SAMPLE];
static float32_t Original_input_y[LENGTH_SAMPLE];
static float32_t Original_input_z[LENGTH_SAMPLE];
//FIR低通时域信号(由原始输入信号经过低通滤波器所得)
static float32_t FIR_output_x[LENGTH_SAMPLE];
static float32_t FIR_output_y[LENGTH_SAMPLE];
static float32_t FIR_output_z[LENGTH_SAMPLE];
//加窗处理后的时域信号
static float32_t FFT_input_x[LENGTH_SAMPLE];
static float32_t FFT_input_y[LENGTH_SAMPLE];
static float32_t FFT_input_z[LENGTH_SAMPLE];
//长度为N/2+1，因为FFT的输出是对称的，只需要保留一半，而加一个点，是为了保留直流分量
static float32_t FFT_output_x[LENGTH_SAMPLE/2+1];
static float32_t FFT_output_y[LENGTH_SAMPLE/2+1];
static float32_t FFT_output_z[LENGTH_SAMPLE/2+1];
//频谱的幅度和频率
//static float32_t frequency;
static float32_t FFT_output_mag_x[LENGTH_SAMPLE/2+1];
static float32_t FFT_output_mag_y[LENGTH_SAMPLE/2+1];
static float32_t FFT_output_mag_z[LENGTH_SAMPLE/2+1];

/* 低通滤波器系数 通过fadtool获取*/
#define NUM_TAPS   29     /* 滤波器系数个数 */
#define BLOCK_SIZE 32     /* 调用一次arm_fir_f32处理的采样点个数 */
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = LENGTH_SAMPLE/BLOCK_SIZE;            /* 需要调用arm_fir_f32的次数 */
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];        /* 状态缓存，大小numTaps + blockSize - 1*/

////800HZ、hann窗、低通、46阶   800HZ
//const float32_t B[NUM_TAPS] = {
//               -0,5.094349399e-05,0.0002126742475,-0.0004989779554,-0.0009244055836,
//   0.001504608314, 0.002256840235,-0.003200700507,-0.004359227605, 0.005760520697,
//   0.007440174464,-0.009445019066, -0.01183902007,  0.01471291855,  0.01820068061,
//   -0.02250909992, -0.02797468193,  0.03518253937,  0.04524371773, -0.06054779515,
//   -0.08732147515,   0.1484210789,   0.4496337175,   0.4496337175,   0.1484210789,
//   -0.08732147515, -0.06054779515,  0.04524371773,  0.03518253937, -0.02797468193,
//   -0.02250909992,  0.01820068061,  0.01471291855, -0.01183902007,-0.009445019066,
//   0.007440174464, 0.005760520697,-0.004359227605,-0.003200700507, 0.002256840235,
//   0.001504608314,-0.0009244055836,-0.0004989779554,0.0002126742475,5.094349399e-05,
//               -0
//};

//// 高通滤波器其系数 通过fadtool获取，29   150HZ
//const float32_t B[NUM_TAPS] = {
//	0.0018157335f,     0.001582013792f,    -6.107207639e-18f,  -0.003683975432f,   -0.008045346476f,
//	-0.008498443291f,  -1.277260999e-17f,  0.01733288541f,     0.03401865438f,     0.0332348831f,
//	-4.021742543e-17f, -0.06737889349f,    -0.1516391635f,     -0.2220942229f,     0.7486887574f,
//	-0.2220942229f,    -0.1516391635f,     -0.06737889349f,    -4.021742543e-17f,  0.0332348831f,
//	0.03401865438f,    0.01733288541f,     -1.277260999e-17f,  -0.008498443291f,   -0.008045346476f,
//	-0.003683975432f,  -6.107207639e-18f,  0.001582013792f,    0.0018157335f
//};
// 低通滤波器系数 通过fadtool获取  29    150HZ
const float32_t B[NUM_TAPS] = {
  -0.001822523074f,  -0.001587929321f,  1.226008847e-18f,  0.003697750857f,  0.008075430058f,
  0.008530221879f,   -4.273456581e-18f, -0.01739769801f,   -0.03414586186f,  -0.03335915506f,
  8.073562366e-18f,  0.06763084233f,    0.1522061825f,     0.2229246944f,    0.2504960895f,
  0.2229246944f,     0.1522061825f,     0.06763084233f,    8.073562366e-18f, -0.03335915506f,
  -0.03414586186f,   -0.01739769801f,   -4.273456581e-18f, 0.008530221879f,  0.008075430058f,
  0.003697750857f,   1.226008847e-18f,  -0.001587929321f,  -0.001822523074f
};

//产生原始信号
void MyDSP_Init(){
//    //将ADXL345三轴数据赋给输入缓冲
    for(int i = 0; i < LENGTH_SAMPLE; i++) {
       Original_input_x[i] = ADXL345_XTest() * 0.00390625f; //转换为g单位
       Original_input_y[i] = ADXL345_YTest() * 0.00390625f;
       Original_input_z[i] = ADXL345_ZTest() * 0.00390625f;
        //调试用，可以注释掉，窗口打印数据，以观察
        printf("%.5f,%.5f,%.5f\n", Original_input_x[i], Original_input_y[i], Original_input_z[i]);
//        // VOFA+格式
//        printf("X: %.5f\n", Original_input_x[i]); // 打印X轴数据
//        //printf("Y: %.5f\n", Original_input_y[i]); // 打印Y轴数据
      //模拟信号
      // Original_input_x[i] = arm_sin_f32(2*3.1415926f*50*i/1000) ;//+ arm_sin_f32(2*3.1415926f*200*i/1000);
      // printf("X: %.5f\n", Original_input_x[i]); // 打印X轴数据
    }
  
    //说明DSP_Init函数已经执行完毕
    printf("DSP_Init completed.\n");
}
//FIR低通滤波器处理
void arm_fir_f32_lp(){
    // 定义FIR滤波器实例
    arm_fir_instance_f32 S;
  	float32_t  *inputF32, *outputF32;

	/* 初始化输入输出缓存指针 */
	inputF32 = &Original_input_x[0];
	outputF32 = &FIR_output_x[0];
    // 初始化FIR滤波器
    arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)B, &firStateF32[0], LENGTH_SAMPLE);
    
    // 对每个轴的数据进行滤波,采用分块处理，避免一次过长
	for(int i=0; i < numBlocks; i++)
	{
		    arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
        arm_fir_f32(&S, Original_input_y + (i * blockSize), FIR_output_y + (i * blockSize), blockSize);
        arm_fir_f32(&S, Original_input_z + (i * blockSize), FIR_output_z + (i * blockSize), blockSize);
        //调试用，可以注释掉，窗口打印数据，以观察
        //printf("FIR Output X: %.2f, Y: %.2f, Z: %.2f\n", FIR_output_x[i], FIR_output_y[i], FIR_output_z[i]);
    }
  
  	/* 打印滤波后结果 */
	for(int i=0; i<LENGTH_SAMPLE; i++)	printf("%.5f,%.5f,%.5f\n", FIR_output_x[i],FIR_output_y[i],FIR_output_z[i]);//VOFA+格式,打印X轴滤波后的数据
    //说明FIR滤波器处理函数已经执行完毕
    printf("FIR filter processing completed.\n");
}
//生成汉宁窗函数并进行加窗处理
/*@par Parameters of the window
  
  | Parameter                             | Value              |
  | ------------------------------------: | -----------------: |
  | Peak sidelobe level                   |           31.5 dB  |
  | Normalized equivalent noise bandwidth |          1.5 bins  |
  | 3 dB bandwidth                        |       1.4382 bins  |
  | Flatness                              |        -1.4236 dB  |
  | Recommended overlap                   |              50 %  |

 */
 float32_t hanning_window[LENGTH_SAMPLE];// 定义汉宁窗数组
void arm_hanning_f32(float32_t * pDst,uint32_t blockSize)
{
   float32_t k = 2.0f / ((float32_t) blockSize);
   float32_t w;
    // 生成汉宁窗函数
   for(uint32_t i=0;i<blockSize;i++)
   {
     w = PI * i * k;
     w = 0.5f * (1.0f - cosf (w));
     pDst[i] = w;
   }
   //分别对 XYZ 轴信号加窗
   for(int i = 0; i < LENGTH_SAMPLE; i++) FFT_input_x[i] = FIR_output_x[i] * hanning_window[i];
   for(int i = 0; i < LENGTH_SAMPLE; i++) FFT_input_y[i] = FIR_output_y[i] * hanning_window[i];
   for(int i = 0; i < LENGTH_SAMPLE; i++) FFT_input_z[i] = FIR_output_z[i] * hanning_window[i];
   
    /* 打印滤波后结果 */
   for(int i=0; i<LENGTH_SAMPLE; i++)	printf("%.5f,%.5f,%.5f\n", FFT_input_x[i],FFT_input_y[i],FFT_input_z[i]);//VOFA+格式,打印X轴滤波后的数据
   // 说明加窗处理函数已经执行完毕
   printf("Hanning window processing completed.\n");
}

//FFT处理
void MyFFT_Process()
{
  
    // 定义FFT变换的结构体
    arm_rfft_fast_instance_f32 S;

    // 初始化FFT变换
    arm_rfft_fast_init_f32(&S, LENGTH_SAMPLE);
    
    // 计算FFT
    arm_rfft_fast_f32(&S, FFT_input_x, FFT_output_x, 0); // X轴FFT
    arm_rfft_fast_f32(&S, FFT_input_y, FFT_output_y, 0); // Y轴FFT
    arm_rfft_fast_f32(&S, FFT_input_z, FFT_output_z, 0); // Z轴FFT
  //arm_cfft_f32(&S , fft_buf_float , 0 , 1);  
  
    // 计算幅度谱
    arm_cmplx_mag_f32(FFT_output_x, FFT_output_mag_x, FFT_POINTS);
    arm_cmplx_mag_f32(FFT_output_y, FFT_output_mag_y, FFT_POINTS);
    arm_cmplx_mag_f32(FFT_output_z, FFT_output_mag_z, FFT_POINTS);
    
  // //打印FFT输出结果
  //   for(int i = 0; i < FFT_POINTS; i++) 
  //       printf("FFT Output X[%d]: %.6f\n", i, FFT_output_mag_x[i]); // VOFA+格式,打印X轴FFT输出的幅度
  
}

//主DSP处理函数
void MyDSP_Process()
{
    // 初始化_产生原始信号
    MyDSP_Init();
    
    // FIR低通滤波处理
    arm_fir_f32_lp();
    
    // 生成汉宁窗函数并进行加窗处理
    //arm_hanning_f32(hanning_window, LENGTH_SAMPLE);
    for(int i = 0; i < LENGTH_SAMPLE; i++) FFT_input_x[i] = Original_input_x[i];
    // FFT处理
    MyFFT_Process();
  
    //利用数学最大值函数找出最大幅度和对应的频率
    arm_max_f32(FFT_output_mag_x, FFT_POINTS, &max_mag_x, &max_index_x);
    arm_max_f32(FFT_output_mag_y, FFT_POINTS, &max_mag_y, &max_index_y);
    arm_max_f32(FFT_output_mag_z, FFT_POINTS, &max_mag_z, &max_index_z);
    //打印最大幅度和对应的频率
    printf("Max Magnitude X: %.6f at Frequency: %.2f Hz\n", max_mag_x, (float32_t)(max_index_x * FREQUENCY_STEP));

    //for(int i = 0; i < LENGTH_SAMPLE/2 + 1; i++)    FFT_output_mag_x[i] = FFT_output_mag_x[i] * (2.0f / LENGTH_SAMPLE); // 归一化
 
    // 打印最大幅度和对应频率
    OLED_Printf(0, 20, OLED_6X8, "Max Mag X:");
    OLED_ShowFloatNum(60, 20, max_mag_x, 4, 4, OLED_6X8); // 显示最大幅度X
    OLED_Printf(0, 28, OLED_6X8, "Freq X:");
    OLED_ShowFloatNum(60, 28, (float32_t)(max_index_x * FREQUENCY_STEP), 4, 4, OLED_6X8); // 显示对应频率X
    OLED_Printf(0, 36, OLED_6X8, "Max Mag Y:");
    OLED_ShowFloatNum(60, 36, max_mag_y, 4, 4, OLED_6X8); // 显示最大幅度Y
    OLED_Printf(0, 44, OLED_6X8, "Freq Y:");
    OLED_ShowFloatNum(60, 44, max_index_y * FREQUENCY_STEP, 4, 4, OLED_6X8); // 显示对应频率Y
    OLED_Printf(0, 52, OLED_6X8, "Max Mag Z:");
    OLED_ShowFloatNum(60, 52, max_mag_z, 4, 4, OLED_6X8); // 显示最大幅度Z
    OLED_Update();
}

void test()
{
//  //测试arm_max_f32函数
//  float32_t test_data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
//  float32_t max_value;
//  uint32_t max_index;
//  arm_max_f32(test_data, 5, &max_value, &max_index);
//  //在OLED上打印测试结果
//  OLED_Printf(0, 0, OLED_6X8, "Max Value: ");
//  OLED_ShowFloatNum(70, 0, max_value, 1, 6, OLED_6X8); // 显示最大值
//  OLED_Printf(0, 8, OLED_6X8, "Max Index: ");
//  OLED_ShowNum(70, 8, max_index, 1, OLED_6X8); // 显示最大值索引
//  OLED_Update();

  //测试arm_cmplx_mag_f32函数
  float32_t complex_data[] = {1.0f, 2.0f, 3.0f, 4.0f}; // 实部和虚部交替存储
  float32_t magnitude[2]; // 存储幅度结果
  arm_cmplx_mag_f32(complex_data, magnitude, 2); // 计算幅度
  // 在OLED上打印测试结果
  OLED_Printf(0, 0, OLED_6X8, "Magnitude: ");
  OLED_ShowFloatNum(70, 0, magnitude[0], 1, 6, OLED_6X8); // 显示第一个幅度
  OLED_ShowFloatNum(70, 8, magnitude[1], 1, 6, OLED_6X8); // 显示第二个幅度
  OLED_Update();
}
/*
*********************************************************************************************************
*	函 数 名: arm_rfft_fast_f32_app
*	功能说明: 调用函数arm_rfft_fast_f32计算1024点实数序列的幅频响应并跟使用函数arm_cfft_f32计算的结果做对比。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
/* 输入和输出缓冲 */
#define TEST_LENGTH_SAMPLES 2048 
static float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES];

static q15_t testInput_q15_50hz[TEST_LENGTH_SAMPLES];
//static q15_t testOutputQ15[TEST_LENGTH_SAMPLES];
static q15_t testOutput_q15_50hz[TEST_LENGTH_SAMPLES];

static q31_t testInput_q31_50hz[TEST_LENGTH_SAMPLES];
//static q31_t testOutputQ31[TEST_LENGTH_SAMPLES];
static q31_t testOutput_q31_50hz[TEST_LENGTH_SAMPLES];

/* 变量 */
uint32_t fftSize = 1024; 
uint32_t ifftFlag = 0; 
uint32_t doBitReverse = 1; 
void arm_rfft_fast_f32_app(void)
{
	uint16_t i;
	arm_rfft_fast_instance_f32 S;
	
	/* 实数序列FFT长度 */
	fftSize = 1024; 
	/* 正变换 */
    ifftFlag = 0; 
	
	/* 初始化结构体S中的参数 */
 	arm_rfft_fast_init_f32(&S, fftSize);
	
	/* 按照实部，虚部，实部，虚部..... 的顺序存储数据 */
	for(i=0; i<1024; i++)
	{
		/* 50Hz正弦波，采样率1KHz */
		testInput_f32_10khz[i] = 1.2f*arm_sin_f32(2*3.1415926f*50*i/1000)+1;
	}
	
	/* 1024点实序列快速FFT */ 
	arm_rfft_fast_f32(&S, testInput_f32_10khz, testOutput_f32_10khz, ifftFlag);
	
	/* 为了方便跟函数arm_cfft_f32计算的结果做对比，这里求解了1024组模值，实际函数arm_rfft_fast_f32
	   只求解出了512组  
	*/ 
 	arm_cmplx_mag_f32(testOutput_f32_10khz, testOutput, fftSize);

	/* 串口打印求解的模值 */
	for(i=0; i<fftSize; i++)
	{
		printf("%f\r\n", testOutput[i]);
	}

	printf("****************************分割线***************************************\r\n");

	for(i=0; i<1024; i++)
	{
		/* 虚部全部置零 */
		testInput_f32_10khz[i*2+1] = 0;
		
		/* 50Hz正弦波，采样率1KHz ,作为实部 */
		testInput_f32_10khz[i*2] = 1.2f*arm_sin_f32(2*3.1415926f*50*i/1000)+1;
	}
	
	arm_cfft_f32(&arm_cfft_sR_f32_len1024, testInput_f32_10khz, ifftFlag, doBitReverse);
	
	/* 求解模值  */ 
 	arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);

	/* 串口打印求解的模值 */
	for(i=0; i<fftSize; i++)
	{
		printf("%f\r\n", testOutput[i]);
	}
	
}