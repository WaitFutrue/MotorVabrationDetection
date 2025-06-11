#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "gpio.h"

typedef enum {
    NO_KEY      = 0,   // �ް�������
    KEY_UP      = 1,   // �ϼ�����
    KEY_DOWN    = 2,   // �¼�����
    KEY_ENTER   = 3,   // ȷ�ϼ�����
    KEY_BACK    = 4    // ���ؼ�����
} Key_State_t;


void Key_Init(void);
uint8_t Key_GetNum(void);
void Key_Tick(void);

#endif
