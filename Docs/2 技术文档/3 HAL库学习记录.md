# 📚STM32 CubeMX配置记录

# 🔜DSP配置

## 配置 Software Packs

- 选中DSP，✔加载LIB——若文件夹中出现**中间件**文件夹则说明成功)
  
  > 此过程总共有两个√，同样的位置，第一次选择库，第二次对DSP lib 打✔
  
  <img title="" src="https://i-blog.csdnimg.cn/blog_migrate/01b4421a088a19218ef1dbc6ba74a682.png" alt="" width="656">

## 配置 Project Manager

- 篮筐第二栏中相关选项也必须✔——配置package

## 配置 KEIL [🔗](%5B%E6%88%91%E7%9A%84%E6%94%B6%E8%97%8F-%E4%B8%AA%E4%BA%BA%E4%B8%AD%E5%BF%83-CSDN%5D(https://i.csdn.net/#/user-center/collection-list?type=1&folder=42348207&key=3))

- 启动FPU，然后添加宏👇
  
  ```bash
  ,ARM_MATH_CM4
  ```
  
  <img src="https://i-blog.csdnimg.cn/blog_migrate/f24f73bcbdf26fcb18d91b39a17dcf96.png" title="" alt="CSDN" width="557">

- 相应的，引入头文件

```bash
#include "arm_math.h"
#include "arm_const_structs.h"
```

- 添加路径

<img src="https://i-blog.csdnimg.cn/blog_migrate/bfbefffb6e6494c61c970472843288a7.png" title="" alt="CSDN" width="574">

- 删除路径

> MidlleWare文件夹下的DSP与这里的冲突！！！
> 
> 还有一个原因就是，中间件文件夹下啥也没有

# ℹ调试配置——printf重定向

- 法一 **不用micro-lib**    在usart.c中加入👇(此方法目前不管用！！！！)

```bash
#include <stdio.h>
int fputc(int ch, FILE* stream)
{
    //USART_SendData(USART1, (unsigned char) ch);
    //while (!(USART1->SR & USART_FLAG_TXE));
    USART_SendChar(USART1, (uint8_t)ch);
    return ch;
}
```

- 无论哪一种方法，在main.h都要加上

```bash
#include "stdio.h"
```

## 修改HAL库(可用但是每次都要改)

# 最近更新⌛2025年3月10日

# 🔛复制工程

1. CUBEMX 工程文件  **.mxproject**

2. KEIL 工程文件  **外面一堆，里面一堆** 仔细检查即可
