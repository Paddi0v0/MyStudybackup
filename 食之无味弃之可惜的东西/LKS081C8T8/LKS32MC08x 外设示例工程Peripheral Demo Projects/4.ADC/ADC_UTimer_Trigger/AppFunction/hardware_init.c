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
 2021/8/27      V1.0           HuangMG           ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();         /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83; /*ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000; /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();             /* GPIO��ʼ�� */
    UTimer_init();           /* UTimer��ʼ�� */
    ADC0_init();             /* ADC��ʼ�� */
	  SoftDelay(100);          /* �ȴ�Ӳ����ʼ�����*/
	  NVIC_SetPriority(ADC0_IRQn, 0); //ADC0�ж����ȼ�����
    NVIC_EnableIRQ(ADC0_IRQn);      //ʹ��ADC0�ж�
	  SYS_WR_PROTECT = 0x0;    /*�ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
    __enable_irq();          /* �����ж� */
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
    Clock_Init(); /* ʱ�ӳ�ʼ�� */
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

    //���ð��� start��P2.11  stop:P2.11
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; //GPIO����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    //����LED1��P0.6  LED2��P0.7  LED3: P0.3
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    /* ����UTimer0  TIM0_CH1: P0.14   TIM0_CH0: P0.15 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_14,AF7_TIMER01); //P0.15����Ϊtimer0�����ģʽ
    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_15,AF7_TIMER01); //P0.15����Ϊtimer0�����ģʽ
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
#define ADC_MODE1 1 //һ�������Ķ�ģʽ����ѡ��
#define ADC_MODE2 0 //һ�������Ķ�ģʽ����ѡ��
#define ADC_MODE4 0 //һ�������Ķ�ģʽ����ѡ��

#if ADC_MODE1
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure); //��ʼ���ṹ��

    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_DAT0_OV_IRQ_EN; //��һ��ɨ������ж�ʹ�ܺ�ADC_DAT0����ֵ�ж�
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                    //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = ADC_UTIMER_T0_TRG;     //UTIMER_T0Ӳ������ADC����
    ADC_InitStructure.MCPWM_Trigger_En = DISABLE;                //�ر�MCPWM��Ӳ���ж�
    ADC_InitStructure.FirSeg_Ch = ADC_2_TIMES_SAMPLE;            //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.Trigger_Cnt = 0;                           /*���δ���ģʽ�´���һ�β�������Ҫ���¼���:0~15 0��ʾ��Ҫһ�δ�����15��ʾ��Ҫ16��*/
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;               /*ADC����1��ģʽ*/
    ADC_InitStructure.DAT0_TH_Type = 1;                          //����ADC�Ƚ�����ֵ�ж�
    ADC0_DAT0_TH = 5000;                                         //����ADC0_DARA0����ADC0_DAT0_THֵʱ������ADC��ֵ�Ƚ��ж�
    ADC_Init(ADC0, &ADC_InitStructure);

    ADC0_IF = 0xff;                                   //���жϱ�־λ
    ADC0_GAIN0 = 0x0;                                 //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8); //������λ����ѹֵ                                                 

}
#endif
#if ADC_MODE2
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure); //��ʼ���ṹ��

    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_EOS1_IRQ_EN;                    //��һ��ɨ������ж�ʹ��
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                                    //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = ADC_UTIMER_T0_TRG | ADC_UTIMER_T1_TRG; //UTIMER_T0Ӳ������ADC����
    ADC_InitStructure.MCPWM_Trigger_En = DISABLE;                                //�ر�MCPWM��Ӳ���ж�
    ADC_InitStructure.FirSeg_Ch = ADC_2_TIMES_SAMPLE;                            //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.SecSeg_Ch = ADC_2_TIMES_SAMPLE;                            //�ڶ��β�������ͨ����: 1~20
                                                                                 //  ADC_InitStructure.Trigger_Cnt = 0; /*���δ���ģʽ�´���һ�β�������Ҫ���¼���:0~15 0��ʾ��Ҫһ�δ�����15��ʾ��Ҫ16��*/
    ADC_InitStructure.Trigger_Mode = ADC_2SEG_TRG;                               /*ADC����2��ģʽ*/

    ADC_Init(ADC0, &ADC_InitStructure);

    //	  ADC0_CHN0 = ADC_CHANNEL_8|(ADC_CHANNEL_8<<8);          //����ͨ��ͨ��1
    ADC0_IF = 0xff;   //���жϱ�־λ
    ADC0_GAIN0 = 0x0; //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
    ADC0_CHN1 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
    //	  ADC0_TRIG=0x4010;

}
#endif
#if ADC_MODE4
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure); //��ʼ���ṹ��

    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_EOS1_IRQ_EN | ADC_EOS2_IRQ_EN | ADC_EOS3_IRQ_EN;                        //��һ���Ķ�ɨ������ж�ʹ��
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                                                                            //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = ADC_UTIMER_T0_TRG | ADC_UTIMER_T1_TRG | ADC_UTIMER_T2_TRG | ADC_UTIMER_T3_TRG; //UTIMER_T0Ӳ������ADC����
    ADC_InitStructure.MCPWM_Trigger_En = DISABLE;                                                                        //�ر�MCPWM��Ӳ���ж�
    ADC_InitStructure.FirSeg_Ch = ADC_1_TIMES_SAMPLE;                                                                    //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.SecSeg_Ch = ADC_1_TIMES_SAMPLE;                                                                    //�ڶ��β�������ͨ����: 1~20
    ADC_InitStructure.ThrSeg_Ch = ADC_1_TIMES_SAMPLE;                                                                    //�����β�������ͨ����: 1~20
    ADC_InitStructure.FouSeg_Ch = ADC_1_TIMES_SAMPLE;                                                                    //���Ķβ�������ͨ����: 1~20
                                                                                                                         //  ADC_InitStructure.Trigger_Cnt = 0; /*���δ���ģʽ�´���һ�β�������Ҫ���¼���:0~15 0��ʾ��Ҫһ�δ�����15��ʾ��Ҫ16��*/
    ADC_InitStructure.Trigger_Mode = ADC_4SEG_TRG;                                                                       /*ADC����2��ģʽ*/

    ADC_Init(ADC0, &ADC_InitStructure);

    //	  ADC0_CHN0 = ADC_CHANNEL_8|(ADC_CHANNEL_8<<8);          //����ͨ��ͨ��1
    ADC0_IF = 0xff;   //���жϱ�־λ
    ADC0_GAIN0 = 0x0; //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
    ADC0_CHN1 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
    //	  ADC0_TRIG=0x4010;
}
#endif


/*******************************************************************************
 �������ƣ�    void UTimer_init(void)
 ����������    UTimerӲ����ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/27      V1.0           HMG              ����
 *******************************************************************************/
void UTimer_init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerCmd(TIMER2, ENABLE);                         /* Timer2 ģ��ʹ�� */
    TIM_TimerStrutInit(&TIM_InitStruct);                  //Timer�ṹ���ʼ��
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH0Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH1 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH1_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH1Output = 0; /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_TH = 48000;    /* ��ʱ���������޳�ʼֵ1000*/
    TIM_InitStruct.Timer_CMP0 = 24000;  /* ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ500 */
    TIM_InitStruct.Timer_CMP1 = 24000;  /* ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ500 */
    TIM_InitStruct.Timer_Filter0 = 0;   /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_Filter1 = 0;
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div2;                                          /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_CH1 | Timer_IRQEna_Zero; /* ����Timerģ��Ƚ��жϺ͹����ж�*/
    TIM_TimerInit(TIMER2, &TIM_InitStruct);

    TIM_TimerCmd(TIMER3, ENABLE);                         /* Timer3 ģ��ʹ�� */
    TIM_TimerStrutInit(&TIM_InitStruct);                  //Timer�ṹ���ʼ��
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH0Output = 0;                   /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP; /* ����Timer CH1 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH1_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH1Output = 0; /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_TH = 48000;    /* ��ʱ���������޳�ʼֵ1000*/
    TIM_InitStruct.Timer_CMP0 = 24000;  /* ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ500 */
    TIM_InitStruct.Timer_CMP1 = 24000;  /* ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ500 */
    TIM_InitStruct.Timer_Filter0 = 0;   /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_Filter1 = 0;
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div2;                                          /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_CH1 | Timer_IRQEna_Zero; /* ����Timerģ��Ƚ��жϺ͹����ж�*/
    TIM_TimerInit(TIMER3, &TIM_InitStruct);
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
 �������ƣ�    void Invers_GPIO(void)
 ����������    LED��ƽ��ת����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
uint8_t Invers_GPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  u16 reg;
	reg = GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin);//��GPIO��ƽ
	if(reg)
	{
		GPIO_ResetBits(GPIOx,GPIO_Pin);
		return 0;
	}
	else
	{
		GPIO_SetBits(GPIOx,GPIO_Pin);
		return 1;
	}
}






