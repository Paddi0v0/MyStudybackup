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
 * �޸����ڣ� 2021��8��27��
 * �� �� �ţ�
 * �� �� �ˣ� HMG
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "basic.h"
#include "hardware_config.h"


/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/27     V1.0            HMG              ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();            /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;    /* ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;    /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();                /* GPIO��ʼ�� */
    ADC0_init();                /* ADC��ʼ�� */
    MCPWM_init();               /* MCPWM��ʼ�� */
    SoftDelay(100);             /* �ȴ�Ӳ����ʼ�����*/
	  NVIC_EnableIRQ(ADC0_IRQn);  /* ʹ��ADC0�ж�*/
    NVIC_SetPriority(ADC0_IRQn, 2); /* ADC0�ж����ȼ�����*/
	  SYS_WR_PROTECT = 0x0;       /*�ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif		
    __enable_irq();             /* �����ж� */
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
#define ADC_MODE1 0 //һ�������Ķ�ģʽ����ѡ��
#define ADC_MODE2 1 //һ�������Ķ�ģʽ����ѡ��
#define ADC_MODE4 0 //һ�������Ķ�ģʽ����ѡ��
#if ADC_MODE1
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);                          //��ʼ���ṹ��
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_DAT0_OV_IRQ_EN; //��һ��ɨ������ж�ʹ�ܺ�ADC_DAT0����ֵ�ж�
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                    //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = DISABLE;               //UTIMER_T0Ӳ������ADC����
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG;       //�ر�MCPWM��Ӳ���ж�
    ADC_InitStructure.FirSeg_Ch = ADC_2_TIMES_SAMPLE;            //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.Trigger_Cnt = 0;                           /*���δ���ģʽ�´���һ�β�������Ҫ���¼���:0~15 0��ʾ��Ҫһ�δ�����15��ʾ��Ҫ16��*/
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;               /*ADC����1��ģʽ*/
    ADC_InitStructure.DAT0_TH_Type = ENABLE;                     //����ADC�Ƚ�����ֵ�ж�ʹ��
    ADC0_DAT0_TH = 5000;                                         //����ADC0_DARA0����ADC0_DAT0_THֵʱ������ADC��ֵ�Ƚ��ж�
    ADC_Init(ADC0, &ADC_InitStructure);
    ADC0_IF = 0xff;                                   //���жϱ�־λ
    ADC0_GAIN0 = 0x0;                                 //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8); //����ͨ��ͨ��1��2
    //	  ADC0_TRIG=0x4010;//��ʼ��������ģʽ
}
#endif
#if ADC_MODE2
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);                                       //��ʼ���ṹ��
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_EOS1_IRQ_EN;                 //��һ��ɨ������ж�ʹ��
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                                 //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = DISABLE;                            //�ر�UTIMER��Ӳ���ж�
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG | ADC_MCPWM_T1_TRG; //MCPWM_T0��MCPWM_T1Ӳ������ADC����
    ADC_InitStructure.FirSeg_Ch = ADC_2_TIMES_SAMPLE;                         //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.SecSeg_Ch = ADC_2_TIMES_SAMPLE;                         //�ڶ��β�������ͨ����: 1~20
    ADC_InitStructure.Trigger_Mode = ADC_2SEG_TRG;                            /*ADC����2��ģʽ*/
    ADC_Init(ADC0, &ADC_InitStructure);
    ADC0_IF = 0xff;   //���жϱ�־λ
    ADC0_GAIN0 = 0x0; //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);                         /*ADC_CHANNEL_9��λ����ѹ�ɼ�ͨ��*/  
    ADC0_CHN1 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
}
#endif
#if ADC_MODE4
void ADC0_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);                                                                             //��ʼ���ṹ��
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_EOS1_IRQ_EN | ADC_EOS2_IRQ_EN | ADC_EOS3_IRQ_EN;                   //��һ���Ķ�ɨ������ж�ʹ��
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;                                                                       //ADC������������
    ADC_InitStructure.UTIMER_Trigger_En = DISABLE;                                                                  //�ر�UTIMER��Ӳ���ж�
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG | ADC_MCPWM_T1_TRG | ADC_MCPWM_T2_TRG | ADC_MCPWM_T3_TRG; //MCPWM_T0Ӳ������ADC����
    ADC_InitStructure.FirSeg_Ch = ADC_1_TIMES_SAMPLE;                                                               //��һ�β�������ͨ����: 1~20
    ADC_InitStructure.SecSeg_Ch = ADC_1_TIMES_SAMPLE;                                                               //�ڶ��β�������ͨ����: 1~20
    ADC_InitStructure.ThrSeg_Ch = ADC_1_TIMES_SAMPLE;                                                               //�����β�������ͨ����: 1~20
    ADC_InitStructure.FouSeg_Ch = ADC_1_TIMES_SAMPLE;                                                               //���Ķβ�������ͨ����: 1~20
    ADC_InitStructure.Trigger_Mode = ADC_4SEG_TRG;                                                                  /*ADC����4��ģʽ*/
    ADC_Init(ADC0, &ADC_InitStructure);
    ADC0_IF = 0xff;   //���жϱ�־λ
    ADC0_GAIN0 = 0x0; //����2/3����
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
    ADC0_CHN1 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8);
}
#endif



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

void MCPWM_init(void)
{
    MCPWM_InitTypeDef MCPWM_InitStructure;

    MCPWM_StructInit(&MCPWM_InitStructure);

    MCPWM_InitStructure.CLK_DIV = PWM_PRSC;                   /* MCPWMʱ�ӷ�Ƶ���� */
    MCPWM_InitStructure.MCLK_EN = ENABLE;                     /* ģ��ʱ�ӿ��� */
    MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE;                /* ����������ʼ����ʹ�ܿ��� */

    MCPWM_InitStructure.MCPWM_PERIOD = PWM_PERIOD;                /* �����������ü�MCPWM�������*/
    MCPWM_InitStructure.TriggerPoint0 = (u16)(10 - PWM_PERIOD);   /* MCPWM_TMR0 ADC�����¼�T0 ���� */
    MCPWM_InitStructure.TriggerPoint1 = (u16)(1000 - PWM_PERIOD); /* MCPWM_TMR1 ADC�����¼�T1 ���� */
    MCPWM_InitStructure.TriggerPoint2 = (u16)(PWM_PERIOD - 1000); /* MCPWM_TMR0 ADC�����¼�T0 ���� */
    MCPWM_InitStructure.TriggerPoint3 = (u16)(PWM_PERIOD - 1);    /* MCPWM_TMR1 ADC�����¼�T1 ���� */

    MCPWM_Init(&MCPWM_InitStructure);

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
