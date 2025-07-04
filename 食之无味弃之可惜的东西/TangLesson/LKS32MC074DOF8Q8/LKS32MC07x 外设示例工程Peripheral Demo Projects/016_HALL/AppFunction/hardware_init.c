/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_init.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ����ʼ������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Olive Wang
 * ������ڣ� 2023��02��22��
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
2023/02/22     V1.0           Olive Wang          ����
*******************************************************************************/
void Hardware_init(void)
{
  __disable_irq();         /* �ر��ж� �ж��ܿ��� */
  SYS_WR_PROTECT = 0x7a83; /*ʹ��ϵͳ�Ĵ���д����*/
  FLASH_CFG |= 0x00080000; /* enable prefetch ��FLASHԤȡ����ʹ��*/
  IWDG_DISABLE();                   /* �رտ��Ź�*/	
	GPIO_init();					 /* GPIO��ʼ�� */
  HALL_init();           /* HALL��ʼ�� */
  SoftDelay(100);          /* �ȴ�Ӳ����ʼ�����*/
	NVIC_EnableIRQ(HALL0_IRQn); /*ʹ��MCPWM�ж�*/
  NVIC_SetPriority(HALL0_IRQn, 3);
	SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
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
2023/02/22       V1.0           Olive Wang          ����
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
 �������ƣ�    void GPIO_init(void)
 ����������    GPIO����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
2023/02/22       V1.0           Olive Wang          ����
 *******************************************************************************/

void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);
	
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
	
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
	
   //GPIO_P0.11/P0.12/P0.13����ΪHALL_IN0/HALL_IN1/HALL_IN2ģ��ͨ��
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_4, AF2_HALL);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_5, AF2_HALL);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_6, AF2_HALL);
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
2023/02/22      V1.0           Olive Wang          ����
 *******************************************************************************/
void SystemInit(void)
{
  Clock_Init(); /* ʱ�ӳ�ʼ�� */
}

/*******************************************************************************
 �������ƣ�    void HALL_init(void)
 ����������    HALL��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
2023/02/22      V1.0           Olive Wang          ����
 *******************************************************************************/
void HALL_init(void)
{	
	  HALL_InitTypeDef HALL_InitStruct;
   
    HALL_StructInit(&HALL_InitStruct);
	
    HALL_InitStruct.FilterLen = 512;                /* Hall�ź������˲����� 512��ʱ������ */
    HALL_InitStruct.ClockDivision = HALL_CLK_DIV8;  /* ����Hallģ��ʱ�ӷ�Ƶϵ�� */
    HALL_InitStruct.Filter75_Ena = DISABLE;         /* Hall�ź��˲���ʽ��7��5ģʽ����ȫ1��Чģʽ */
    HALL_InitStruct.HALL_Ena = ENABLE;              /* ģ��ʹ�� */
    HALL_InitStruct.Capture_IRQ_Ena = ENABLE;       /* ��׽�ж�ʹ�� */
    HALL_InitStruct.OverFlow_IRQ_Ena = ENABLE;      /* ��ʱ�ж�ʹ�� */
    HALL_InitStruct.CountTH = 960000;              /* Hallģ�����ģֵ����������ģֵ�������ʱ�ж� */
		HALL_InitStruct.softIE = ENABLE;								 /* ����ж�ʧ�� */
	
    HALL_Init(&HALL_InitStruct);/* HALL���� */
		HALL_Cmd(ENABLE);/* HALLʹ�� */
}

/*******************************************************************************
 �������ƣ�    int GPIO_KEY_Scan(u8 mode)
 ����������    ����������
 ���������    mode:0,��֧��������;1,֧��������;
 ���������    ��
 �� �� ֵ��    0��û���κΰ�������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
2023/02/22      V1.0           Olive Wang            ����
 *******************************************************************************/
int GPIO_KEY_Scan(u8 mode)
{
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(key_start ==0||key_stop==0))
	{
		SoftDelay(10);//ȥ���� 
		key_up=0;
		if(key_start==0)return start_PRES;
		else if(key_stop==0)return stop_PRES;
	}else if(key_start==1&&key_stop==1) key_up=1; 	    
 	return 0;// �ް�������

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
2023/02/22       V1.0           Olive Wang          ����
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
2023/02/22     V1.0          Olive Wang             ����
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

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
