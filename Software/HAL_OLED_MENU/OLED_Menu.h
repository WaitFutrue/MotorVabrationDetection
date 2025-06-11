#ifndef __OLED_MENU_H_
#define __OLED_MENU_H_
/*包含头文件*/
#include "Key.h"

#include "MyDSP.h"
#include "ds18b20.h"
/*宏定义*/
#define OLED_RECT_WIDTH 128 //OLED矩形宽度
#define OLED_RECT_HEIGHT 16 //OLED矩形高度
#define RECT_STEP OLED_RECT_HEIGHT/flag_subMenu
/*变量声明*/
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
    MENU_INDEX_MAIN=1, //主菜单索引值
    MENU_INDEX_SUB, //子菜单索引值
    MENU_INDEX_TEST //测试菜单索引值
} MenuIndex;

extern uint8_t Num_Choose; //菜单索引值与按键值
extern uint16_t flag_subMenu,flag_test; //用于判断是否在子菜单中
extern uint16_t flag_adxl345,flag_oled,flag_ds18b20,flag_usart,flag_key;//测试标志位
/*函数声明*/
void Choose_menu(void);
void function_test(void);
void Work_Mode(void);
#endif
