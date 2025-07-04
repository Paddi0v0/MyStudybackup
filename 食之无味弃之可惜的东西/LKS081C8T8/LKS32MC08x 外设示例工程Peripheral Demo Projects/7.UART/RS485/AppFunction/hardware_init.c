/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_init.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ����ʼ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2022��4��14��
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "lks32mc08x_lib.h"
#include "hardware_init.h"
#include "basic.h"

/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();                 /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;         /* ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;         /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();                     /* GPIO��ʼ�� */
    UART_init();                     /* UART_init ��ʼ�� */
    SoftDelay(100);                  /* �ȴ�Ӳ����ʼ�����*/
    NVIC_EnableIRQ(UART0_IRQn);      /* ʹ��UART_IRQn�ⲿ�ж�*/
    NVIC_SetPriority(UART0_IRQn, 0); /* UART_IRQn�ⲿ�ж����ȼ�����Ϊ0*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
    __enable_irq();                  /* �����ж� */
}

/*******************************************************************************
 �������ƣ�    void GPIO_init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /*UART0 P0.15-RX0*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; /*GPIO����Ϊ����ģʽ*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;   /*PIN15*/
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    /*UART0 P1.0-TX0*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;/*GPIO����Ϊ���ģʽ*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;    /*PIN0*/
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_15, AF4_UART); // P0.15���ó�UART1_RX
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF4_UART);  // P1.0 ���ó�UART1_TX
}

/*******************************************************************************
 �������ƣ�    void Clock_Init(void)
 ����������    ʱ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/10/23      V2.0         HuangMG        ����MCU��Դ��⹦��
 *******************************************************************************/
void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 |= BIT15;     /* BIT15:PLLPDN */
		#if POWER_MODE
		SYS_VolSelModule(MCU_POWER_5v0);       /* MCU�����ѹ��1��5V��0;3.3V*/
		#else
		SYS_VolSelModule(MCU_POWER_3v3);       /* MCU�����ѹ��1��5V��0;3.3V*/
		#endif
	  SoftDelay(100);            /* �ȴ�PLL�ȶ�*/
	  SYS_WR_PROTECT = 0x7a83;       /* ���ϵͳ�Ĵ���д���� */
	
    SYS_CLK_CFG = 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� */
	  SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
}

/*******************************************************************************
 �������ƣ�    void SystemInit(void)
 ����������    Ӳ��ϵͳ��ʼ��������ʱ�ӳ�ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/
void SystemInit(void)
{
    Clock_Init(); /* ʱ�ӳ�ʼ��*/
}

/*******************************************************************************
 �������ƣ�    void UART_init(void)
 ����������    UART�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            ����
 *******************************************************************************/
void UART_init(void)
{
    UART_InitTypeDef UART_InitStruct;

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate     = 9600;                 /* ���ò�����9600 */
    UART_InitStruct.WordLength   = UART_WORDLENGTH_8b;   /* �������ݳ���8λ */
    UART_InitStruct.StopBits     = UART_STOPBITS_1b;     /* ֹͣλ����1λ */
    UART_InitStruct.FirstSend    = UART_FIRSTSEND_LSB;   /* �ȷ���LSB */
    UART_InitStruct.ParityMode   = UART_Parity_NO;       /* ����żУ�� */
    UART_InitStruct.RXD_INV      = DISABLE;              /* RXD��ƽ�������*/
    UART_InitStruct.TXD_INV      = DISABLE;              /* TXD��ƽ�������*/
    UART_InitStruct.MultiDropEna = ENABLE;               /* ʹ�� Multi-drop*/
    UART_InitStruct.Match485Addr = 0x32;                 /* 485ͨ��ʱ��ƥ���ַ*/	
    /*ʹ�ܽ��պͷ�������ж�*/
    UART_InitStruct.IRQEna = UART_IRQEna_SendOver | UART_IRQEna_RcvOver;
    UART_Init(UART0, &UART_InitStruct);
}

/**********************ʹ��printf������Ҫ��ӵĺ���*****************************************/
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
    /* ����һ���ֽ����ݵ����� */
    UART_SendData(UART0, (uint8_t)ch);
    /* ���ͻ���״̬λ */
    while ((UART0->STT & BIT0) == FALSE)
        ; // UART0->STT���ͻ���״̬λ��

    return (ch);
}

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    ��ʱ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            ����
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    u32 t_cnt;

    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __nop();
    }
}

/*******************************************************************************
 �������ƣ�    void Invers_GPIO(void)
 ����������    LED��ƽ��ת����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0           HuangMG            ����
 *******************************************************************************/
void Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) // GPIO��ƽ��ת����
{
    u16 reg;
    reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); //��GPIO��ƽ
    if (reg)
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    }
    else
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);
    }
}
