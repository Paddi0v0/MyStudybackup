/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Main.c
 * �ļ���ʶ��
 * ����ժҪ�� ����������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� ����
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2015��11��5��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�����
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#include "basic.h"
#include "lks32mc08x.h"
#include "lks32mc08x_adc.h"
#include "freemaster.h"
//#include "absacc.h"
#include "lks32mc08x_uart.h"
#include "lks32mc08x_Gpio.h"
#include "lks32mc08x_tim.h"
#include "hardware_config.h"
#include "lks32mc08x_spi.h"
#include "lks32mc08x_i2c.h"
#include "test_spi.h"
void UART1_SENDDATA(UINT8 n);
void Clock_Init(void);
void UART0_init(void);
void ADC1_init(void);
void Hardware_init(void);
void MCPWM_init(void);
void UTimer_init(void);
void GPIO_init(void);
void UART0_SENDDATA(UINT8 n);
void delay(u32 cnt);
void CMP_Init(void);


/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
int main(void)
{

   Hardware_init();

	 ADC0_CHN0 = 0xb0b;

   for(;;)
   {
		 
		  delay(0x2fffff);

	 }

}
   
void delay(u32 cnt)
{
   u32 t_cnt;
	
   for(t_cnt = 0; t_cnt < cnt; t_cnt++)
   {
      __nop();
   }
	
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
