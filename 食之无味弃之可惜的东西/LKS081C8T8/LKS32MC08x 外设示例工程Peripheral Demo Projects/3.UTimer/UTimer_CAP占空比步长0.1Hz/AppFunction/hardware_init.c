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

stru_PWMFreqCat  gS_PWMFreqCat;
/*******************************************************************************
 �������ƣ�    void Hardware_init(void)
 ����������    Ӳ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/25      V1.0          HuangMG            ����
 *******************************************************************************/
void Hardware_init(void)
{
    __disable_irq();                  /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;          /* ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;          /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();                      /* GPIO��ʼ�� */
    UTimer_init();                    /* UTimer��ʼ�� */
	  SoftDelay(100);                   /* �ȴ�Ӳ����ʼ�����*/
    NVIC_SetPriority(TIMER3_IRQn, 0); /* TIMER0�ж����ȼ����ñ������*/
    NVIC_EnableIRQ(TIMER3_IRQn);      /* ʹ��UTimer��ʱ���ж�*/
	  SYS_WR_PROTECT = 0x0;             /*�ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif
    __enable_irq();                   /* �����ж� */
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
 2021/8/25      V1.0         HuangMG             ����
 *******************************************************************************/
void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��

    /* ����LED1��P0.6  LED2��P0.7  LED3: P0.3*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    /* P2.8 PWM�����ź�����*/
    GPIO_StructInit(&GPIO_InitStruct); //��ʼ���ṹ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//GPIO����ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
		GPIO_PinAFConfig(GPIO2, GPIO_PinSource_8, AF8_TIMER23);//P2.8����UTIMER3_CH0
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
    TIM_TimerStrutInit(&TIM_InitStruct);
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CAP;      /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_RiseFall; /*˫�ز���*/
    TIM_InitStruct.Timer_TH = 9600000;                         /* ���ü���������ģֵ */
    TIM_InitStruct.Timer_CMP0 = 0;                             /* ���ñȽ�ģʽ��CH0�Ƚ�ֵ */
    TIM_InitStruct.Timer_Filter0 = 15;                         /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div1;              /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_Zero;/* ����Timerģ��CH0�����ж� */
    TIM_TimerInit(TIMER3, &TIM_InitStruct);
		TIM_TimerCmd(TIMER3, ENABLE);                          /* Timer3 ģ��ʹ�� */
}

/*******************************************************************************
 �������ƣ�    RAM_FUNC void Utimer3_CapFrwq(u8 * temp)
 ����������    utimer��������PWMռ�ձȺ���
 ���������    temp���ж�0%��100%����ռ�ձȱ�־λ
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/6/14      V1.0           HuangMG           ����
 *******************************************************************************/
RAM_FUNC void Utimer3_CapFrwq(u8* temp)
{
		static u32 InPut_1 = 0;
		static u32 InPut_2 = 0;
		static u8  Clear_Cntflag = 0;
		static u8  Start_flag = 0;        //�����һ�������ر�־
  		if(Clear_Cntflag == 0)
		{
			*temp = 1;
			gS_PWMFreqCat.LEVEL_HL = 0;
			Clear_Cntflag = 1;
			Start_flag = 0;
			UTIMER_UNT3_CNT = 0;		
		}else{
			 
			 switch(Start_flag)
			 {
				 case 0: 
					 if((SPsetup_Voltage != 0))//��һ����������
						{
							InPut_1 = UTIMER_UNT3_CMP0;			
						}else{  //��һ����������
							Start_flag = 2;
							InPut_1 = UTIMER_UNT3_CMP0;	
						}
				 break;
				 case 1: 
							InPut_2 = UTIMER_UNT3_CMP0;		
				 break;
				 case 2: 
							gS_PWMFreqCat.InPut_risitimevalue  = InPut_2 - InPut_1;//�ߵ�ƽ����ֵ
							gS_PWMFreqCat.InPut_falltimevalue = UTIMER_UNT3_CMP0 - InPut_2;//�͵�ƽ����ֵ
							Clear_Cntflag = 0;
				 break;
				 case 3: 
							InPut_2 = UTIMER_UNT3_CMP0;		
				 break;
				 case 4: 
							gS_PWMFreqCat.InPut_risitimevalue  = UTIMER_UNT3_CMP0 - InPut_2;//�ߵ�ƽ����ֵ
							gS_PWMFreqCat.InPut_falltimevalue = InPut_2 - InPut_1;//�͵�ƽ����ֵ
							Clear_Cntflag = 0;	 
				 break;			 
			 }
				Start_flag++;		 
			}

}


/*******************************************************************************
 �������ƣ�    void Voltage_Control(void)
 ����������    UTIMER����PWMռ�ձ�
 ���������    
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0          HuangMG          ����
 *******************************************************************************/
void Voltage_Control(void)
{

  float Duty_Ratio = 0;

	if(gS_PWMFreqCat.LEVEL_HL == 1)     /*�������ռ�ձ�*/
	{
		 Duty_Ratio = 1000;
	}else if(gS_PWMFreqCat.LEVEL_HL == 2)/*������Сռ�ձ�*/
	{
		 Duty_Ratio  = 0;
	
	}
	else{	
	  Duty_Ratio = (((float)gS_PWMFreqCat.InPut_risitimevalue)/(gS_PWMFreqCat.InPut_risitimevalue + gS_PWMFreqCat.InPut_falltimevalue)*1000)+1;      		 
  }
	  gS_PWMFreqCat.InPut_DutyCycle = (u16)Duty_Ratio;			
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




