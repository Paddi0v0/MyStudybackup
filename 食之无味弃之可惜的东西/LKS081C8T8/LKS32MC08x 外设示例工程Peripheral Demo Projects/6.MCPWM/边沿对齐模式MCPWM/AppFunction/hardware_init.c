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
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();            /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;    /* ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;    /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();                /* GPIO��ʼ�� */
    MCPWM_init();               /* MCPWM��ʼ�� */
	  SoftDelay(100);             /* �ȴ�Ӳ����ʼ�����*/
	  SYS_WR_PROTECT = 0x0;       /* �ر�ϵͳ�Ĵ���д����*/
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

    //GPIO_P1.4��P1.11����Ϊ���ģʽ��MCPWM����
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_4, AF3_MCPWM); //GPIO_PinSource_4��ʾP1�ĵ�������
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_5, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_6, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_7, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_8, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_10, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_11, AF3_MCPWM);
	
	
//	  /* ����դ�����������ͺţ�P1.12/ P1.15/ P3.13����IO�ڱ�������Ϊ���̬ */
//	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
//    GPIO_Init(GPIO1, &GPIO_InitStruct);
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
//    GPIO_Init(GPIO3, &GPIO_InitStruct);
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
    ADC0_CHN0 = ADC_CHANNEL_9 | (ADC_CHANNEL_9 << 8); //����ͨ��ͨ��1��2
    //	  ADC0_TRIG=0x4010;//��ʼ��������ģʽ
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

    MCPWM_InitStructure.CLK_DIV = PWM_PRSC;                /* MCPWMʱ�ӷ�Ƶ���� */
    MCPWM_InitStructure.MCLK_EN = ENABLE;                  /* ģ��ʱ�ӿ��� */
    MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE;             /* ����������ʼ����ʹ�ܿ��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH0 = EDGE_PWM_MODE; /* MCPWM CH0����ģʽ�����ض���PWMģʽ */
    MCPWM_InitStructure.MCPWM_WorkModeCH1 = EDGE_PWM_MODE; /* ͨ������ģʽ���ã����Ķ������ض��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH2 = EDGE_PWM_MODE;
    MCPWM_InitStructure.MCPWM_WorkModeCH3 = EDGE_PWM_MODE;

    MCPWM_InitStructure.GPIO_BKIN_Filter = 12; /* ��ͣ�¼�(����IO���ź�)�����˲���ʱ������ */
    MCPWM_InitStructure.CMP_BKIN_Filter = 12;  /* ��ͣ�¼�(���ԱȽ����ź�)�����˲���ʱ������ */

    MCPWM_InitStructure.MCPWM_PERIOD = PWM_PERIOD;              /* �����������ü�MCPWM�������*/
    MCPWM_InitStructure.TriggerPoint0 = (u16)(10 - PWM_PERIOD); /* MCPWM_TMR0 ADC�����¼�T0 ���� */
    MCPWM_InitStructure.TriggerPoint1 = (u16)(PWM_PERIOD - 1);  /* MCPWM_TMR1 ADC�����¼�T1 ���� */
    MCPWM_InitStructure.DeadTimeCH0N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH0P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2P = DEADTIME; /* ����ʱ������ */
    MCPWM_InitStructure.DeadTimeCH3N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH3P = DEADTIME; /* ����ʱ������ */

#if (PRE_DRIVER_POLARITY == P_HIGH__N_LOW)           /* CHxP ����Ч�� CHxN�͵�ƽ��Ч */
    MCPWM_InitStructure.CH0N_Polarity_INV = ENABLE;  /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE; /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH1N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE;
		MCPWM_InitStructure.CH3N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH3P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH0N_CH0P = DISABLE; /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH1N_CH1P = DISABLE; /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH2N_CH2P = DISABLE; /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH3N_CH3P = DISABLE; /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH0P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = HIGH_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = LOW_LEVEL;  /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = HIGH_LEVEL; /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = HIGH_LEVEL;
    MCPWM_InitStructure.CH3P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH3N_default_output = HIGH_LEVEL;
#else
#if (PRE_DRIVER_POLARITY == P_HIGH__N_HIGH) /* CHxP ����Ч�� CHxN�ߵ�ƽ��Ч */
    MCPWM_InitStructure.CH0N_Polarity_INV = DISABLE; /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE; /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH1N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH3N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH3P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH0N_CH0P = DISABLE; /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH1N_CH1P = DISABLE; /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH2N_CH2P = DISABLE; /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH3N_CH3P = DISABLE; /* ͨ������ѡ������ */
    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH0P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = LOW_LEVEL; /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = LOW_LEVEL; /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH3P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH3N_default_output = LOW_LEVEL;

#endif
#endif

    MCPWM_InitStructure.DebugMode_PWM_out = ENABLE; /* �ڽ��Ϸ�����debug����ʱ����ͣMCU����ʱ��ѡ���PWMͨ��������������ź�
                                                                 �������Ĭ�ϵ�ƽ�������������� ENABLE:������� DISABLE:���Ĭ�ϵ�ƽ*/

    MCPWM_InitStructure.MCPWM_T0_UpdateEN = ENABLE;  /* MCPWM T0�¼�����ʹ�� */
    MCPWM_InitStructure.MCPWM_T1_UpdateEN = DISABLE; /* MCPWM T1�¼����� ��ֹ*/

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    MCPWM_InitStructure.T1_Update_INT_EN = ENABLE; /* T1�����¼� �ж�ʹ�ܻ�ر� */
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    MCPWM_InitStructure.T0_Update_INT_EN = DISABLE; /* T0�����¼� �ж�ʹ�ܻ�ر� */
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT) || (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
    MCPWM_InitStructure.T0_Update_INT_EN = DISABLE; /* T0�����¼� �ж�ʹ�ܻ�ر� */
#endif
#endif
#endif

    MCPWM_InitStructure.FAIL0_INT_EN = DISABLE;             /* FAIL0�¼��� �ж�ʹ�ܻ�ر� */
    MCPWM_InitStructure.FAIL0_INPUT_EN = DISABLE;           /* FAIL0 ���빦��ʹ��*/
    MCPWM_InitStructure.FAIL0_Signal_Sel = FAIL_SEL_CMP;    /* FAIL0�¼��ź�ѡ�񣬱Ƚ�����IO�� */
    MCPWM_InitStructure.FAIL0_Polarity = HIGH_LEVEL_ACTIVE; /* FAIL0�¼�����ѡ�񣬸���Ч */

    MCPWM_InitStructure.FAIL1_INT_EN = DISABLE;             /* FAIL1�¼� �ж�ʹ�ܻ�ر� */
    MCPWM_InitStructure.FAIL1_INPUT_EN = DISABLE;           /* FAIL1ͨ����ͣ���ܴ򿪻�ر� */
    MCPWM_InitStructure.FAIL1_Signal_Sel = FAIL_SEL_CMP;    /* FAIL1�¼��ź�ѡ�񣬱Ƚ�����IO�� */
    MCPWM_InitStructure.FAIL1_Polarity = HIGH_LEVEL_ACTIVE; /* FAIL1�¼�����ѡ�񣬸���Ч�����Ч */
    MCPWM_Init(&MCPWM_InitStructure);

    MCPWM_TH00 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH01 = (PWM_PERIOD >> 2);
    MCPWM_TH10 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH11 = (PWM_PERIOD >> 2);
    MCPWM_TH20 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH21 = (PWM_PERIOD >> 2);
    MCPWM_TH30 = (s16)(-(PWM_PERIOD >> 2));
    MCPWM_TH31 = (PWM_PERIOD >> 2);

    PWMOutputs(ENABLE); //���ʹ��
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
 �������ƣ�    uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
 ����������    GPIO��ƽ��ת����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/14     V1.0          HuangMG             ����
 *******************************************************************************/
uint8_t Invers_GPIO(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    u16 reg;
    reg = GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin); //��GPIO��ƽ
    if (reg)
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
        return 0;
    }
    else
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);
        return 1;
    }
}



