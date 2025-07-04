#include "basic.h"
#include "lks32mc08x.h"
#include "lks32mc08x_adc.h"
#include "freemaster.h"
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x_flash.h"
#include "lks32mc08x_Gpio.h"
#include "lks32mc08x_uart.h"
#include "hardware_config.h"
#include "lks32mc08x_hall.h"
#include "lks32mc08x_tim.h"
#include "lks32mc08x_cmp.h"

void PGA_Init(void);
void DAC_Init(void);
void CMP_Init(void);
void MCPWM_init(void);
void UTimer_init(void);
void GPIO_init(void);
void UART1_init(void);
void ADC0_init(void);
void get_ic_temp_init(void);
void delay(u16 cnt);
void Clock_Init(void);


/*******************************************************************************
 �������ƣ�    void SystemInit(void)
 ����������    У��ֵ��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/
void SystemInit(void)
{

    Clock_Init();

}

/*******************************************************************************
 �������ƣ�    void PGA_Init(void)
 ����������    PGA��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/15      V1.0           Howlet Li          ����
 *******************************************************************************/
void PGA_Init(void)
{
	SYS_WR_PROTECT = 0x7a83;
    SYS_AFE_REG5 |= 0x20ff;



    SYS_AFE_REG0 = (SYS_AFE_REG0 & 0xff00); // 4��OPA��������
}

/*******************************************************************************
 �������ƣ�    void CMP_Init(void)
 ����������    �Ƚ�����ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/15      V1.0           Howlet Li          ����
 *******************************************************************************/
void CMP_Init(void)
{
	  CMP_InitTypeDef CMP_InitStruct;
	
	  CMP_StructInit(&CMP_InitStruct);
	
	  CMP_InitStruct.CMP0_EN = ENABLE;
	  CMP_InitStruct.CMP0_SELN = SELN_DAC;
	  CMP_InitStruct.CMP0_SELP = SELP_CMP_IP1;
	
	  CMP_InitStruct.CMP0_IE = ENABLE;/* �Ƚ���0�ź��ж�ʹ�� */
	  CMP_InitStruct.CMP0_InEnable = ENABLE; /* �Ƚ����ź�����ʹ�� */
	  CMP_InitStruct.CMP_FltCnt = 15; /* ���˲����=tclk ����*16*CMP_FltCnt */
    CMP_InitStruct.CMP_CLK_EN = ENABLE; /* ʱ��ʹ�� */
	
	  Comparator_init(&CMP_InitStruct);
	

	  	
}

/*******************************************************************************
 �������ƣ�    void DAC_Init(void)
 ����������    DAC��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/23      V1.0           Howlet Li          ����
 *******************************************************************************/
void DAC_Init(void)
{
    SYS_AFE_REG5 |= 0x1000; /* BIT12 ADCʹ�ܿ��� */
	
	  SYS_AFE_REG1 |= (0x01<<6); /* ����DAC������Ϊ1.2V��00:3V| 01:1.2V| 10:4.85V */


	  SYS_AFE_DAC = 700;
	
	
}
/*******************************************************************************
 �������ƣ�    void Clock_Init(void)
 ����������    ʱ������
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
void Clock_Init(void)
{

	  SYS_WR_PROTECT = 0x7a83;
	  SYS_AFE_REG5 |= 0x8000;
    SYS_CLK_CFG |= 0x000001ff;/* select fast clock */
    SYS_CLK_DIV2  = 0x0000;
    SYS_CLK_FEN = 0xff;
//	  SYS_AFE_REGC = 0x02; /* TRIM RCH */
}



/*******************************************************************************
 �������ƣ�    void UART1_init(void)
 ����������    UART1�Ĵ�������
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
void UART1_init(void)
{

   UART_InitTypeDef UART_InitStruct;

   UART_StructInit(&UART_InitStruct);
   UART_InitStruct.BaudRate = 9600;
   UART_InitStruct.WordLength = UART_WORDLENGTH_8b;
   UART_InitStruct.StopBits = UART_STOPBITS_1b;
   UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;
   UART_InitStruct.ParityMode = UART_Parity_NO;
   UART_InitStruct.IRQEna = 0;
   UART_Init(UART1, &UART_InitStruct);

}

/*******************************************************************************
 �������ƣ�    void UART1_SENDDATA(void)
 ����������    UART1���ͳ���
 �����ı�    ��
 ���������    n����Ҫ���͵�ֵ
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UART1_SENDDATA(UINT8 n)
{
    UART1_BUFF = n;
}

/*******************************************************************************
 �������ƣ�    void ADC0_init(void)
 ����������    ADC0Ӳ����ʼ��
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
void ADC0_init(void)
{

    ADC_InitTypeDef ADC_InitStructure;

    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN;
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;
    ADC_InitStructure.FirSeg_Ch = 8;
    ADC_InitStructure.SecSeg_Ch = 2;
    ADC_InitStructure.Trigger_Cnt = 0;
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG;
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;
    ADC_Init(ADC0, &ADC_InitStructure);

	
    //channel select
		
		ADC0_IF = 0xff;
 	  
/* ��ѹͨ����ӦADC_CH4 */		
/* ת�Ѷ�ӦADC_CH9 */		
/* ĸ�ߵ���OPA3 */	
/* A�����OPA0 */	
/* B�����OPA1 */		
/* C�����OPA2 */
}

/*******************************************************************************
 �������ƣ�    void HALL_Init(void)
 ����������    GPIOӲ����ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2018/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void HALL_Perip_Init(void)
{
	
	 HALL_InitTypeDef HALL_InitStruct;
	 HALL_StructInit(&HALL_InitStruct);
	
	 HALL_InitStruct.FilterLen = 512;
   HALL_InitStruct.ClockDivision = HALL_CLK_DIV1;
   HALL_InitStruct.Filter75_Ena = DISABLE;
   HALL_InitStruct.HALL_Ena = ENABLE;
   HALL_InitStruct.Capture_IRQ_Ena = ENABLE;
   HALL_InitStruct.OverFlow_IRQ_Ena = ENABLE;
   HALL_InitStruct.CountTH = 9600000;
	 HALL_InitStruct.softIE = DISABLE;
	
	 HALL_Init(&HALL_InitStruct);
}
/*******************************************************************************
 �������ƣ�    void GPIO_init(void)
 ����������    GPIOӲ����ʼ��
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

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    /*  |-- P0.11 P0.12 P0.13 Hall Signal Input--| P0.14 һ��ͨ | P0.15 Power Select */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
//    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_11, AF2_HALL);
//    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_12, AF2_HALL);
//    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_13, AF2_HALL);



    /* P1.0 AntiThief | P1.10 Direct Input | P1.11 High Speed input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 ;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    /* MCPWM P1.4~P1.9 */
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
		
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_4, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_5, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_6, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_7, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_8, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF3_MCPWM);
    

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_10 | GPIO_Pin_11 ;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    
    /* P2.5 UART1 TXD */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 ;
    GPIO_Init(GPIO2, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_4, AF4_UART);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_5, AF4_UART);
		
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 ;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIO2, GPIO_PinSource_2, AF0_GPIO);
    GPIO_SetBits(GPIO2,GPIO_Pin_2);
}

/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
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
void Hardware_init(void)
{
    __disable_irq();/* �ر��ж� �ж��ܿ��� */

    FLASH_CFG |= 0x00080000; /* enable prefetch */
	  SYS_WR_PROTECT = 0x7a83;

    UART1_init();   /* ���ڳ�ʼ�� */
    ADC0_init();    /* ADC��ʼ�� */
    MCPWM_init();   /* PWM��ʼ�� */
    UTimer_init();
    GPIO_init();    /* GPIO��ʼ�� */
    DAC_Init();
    PGA_Init();
    CMP_Init();
	  HALL_Perip_Init();

    DSP_SC = BIT1; /* DSP Paused */




    delay(100);

    NVIC_SetPriority(UART1_IRQn, 2);
    NVIC_SetPriority(ADC0_IRQn, 1);

    NVIC_SetPriority(HALL_IRQn, 2);
    NVIC_SetPriority(MCPWM_IRQn, 2);
    NVIC_SetPriority(CMP_IRQn, 0);
    NVIC_SetPriority(TIMER0_IRQn, 2);

    
//   NVIC_EnableIRQ (GPIO_IRQn);
    NVIC_EnableIRQ(ADC0_IRQn);   /* enable the ADC0 interrupt */
//    NVIC_EnableIRQ(ADC1_IRQn); /* enable the ADC1 interrupt */
    NVIC_EnableIRQ (HALL_IRQn);
    NVIC_EnableIRQ (TIMER0_IRQn);
//   NVIC_EnableIRQ (TIMER1_IRQn);
//   NVIC_EnableIRQ (TIMER2_IRQn);
//   NVIC_EnableIRQ (MCPWM_IRQn);
//    NVIC_EnableIRQ (CMP_IRQn);
#ifdef FMSTR_FUNCTION	
    NVIC_EnableIRQ (UART1_IRQn);
    FMSTR_Init();
#else
    NVIC_DisableIRQ (UART1_IRQn);
#endif
    __enable_irq();/* �����ж� */

}

/*******************************************************************************
 �������ƣ�    void UTimer_init(void)
 ����������    UTimerӲ����ʼ��
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
void UTimer_init(void)
{
   TIM_TimerInitTypeDef TIM_InitStruct;
   TIM_TimerCmd(TIMER0, ENABLE);
   TIM_TimerStrutInit(&TIM_InitStruct);
   TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP;
   TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_None;
   TIM_InitStruct.Timer_CH0Output = 0;
   TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP;
   TIM_InitStruct.Timer_CH1_CapMode = TIMER_CapMode_None;
   TIM_InitStruct.Timer_CH1Output = 0;
   TIM_InitStruct.Timer_TH = 48000;
   TIM_InitStruct.Timer_CMP0 = 240000;
   TIM_InitStruct.Timer_CMP1 = 500;
   TIM_InitStruct.Timer_Filter0 = 0;
   TIM_InitStruct.Timer_Filter1 = 0;
   TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div1;
   TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_Zero;
   TIM_TimerInit(TIMER0, &TIM_InitStruct);
   


}

/*******************************************************************************
 �������ƣ�    void MCPWM_init(void)
 ����������    MCPWMӲ����ʼ��
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
void MCPWM_init(void)
{
	  MCPWM_InitTypeDef MCPWM_InitStructure;

    MCPWM_StructInit(&MCPWM_InitStructure);
	
	  MCPWM_InitStructure.CLK_DIV = 0;
	  MCPWM_InitStructure.MCLK_EN = ENABLE;
	  MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE; /* ʱ������ */
	  MCPWM_InitStructure.MCPWM_WorkModeCH0 = CENTRAL_PWM_MODE;
	  MCPWM_InitStructure.MCPWM_WorkModeCH1 = CENTRAL_PWM_MODE;
		MCPWM_InitStructure.MCPWM_WorkModeCH2 = CENTRAL_PWM_MODE; /* ����ģʽ���� */	
	
	  MCPWM_InitStructure.GPIO_BKIN_Filter = 12;
	  MCPWM_InitStructure.CMP_BKIN_Filter = 12; /* �˲���ʱ������ */
	
	  MCPWM_InitStructure.MCPWM_PERIOD = PWM_PERIOD;
	  MCPWM_InitStructure.TriggerPoint0 = (u16)(100-PWM_PERIOD);
	  MCPWM_InitStructure.TriggerPoint1 = (u16)(PWM_PERIOD-1);
	  MCPWM_InitStructure.DeadTimeCH0N = DEADTIME;
	  MCPWM_InitStructure.DeadTimeCH0P = DEADTIME;
	  MCPWM_InitStructure.DeadTimeCH1N = DEADTIME;
	  MCPWM_InitStructure.DeadTimeCH1P = DEADTIME;
		MCPWM_InitStructure.DeadTimeCH2N = DEADTIME;
	  MCPWM_InitStructure.DeadTimeCH2P = DEADTIME; /* ���ڣ�����ʱ������ */
	
	  MCPWM_InitStructure.CH0N_Polarity_INV = ENABLE;
	  MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE;
	  MCPWM_InitStructure.CH1N_Polarity_INV = ENABLE;
	  MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
	  MCPWM_InitStructure.CH2N_Polarity_INV = ENABLE;
	  MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE; /* ����ѡ������ */
		
	  MCPWM_InitStructure.Switch_CH0N_CH0P = ENABLE; /* ͨ������ѡ������ */
		MCPWM_InitStructure.Switch_CH1N_CH1P = ENABLE; /* ͨ������ѡ������ */
		MCPWM_InitStructure.Switch_CH2N_CH2P = ENABLE; /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
		                                                ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ�������*/
		MCPWM_InitStructure.CH0P_default_output = HIGH_LEVEL;
		MCPWM_InitStructure.CH0N_default_output = LOW_LEVEL;
		MCPWM_InitStructure.CH1P_default_output = HIGH_LEVEL;
		MCPWM_InitStructure.CH1N_default_output = LOW_LEVEL;
		MCPWM_InitStructure.CH2P_default_output = HIGH_LEVEL;
		MCPWM_InitStructure.CH2N_default_output = LOW_LEVEL; 
		
		MCPWM_InitStructure.DebugMode_PWM_out = ENABLE;
		
		MCPWM_InitStructure.MCPWM_T0_UpdateEN = ENABLE;
		MCPWM_InitStructure.MCPWM_T1_UpdateEN = DISABLE; /* �����¼����� */
		
		MCPWM_InitStructure.T0_Update_INT_EN = DISABLE;  /* T0���� �ж�ʹ�� */
		MCPWM_InitStructure.FAIL0_INT_EN = DISABLE; /* FAIL0 �ж�ʹ�� */
		
		MCPWM_Init(&MCPWM_InitStructure);
		


}


