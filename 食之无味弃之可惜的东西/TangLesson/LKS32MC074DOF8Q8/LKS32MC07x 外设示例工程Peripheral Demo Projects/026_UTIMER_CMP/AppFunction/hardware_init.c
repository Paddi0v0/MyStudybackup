/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_init.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ����ʼ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� HuangMG
 * ������ڣ� 2023��3��2��
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
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();                  /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;
    
    FLASH_CFG |= 0x00080000;          /* enable prefetch */
    IWDG_DISABLE();                   /* �رտ��Ź�*/
	  GPIO_init();                      /* UTimer��ʼ�� */
	  UTimer_init();                    /* UTimer��ʼ�� */
  	SoftDelay(100);                   /* �ȴ�Ӳ����ʼ�����*/
	  NVIC_SetPriority(TIMER0_IRQn, 0); /*TIMER0�ж����ȼ�����*/
    NVIC_EnableIRQ(TIMER0_IRQn);      /*ʹ��UTimer��ʱ���ж�*/
	  NVIC_SetPriority(TIMER3_IRQn, 1); /*TIMER3�ж����ȼ�����*/
    NVIC_EnableIRQ(TIMER3_IRQn);      /*ʹ��UTimer��ʱ���ж�*/
    SYS_WR_PROTECT = 0;               /* �ر�ϵͳ�Ĵ���д����*/
    __enable_irq();                   /* �������ж� */
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
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void Clock_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 = BIT15;      /* BIT15:PLLPDN ��PLL */
    SoftDelay(100);            /* ��ʱ100us, �ȴ�PLL�ȶ� 21.4.17*/
    SYS_CLK_CFG = 0x000001ff;  /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� */
    SYS_WR_PROTECT = 0;        /* �ر�ϵͳ�Ĵ���д����*/
}

/*******************************************************************************
 �������ƣ�    void Reg_Clr(void)
 ����������    �Ĵ�������
 ���������    addr ����Ҫ����Ĵ�������ʼ��ַ 
               nSize����Ҫ����ļĴ�������
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void Reg_Clr(u32 addr, u8 nSize)
{
   while(nSize --)
   {
      REG32(addr) = 0;
      addr += 4;
   }
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
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void SystemInit(void)
{
    Clock_Init();  /* ʱ�ӳ�ʼ�� */
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
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��

    //����P2.7 UTIMER0_CH0  
	  GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
	  GPIO_PinAFConfig(GPIO2,GPIO_PinSource_7,AF7_TIMER01); //P2.7����Ϊtimer0�����ģʽ
		
	  //����P2.11 UTIMER2_CH0 ��ΪUTIMER3���ⲿʱ�������
	  GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;  //GPIO����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIO2,GPIO_PinSource_11,AF8_TIMER23); //P2.11����Ϊtimer2_CH0������ģʽ
		
	  //����P1.3 UTIMER3_CH1
	  GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1,GPIO_PinSource_3,AF8_TIMER23); //P3.1����Ϊtimer3�����ģʽ
}

/*******************************************************************************
 �������ƣ�    void UTimer_init(void)
 ����������    UTimerӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void UTimer_init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerStrutInit(&TIM_InitStruct);                  /* Timer�ṹ���ʼ��*/
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH1 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH1Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_TH = 48000;    									/* ��ʱ���������޳�ʼֵ48000*/
    TIM_InitStruct.Timer_CMP0 = 24000;  									/* ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ24000 */
    TIM_InitStruct.Timer_CMP1 = 24000;  									/* ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ24000 */
    TIM_InitStruct.Timer_FLT = 0;  										    /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_ClockDiv = TIMER_CLK_DIV1;       /* ����Timerģ��ʱ��2��Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_CH1 | Timer_IRQEna_ZC; /* ����Timerģ��Ƚ��жϺ͹����ж�*/
    TIM_TimerInit(UTIMER0, &TIM_InitStruct); 
    TIM_TimerCmd(UTIMER0, ENABLE);                        /* Timer0 ģ��ʹ�� */	
	  
	  TIM_TimerStrutInit(&TIM_InitStruct);                   /* Timer�ṹ���ʼ��*/
/********************ʹ�ö�ʱ��2��ͨ��0����Ϊ��ʱ��3���ⲿʱ�����ó���*********************/	
    TIM_InitStruct.Timer_XCLK_EN = ENABLE;                /*���ö�ʱ��3ʱ��ʹ���ⲿʱ��*/
	  TIM_InitStruct.Timer_EVT = TIMER2_EVT_CH0;            /*���ö�ʱ��3���ⲿʱ����Դ�ڶ�ʱ��2��ͨ��0��������ź�*/
    SYS_ModuleClockCmd(SYS_Module_TIMER2, ENABLE);        /*ʹ�ܶ�ʱ��2��ʱ���ſأ����ڶԶ�ʱ��2�����ź��˲�*/
		UTIMER2_FLT = 0;                                      /*���ö�ʱ��ͨ���������˲�ֵ*/
		UTIMER2_CFG |= 0x00<<4;                               /*���ö�ʱ��2���źŲ���ģʽͨ��0�ź���Դ�ڸö�ʱ����ͨ��0*/
/********************��ʱ��3ͨ��1���PWM�������ó���*********************/			
	  TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH1 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH1Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_TH = 500;    									  /* ��ʱ���������޳�ʼֵ48000*/
    TIM_InitStruct.Timer_CMP0 = 250;  									  /* ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ24000 */
    TIM_InitStruct.Timer_CMP1 = 250;  									  /* ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ24000 */
    TIM_InitStruct.Timer_FLT = 0;  										    /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_ClockDiv = TIMER_CLK_DIV2;       /* ����Timerģ��ʱ��2��Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_CH1 | Timer_IRQEna_ZC; /* ����Timerģ��Ƚ��жϺ͹����ж�*/
    TIM_TimerInit(UTIMER3, &TIM_InitStruct); 
    TIM_TimerCmd(UTIMER3, ENABLE);                        /* Timer3 ģ��ʹ�� */
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
 2023/3/2      V1.0          HuangMG            ����
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for(t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}




