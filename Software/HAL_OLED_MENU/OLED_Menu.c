#include "OLED_Menu.h"
#include "usart.h"


uint8_t Num_Choose=1;
uint16_t flag_subMenu=1,flag_test=0; //�����ж��Ƿ����Ӳ˵���
uint16_t flag_adxl345,flag_oled,flag_ds18b20,flag_usart,flag_key;//���Ա�־
/********************************************************************
   * @brief  : ���˵���ʾ
   * @param  :  �ڸ�����ɫ����
   * @retval :  �������1��ģ����ԣ�2������ҵ��3��������Ϣ
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

//�л��˵����˺���ֻ�ı�����ֵ�뷴ɫ�򣬺���ʾ�����޹�
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void Choose_menu()
{
  //pre_key=Key_GetNum(); //��¼�ϴΰ���ֵ
  switch (Key_GetNum()){
    case  NO_KEY:break;
    //�ϼ�
    case  KEY_UP: 
                OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
                if (Num_Choose > 1) Num_Choose--; //����
                OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);// color reversion
                OLED_Update(); 
     break;
    //�¼�
    case  KEY_DOWN:
                  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);
                  if(Num_Choose<(3*flag_subMenu)) Num_Choose++;else ;//����
                  OLED_ReverseArea(0,Num_Choose*RECT_STEP,OLED_RECT_WIDTH,OLED_RECT_HEIGHT/flag_subMenu);// color reversion
                  OLED_Update();
                  
     break;
    //ȷ�ϼ�
    case  KEY_ENTER://�����Ѿ�������ȷ�ϼ����϶�Ҫ����,��©����������������
                    
                    if(flag_subMenu==1){//��������˵�����Ҳ��Ĭ�ϵ�
                      flag_subMenu=2; //��ȷ�ϼ����϶������Ӳ˵�
                      if(Num_Choose==MENU_FUNC_TEST){
                        Num_Choose=2;flag_test=1; //���빦�ܲ����Ӳ˵�
                        display_sub_menu(); //��ʾ�Ӳ˵�
                      }
                      else if(Num_Choose==MENU_ABOUT_ME){
                        display_about_me();//��ʾ�������Ӳ˵�
                      }
                      else if (Num_Choose==MENU_WORK_MODE)//���������ͷϷ
                      {//�źŴ�������־������
                        
                        OLED_Clear(); //OLED_Update();//����
                        Work_Mode();//MENU 2
                      }
                    }
                    else if ( (flag_subMenu==2) && (flag_test==1) ){//����ڲ����Ӳ˵�
                      OLED_Clear();
                      if (Num_Choose == ADXL345_TEST)     flag_adxl345=1;
                      else if (Num_Choose == OLED_TEST)   flag_oled=1;
                      else if (Num_Choose == DS18B20_TEST)flag_ds18b20=1; 
                      else if (Num_Choose == USART_TEST)  flag_usart=1;
                      else if (Num_Choose == KEY_TEST)    flag_key=1;
                      else;
                    }else ;
                    
     break;
    //���ؼ�
    case  KEY_BACK:
                      Num_Choose=1;//�������˵�ʱ������ֵ��1
                      flag_subMenu=1; //�������˵�
                      flag_test=0; //�������˵�ʱ�������Ӳ˵���־λ���� 
                      display_main_menu(); //��ʾ���˵�
                      break;
    default:break;    
  }
}
//���ݱ�־λ������Ӧ������
/********************************************************************
   * @brief  : 
   * @param  :
   * @retval :
*********************************************************************/
void function_test(){
  if(flag_adxl345)
  {
    OLED_Printf(27, 0, OLED_6X8, "ADXL345_TEST");
    OLED_Printf(0, 8, OLED_6X8, "ADXL345_ID:"); //��ʾADXL345 ID
    OLED_ShowNum(100, 8, Get_Adxl345_ID(), 2, OLED_6X8); //��ʾ����ID
    do
    {
      //OLED��ʾADXL345 XYZԭʼƫ��ֵ
      OLED_ShowNum(0, 24, ADXL345_XTest(), 6, OLED_6X8); //��ʾX������
      OLED_ShowNum(0, 32, ADXL345_YTest(), 6, OLED_6X8); //��ʾY������
      OLED_ShowNum(0, 40, ADXL345_ZTest(), 6, OLED_6X8); //��ʾZ������
      OLED_Update();
    } while (Key_GetNum()!=KEY_BACK);//�ȴ����·��ؼ�
    display_sub_menu();//��ʾ��һ���˵�����
    flag_adxl345=0; //������ɺ�����
} else if (flag_oled) 
    {
      OLED_Printf(0, 0, OLED_6X8, "OLED Test Running...");
      OLED_Update();
      OLED_Test();//һ��������ȫ��
      display_sub_menu();//��ʾ��һ���˵�����
      flag_oled=0; //������ɺ�����
    } else if (flag_ds18b20) 
      {
        OLED_Printf(27, 0, OLED_6X8, "DS18B20_TEST");
        OLED_Printf(0, 16, OLED_6X8, "DS18B20 initing..."); //��ʾDS18B20 ID
        OLED_Update();
        OLED_ShowFloatNum(0, 16, (short)(DS18B20_Get_Temperature()) / 10.0f, 2, 1, OLED_6X8); //��ʾ�¶�ֵ
        HAL_Delay(1000); //��ʱ1�룬�ȴ�DS18B20�ȶ�
        OLED_ClearArea(0, 16,128, 8); //�����ʾ����
        do
        {
          
          //OLED��ʾDS18B20�¶�ֵ������DS18B20_Get_Temperature�������ص���short���ͣ���Ҫת������
          OLED_ShowFloatNum(0, 16, (short)(DS18B20_Get_Temperature()) / 10.0f, 2, 1, OLED_6X8); //��ʾ�¶�ֵ
          OLED_Update();
        } while (Key_GetNum()!=KEY_BACK);//�ȴ����·��ؼ�
        display_sub_menu();//��ʾ��һ���˵�����
        flag_ds18b20=0; //������ɺ�����
      } else if (flag_usart)
        {
          OLED_Printf(27, 0, OLED_6X8, "USART_TEST");
          OLED_Printf(0, 10, OLED_6X8, "USART BAUD:"); //��ʾUSART ID
          OLED_ShowNum(70, 10, 115200, 6, OLED_6X8); //��ʾ������
          OLED_Printf(0, 20, OLED_6X8, "USART_Send Running...");
          OLED_Printf(0, 30, OLED_6X8, "Send data: Hello ZYZ!");
          OLED_Printf(0, 40, OLED_6X8, "Press BACK to exit");
          OLED_Update();
          do
          {
            //���ڷ�������
            printf("Hello, I'm ZYZ!\r\n"); //���Ͳ�������
            //���ڽ�������,�ڱ���Ŀ��δ�õ�����������
          } while (Key_GetNum()!=KEY_BACK);//�ȴ����·��ؼ�
          display_sub_menu();//��ʾ��һ���˵�����
          flag_usart=0; //������ɺ�����
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
            } while (Key_GetNum()!=KEY_BACK);//�ȴ����·��ؼ�
            display_sub_menu();
            flag_key=0; //������ɺ�����
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
    //����������źŴ���������
    do
    {
      //�źŴ������
      ADXL345_Calibrate(500);//У׼ADXL345��500����ƽ��
      MyDSP_Process();

    } while (Key_GetNum()!=KEY_BACK);//�ȴ����·��ؼ�
    flag_subMenu=1; //�������˵�
    Num_Choose=1; //�������˵�ʱ������ֵ��1
    display_main_menu();//������һ���˵�����
    
    
}
