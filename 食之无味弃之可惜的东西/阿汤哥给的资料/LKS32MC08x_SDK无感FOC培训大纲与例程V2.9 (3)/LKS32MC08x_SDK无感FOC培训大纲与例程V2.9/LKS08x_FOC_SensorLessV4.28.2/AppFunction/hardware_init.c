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
#include "basic.h"
#include "hardware_config.h"
#include "global_variable.h"
#include "MC_Parameter.h"
#include "SEGGER_RTT.h"

void UART_init(void);
void HALL_Perip_init(void);
void SoftDelay(u32 cnt);


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
    
    FLASH_CFG |= 0x00080000;          /* enable prefetch */

    DSP_Init();                       /* DSPģ���ʼ��*/
    UART_init();                      /* ���ڳ�ʼ��UART0*/
    ADC0_init();                      /* ADC��ʼ�� */
    MCPWM_init();                     /* PWM��ʼ�� */
    UTimer_init();                    /* ͨ�ü�������ʼ�� */
    GPIO_init();                      /* GPIO��ʼ�� */
    DAC_init();                       /* DAC ��ʼ�� */
    PGA_init();                       /* PGA ��ʼ�� */
    CMP_init();                       /* �Ƚ�����ʼ�� */
    HALL_Perip_init();                /* HALLģ���ʼ�� */
    TempSensor_Init();                /* �¶ȴ�������ʼ�� */

    SoftDelay(100);                   /* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */

    DSP_SC = BIT1;                    /* DSP Paused */
     
    NVIC_SetPriority(PWRDN_IRQn, 0);  /*PWRDN_IRQn�ж����ȼ�����Ϊ0*/
    NVIC_SetPriority(UART0_IRQn, 2);  /* ����UART0�ж����ȼ�Ϊ2 | ��0��1��2��3�ļ��ж����ȼ���0Ϊ���*/
    NVIC_SetPriority(ADC0_IRQn, 1);
    NVIC_SetPriority(HALL_IRQn, 2);
    NVIC_SetPriority(MCPWM_IRQn, 2);
    NVIC_SetPriority(CMP_IRQn, 0);    /* ����CMP_IRQn�ж����ȼ�Ϊ0 | ��0��1��2��3�ļ��ж����ȼ���0Ϊ���*/
    NVIC_SetPriority(TIMER0_IRQn, 2);

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    NVIC_SetPriority(MCPWM_IRQn, 1);   
    NVIC_EnableIRQ (MCPWM_IRQn);
#else
    NVIC_EnableIRQ(ADC0_IRQn);        /* enable the ADC0 interrupt */
#endif

#if ((ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR) || (TRUEHALL2SENSORLESS_ANGLE_VERIFY == TRUE))
    NVIC_EnableIRQ (HALL_IRQn);       /* enable the HALL interrupt */
#endif
    NVIC_EnableIRQ (TIMER0_IRQn);
    NVIC_EnableIRQ (CMP_IRQn);        /* �򿪱Ƚ����ж� */ 
    NVIC_EnableIRQ(PWRDN_IRQn);       /* �򿪵�Դ����ж� */ 
		
#if (RTT_FUNCTION == FUNCTION_ON)
    /* JScope RTTģʽ��ʼ�� */
    SEGGER_RTT_ConfigUpBuffer(1, "JScope_i2i2i2", bRttBuf, 2048, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
#endif 
	
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif		
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
 �������ƣ�    void Reg_Reset(void)
 ����������    �Ĵ�����λ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/3/14      V1.0           Howlet Li          ����
 *******************************************************************************/
void Reg_Reset(void)
{
   SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
   SYS_SFT_RST = 0xfff;
  
   Reg_Clr((u32)(&SYS_AFE_REG0),8); /* ��SYS_AFE_REG0~SYS_AFE_REG7 */
   Reg_Clr((u32)(&ADC0_CHN0),12);
   Reg_Clr((u32)(&ADC0_GAIN0),2);
   Reg_Clr((u32)(&ADC0_IE),6);

   Reg_Clr((u32)(&DMA_CCR0),18);
   Reg_Clr(GPIO0_BASE,12);          /* ��GPIO0 ��ص�12���Ĵ��� */
   Reg_Clr(GPIO1_BASE,12);
   Reg_Clr(GPIO2_BASE,12);
   Reg_Clr(GPIO3_BASE,12);   
   Reg_Clr(EXTI_BASE,6);
   Reg_Clr(CMP_BASE,5);
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
//    Reg_Reset();   /* ��ʼ����������Ĵ��� */
    Clock_Init();  /* ʱ�ӳ�ʼ�� */
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
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_OPA0, ENABLE);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_OPA1, ENABLE);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_OPA2, ENABLE);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_OPA3, ENABLE);
    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */   
    SYS_AFE_REG0 = OPA0_GIAN | OPA1_GIAN | OPA2_GIAN | OPA3_GIAN; /* 4��OPA�������� */
	  SYS_WR_PROTECT = 0x0;      /* ����ϵͳ�Ĵ���д���� */
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
    CMP_InitTypeDef CMP_InitStruct;

    CMP_StructInit(&CMP_InitStruct);

    CMP_InitStruct.CMP0_EN = DISABLE;           /* �Ƚ���0���� */
    CMP_InitStruct.CMP0_SELN = CMP_SELN_DAC;    /* CMP0_N �ڲ��ο� */
    CMP_InitStruct.CMP0_SELP = CMP0_SELP_OPA0_IP; /* CMP0_P CMP0_OPA0IP */
    CMP_InitStruct.CMP0_IN_EN = DISABLE;        /* �Ƚ����ź�����ʹ�� */
    CMP_InitStruct.CMP0_IE = DISABLE;           /* �Ƚ���0�ź��ж�ʹ�� */

    CMP_InitStruct.CMP1_EN = ENABLE;            /* �Ƚ���1���� */
    CMP_InitStruct.CMP1_SELN = CMP_SELN_DAC;    /* CMP1_N �ڲ��ο� */
    CMP_InitStruct.CMP1_SELP = CMP1_SELP_IP0;    /* CMP1_P CMP1_IP0 */
    CMP_InitStruct.CMP1_IN_EN = ENABLE;         /* �Ƚ����ź�����ʹ�� */  
    CMP_InitStruct.CMP1_IE = ENABLE;            /* �Ƚ���1�ź��ж�ʹ�� */
    
    CMP_InitStruct.FIL_CLK10_DIV16 = 15;        /* ���˲����=tclk ����*16*CMP_FltCnt */
	  CMP_InitStruct.FIL_CLK10_DIV2 = 0x1;        /*0x0:1��Ƶ��0x1:2��Ƶ,0x2:4��Ƶ,0x3:8��Ƶ*/
	
    CMP_InitStruct.CLK10_EN = ENABLE;           /* ʱ��ʹ�� */

    CMP_Init(&CMP_InitStruct);

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
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_DAC, ENABLE);
    SYS_WR_PROTECT = 0x7a83;
  
    SYS_AFE_REG1 &= ~((u32)0x03 << 6);
    SYS_AFE_REG1 |= (DAC_RANGE_1V2 << 6);  /* ����DAC������Ϊ1.2V��00:3V| 01:1.2V| 10:4.85V */
  
    if(((SYS_AFE_REG1 >> 6)&0x03) == DAC_RANGE_1V2)
    {/* ����DAC 1.2V����У��ֵ */
       SYS_AFE_DAC_DC = Read_Trim(0x00000344);
       SYS_AFE_DAC_AMC = Read_Trim(0x00000334);
    }
    else if(((SYS_AFE_REG1 >> 6)&0x03) == DAC_RANGE_3V0)
    { /* ����DAC 3.0V����У��ֵ */
       SYS_AFE_DAC_DC = Read_Trim(0x00000340);
       SYS_AFE_DAC_AMC = Read_Trim(0x00000330);
    }
    else if(((SYS_AFE_REG1 >> 6)&0x03) == DAC_RANGE_4V85)
    { /* ����DAC 4.85V����У��ֵ */
       SYS_AFE_DAC_DC = Read_Trim(0x00000348);
       SYS_AFE_DAC_AMC = Read_Trim(0x00000338);
    }

    SYS_AFE_DAC = 147;                     /* 1.2*147/4096/0.005 = 6A, ����0.005Ϊĸ�߲�������*/ 
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
    UART_InitStruct.BaudRate = 38400;                 /* ���ò�����38400 */
    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;  /* �������ݳ���8λ */
    UART_InitStruct.StopBits = UART_STOPBITS_1b;
    UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;   /* �ȷ���LSB */
    UART_InitStruct.ParityMode = UART_Parity_NO;      /* ����żУ�� */
    UART_InitStruct.IRQEna = 0;
    UART_Init(UART0, &UART_InitStruct);

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

///*******************************************************************************
// �������ƣ�    void UART0_init(void)
// ����������    UART1�Ĵ�������
// ���������    ��
// ���������    ��
// �� �� ֵ��    ��
// ����˵����
// �޸�����      �汾��          �޸���            �޸�����
// -----------------------------------------------------------------------------
// 2015/11/5      V1.0           Howlet Li          ����
// *******************************************************************************/
//void UART1_init(void)
//{

//    UART_InitTypeDef UART_InitStruct;
//    
//    UART_StructInit(&UART_InitStruct);
//    UART_InitStruct.BaudRate = 38400;                 /* ���ò�����38400 */
//    UART_InitStruct.WordLength = UART_WORDLENGTH_8b;  /* �������ݳ���8λ */
//    UART_InitStruct.StopBits = UART_STOPBITS_1b;
//    UART_InitStruct.FirstSend = UART_FIRSTSEND_LSB;   /* �ȷ���LSB */
//    UART_InitStruct.ParityMode = UART_Parity_NO;      /* ����żУ�� */
//    UART_InitStruct.IRQEna = 0;
//    UART_Init(UART1, &UART_InitStruct);

//}


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
void ADC0_init(void)
{

    ADC_InitTypeDef ADC_InitStructure;
    
    ADC_StructInit(&ADC_InitStructure);
    
    ADC_InitStructure.Align = ADC_LEFT_ALIGN;               /* ADCת���������������ʽ */
    ADC_InitStructure.Trigger_Cnt = 0;                      /* ����һ�δ����¼�ִ��һ��ת������ */  
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    ADC_InitStructure.FirSeg_Ch = 2;                        /* ��һ�ι�����6��ͨ�� */
    ADC_InitStructure.SecSeg_Ch = 8;                        /* �ڶ��ι�����0��ͨ�� */
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG | ADC_MCPWM_T1_TRG;;  /* ��MCPWM_T0 MCPWM_T1Ӳ�������¼� */
    ADC_InitStructure.Trigger_Mode = ADC_2SEG_TRG;          /* ����ADCת��ģʽΪ˫��ʽ���� */ 
    ADC_InitStructure.IE = ADC_EOS1_IRQ_EN;                 /* ADC��һ�β��������ж�ʹ�� */  
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    ADC_InitStructure.FirSeg_Ch = 10;                        /* ��һ�ι�����6��ͨ�� */
    ADC_InitStructure.SecSeg_Ch = 0;                        /* �ڶ��ι�����0��ͨ�� */
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG ; /* ��MCPWM_T0 MCPWM_T1Ӳ�������¼� */
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;          /* ����ADCת��ģʽΪ����ʽ���� */
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN;                 /* ADC��һ�β��������ж�ʹ�� */  
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET)) 
    ADC_InitStructure.FirSeg_Ch = 6;                        /* ��һ�ι�����6��ͨ�� */
    ADC_InitStructure.SecSeg_Ch = 0;                        /* �ڶ��ι�����0��ͨ�� */
    ADC_InitStructure.MCPWM_Trigger_En = ADC_MCPWM_T0_TRG ; /* ��MCPWM_T0 MCPWM_T1Ӳ�������¼� */
    ADC_InitStructure.Trigger_Mode = ADC_1SEG_TRG;          /* ����ADCת��ģʽΪ����ʽ���� */
    ADC_InitStructure.IE = ADC_EOS0_IRQ_EN;                 /* ADC��һ�β��������ж�ʹ�� */  
#endif  
#endif  
#endif

    ADC_Init(ADC0, &ADC_InitStructure);

    ADC0_IF |= 0xFF;
    
    ADC_NormalModeCFG();
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
void HALL_Perip_init(void)
{

    HALL_InitTypeDef HALL_InitStruct;
   
    HALL_StructInit(&HALL_InitStruct);
    HALL_InitStruct.FilterLen = 512;                /* Hall�ź������˲����� 512��ʱ������ */
    HALL_InitStruct.ClockDivision = HALL_CLK_DIV1;  /* ����Hallģ��ʱ�ӷ�Ƶϵ�� */
    HALL_InitStruct.Filter75_Ena = DISABLE;         /* Hall�ź��˲���ʽ��7��5ģʽ����ȫ1��Чģʽ */
    HALL_InitStruct.HALL_Ena = ENABLE;              /* ģ��ʹ�� */
    HALL_InitStruct.Capture_IRQ_Ena = ENABLE;       /* ��׽�ж�ʹ�� */
    HALL_InitStruct.OverFlow_IRQ_Ena = ENABLE;      /* ��ʱ�ж�ʹ�� */
    HALL_InitStruct.CountTH = 9600000;              /* Hallģ�����ģֵ����������ģֵ�������ʱ�ж� */

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

    /* MCPWM P1.4~P1.9 */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_4, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_5, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_6, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_7, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_8, AF3_MCPWM);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_9, AF3_MCPWM);
    /* ����դ�����������ͺţ�P1.12/ P1.15/ P3.13����IO�ڱ�������Ϊ���̬ */
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
//    GPIO_Init(GPIO1, &GPIO_InitStruct);
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
//    GPIO_Init(GPIO3, &GPIO_InitStruct);
	
	  /*KEY_START P2.11 ,KEY_STOP P2.12*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIO2, &GPIO_InitStruct);


#if ((ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR) || (HALL2SENSORLESS_ANGLE_VERIFY == TRUE)) 
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_4, AF2_HALL);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_5, AF2_HALL);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_6, AF2_HALL);
#endif    
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
    TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP;  /* ����Timer CH0 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH0_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH0Output = 0;                    /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP;  /* ����Timer CH1 Ϊ�Ƚ�ģʽ */
    TIM_InitStruct.Timer_CH1_CapMode = TIMER_CapMode_None;
    TIM_InitStruct.Timer_CH1Output = 0;                    /* ����������ʱ���Ƚ�ģʽ������Կ��� */
    TIM_InitStruct.Timer_TH = 48000;                       /* ���ü���������ģֵ */
    TIM_InitStruct.Timer_CMP0 = 24000;                     /* ���ñȽ�ģʽ��CH0�Ƚ�ֵ */
    TIM_InitStruct.Timer_CMP1 = 500;
    TIM_InitStruct.Timer_Filter0 = 0;                      /* ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ */
    TIM_InitStruct.Timer_Filter1 = 0;
    TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div1;          /* ����Timerģ�����ݷ�Ƶϵ�� */
    TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_Zero;       /* ����Timerģ���0�ж� */
    TIM_TimerInit(TIMER0, &TIM_InitStruct);
    TIM_TimerCmd(TIMER0, ENABLE);                          /* Timer0 ģ��ʹ�� */
}

void MCPWM_init(void)
{
    MCPWM_InitTypeDef MCPWM_InitStructure;

    MCPWM_StructInit(&MCPWM_InitStructure);
    
    MCPWM_InitStructure.CLK_DIV = 0;                          /* MCPWMʱ�ӷ�Ƶ���� */
    MCPWM_InitStructure.MCLK_EN = ENABLE;                     /* ģ��ʱ�ӿ��� */
    MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE;                /* ����������ʼ����ʹ�ܿ��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH0 = CENTRAL_PWM_MODE;
    MCPWM_InitStructure.MCPWM_WorkModeCH1 = CENTRAL_PWM_MODE; /* ͨ������ģʽ���ã����Ķ������ض��� */
    MCPWM_InitStructure.MCPWM_WorkModeCH2 = CENTRAL_PWM_MODE;

    MCPWM_InitStructure.GPIO_BKIN_Filter = 12;                /* ��ͣ�¼�(����IO���ź�)�����˲���ʱ������ */
    MCPWM_InitStructure.CMP_BKIN_Filter = 12;                 /* ��ͣ�¼�(���ԱȽ����ź�)�����˲���ʱ������ */

    MCPWM_InitStructure.MCPWM_PERIOD = PWM_PERIOD;            /* ������������ */
    MCPWM_InitStructure.TriggerPoint0 = (u16)(10 - PWM_PERIOD); /* MCPWM_TMR0 ADC�����¼�T0 ���� */
    MCPWM_InitStructure.TriggerPoint1 = (u16)(PWM_PERIOD - 1);/* MCPWM_TMR1 ADC�����¼�T1 ���� */
    MCPWM_InitStructure.DeadTimeCH0N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH0P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH1P = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2N = DEADTIME;
    MCPWM_InitStructure.DeadTimeCH2P = DEADTIME;              /* ����ʱ������ */

#if (PRE_DRIVER_POLARITY == P_HIGH__N_LOW)                    /* CHxP ����Ч�� CHxN�͵�ƽ��Ч */
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
    MCPWM_InitStructure.CH0P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = HIGH_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = HIGH_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = HIGH_LEVEL;
#else
#if (PRE_DRIVER_POLARITY == P_HIGH__N_HIGH)                    /* CHxP ����Ч�� CHxN�ߵ�ƽ��Ч */
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
    MCPWM_InitStructure.CH0P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH0N_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH1P_default_output = LOW_LEVEL;      /* CH1P��Ӧ�����ڿ���״̬����͵�ƽ */
    MCPWM_InitStructure.CH1N_default_output = LOW_LEVEL;     /* CH1N��Ӧ�����ڿ���״̬����ߵ�ƽ */
    MCPWM_InitStructure.CH2P_default_output = LOW_LEVEL;
    MCPWM_InitStructure.CH2N_default_output = LOW_LEVEL;
#endif
#endif

    MCPWM_InitStructure.DebugMode_PWM_out = ENABLE;           /* �ڽ��Ϸ�����debug����ʱ����ͣMCU����ʱ��ѡ���PWMͨ��������������ź�
                                                                 �������Ĭ�ϵ�ƽ�������������� ENABLE:������� DISABLE:���Ĭ�ϵ�ƽ*/

    MCPWM_InitStructure.MCPWM_T0_UpdateEN = ENABLE;           /* MCPWM T0�¼�����ʹ�� */
    MCPWM_InitStructure.MCPWM_T1_UpdateEN = DISABLE;          /* MCPWM T1�¼����� ��ֹ*/

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    MCPWM_InitStructure.T1_Update_INT_EN = ENABLE;           /* T0�����¼� �ж�ʹ�ܻ�ر� */
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    MCPWM_InitStructure.T0_Update_INT_EN = DISABLE;           /* T0�����¼� �ж�ʹ�ܻ�ر� */
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET)) 
    MCPWM_InitStructure.T0_Update_INT_EN = DISABLE;           /* T0�����¼� �ж�ʹ�ܻ�ر� */
#endif    
#endif
#endif

    MCPWM_InitStructure.FAIL0_INT_EN = DISABLE;               /* FAIL0�¼� �ж�ʹ�ܻ�ر� */
    MCPWM_InitStructure.FAIL0_INPUT_EN = DISABLE;             /* FAIL0ͨ����ͣ���ܴ򿪻�ر� */
    MCPWM_InitStructure.FAIL0_Signal_Sel = FAIL_SEL_CMP;      /* FAIL0�¼��ź�ѡ�񣬱Ƚ�����IO�� */
    MCPWM_InitStructure.FAIL0_Polarity = HIGH_LEVEL_ACTIVE;   /* FAIL0�¼�����ѡ�񣬸���Ч */

    MCPWM_InitStructure.FAIL1_INT_EN = ENABLE;                /* FAIL1�¼� �ж�ʹ�ܻ�ر� */
    MCPWM_InitStructure.FAIL1_INPUT_EN = ENABLE;              /* FAIL1ͨ����ͣ���ܴ򿪻�ر� */
    MCPWM_InitStructure.FAIL1_Signal_Sel = FAIL_SEL_CMP;      /* FAIL1�¼��ź�ѡ�񣬱Ƚ�����IO�� */
    MCPWM_InitStructure.FAIL1_Polarity = HIGH_LEVEL_ACTIVE;   /* FAIL1�¼�����ѡ�񣬸���Ч�����Ч */ 
  
    MCPWM_Init(&MCPWM_InitStructure);
    
    mIPD_CtrProc.hDriverPolarity = MCPWM_IO01;                /* ������������ */
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
    MCPWM_TMR1 = (-600);  /*ADC����ʱ��1����ΪPWM���ĵ����ƫ600��Clock*/
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
    MCPWM_TH00 = (-(PWM_PERIOD >> 2));
    MCPWM_TH01 = (PWM_PERIOD >> 2);
    MCPWM_TH10 = (-(PWM_PERIOD >> 2));
    MCPWM_TH11 = (PWM_PERIOD >> 2);
    MCPWM_TH20 = (-(PWM_PERIOD >> 2));
    MCPWM_TH21 = (PWM_PERIOD >> 2);

    PWMOutputs(ENABLE);
    while(1)
    {
    }
}

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT) 
/*******************************************************************************
 �������ƣ�    void MCPWM0_RegUpdate(void)
 ����������    ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void MCPWM0_RegUpdate(void)
{
	
    MCPWM_TH00 = struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU;
    MCPWM_TH01 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
	  if(struFOC_CtrProc.bMotorDirtionCtrlPhase == CW)
		{
			MCPWM_TH10 = struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV;
			MCPWM_TH11 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;

			MCPWM_TH20 = struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW;
			MCPWM_TH21 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;
		}
    else{
	    MCPWM_TH10 = struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW;
      MCPWM_TH11 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

      MCPWM_TH20 = struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV;
      MCPWM_TH21 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
    }

}





#else

#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)\
     || (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
/*******************************************************************************
 �������ƣ�    void MCPWM0_RegUpdate(void)
 ����������    ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void MCPWM0_RegUpdate(void)
{

    MCPWM_TH00 = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
    MCPWM_TH01 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
	  if(struFOC_CtrProc.bMotorDirtionCtrlPhase == CW)
		{
			MCPWM_TH10 = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
			MCPWM_TH11 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;

			MCPWM_TH20 = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;
			MCPWM_TH21 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;
	}else{
	    MCPWM_TH10 = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;
      MCPWM_TH11 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

      MCPWM_TH20 = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
      MCPWM_TH21 = struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
   }

}


#endif

#ifdef TEST_FLASH
void Flash_Test(void)
{
/**********************�����ǵ���Flash���������ʾ��****************************/
 
    u8 testBuf[512] = {1,2,3,4,5,};
    volatile u8 flashRlt;
    
    erase_flag = 0x9A0D361F;   //д�������� 
    EraseSector(0x7800,0);
    progm_flag = 0x9AFDA40C;   //д�������
    flashRlt = ProgramPage(0x7800, 512, (u8*)&testBuf[0], 0);
/*******************************************************************************/
}
#endif

#endif


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




