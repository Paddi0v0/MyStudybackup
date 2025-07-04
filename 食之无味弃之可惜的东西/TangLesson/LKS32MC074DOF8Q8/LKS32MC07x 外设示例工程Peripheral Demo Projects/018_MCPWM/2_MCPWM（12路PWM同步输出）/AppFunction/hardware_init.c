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

#define  CNT0_PRE_DRIVER_POLARITY           P_HIGH__N_HIGH
#define  CNT1_PRE_DRIVER_POLARITY           P_HIGH__N_LOW

/* ----------------------PWM Ƶ�ʼ���������----------------------------------- */
#define MCU_MCLK                       (96000000LL)       /**<  PWMģ��������Ƶ */
#define PWM_MCLK                       ((u32)MCU_MCLK)    /**<  PWMģ��������Ƶ */
#define PWM_PRSC                       ((u8)0)            /**< *< *<  PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ0                      ((u16)16000)       /**< *<  MCPWM_CNT0PWMն��Ƶ�� */
#define PWM_FREQ1                      ((u16)16000)       /**<  MCPWM_CNT1PWMն��Ƶ�� */


/**<  �������PWM ���ڼ�����ֵ */
#define MCPWM_PERIOD0                    ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ0 *(PWM_PRSC+1))))
#define MCPWM_PERIOD1                    ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ1 *(PWM_PRSC+1))))
/**<  PFC����PWM ���ڼ�����ֵ */
#define PFC_PERIOD                     ((u16) (PWM_MCLK / (u32)(2 * PFC_FREQ *(PWM_PRSC+1))))

#define DEADTIME_NS                    ((u16)1200)       /**<  ����ʱ�� */
#define DEADTIME                       (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)



void GPIO0_Pin14Init(void);
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
    __disable_irq();                  /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;
    IWDG_DISABLE();                   /* �رտ��Ź�*/
    FLASH_CFG |= 0x00080000;          /* enable prefetch */

	  GPIO0_Pin14Init();
    MCPWM_init();                     /* PWM��ʼ�� */
    GPIO_init();                      /* GPIO��ʼ�� */
    SoftDelay(100);                   /* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */

    NVIC_SetPriority(ADC0_IRQn, 1);
		NVIC_SetPriority(ADC1_IRQn, 1);
		NVIC_SetPriority(MCPWM0_IRQn, 1);

    NVIC_EnableIRQ(ADC0_IRQn);        /* enable the ADC0 interrupt */
    NVIC_EnableIRQ(ADC1_IRQn);
		NVIC_EnableIRQ(MCPWM0_IRQn);

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
 2015/11/5      V1.0           Howlet Li          ����

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/4/17      V1.1           Howlet Li          ������ʱ�������ȴ�PLL�ȶ�
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
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);

    /* MCPWM P1.4~P1.9 */
	  GPIO_PinAFConfig(GPIO1, GPIO_PinSource_4, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_5, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_6, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_7, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_8, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF3_MCPWM);
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
	  
	  /* MCPWM P1.10,P1.11;P3.10,P3.11;P2.9,P2.10 */
	  GPIO_PinAFConfig(GPIO1, GPIO_PinSource_10, AF3_MCPWM);//CH3N
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_11, AF3_MCPWM);//CH3N
    GPIO_PinAFConfig(GPIO3, GPIO_PinSource_10, AF3_MCPWM);//CH4P
    GPIO_PinAFConfig(GPIO3, GPIO_PinSource_11, AF3_MCPWM);//CH4N
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_9,  AF3_MCPWM);//CH5P
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_10, AF3_MCPWM);//CH5N
		
		GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
    GPIO_Init(GPIO1, &GPIO_InitStruct);
		
		GPIO_StructInit(&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
    GPIO_Init(GPIO3, &GPIO_InitStruct);
		
		GPIO_StructInit(&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    
	  GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;  //GPIO����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

}
void GPIO0_Pin14Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);
		GPIO_PinAFConfig(GPIO0, GPIO_PinSource_14, AF3_MCPWM);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_14 ;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
}


void MCPWM_init(void)
{
    MCPWM_InitTypeDef MCPWM_InitStructure;
    MCPWM_StructInit(&MCPWM_InitStructure);
    
	  MCPWM_InitStructure.MCLK_EN = DISABLE;              /* ģ��ʱ�ӿ��� */
	
    MCPWM_InitStructure.CLK_DIV = 0;                   /* MCPWMʱ�ӷ�Ƶ���� */
	
    MCPWM_InitStructure.IO_CMP_FLT_CLKDIV  = 12;  /* ��ͣ�¼�(����IO�ڻ�Ƚ����ź�)�����˲���ʱ������ */
	
	  MCPWM_InitStructure.AUEN = MCPWM0_ALL_AUPDAT; /*�Զ�����ʹ��*/
	  
	  /* MCPWM0_CNT0 */
	  
    MCPWM_InitStructure.BASE_CNT0_EN = ENABLE;                /* ����������ʼ����ʹ�ܿ��� */
	  MCPWM_InitStructure.TH0 = MCPWM_PERIOD0;                    /* �����������ü�MCPWM�������*/
	
	  MCPWM_InitStructure.TH00 = (u16)(-(MCPWM_PERIOD0 >> 2));
	  MCPWM_InitStructure.TH01 = (MCPWM_PERIOD0 >> 2);
	  MCPWM_InitStructure.TH10 = (u16)(-(MCPWM_PERIOD0 >> 2));
	  MCPWM_InitStructure.TH11 = (MCPWM_PERIOD0 >> 2);
	  MCPWM_InitStructure.TH20 = (u16)(-(MCPWM_PERIOD0 >> 2));
	  MCPWM_InitStructure.TH21 = (MCPWM_PERIOD0 >> 2);
		
    MCPWM_InitStructure.MCPWM_WorkModeCH0 = MCPWM0_CENTRAL_PWM_MODE; /* MCPWM CH0����ģʽ�����Ķ���PWMģʽ */
    MCPWM_InitStructure.MCPWM_WorkModeCH1 = MCPWM0_CENTRAL_PWM_MODE; /* ͨ������ģʽ���ã����Ķ������ض��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH2 = MCPWM0_CENTRAL_PWM_MODE;
    MCPWM_InitStructure.DeadTimeCH012N = DEADTIME;/* ����ʱ������ */
    MCPWM_InitStructure.DeadTimeCH012P = DEADTIME;
	  MCPWM_InitStructure.CMP_CTRL_CNT0  = DISABLE ;/* CMP����CNT0ʧ�� */
	  MCPWM_InitStructure.EVT_CNT0_EN    = DISABLE ;/* MCPWM_CNT1�ⲿ����ʧ�� */
	  MCPWM_InitStructure.EVT0           = DISABLE ;
	
	  MCPWM_InitStructure.TR0_UP_INTV     = DISABLE;
	  MCPWM_InitStructure.TR0_T0_UpdateEN = ENABLE ;
	  MCPWM_InitStructure.TR0_T1_UpdateEN = DISABLE ;
		MCPWM_InitStructure.TR0_AEC         = DISABLE;
		
		MCPWM_InitStructure.T0_Update0_INT_EN = DISABLE; /* T0�����¼� �жϹر� */
		MCPWM_InitStructure.T1_Update0_INT_EN = ENABLE ; /* T1�����¼� �ж�ʹ�� */
		MCPWM_InitStructure.Update1_INT_EN = ENABLE;     /* CNT0 �����¼� �ж�ʹ��  */
		
	  #if (CNT0_PRE_DRIVER_POLARITY == P_HIGH__N_LOW)                    /* CHxP ����Ч�� CHxN�͵�ƽ��Ч */
    MCPWM_InitStructure.CH0N_Polarity_INV = ENABLE;           /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE;          /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH1N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH0N_CH0P =  DISABLE;           /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH1N_CH1P =  DISABLE;           /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH2N_CH2P =  DISABLE;           /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH0P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = MCPWM0_HIGH_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = MCPWM0_LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = MCPWM0_HIGH_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = MCPWM0_HIGH_LEVEL;
#else
#if (CNT0_PRE_DRIVER_POLARITY == P_HIGH__N_HIGH)                    /* CHxP ����Ч�� CHxN�ߵ�ƽ��Ч */
    MCPWM_InitStructure.CH0N_Polarity_INV = DISABLE;           /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH0P_Polarity_INV = DISABLE;          /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH1N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH1P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH2P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH0N_CH0P =  DISABLE;           /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH1N_CH1P =  DISABLE;           /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH2N_CH2P =  DISABLE;           /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH0P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = MCPWM0_LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = MCPWM0_LOW_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = MCPWM0_LOW_LEVEL;
#endif
#endif

    MCPWM_InitStructure.CH0N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH0P_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH1N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH1P_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH2N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH2P_FAIL_EN = ENABLE ;
		
    MCPWM_InitStructure.FAIL0_INPUT_EN   = DISABLE ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL0_INT_EN     = DISABLE;
		MCPWM_InitStructure.FAIL0_Signal_Sel = MCPWM0_FAIL_SEL_CMP ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL0_Polarity   = MCPWM0_HIGH_LEVEL_ACTIVE ;
		
		MCPWM_InitStructure.FAIL1_INPUT_EN   = DISABLE ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL1_INT_EN     = DISABLE;
		MCPWM_InitStructure.FAIL1_Signal_Sel = MCPWM0_FAIL_SEL_IO ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL1_Polarity   = MCPWM0_HIGH_LEVEL_ACTIVE ;
		
		MCPWM_InitStructure.HALT_PRT0        = DISABLE ;
		MCPWM_InitStructure.FAIL_0CAP        = DISABLE ;
		
		
	  /* MCPWM0_CNT1 */
		MCPWM_InitStructure.BASE_CNT1_EN = ENABLE;                /* ����������ʼ����ʹ�ܿ��� */
	  MCPWM_InitStructure.TH1 = MCPWM_PERIOD1;              /* �����������ü�MCPWM�������*/
		MCPWM_InitStructure.TH30 = (u16)(-(MCPWM_PERIOD1 >> 2));
		MCPWM_InitStructure.TH31 = (MCPWM_PERIOD1 >> 2);
		MCPWM_InitStructure.TH40 = (u16)(-(MCPWM_PERIOD1 >> 2));
		MCPWM_InitStructure.TH41 = (MCPWM_PERIOD1 >> 2);
		MCPWM_InitStructure.TH50 = (u16)(-(MCPWM_PERIOD1 >> 2));
		MCPWM_InitStructure.TH51 = (MCPWM_PERIOD1 >> 2);
    
    MCPWM_InitStructure.MCPWM_WorkModeCH3 = MCPWM0_CENTRAL_PWM_MODE; /* MCPWM CH0����ģʽ�����Ķ���PWMģʽ */
    MCPWM_InitStructure.MCPWM_WorkModeCH4 = MCPWM0_CENTRAL_PWM_MODE; /* ͨ������ģʽ���ã����Ķ������ض��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH5 = MCPWM0_CENTRAL_PWM_MODE;

    MCPWM_InitStructure.DeadTimeCH345N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH345P = DEADTIME;
    MCPWM_InitStructure.CMP_CTRL_CNT1  = DISABLE ;/* CMP����CNT1ʧ�� */
	  MCPWM_InitStructure.EVT_CNT1_EN    = DISABLE ;/* MCPWM_CNT1�ⲿ����ʧ�� */
    MCPWM_InitStructure.EVT1           = DISABLE ;
    
		MCPWM_InitStructure.TR1_UP_INTV     = DISABLE;
	  MCPWM_InitStructure.TR1_T0_UpdateEN = ENABLE ;
	  MCPWM_InitStructure.TR1_T1_UpdateEN = DISABLE ;
		MCPWM_InitStructure.TR1_AEC         = DISABLE;
	
    MCPWM_InitStructure.T0_Update1_INT_EN = DISABLE;  /* CNT1 T0�����¼� �ж�ʹ�� */
		MCPWM_InitStructure.T1_Update1_INT_EN = ENABLE ;  /* CNT1 T1�����¼� �жϹر�  */
		MCPWM_InitStructure.Update1_INT_EN = ENABLE;      /* CNT1 �����¼� �ж�ʹ��  */
		
    MCPWM_InitStructure.TMR0 = (u16)(10 - MCPWM_PERIOD1); /* MCPWM_TMR0  ���� */
    MCPWM_InitStructure.TMR1 = (u16)(MCPWM_PERIOD1 - 1);  /* MCPWM_TMR1  ���� */
		MCPWM_InitStructure.TMR2 = (u16)(10 - MCPWM_PERIOD1); /* MCPWM_TMR2  ���� */
    MCPWM_InitStructure.TMR3 = (u16)(MCPWM_PERIOD1 - 1);  /* MCPWM_TMR3  ���� */ 

#if (CNT1_PRE_DRIVER_POLARITY == P_HIGH__N_LOW)                    /* CHxP ����Ч�� CHxN�͵�ƽ��Ч */
    MCPWM_InitStructure.CH3N_Polarity_INV = ENABLE;           /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH3P_Polarity_INV = DISABLE;          /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH4N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH4P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH5N_Polarity_INV = ENABLE;
    MCPWM_InitStructure.CH5P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH3N_CH3P =  DISABLE;           /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH4N_CH4P =  DISABLE;           /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH5N_CH5P =  DISABLE;           /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH3P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH3N_default_output = MCPWM0_HIGH_LEVEL;
    MCPWM_InitStructure.CH4P_default_output = MCPWM0_LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH4N_default_output = MCPWM0_HIGH_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH5P_default_output = MCPWM0_LOW_LEVEL;
    MCPWM_InitStructure.CH5N_default_output = MCPWM0_HIGH_LEVEL;
#else
#if (CNT1_PRE_DRIVER_POLARITY == P_HIGH__N_HIGH)                    /* CHxP ����Ч�� CHxN�ߵ�ƽ��Ч */
    MCPWM_InitStructure.CH3N_Polarity_INV = DISABLE;           /* CH0Nͨ������������� | ���������ȡ�����*/
    MCPWM_InitStructure.CH3P_Polarity_INV = DISABLE;          /* CH0Pͨ������������� | ���������ȡ����� */
    MCPWM_InitStructure.CH4N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH4P_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH5N_Polarity_INV = DISABLE;
    MCPWM_InitStructure.CH5P_Polarity_INV = DISABLE;

    MCPWM_InitStructure.Switch_CH3N_CH3P =  DISABLE;           /* ͨ������ѡ������ | CH0P��CH0N�Ƿ�ѡ���źŽ��� */
    MCPWM_InitStructure.Switch_CH4N_CH4P =  DISABLE;           /* ͨ������ѡ������ */
    MCPWM_InitStructure.Switch_CH5N_CH5P =  DISABLE;           /* ͨ������ѡ������ */

    /* Ĭ�ϵ�ƽ���� Ĭ�ϵ�ƽ�������MCPWM_IO01��MCPWM_IO23�� BIT0��BIT1��BIT8��BIT9��BIT6��BIT14
                                                     ͨ�������ͼ��Կ��Ƶ�Ӱ�죬ֱ�ӿ���ͨ����� */
    MCPWM_InitStructure.CH3P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH3N_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH4P_default_output = LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH4N_default_output = LOW_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH5P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH5N_default_output = LOW_LEVEL;
#endif
#endif

    MCPWM_InitStructure.CH3N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH3P_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH4N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH4P_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH5N_FAIL_EN = ENABLE ;
		MCPWM_InitStructure.CH5P_FAIL_EN = ENABLE ;
		
    MCPWM_InitStructure.FAIL2_INPUT_EN   = DISABLE ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL2_INT_EN     = DISABLE;
		MCPWM_InitStructure.FAIL2_Signal_Sel = MCPWM0_FAIL_SEL_CMP ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL2_Polarity   = MCPWM0_HIGH_LEVEL_ACTIVE ;
		
		MCPWM_InitStructure.FAIL3_INPUT_EN   = DISABLE ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL3_INT_EN     = DISABLE;
		MCPWM_InitStructure.FAIL3_Signal_Sel = MCPWM0_FAIL_SEL_CMP ;//FAIL_0CAP
		MCPWM_InitStructure.FAIL3_Polarity   = MCPWM0_HIGH_LEVEL_ACTIVE ;
		
		MCPWM_InitStructure.HALT_PRT1        = DISABLE ;
		MCPWM_InitStructure.FAIL_1CAP        = DISABLE ;
		
		MCPWM_InitStructure.TMR2_TB        = 1;/* MCPWM TMR2ʱ������������ѡ�� 0��ʱ��0��1��ʱ��1 */
	  MCPWM_InitStructure.TMR3_TB        = 1;/* MCPWM TMR3ʱ������������ѡ�� 0��ʱ��0��1��ʱ��1 */

    MCPWM_Init(&MCPWM_InitStructure);

/*************************MCPWM 12·PWM���ͬ������**********************************/
		MCPWM0_CNT0 = 0;    //���CNT0������
		MCPWM0_CNT1 = 0;    //���CNT1������
		MCPWM0_UPDATE |= (BIT11|BIT27);//�ֶ�����CNT0��CNT1����ʹ�õ�Ӱ�ӼĴ���
		MCPWM0_PRT = 0xDEAD;//���д�����Ĵ���
		MCPWM0_TCLK |= BIT2;//MCPWM ����ʱ��ʹ��
		MCPWM0_PRT = 0x0;
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
    MCPWM0_TH00 = (u16)(-(MCPWM_PERIOD0 >> 2));
    MCPWM0_TH01 = (MCPWM_PERIOD0 >> 2);
    MCPWM0_TH10 = (u16)(-(MCPWM_PERIOD0 >> 2));
    MCPWM0_TH11 = (MCPWM_PERIOD0 >> 2);
    MCPWM0_TH20 = (u16)(-(MCPWM_PERIOD0 >> 2));
    MCPWM0_TH21 = (MCPWM_PERIOD0 >> 2);
    
    MCPWM0_TH30 = (u16)(-(MCPWM_PERIOD1 >> 2));
    MCPWM0_TH31 = (MCPWM_PERIOD1 >> 2);
    MCPWM0_TH40 = (u16)(-(MCPWM_PERIOD1 >> 2));
    MCPWM0_TH41 = (MCPWM_PERIOD1 >> 2);
    MCPWM0_TH50 = (u16)(-(MCPWM_PERIOD1 >> 2));
    MCPWM0_TH51 = (MCPWM_PERIOD1 >> 2);

    PWMOutputs(ENABLE);
    while(1)
    {
    }
}



