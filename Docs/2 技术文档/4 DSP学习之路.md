# 📆最近更新： 2025年3月17日

# 📌汉宁窗

## 作用

- 频域分析中，减少**频谱泄露**，提高频谱**分辨率**

## 原理

- 通过**加权信号**的采样点来实现，权重随着**采样点位置**而变化

- 形状类似**余弦波的平方**——这有助于信号在**窗口边缘**逐渐趋于零，从而减少**信号突然截断**造成的误差

## 示例

将原始信号和汉宁窗相乘

```c
#define WINDOW_SIZE 128 //假设窗大小为128
float window[WINDOW_SIZE];

// 创建汉宁窗
for (int i = 0; i < WINDOW_SIZE; ++i) {
    window[i] = 0.5 * (1 - cos(2 * M_PI * i / (WINDOW_SIZE - 1)));
}

// 应用汉宁窗到信号
for (int i = 0; i < WINDOW_SIZE; ++i) {
    x_filtered[i] = X_SIGNAL() * window[i];
}
```

# 📌平均滤波

 旨在减少噪声，平滑信号

## 1、简单移动平均滤波器

- 通过累加一定数量的样本，然后求取平均值，来**去除偏置或直流分量**  

- **减少随机噪声**的影响，提供平滑的结果

```c
for(i = 0; i < width; i++)//求和
    x += x_signal();
//下面求平均数
x = x / width;
```

## 2、加权移动平均（2025年3月17日，待更新⏳）

1. **加权移动平均滤波器**：给不同的样本赋予不同的权重，最近的数据点可能被赋予更高的权重。
2. **指数移动平均滤波器**：每个输出值是当前输入值和前一输出值的加权平均，其中较新的数据点通常具有更高的权重。
3. **高斯滤波器**：使用高斯分布作为权重进行加权平均，以达到平滑效果。

# 📌FFT入门例程

```c
  // Process the DSP
    //定义输入输出缓存区
    static float32_t test_input[SAMPLE_LENGTH];
    static float32_t test_output[SAMPLE_LENGTH/2+1];// 只需要一半加上直流分量

    //初始化输入数据
    for(uint32_t i=0;i<SAMPLE_LENGTH;i++)
        test_input[i]=arm_sin_f32((2*PI*5)*(i/SAMPLE_LENGTH));//sinθ→2Πf；i索引，1/SAMPLE为采样间隔（秒）

    //创建并初始化FFT实例
    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S,SAMPLE_LENGTH/2);

    //执行FFT
    arm_rfft_fast_f32(&S,test_input,test_output,0);

    // 打印FFT结果
    printf("Index\tFrequency (Hz)\tMagnitude (dB)\tPhase (radians)\r\n");
    for(uint32_t i = 0; i < (SAMPLE_LENGTH / 2 + 1); i++) {
        float32_t real = test_output[i];//实部
        float32_t imag = test_output[SAMPLE_LENGTH / 2 + 1 + i];//虚部

        float32_t magnitude = sqrtf(real * real + imag * imag);//幅值
        if (magnitude > 0) {//处理非零值
            magnitude = 20 * log10f(magnitude); // 转换为dB
        } else {
            magnitude = -INFINITY; // 处理零值
        }//有问标题🈶

        float32_t phase = atan2f(imag, real);
        float32_t frequency = i * (SAMPLE_RATE / (float32_t)SAMPLE_LENGTH);

        printf("%u\t%f Hz\t%f dB\t%f radians\r\n", i, frequency, magnitude, phase);
    }
```

## 注释

- FFT输出结果的数组，交错方式存储数据——偶数实部，奇数虚部

- 为什么加上直流分量要加一？（🚩待解决，2025年3月17日）

- 时域方面——输入数组，输出数组 →把要被分析的原始信号赋给数组→ 创建FFT实例，初始化（类比STM32中的GPIO初始化） → 执行FFT → 处理FFT结果（可以解算，也可以打印）

- 幅值——实部方和虚部方求和开根➡️20对数就是分贝

- 频率——根据频率分辨率来
