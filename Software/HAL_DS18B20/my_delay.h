#ifndef __MY_DELAY_H_
#define __MY_DELAY_H_
/*����ͷ�ļ�*/
#include "main.h"                  // Device header

/*λ����*/
#define SYSTEM_SUPPORT_OS		0		//����ϵͳ�ļ����Ƿ�֧��OS

/*��������*/

/*��������*/
void delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);

#endif
