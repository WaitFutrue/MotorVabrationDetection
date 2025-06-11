#include "ADXL345.h"
#include "stdio.h"

#define SET_SPI_CS_H	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_SET);//SPI CS ����  ��
#define SET_SPI_CS_L	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port,SPI1_CS_Pin,GPIO_PIN_RESET);//SPI CS ���� ��

/********************************************************************
   * @brief  : д�ֽ�
   * @param  : ��ַ������
   * @retval :
*********************************************************************/
void ADXL345_Write(uint8_t addr, uint8_t value)
{
	addr &= 0x3F;
	SET_SPI_CS_L;

//ʹ��DMA����SPI����
	HAL_SPI_Transmit_DMA(&hspi1, &addr, 1);  // HAL_SPI_Transmit(&hspi1, &addr, 1, 10);
	HAL_SPI_Transmit_DMA(&hspi1, &value, 1); // HAL_SPI_Transmit(&hspi1, &value, 1, 10);
//�ȴ��������
	//while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	
	SET_SPI_CS_H;
}
/********************************************************************
   * @brief  : ���ֽ�
   * @param  : ��ַ������
   * @retval :
*********************************************************************/
void ADXL345_Read(uint8_t addr, uint8_t *value)
{
	addr &= 0x3F;	
	addr |= (0x80);
	SET_SPI_CS_L;

//ʹ��DMA����SPI����
	HAL_SPI_Transmit_DMA(&hspi1, &addr, 1);  // HAL_SPI_Transmit(&hspi1, &addr, 1, 10);
	HAL_SPI_Receive_DMA(&hspi1, value, 1);   // HAL_SPI_Receive(&hspi1, value, 1, 10);

	SET_SPI_CS_H;
}
/********************************************************************
   * @brief  : ��ȡIDֵ
   * @param  :
   * @retval :���ض�ȡ����IDֵ
*********************************************************************/
uint8_t Get_Adxl345_ID(void)
{
	uint8_t DEVICEID = 0x00;
	uint8_t result = 0;
	ADXL345_Read(DEVICEID, &result);

	return result;
}
/********************************************************************
   * @brief  : ��ʼ��������
   * @param  :
   * @retval :��ӡ��ȷֵ
*********************************************************************/
void ADXL345_Init(void)
{
	while (Get_Adxl345_ID() != I_M_DEVID)//e5
	{
		printf_DMA("ADXL345 Init Fail:%x\r\n", Get_Adxl345_ID());
		HAL_Delay(1000);
	}

	ADXL345_Write(INT_ENABLE, 0x00);    //�ж�ʧ��
	ADXL345_Write(DATA_FORMAT, 0x0B);   //�Ҷ��룿
	ADXL345_Write(BW_RATE, 0x10|0X0F);       //������ʣ�0~15_0X0~0XF,���3200HZ
	ADXL345_Write(POWER_CTL, 0x08);     //�Զ�˯��
	ADXL345_Write(INT_ENABLE, 0x14);

	printf_DMA("ADXL345 Init Success\r\nThe ID is %x\r\n",Get_Adxl345_ID());
}
/*******************************************************************************
* ������     ��ADXL345У׼����
* �������� ��width --> ƽ�����ڿ��
* ����ֵ	   ��
*******************************************************************************/

void ADXL345_Calibrate(int width) {
	uint8_t x0, x1, y0, y1, z0, z1;
	int16_t sumx = 0, sumy = 0, sumz = 0;	
    int16_t x_avg;
    int16_t y_avg;
    int16_t z_avg;
    
    // 1. ���ƫ������
    ADXL345_Write(OFSX, 0);
    ADXL345_Write(OFSY, 0);
    ADXL345_Write(OFSZ, 0);
	// 2. ��ȡԭʼ����ֵ_����ƽ�����ڴ���
	//���ƶ�ƽ���˲�
    for(int i=0;i<width;i++)
	{
		ADXL345_Read(DATA_X0, &x0);
		ADXL345_Read(DATA_X1, &x1);
		ADXL345_Read(DATA_Y0, &y0);
		ADXL345_Read(DATA_Y1, &y1);
		ADXL345_Read(DATA_Z0, &z0);
		ADXL345_Read(DATA_Z1, &z1);
		//ƴ�����ݡ������������datasheet����P27
		int16_t x_data = (x1 << 8) | x0;
		int16_t y_data = (y1 << 8) | y0;
		int16_t z_data = (z1 << 8) | z0;
		//���
		sumx += x_data;
		sumy += y_data;
		sumz += z_data;
	}
	//��ƽ����
	x_avg = sumx / width;
	y_avg = sumy / width;
	z_avg = sumz / width;
	//printf("Average: x_avg=%d, y_avg=%d, z_avg=%d\r\n", x_avg, y_avg, z_avg);
    
	// 3. ����ƫ��ֵ(����ֵ��0, 0, -256)��
	//ȫ�ֱ��ʣ�16g�£�������ӣ�256LSB/g����0.00390625g/LSB��3.90625mg/LSB
	// 0g��Ӧ��ֵΪ512��-1g��Ӧ��ֵΪ-256����ʽ��ƫ��ֵ = -��ʵ��ƽ��ֵ - ����ֵ�� / �������
	int8_t ofs_x = -(x_avg - 0) / 3.90625;
	int8_t ofs_y = -(y_avg - 0) / 3.90625;
	int8_t ofs_z = -(z_avg - 512) / 3.90625;
    
	//4. д��ƫ�ƼĴ��� ȷ����-128~+127��Χ��
    ADXL345_Write(OFSX, ofs_x & 0XFF);
    ADXL345_Write(OFSY, ofs_y & 0XFF);
    ADXL345_Write(OFSZ, ofs_z & 0XFF);
    // ��֤���
    //printf("Calibrated: OFSX=%d, OFSY=%d, OFSZ=%d\r\n", ofs_x, ofs_y, ofs_z);
}
/*******************************************************************************
* ������     ����ӡ������ٶȣ�g��λ��
* �������� ��
* ����ֵ	   ��
*******************************************************************************/
void printAccelerationG()
{
    //�Ż��󣨼��ٱ�����ֵ���Լ�����VOFA�е�ë�̣�
        printf("%.5f,%.5f,%.5f\r\n", (float)ADXL345_XTest()*0.00390625f, (float)ADXL345_YTest()*0.00390625f, (float)ADXL345_ZTest()*0.00390625f);
}

//��ȡX������
short ADXL345_XTest(void)
{
	short x;
    
	uint8_t xl, xh;

	ADXL345_Read(DATA_X0, &xl);
	ADXL345_Read(DATA_X1, &xh);

	x = (short)(((uint16_t)xh << 8) + xl);

	//printf("X:%d\r\n", x);
    return  x;
}
//��ȡY������
short ADXL345_YTest(void)
{
	short Y;
	uint8_t Yl, Yh;

	ADXL345_Read(DATA_Y0, &Yl);
	ADXL345_Read(DATA_Y1, &Yh);

	Y = (short)(((uint16_t)Yh << 8) + Yl);

	//printf("Y:%d\r\n", Y);
    return Y;
}
//��ȡZ������
short ADXL345_ZTest(void)
{
	short Z;
	uint8_t Zl, Zh;

	ADXL345_Read(DATA_Z0, &Zl);
	ADXL345_Read(DATA_Z1, &Zh);

	Z = (short)(((uint16_t)Zh << 8) + Zl);

	//printf("Z:%d\r\n", Z);
    return Z;
}
/*******************************************************************************
* ������     ����ӡ�¶�-�������ݱ��
* �������� ������ʽ�����¶ȣ�X��Y��Z
* ����ֵ	   ���˺��������������߷����׶�
*******************************************************************************/
void printTable()
{
	printf("Temperature\tX\tY\tZ\r\n");
	printf("------------------------------------------------\r\n");
	for (unsigned int i = 0; i < 1000; i++)
	{
		printf("%.2f\t%.2f\t%.2f\t%.2f\r\n", (float)DS18B20_Get_Temperature(), (float)ADXL345_XTest(), (float)ADXL345_YTest(), (float)ADXL345_ZTest());
		//HAL_Delay(1);//����65536*1ms
	}
}

_Bool flag_first=1;
short TempChange,PreTemp;

void ADXL345_TEMP_Calibrate()
{
  if(flag_first){ //��һ��
    PreTemp=DS18B20_Get_Temperature();  //�ȼ�¼һ����ʷֵ
    //TempChange=DS18B20_Get_Temperature()-PreTemp;//������
    flag_first=0;}//�Ժ���Ҳ���ô˱�־λ
  else {  //��һ��֮��
    if((DS18B20_Get_Temperature()-PreTemp)>5){  //ͨ�����ڵ��¶Ⱥ���ʷֵ����
      ADXL345_Calibrate(600); //���������ͽ���У׼
      PreTemp=DS18B20_Get_Temperature();  //ͬʱˢ����ʷֵ
    }//���û�нϴ������ǾͲ����κμ��㣬Ҳ��ˢ����ʷֵ
  }
}
