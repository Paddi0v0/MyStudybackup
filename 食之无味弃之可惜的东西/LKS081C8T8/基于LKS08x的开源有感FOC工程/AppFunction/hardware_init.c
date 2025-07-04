#include "global_variable.h"
#include "hardware_config.h"
#include "function_config.h" 

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
    __disable_irq();/* �ر��ж� �ж��ܿ��� */
	SYS_WR_PROTECT = 0x7a83;

    FLASH_CFG |= 0x00080000; /* enable prefetch */

	DSP_Init();                       	/* DSPģ���ʼ��*/
	ADC_init();    						/* ADC��ʼ�� */
	MCPWM_init();   					/* PWM��ʼ�� */
    UTimer_init(); 						/* ͨ�ü�������ʼ�� */
    GPIO_init();    					/* GPIO��ʼ�� */
    DAC_init();                       	/* DAC ��ʼ�� */
    PGA_init();                       	/* PGA ��ʼ�� */
    CMP_init();                       	/* �Ƚ�����ʼ�� */
    HALL_Perip_Init();                	/* HALLģ���ʼ�� */
	
	SoftDelay(100);                   	/* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */

    DSP_SC = BIT1; /* DSP Paused */

    NVIC_SetPriority(ADC0_IRQn, 1);
    NVIC_SetPriority(HALL_IRQn, 2);
    NVIC_SetPriority(CMP_IRQn, 0);
    NVIC_SetPriority(TIMER0_IRQn, 2);

    NVIC_EnableIRQ(ADC0_IRQn);   /* enable the ADC0 interrupt */
    NVIC_EnableIRQ (TIMER0_IRQn);
    NVIC_EnableIRQ (CMP_IRQn);
	
    __enable_irq();/* �����ж� */
}

/*******************************************************************************
 �������ƣ�    void DSP_Init(void)
 ����������    DSPʱ��ʹ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/6/28      V1.0           cfwu          ����
 *******************************************************************************/

void DSP_init(void)
{
	SYS_CLK_FEN |= SYS_Module_DSP;
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

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/4/17      V1.1           Howlet Li          ������ʱ�������ȴ�PLL�ȶ�
 *******************************************************************************/

void Clock_init(void)
{
    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 |= BIT15;     /* BIT15:PLLPDN ��PLL */
    SoftDelay(100);            /* ��ʱ100us, �ȴ�PLL�ȶ� 21.4.17*/
    SYS_CLK_CFG |= 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� */
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
 2021/3/14      V1.0           Howlet Li          ����
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
 2016/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/

void SystemInit(void)
{
    Clock_init();  	/* ʱ�ӳ�ʼ�� */
}

/*******************************************************************************
 �������ƣ�    void TempSensor_Init(void)
 ����������    �¶ȴ�������ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void TempSensor_Init(void)
{

    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 |= BIT13;     /* ���¶ȴ��������� */
  
    m_TempertureCof.nCofA    = Read_Trim(0x00000398);
    m_TempertureCof.nOffsetB = Read_Trim(0x0000039C);

}

/*******************************************************************************
 �������ƣ�    void PGA_Init(void)
 ����������    PGA��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/15      V1.0           Howlet Li          ����
 *******************************************************************************/

void PGA_init(void)
{
	SYS_AFE_REG5 |= 0x20ff;
    
    SYS_AFE_REG0 = OPA0_GIAN | OPA1_GIAN | OPA2_GIAN | OPA3_GIAN; /* 4��OPA�������� */ 
}

/*******************************************************************************
 �������ƣ�    void CMP_Init(void)
 ����������    �Ƚ�����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/15      V1.0           Howlet Li          ����
 *******************************************************************************/

void CMP_init(void)
{
//    CMP_InitTypeDef CMP_InitStruct;

//    CMP_StructInit(&CMP_InitStruct);

//    CMP_InitStruct.CMP0_EN = ENABLE;
//    CMP_InitStruct.CMP0_SELN = SELN_DAC;		/* ��������оƬ�ڲ�DAC */
//    CMP_InitStruct.CMP0_SELP = SELP_CMP_IP0;	/* ��������IP0���� */

//    CMP_InitStruct.CMP0_IE = ENABLE;			/* �Ƚ���0�ź��ж�ʹ�� */
//    CMP_InitStruct.CMP0_InEnable = ENABLE; 		/* �Ƚ����ź�����ʹ�� */

//    CMP_InitStruct.CMP_FltCnt = 15; 			/* ���˲����=tclk ����*16*CMP_FltCnt */
//    CMP_InitStruct.CMP_CLK_EN = ENABLE; 		/* ʱ��ʹ�� */

//    Comparator_init(&CMP_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void DAC_Init(void)
 ����������    DAC��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/3/23      V1.0           Howlet Li          ����
 *******************************************************************************/

void DAC_init(void)
{
	SYS_AFE_REG5 |= 0x1000;
	
    SYS_WR_PROTECT = 0x7a83;

    SYS_AFE_DAC = 512;                  /* 3.0*512/4096/(0.005/2) = 150A, ����0.005Ϊĸ�߲�������,2Ϊ�����������*/ 
}

/*******************************************************************************
 �������ƣ�    void UART_init(void)
 ����������    UART0�Ĵ�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void UART_init(void)
{
    UART_InitTypeDef UART_InitStruct;

    UART_StructInit(&UART_InitStruct);
    UART_InitStruct.BaudRate = 9600;
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;
    UART_InitStruct.StopBits = UART_STOPBITS_1b;
    UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;
    UART_InitStruct.ParityMode = UART_Parity_NO;
    UART_InitStruct.IRQEna = UART_IRQEna_SendOver | UART_IRQEna_RcvOver;
    UART_Init(UART1, &UART_InitStruct);
}

/*******************************************************************************
 �������ƣ�    void UART0_SENDDATA(void)
 ����������    UART0���ͳ���
 ���������    n����Ҫ���͵�ֵ
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void UART0_SENDDATA(UINT8 n)
{
    UART0_BUFF = n;
}

/*******************************************************************************
 �������ƣ�    void UART1_SENDDATA(void)
 ����������    UART1���ͳ���
 ���������    n����Ҫ���͵�ֵ
 ���������    ��
 �� �� ֵ��    ��
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
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void ADC_init(void) 
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN;
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;
    ADC_InitStructure.FirSeg_Ch = 8;
    ADC_InitStructure.SecSeg_Ch = 0;
    ADC_InitStructure.Trigger_Cnt = 0;
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG;
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;
    ADC_Init(ADC0, &ADC_InitStructure);

    ADC0_IF = 0xff;

    ADC_NormalModeCFG();
}

/*******************************************************************************
 �������ƣ�    void ADC_NormalModeCFG(void)
 ����������    ADC��������ģʽͨ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void ADC_NormalModeCFG(void)
{
    ADC0_CHN0 = ADC0_CUR_A_1ST_MSK | ADC1_CUR_B_1ST_MSK;
    ADC0_CHN1 = ADC0_CUR_A_1ST_MSK | ADC1_CUR_B_1ST_MSK;
    ADC0_CHN2 = ADC0_3th_MSK | ADC1_3th_MSK;
    ADC0_CHN3 = ADC0_4th_MSK | ADC1_4th_MSK;
}

/*******************************************************************************
 �������ƣ�    void HALL_Init(void)
 ����������    GPIOӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2018/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void HALL_Perip_Init(void)
{
    HALL_InitTypeDef HALL_InitStruct;
    HALL_StructInit(&HALL_InitStruct);

    HALL_InitStruct.FilterLen = 4096;
    HALL_InitStruct.ClockDivision = HALL_CLK_DIV1;
    HALL_InitStruct.Filter75_Ena = DISABLE;
    HALL_InitStruct.HALL_Ena = ENABLE;
    HALL_InitStruct.Capture_IRQ_Ena = ENABLE;
    HALL_InitStruct.OverFlow_IRQ_Ena = ENABLE;
    HALL_InitStruct.CountTH = 96000000;
    HALL_InitStruct.softIE = DISABLE;

    HALL_Init(&HALL_InitStruct);
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
    GPIO_StructInit(&GPIO_InitStruct);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIO0, GPIO_PinSource_11, AF2_HALL);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_12, AF2_HALL);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_13, AF2_HALL);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
	
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_4, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_5, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_6, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_7, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_8, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF3_MCPWM);
	
	GPIO0_PUE = GPIO_Pin_3;
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
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
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
    MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE; 					/* ʱ������ */
    MCPWM_InitStructure.MCPWM_WorkModeCH0 = CENTRAL_PWM_MODE;
    MCPWM_InitStructure.MCPWM_WorkModeCH1 = CENTRAL_PWM_MODE;
    MCPWM_InitStructure.MCPWM_WorkModeCH2 = CENTRAL_PWM_MODE; 	/* ����ģʽ���� */

    MCPWM_InitStructure.GPIO_BKIN_Filter = 12;
    MCPWM_InitStructure.CMP_BKIN_Filter = 12; 					/* �˲���ʱ������ */

    MCPWM_InitStructure.MCPWM_PERIOD = PWM_PERIOD;
    MCPWM_InitStructure.TriggerPoint0 = (u16)(100 - PWM_PERIOD);
    MCPWM_InitStructure.TriggerPoint1 = (u16)(PWM_PERIOD - 1);
    MCPWM_InitStructure.DeadTimeCH0N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH0P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2P = DEADTIME; 				/* ���ڣ�����ʱ������ */

    MCPWM_InitStructure.CH0N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH1N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE; 			/* ����ѡ������ */

    MCPWM_InitStructure.Switch_CH0N_CH0P = ENABLE; 				/* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH1N_CH1P = ENABLE; 				/* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH2N_CH2P = ENABLE; 				/* ͨ������ѡ������ */

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
    MCPWM_InitStructure.MCPWM_T1_UpdateEN = DISABLE; 				/* �����¼����� */

    MCPWM_InitStructure.T0_Update_INT_EN = DISABLE;  				/* T0���� �ж�ʹ�� */

#if (CMP_CHOICE == 0)
    MCPWM_InitStructure.FAIL0_INT_EN = ENABLE; 						/* FAIL0 �ж�ʹ�� */
    MCPWM_InitStructure.FAIL0_INPUT_EN = ENABLE;
    MCPWM_InitStructure.FAIL0_Signal_Sel = HIGH_LEVEL;
    MCPWM_InitStructure.FAIL0_Polarity = LOW_LEVEL; 
#else
#if (CMP_CHOICE == 1) 
    MCPWM_InitStructure.FAIL1_INT_EN = ENABLE; 						/* FAIL0 �ж�ʹ�� */
    MCPWM_InitStructure.FAIL1_INPUT_EN = ENABLE;
    MCPWM_InitStructure.FAIL1_Signal_Sel = HIGH_LEVEL;
    MCPWM_InitStructure.FAIL1_Polarity = LOW_LEVEL; 
#endif  
#endif

    MCPWM_Init(&MCPWM_InitStructure);
}

/*******************************************************************************
 �������ƣ�    void seekPosADCsetting(void)
 ����������    ����IPD����ǰ������ADC����ģʽ��Ϊ��ʼλ�ü����׼��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2017/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void seekPosADCsetting(void)
{
    ADC0_TRIG = BIT1;          /*Ӳ������ADC MCPWM T1��ʹ�� */
    ADC0_CFG |= BIT2;          /*״̬����λ��idle״̬����ɺ��Զ���0 */
    MCPWM_TMR0 = PWM_PERIOD;   /*ADC����ʱ��0����Ϊ���Źض����ĵ� */
    MCPWM_TMR1 = (s16)(-600);  /*ADC����ʱ��1����ΪPWM���ĵ����ƫ600��Clock*/
}

/*******************************************************************************
 �������ƣ�    void DebugPWM_OutputFunction(void)
 ����������    PWM������ܵ���   ���25%ռ�ձ�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2017/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void DebugPWM_OutputFunction(void)
{
    MCPWM_TH00 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH01 = (PWM_PERIOD >> 2);
    MCPWM_TH10 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH11 = (PWM_PERIOD >> 2);
    MCPWM_TH20 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH21 = (PWM_PERIOD >> 2);

    PWMOutputs(ENABLE);
    while(1)
    {
    }
}

/*******************************************************************************
 �������ƣ�    void MCPWM0_RegUpdate(stru_FOC_CurrLoopDef *this)
 ����������    ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void MCPWM0_RegUpdate(stru_FOC_CurrLoopDef *this)
{
	MCPWM_TH00 = -this->struVoltUVW_PWM.u;
	MCPWM_TH01 = this->struVoltUVW_PWM.u;

	MCPWM_TH10 = -this->struVoltUVW_PWM.v;
	MCPWM_TH11 = this->struVoltUVW_PWM.v;

	MCPWM_TH20 = -this->struVoltUVW_PWM.w;
	MCPWM_TH21 = this->struVoltUVW_PWM.w;
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
        __nop();
    }
}

/*******************************************************************************
 �������ƣ�    void CopyFromBuffer(u8* nDestAddr, u8* pSrcBuff, u16 nSize)
 ����������    ��������
 ���������    nDestAddr��Ҫ�������������飬pSrcBuff��ſ����������飬nSize ��������
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void CopyFromBuffer(u8* nDestAddr, u8* pSrcBuff, u16 nSize)
{
    u8* ps = (u8*) pSrcBuff;
    u8* pd = (u8*) nDestAddr;

    while(nSize--)
        *pd++ = *ps++;
}
