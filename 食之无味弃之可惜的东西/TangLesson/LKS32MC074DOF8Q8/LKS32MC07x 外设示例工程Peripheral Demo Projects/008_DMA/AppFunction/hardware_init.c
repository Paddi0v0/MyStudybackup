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
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "hardware_init.h"

/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();         /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83; /*ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000; /* enable prefetch ��FLASHԤȡ����ʹ��*/
		IWDG_DISABLE();						/*�رտ��Ź�*/
		DMA_init(); 							/*DMA���ó�ʼ��*/
		GPIO_init();
		UART1_init();
		SoftDelay(100);                  /* �ȴ�Ӳ����ʼ�����*/
		NVIC_EnableIRQ(DMA0_IRQn);
		NVIC_SetPriority(DMA0_IRQn, 1);
	  SYS_WR_PROTECT = 0x0;    /*�ر�ϵͳ�Ĵ���д����*/
	  __enable_irq();          /* �������ж� */
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
    GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��
	   //����UART0_RXD  P2.8
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
	
    //����UART0_TXD  P3.9
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO3, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_8, AF4_UART);
    GPIO_PinAFConfig(GPIO3, GPIO_PinSource_9, AF4_UART);
}


/*******************************************************************************
 �������ƣ�    void UART0_init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UART0_init(void)
{
    UART_InitTypeDef UART_InitStruct;
    
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = 38400;                 /* ���ò�����38400 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;  /* �������ݳ���8λ */
    UART_InitStruct.StopBits = UART_STOPBITS_1b;
    UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;   /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;      /* ����żУ�� */
    UART_InitStruct.IRQEna = 0;
    UART_Init(UART0, &UART_InitStruct);

}

/*******************************************************************************
 �������ƣ�    void UART1_init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/02/11      V1.0           ywx          ����
 *******************************************************************************/
void UART1_init(void)
{
    UART_InitTypeDef UART_InitStruct;
    
    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = 38400;                 /* ���ò�����38400 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;  /* �������ݳ���8λ */
    UART_InitStruct.StopBits = UART_STOPBITS_1b;
    UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;   /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;      /* ����żУ�� */
		UART_InitStruct.DMARE = TX_DMA_RE;
    UART_Init(UART1, &UART_InitStruct);

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
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;      /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 |= BIT15 ;       /* BIT15:PLLPDN*/
    SoftDelay(100);               /* �ȴ�Ӳ����ʼ�����*/	
	  SYS_CLK_CFG = 0x000001ff;     /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� ; ��������ʱ��*/
    SYS_WR_PROTECT = 0x0;         /*�ر�ϵͳ�Ĵ���д����*/
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
    Clock_Init(); /* ʱ�ӳ�ʼ�� */
}

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    �����ʱ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for(t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}

/*******************************************************************************
 �������ƣ�    void DMA_init(void)
 ����������    DMA��ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ʣ�����ж�DMA_UART�Ĵ��䷽��
               ��DMA_DIR����DMA_UART�Ĵ��䷽��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/02/22      V1.0             ywx                ����
 *******************************************************************************/
u8 DMA_TX[13] = {'L', 'K', 'S', '_','D','M','A', '_', 'T', 'e','s','t','\n'};/*�������ݶ���*/
void DMA_init(void)
{
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.IRQ_EN = ENABLE;
    DMA_InitStruct.CIRC = DISABLE;
    DMA_InitStruct.SINC = ENABLE;
    DMA_InitStruct.DINC = DISABLE;
    DMA_InitStruct.SBTW = DMA_BYTE_TRANS;
    DMA_InitStruct.DBTW = DMA_BYTE_TRANS;
    DMA_InitStruct.REN.B.UART1_TX_EN = ENABLE;
    DMA_InitStruct.TMS = 13;
    DMA_InitStruct.RMODE = ENABLE;
    DMA_InitStruct.SADR = (u32)DMA_TX;
    DMA_InitStruct.DADR = (u32)&UART1_BUFF;
    DMA_InitStruct.EN = ENABLE;
    DMA_Init(DMA_CHN3, &DMA_InitStruct);
}


