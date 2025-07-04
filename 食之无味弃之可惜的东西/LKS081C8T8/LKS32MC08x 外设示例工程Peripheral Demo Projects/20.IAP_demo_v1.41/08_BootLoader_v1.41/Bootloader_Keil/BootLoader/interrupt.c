#include "comdata.h"
#include "hardinit.h"

/*******************************************************************************
 �������ƣ�    void PWRDN_IRQHandler(void)
 ����������    ��ԴǷѹ�жϺ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/10/19      V1.0           HuangMG           ����
 *******************************************************************************/
void PWRDN_IRQHandler(void)
{
   SYS_VolSelModuleIRQ(); 
}

extern volatile u8 jumpFlg;
extern UINT32 tickCount;

void UTIMER0_IRQHandler(void)
{
    if(UTIMER_IF & BIT0)
    {
        UTIMER_IF = BIT0;
        tickCount++;

        if(tickCount == 100)				// 100*0.5 = 50ms
        {
            //�����ȴ�ʱ�䣬ֱ�ӽ����û�����
			      jumpFlg = 1;
            tickCount = 0;
        }
    }
}

void TIMER1_IRQHandler(void)
{
    while(1);
}

void TIMER2_IRQHandler(void)
{
    while(1);
}

void TIMER3_IRQHandler(void)
{
    while(1);
}

void ENCODER0_IRQHandler(void)
{
    while(1);
}

void ENCODER1_IRQHandler(void)
{
    while(1);
}

void I2C0_IRQHandler(void)
{
    while(1);
}

void GPIO_IRQHandler(void)
{
    while(1);
}

static u8 revData0 = 0;
void UART0_IRQHandler(void)
{
    if(UART0_IF & BIT1)								//�������
    {
        UART0_IF = BIT1;
		revData0 = UART0_BUFF;
        AddRecvData(revData0);
    }
}

static u8 revData1 = 0;
void UART1_IRQHandler(void)
{
	if(UART1_IF & BIT1)								//�������
    {
		UART1_IF = BIT1;
		revData1 = UART1_BUFF;
		AddRecvData(revData1);
    }
}

void HALL_IRQHandler(void)
{
    while(1);
}

void ADC0_IRQHandler(void)
{
    while(1);
}

void ADC1_IRQHandler(void)
{
    while(1);
}

void MCPWM_IRQHandler(void)
{
    while(1);
}

