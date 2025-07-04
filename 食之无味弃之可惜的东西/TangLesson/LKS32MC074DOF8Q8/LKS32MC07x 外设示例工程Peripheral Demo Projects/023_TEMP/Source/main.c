/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Main.c
 * �ļ���ʶ��
 * ����ժҪ�� ����������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/
#include "hardware_init.h"
#include "lks32mc07x_lib.h"

void Hardware_init(void);

/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ����ADC0���β���ģʽ���ֱ����3.6V��7.2V�������̶�ͨ��9���в�����
               �Աȼ����������������ADCSample[]
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/3/9      V1.0           Zhujie             ����
 *******************************************************************************/

u32 Cnt = 0;
s16 ADCSampleValue = 0;
s16 CurrentTempature = 0;
int main(void)
{
    Hardware_init();      /* Ӳ����ʼ�� */
	  PWMOutputs(ENABLE);
    for(;;)
    {
			  Cnt ++;
			  if(Cnt < 100000)
				{
				    GPIO_SetBits (GPIO0 ,GPIO_Pin_6);
					  GPIO_ResetBits (GPIO0 ,GPIO_Pin_7);
				}
				else if(Cnt < 200000)
				{
				    GPIO_ResetBits (GPIO0 ,GPIO_Pin_6);
					  GPIO_SetBits (GPIO0 ,GPIO_Pin_7);
				}
				else
				{
				    Cnt = 0;
				}
				ADCSampleValue = ADC_GetConversionValue(ADC1,DAT1);
        CurrentTempature = GetCurrentTemperature(ADCSampleValue) ;
			  SoftDelay(10);
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

