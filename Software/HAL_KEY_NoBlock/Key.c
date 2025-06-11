#include "Key.h"                  // Device header


uint8_t Key_Num;

void Key_Init(void)
{
//�Ѿ�ͨ��CUBEMX���ã���GPIO.H��Ĭ������
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}

uint8_t Key_GetState(void)
{//��������
	if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == 0)		    return KEY_UP;
	if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == 0)		return KEY_DOWN;
	if (HAL_GPIO_ReadPin(KEY_ENTER_GPIO_Port, KEY_ENTER_Pin) == 0)	return KEY_ENTER;
	if (HAL_GPIO_ReadPin(KEY_BACK_GPIO_Port, KEY_BACK_Pin) == 0)		return KEY_BACK;
	return NO_KEY;//δ����������0
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;
	
	Count ++;
	if (Count >= 20)//10��Ƶ
	{
		Count = 0;//20ms��һ��
		
		PrevState = CurrState;//��¼
		CurrState = Key_GetState();//ˢ��
		
		if (CurrState== 0 && PrevState!= 0)//��ǰ�ѳ�����20msǰû������Ҳ����ȷ�ϳ���
		{
			Key_Num = PrevState;//
		}
	}
}
