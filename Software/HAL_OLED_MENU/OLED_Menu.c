#include "OLED_Menu.h"
#include "usart.h"


uint8_t Num_Choose=1;
uint16_t flag_subMenu=1,flag_test=0; //用于判断是否在子菜单中
uint16_t flag_adxl345,flag_oled,flag_ds18b20,flag_usart,flag_key;//测试标志
/********************************************************************
   * @brief  : 主菜单显示
   * @param  :  内附区域反色函数
   * @retval :  有三项——1、模块测试；2、核心业务；3、毕设信息
*********************************************************************/
void display_main_menu(void)
{
  OLED_Clear();
  OLED_Printf(64 - 1 - 36, 0, OLED_8X16, "Main Menu");
  OLED_Printf(0, 16, OLED_8X16, "1.Func_TEST");
  OLED_Printf(0, 32, OLED_8X16, "2.Work Mode");
  OLED_Printf(0, 48, OLED_8X16, "3.About me");
  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
  OLED_Update();
}
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void display_sub_menu(void)
{
  OLED_Clear();
  OLED_Printf(64 - 1 - 36, 0, OLED_8X16, "NO.1 Menu");
  OLED_Printf(0, 16, OLED_6X8, "1.ADXL345_TEST");
  OLED_Printf(0, 24, OLED_6X8, "2.OLED_TEST");
  OLED_Printf(0, 32, OLED_6X8, "3.DS18B20_TEST");
  OLED_Printf(0, 40, OLED_6X8, "4.USART_TEST");
  OLED_Printf(0, 48, OLED_6X8, "5.KEY_TEST");
  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
  OLED_Update();
}
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void display_about_me(void)
{
  OLED_Clear();
  OLED_Printf(0, 20, OLED_6X8, "Autor:Zhang Yizhe");
  OLED_Printf(0, 40, OLED_6X8, "DesignFor25~Grduation");
  OLED_Update();
}

//切换菜单，此函数只改变索引值与反色框，和显示内容无关
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void Choose_menu()
{
  //pre_key=Key_GetNum(); //记录上次按键值
  switch (Key_GetNum()){
    case  NO_KEY:break;
    //上键
    case  KEY_UP: 
                OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
                if (Num_Choose > 1) Num_Choose--; //限制
                OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);// color reversion
                OLED_Update(); 
     break;
    //下键
    case  KEY_DOWN:
                  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
                  if(Num_Choose<(3*flag_subMenu)) Num_Choose++;else ;//限制
                  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);// color reversion
                  OLED_Update();
                  
     break;
    //确认键
    case  KEY_ENTER://现在已经按下了确认键，肯定要清屏,别漏掉清屏操作！！！
                    
                    if(flag_subMenu==1){//如果在主菜单，这也是默认的
                      flag_subMenu=2; //按确认键，肯定进入子菜单
                      if(Num_Choose==MENU_FUNC_TEST){
                        Num_Choose=2;flag_test=1; //进入功能测试子菜单
                        display_sub_menu(); //显示子菜单
                      }
                      else if(Num_Choose==MENU_ABOUT_ME){
                        display_about_me();//显示关于我子菜单
                      }
                      else if (Num_Choose==MENU_WORK_MODE)//这里才是重头戏
                      {//信号处理程序标志启动，
                        
                        OLED_Clear(); //OLED_Update();//清屏
                        Work_Mode();//MENU 2
                      }
                    }
                    else if ( (flag_subMenu==2) && (flag_test==1) ){//如果在测试子菜单
                      OLED_Clear();
                      if (Num_Choose == ADXL345_TEST)     flag_adxl345=1;
                      else if (Num_Choose == OLED_TEST)   flag_oled=1;
                      else if (Num_Choose == DS18B20_TEST)flag_ds18b20=1; 
                      else if (Num_Choose == USART_TEST)  flag_usart=1;
                      else if (Num_Choose == KEY_TEST)    flag_key=1;
                      else;
                    }else ;
                    
     break;
    //返回键
    case  KEY_BACK:
                      Num_Choose=1;//返回主菜单时，索引值归1
                      flag_subMenu=1; //返回主菜单
                      flag_test=0; //返回主菜单时，测试子菜单标志位清零 
                      display_main_menu(); //显示主菜单
                      break;
    default:break;    
  }
}
//根据标志位启动对应的任务
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void function_test(){
  if(flag_adxl345)
  {
    OLED_Printf(27, 0, OLED_6X8, "ADXL345_TEST");
    OLED_Printf(0, 8, OLED_6X8, "ADXL345_ID:"); //显示ADXL345 ID
    OLED_ShowNum(100, 8, Get_Adxl345_ID(), 2, OLED_6X8); //显示器件ID
    do
    {
      //OLED显示ADXL345 XYZ原始偏移值
      OLED_ShowNum(0, 24, ADXL345_XTest(), 6, OLED_6X8); //显示X轴数据
      OLED_ShowNum(0, 32, ADXL345_YTest(), 6, OLED_6X8); //显示Y轴数据
      OLED_ShowNum(0, 40, ADXL345_ZTest(), 6, OLED_6X8); //显示Z轴数据
      OLED_Update();
    } while (Key_GetNum()!=KEY_BACK);//等待按下返回键
    display_sub_menu();//显示上一级菜单界面
    flag_adxl345=0; //测试完成后清零
} else if (flag_oled) 
    {
      OLED_Printf(0, 0, OLED_6X8, "OLED Test Running...");
      OLED_Update();
      OLED_Test();//一个动画，全屏
      display_sub_menu();//显示上一级菜单界面
      flag_oled=0; //测试完成后清零
    } else if (flag_ds18b20) 
      {
        OLED_Printf(27, 0, OLED_6X8, "DS18B20_TEST");
        OLED_Printf(0, 16, OLED_6X8, "DS18B20 initing..."); //显示DS18B20 ID
        OLED_Update();
        OLED_ShowFloatNum(0, 16, (short)(DS18B20_Get_Temperature()) / 10.0f, 2, 1, OLED_6X8); //显示温度值
        HAL_Delay(1000); //延时1秒，等待DS18B20稳定
        OLED_ClearArea(0, 16,128, 8); //清除显示区域
        do
        {
          
          //OLED显示DS18B20温度值，其中DS18B20_Get_Temperature（）返回的是short类型，需要转换类型
          OLED_ShowFloatNum(0, 16, (short)(DS18B20_Get_Temperature()) / 10.0f, 2, 1, OLED_6X8); //显示温度值
          OLED_Update();
        } while (Key_GetNum()!=KEY_BACK);//等待按下返回键
        display_sub_menu();//显示上一级菜单界面
        flag_ds18b20=0; //测试完成后清零
      } else if (flag_usart)
        {
          OLED_Printf(27, 0, OLED_6X8, "USART_TEST");
          OLED_Printf(0, 10, OLED_6X8, "USART BAUD:"); //显示USART ID
          OLED_ShowNum(70, 10, 115200, 6, OLED_6X8); //显示波特率
          OLED_Printf(0, 20, OLED_6X8, "USART_Send Running...");
          OLED_Printf(0, 30, OLED_6X8, "Send data: Hello ZYZ!");
          OLED_Printf(0, 40, OLED_6X8, "Press BACK to exit");
          OLED_Update();
          do
          {
            //串口发送数据
            printf("Hello, I'm ZYZ!\r\n"); //发送测试数据
            //串口接收数据,在本项目中未用到，不做测试
          } while (Key_GetNum()!=KEY_BACK);//等待按下返回键
          display_sub_menu();//显示上一级菜单界面
          flag_usart=0; //测试完成后清零
        } else if (flag_key) 
          {
            OLED_Printf(0, 0, OLED_6X8, "Test BACK_KEY?");
            OLED_Printf(0, 8, OLED_6X8, "Press it!");
            OLED_Update();
            do
            {
              OLED_ShowNum(63,24, HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin), 1, OLED_8X16);
              OLED_ShowNum(63,48, HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin), 1, OLED_8X16);
              OLED_ShowNum(31,31, HAL_GPIO_ReadPin(KEY_ENTER_GPIO_Port, KEY_ENTER_Pin), 1, OLED_8X16);
              OLED_ShowNum(95,31, HAL_GPIO_ReadPin(KEY_BACK_GPIO_Port, KEY_BACK_Pin), 1, OLED_8X16);OLED_Update();
            } while (Key_GetNum()!=KEY_BACK);//等待按下返回键
            display_sub_menu();
            flag_key=0; //测试完成后清零
          } else ;
}
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void Work_Mode()
{
    OLED_Printf(0, 0, OLED_6X8, "Work Mode Running...");
    OLED_Printf(0, 8, OLED_6X8, "Press BACK to exit");
    OLED_Update();
    //在这里添加信号处理程序代码
    do
    {
      //信号处理代码
      ADXL345_Calibrate(500);//校准ADXL345，500次做平均
      MyDSP_Process();

    } while (Key_GetNum()!=KEY_BACK);//等待按下返回键
    flag_subMenu=1; //返回主菜单
    Num_Choose=1; //返回主菜单时，索引值归1
    display_main_menu();//返回上一级菜单界面
    
    
}
