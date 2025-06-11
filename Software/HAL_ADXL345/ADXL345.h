#ifndef __ADXL345_H_
#define __ADXL345_H_
/*����ͷ�ļ�*/
#include "spi.h"
#include "gpio.h"//��ΪҪ��PA4���������
#include "usart.h"
#include "ds18b20.h"

/*λ����*/
#define SPI1_CS_GPIO_Port  GPIOA
#define SPI1_CS_Pin        GPIO_PIN_4

/*��������*/
/******************ADXL345�Ĵ��������**********************/
#define DEVICE_ID           0X00        //��ȡ����ID,0XE5
#define THRESH_TAP          0X1D        //�û���ֵ
#define OFSX                0X1E	//x�����ƫ��ֵ
#define OFSY                0X1F    //y�����ƫ��ֵ
#define OFSZ                0X20    //z�����ƫ��ֵ
#define DUR                 0X21    //�û�����ʱ��
#define Latent              0X22    //�û��ӳ�ʱ��
#define Window              0X23    //�û�����ʱ��
#define THRESH_ACK          0X24    //�û���ֵ
#define THRESH_INACT        0X25    //������ֵ
#define TIME_INACT          0X26    //����ʱ��
#define ACT_INACT_CTL       0X27    //���м�����
#define THRESH_FF           0X28    //����������ֵ
#define TIME_FF             0X29    //��������ʱ��
#define TAP_AXES            0X2A    //�û������
#define ACT_TAP_STATUS      0X2B    //�û�״̬
#define BW_RATE             0X2C    //������ʺʹ���
#define POWER_CTL           0X2D    //��Դ����
 
#define INT_ENABLE          0X2E    //�ж�ʹ��
#define INT_MAP             0X2F    //�ж�ӳ��
#define INT_SOURCE          0X30    //�ж�Դ
#define DATA_FORMAT        0X31 //���ݸ�ʽ����
#define DATA_X0            0X32 //X������0
#define DATA_X1            0X33 //X������1
#define DATA_Y0            0X34 //Y������0
#define DATA_Y1            0X35 //Y������1
#define DATA_Z0            0X36 //Z������0
#define DATA_Z1            0X37 //Z������1
#define FIFO_CTL            0X38    //FIFO����
#define FIFO_STATUS         0X39    //FIFO״̬
 
#define I_M_DEVID      ((uint8_t)0XE5) //����ID=0XE5
/******************ADXL345ָ�**********************/

/*��������*/
//��ʼ��
void ADXL345_Init(void);
//��ʼ��У׼
void ADXL345_Calibrate(int width) ;
//��ȡX������
short ADXL345_XTest(void);
//��ȡY������
short ADXL345_YTest(void);
//��ȡZ������
short ADXL345_ZTest(void);
//��ӡ���ٶ�ֵ
void printAccelerationG(void);
//
void printTable(void);
//
void ADXL345_TEMP_Calibrate(void);
//��ȡIDֵ
uint8_t Get_Adxl345_ID(void);
#endif
