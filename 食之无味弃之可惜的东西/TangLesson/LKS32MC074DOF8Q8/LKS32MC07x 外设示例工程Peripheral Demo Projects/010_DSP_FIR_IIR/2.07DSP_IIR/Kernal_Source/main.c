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
#include "lks32mc07x_lib.h"
#include "hardware_init.h"

void UART_SetBaud(uint32_t Uart0Baud, uint32_t Uart1Baud);
/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 ���������    ��         
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ʵ��Ҫ��;ʹ��LED1 P0.6 IOÿ��һ��ʱ�䷭ת��ƽ��

 ����Ч����    ������¼��ŸLKS32MC071��Сϵͳ�壬ʹ��ʾ��������P0.6�ڲ���Ϊ�ߵ͵�ƽ��ת����ʹ�õװ�۲�LED1��˸��

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/26     V1.0          cakezhang             ����
 *******************************************************************************/
void dsp_fun_init(void);
int32_t test_dsp_datain;
int32_t test_dsp_dataincnt;
int32_t test_dsp_dataout;
const int data_input[100] = {
    0x0,
    0x54CB,
    0x4F1B,
    0xEE3,
    0x0,
    0x434B,
    0x7FFF,
    0x5DFE,
    0x0,
    0xFFFFD4CC,
    0x0,
    0x2B34,
    0x0,
    0xFFFFA202,
    0xFFFF8001,
    0xFFFFBCB5,
    0x0,
    0xFFFFF11D,
    0xFFFFB0E5,
    0xFFFFAB35,
    0x0,
    0x54CB,
    0x4F1B,
    0xEE3,
    0x0,
    0x434B,
    0x7FFF,
    0x5DFE,
    0x0,
    0xFFFFD4CC,
    0x0,
    0x2B34,
    0x0,
    0xFFFFA202,
    0xFFFF8001,
    0xFFFFBCB5,
    0x0,
    0xFFFFF11D,
    0xFFFFB0E5,
    0xFFFFAB35,
    0x0,
    0x54CB,
    0x4F1B,
    0xEE3,
    0x0,
    0x434B,
    0x7FFF,
    0x5DFE,
    0x0,
    0xFFFFD4CC,
    0x0,
    0x2B34,
    0x0,
    0xFFFFA202,
    0xFFFF8001,
    0xFFFFBCB5,
    0x0,
    0xFFFFF11D,
    0xFFFFB0E5,
    0xFFFFAB35,
    0x0,
    0x54CB,
    0x4F1B,
    0xEE3,
    0x0,
    0x434B,
    0x7FFF,
    0x5DFE,
    0x0,
    0xFFFFD4CC,
    0x0,
    0x2B34,
    0x0,
    0xFFFFA202,
    0xFFFF8001,
    0xFFFFBCB5,
    0x0,
    0xFFFFF11D,
    0xFFFFB0E5,
    0xFFFFAB35,
    0x0,
    0x54CB,
    0x4F1B,
    0xEE3,
    0x0,
    0x434B,
    0x7FFF,
    0x5DFE,
    0x0,
    0xFFFFD4CC,
    0x0,
    0x2B34,
    0x0,
    0xFFFFA202,
    0xFFFF8001,
    0xFFFFBCB5,
    0x0,
    0xFFFFF11D,
    0xFFFFB0E5,
    0xFFFFAB35};

void dsp_datain(int32_t val);
void dsp_delay(void);
int32_t dsp_getexitdata(void);
int main(void)
{
    IWDG_DISABLE();
	Hardware_init(); /* Ӳ����ʼ�� */
    __enable_irq();
    for (;;)
    {
        test_dsp_dataincnt += 1;
        if (test_dsp_dataincnt > 100)
        {
            test_dsp_dataincnt = 0;
        }
        test_dsp_datain = data_input[test_dsp_dataincnt];
        dsp_datain(test_dsp_datain);
        dsp_delay();
        test_dsp_dataout = dsp_getexitdata();
        for (int32_t i = 0; i < 100000; i++)
        {
            __NOP();
        }
    }
}

void UART_SetBaud(uint32_t Uart0Baud, uint32_t Uart1Baud)
{
    uint32_t d0 = SYS_ReadMcuClk() / Uart0Baud;
    uint32_t d1 = SYS_ReadMcuClk() / Uart1Baud;
    uint32_t max = d0 > d1 ? d0 : d1;
    uint32_t d2;
    d2 = max >> 16;
    d2 += 1;
    d0 /= d2;
    d1 /= d2;
    d0 -= 1;
    d1 -= 1;
    UART0->DIVL = d0 & 0xFF;
    UART0->DIVH = (d0 & 0xFF00) >> 8;
    UART1->DIVL = d1 & 0xFF;
    UART1->DIVH = (d1 & 0xFF00) >> 8;
    SYS->WR_PROTECT = 0x7A83;
    SYS_CLK_DIV2 = d2 - 1;
    SYS->WR_PROTECT = 0x0;
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

