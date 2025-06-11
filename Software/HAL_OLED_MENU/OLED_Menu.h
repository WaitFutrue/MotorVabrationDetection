#ifndef __OLED_MENU_H_
#define __OLED_MENU_H_
/*����ͷ�ļ�*/
#include "Key.h"

#include "MyDSP.h"
#include "ds18b20.h"
/*�궨��*/
#define OLED_RECT_WIDTH 128 //OLED���ο��
#define OLED_RECT_HEIGHT 16 //OLED���θ߶�
#define RECT_STEP OLED_RECT_HEIGHT/flag_subMenu
/*��������*/
typedef enum {
    MENU_FUNC_TEST=1,
    MENU_WORK_MODE,
    MENU_ABOUT_ME
} MenuState;
typedef enum {
    ADXL345_TEST=2,
    OLED_TEST,
    DS18B20_TEST,
    USART_TEST,
    KEY_TEST
} TestMenu;
typedef enum{
    MENU_INDEX_MAIN=1, //���˵�����ֵ
    MENU_INDEX_SUB, //�Ӳ˵�����ֵ
    MENU_INDEX_TEST //���Բ˵�����ֵ
} MenuIndex;

extern uint8_t Num_Choose; //�˵�����ֵ�밴��ֵ
extern uint16_t flag_subMenu,flag_test; //�����ж��Ƿ����Ӳ˵���
extern uint16_t flag_adxl345,flag_oled,flag_ds18b20,flag_usart,flag_key;//���Ա�־λ
/*��������*/
void Choose_menu(void);
void function_test(void);
void Work_Mode(void);
#endif
