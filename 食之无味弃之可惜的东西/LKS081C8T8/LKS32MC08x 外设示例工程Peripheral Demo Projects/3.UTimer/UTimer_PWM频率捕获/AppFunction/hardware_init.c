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

RC_t gS_UTIMER_RC;

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
    GPIO_init();             /* GPIO��ʼ�� */
    UTimer_init();           /* UTimer��ʼ�� */
    SoftDelay(100);          /* �ȴ�Ӳ����ʼ�����*/
    NVIC_SetPriority(TIMER0_IRQn, 0); /*TIMER0�ж����ȼ����ñ������*/
    NVIC_EnableIRQ(TIMER0_IRQn);      /*ʹ��UTimer��ʱ���ж�*/
	  SYS_WR_PROTECT = 0x0;    /*�ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
	  __enable_irq();          /* �����ж� */
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

    /* ����UTimer0  TIM0_CH1: P0.14*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0,GPIO_PinSource_14,AF7_TIMER01); //P0.14����Ϊtimer0�����ģʽ
		
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
 ����˵����    ��ʱ�� 96000/96000000 = 0.001s = 1kHz
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UTimer_init(void)
{
    TIM_TimerInitTypeDef TIM_InitStruct;

    TIM_TimerStrutInit(&TIM_InitStruct);                   //Timer�ṹ���ʼ��
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CAP;  /* ����Timer0 CH0 Ϊ����ģʽ */
    TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_Fall; /*�½��ز���*/
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CAP;  /* ����Timer0 CH1 Ϊ����ģʽ */
    TIM_InitStruct.Timer_CH1_CapMode = TIMER_CapMode_Rise; /*�����ز���*/
    TIM_InitStruct.Timer_TH = 60000;                         /* ��ʱ��0��1�������޳�ʼֵ���65535*/

    TIM_InitStruct.Timer_Filter0 = 15; /*CH0�˲����˲����ΪTimer_Filter0*8��ʱ������*/
    TIM_InitStruct.Timer_Filter1 = 15;

    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div1;                       /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH1 | Timer_IRQEna_Zero; /* ����Timerģ��Ƚ��ж�*/
    TIM_TimerInit(TIMER0, &TIM_InitStruct);   
	
	  TIM_TimerCmd(TIMER0, ENABLE); /* Timer0 ģ��ʹ�� */
}



/*******************************************************************************
 �������ƣ�    u16 InPutPulse_filter(u32 temp)
 ����������    UTIMER����PWMƵ���˲�����
 ���������    temp���˲�ϵ�� ��Χ 0 ~ 1
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ͨ�˲�ϵ������temp��ϵ��Խ���˲����Խ�ߣ�Ƶ�ʲ���ԽС����Ƶ�ʵ����ӳ�Խ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/6/14      V1.0           HuangMG           ����
 *******************************************************************************/
u16 InPutPulse_filter(u16 temp)
{
		static u32 s_temp0 = 0;
		static u32 s_temp1 = 0;
		static u32 s_temp2 = 0;
		static u32 s_temp3 = 0;
		static u32 s_temp4 = 0;
	  static u32 s_temp5 = 0;
	  static u32 s_tempout = 0;
	  s_temp4 = s_temp3;
    s_temp3 = s_temp2;
		s_temp2 = s_temp1;
		s_temp1 = s_temp0;
		s_temp0 = temp;
	
	  if(temp == s_temp0 )
		{
			if(temp == s_temp1)
			{
				if(temp == s_temp2)
				{
					if(temp == s_temp3)
					{
						if(temp == s_temp4)
						{
							s_tempout = temp;
						}else{
									s_tempout = s_temp5;
								 }
					}else{
								s_tempout = s_temp5;
							 }
				}else{
								s_tempout = s_temp5;
							}
			}else{
						 s_tempout = s_temp5;
					 }
		 }else{
			   s_tempout = s_temp5;
		 }
     s_temp5 = s_tempout;
		 
		 return (u16)lowPass_filter(&gS_UTIMER_RC,s_tempout);//��ͨ�˲�
}

/*******************************************************************************
 �������ƣ�    s16 lowPass_filter(RC_t *rc,s16 signal)
 ����������    ��ͨ�˲�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
s16 lowPass_filter(RC_t *rc,s16 signal)
{
    s32 t_inter;
    t_inter = (signal - (s16)(rc->yk_1>>16)) * (rc->coef);
    rc->yk_1 += t_inter;
    return(rc->yk_1>>16);
}
/*******************************************************************************
 �������ƣ�    void InPutPulse_init(float temp)
 ����������    UTIMER����PWMƵ�ʱ��������
 ���������    temp���˲�ϵ�� ��Χ 0 ~ 1
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ͨ�˲�ϵ������temp��ϵ��Խ���˲����Խ�ߣ�Ƶ�ʲ���ԽС����Ƶ�ʵ����ӳ�Խ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void InPutPulse_init(float temp)
{
  gS_UTIMER_RC.coef = (u16)(65535 * temp);
	gS_UTIMER_RC.yk_1 = 0;
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


