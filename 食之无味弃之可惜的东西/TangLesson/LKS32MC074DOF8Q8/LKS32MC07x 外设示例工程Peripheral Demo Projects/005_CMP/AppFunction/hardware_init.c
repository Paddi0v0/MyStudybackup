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
#include "lks32mc07x_cmp.h"


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
	GPIO_init();             /* GPIO��ʼ�� */
  CMP_init();              /* �Ƚ�����ʼ�� */
  SoftDelay(100);          /* �ȴ�Ӳ����ʼ�����*/
	NVIC_SetPriority(CMP_IRQn, 8); // CMP�ж����ȼ�����
  NVIC_EnableIRQ(CMP_IRQn);      // ʹ��CMP��ʱ���ж�
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
 �������ƣ�    void CMP_init(void)
 ����������    CMP��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
2023/02/22      V1.0           Olive Wang          ����
 *******************************************************************************/
void CMP_init(void)
{	
	CMP_InitTypeDef CMP_InitStre;
	CMP_StructInit(&CMP_InitStre);
	
	CMP_InitStre.CLK_COM_DIV = 0;									/* �Ƚ��������˲�ʱ�ӷ�Ƶ*/
	CMP_InitStre.FT = DISABLE;										/* �Ƚ������ٱȽ�*/
	CMP_InitStre.HYS = CMP_HYS_20mV;							/* �Ƚ����ͻص�ѹ*/
	
	CMP_InitStre.CMP0.SELP = CMP0_SELP_IP1; 		  /* �Ƚ���0�����ź�ѡ�� */
	CMP_InitStre.CMP0.SELN = CMP_SELN_REF; 				/* �Ƚ���0�����ź�ѡ�� */
	CMP_InitStre.CMP0.RE = DISABLE;								/* �Ƚ���0DMAʧ��*/
	CMP_InitStre.CMP0.POL= CMP_HIGH_LEVEL;				/* �Ƚ���0�ߵ�ƽ�����Ч*/
	CMP_InitStre.CMP0.IRQ_TRIG = IRQ_LEVEL_TRIG_MODE; /* �Ƚ���0��ƽ�����ж�ģʽ*/
	CMP_InitStre.CMP0.IN_EN = ENABLE; 			 			/* �Ƚ���0�ź�����ʹ�� */
	CMP_InitStre.CMP0.IE = ENABLE;  							/* �Ƚ���0�ź��ж�ʹ�� */
	CMP_InitStre.CMP0.FIL_CLK_DIV16 = 2; 					/* ���˲����=tclk ����*16*CMP_FltCnt (CMP_FltCnt��Ƶϵ��,0~15)*/
	CMP_InitStre.CMP0.FIL_CLK_DIV2 = 2;  					/* �Ƚ��� 2/1/0 �˲�ʱ��ʹ�� */
	CMP_InitStre.CMP0.CLK_EN = ENABLE;						/* �Ƚ���ʱ��ʹ��*/
	CMP_InitStre.CMP0.EN = ENABLE;       				  /* �Ƚ���0���� ����SYS_AFE_REG5 */
	CMP_Init(&CMP_InitStre);										/* �Ƚ�����ʼ�� */
	
	CMP_Cmd(CMP_CHN_0,ENABLE);/* �Ƚ���0ʱ��ʹ��*/
}
/*******************************************************************************
 �������ƣ�    void GPIO_init(void)
 ����������    GPIO��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
2023/02/22      V1.0           Olive Wang            ����
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

    //CMP0_OUT  GPIO_P0.14����Ϊģ��ͨ��
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
	
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_14, AF1_SYSAF);
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
2023/02/22       V1.0        Olive Wang          ����
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
