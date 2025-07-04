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


can_parameter can_par =
{
        0x00,
        0x00,
        0x00,
        {0, 0},
        {0, 0},
        0x00,
};
		
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
    __disable_irq();               /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;       /*ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;       /* enable prefetch ��FLASHԤȡ����ʹ��*/
    GPIO_init();                   /* GPIO��ʼ��*/
    CAN_init();                    /* CAN��ʼ��*/
	  SoftDelay(100);                /* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */
	  NVIC_EnableIRQ(DMA_IRQn);      /* ʹ��DMA�ж� */
    NVIC_SetPriority(DMA_IRQn, 1); /* ����DMA�ж����ȼ�*/	
	  SYS_WR_PROTECT = 0x0;          /*�ر�ϵͳ�Ĵ���д����*/
	
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
    __enable_irq();                /* �����ж� */
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
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;//GPIO����ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO2, &GPIO_InitStruct);	
	  //����LED1��P0.6  LED2��P0.7  LED3: P0.3
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO���ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO0, &GPIO_InitStruct);	
		
	  /*P0.7 CAN TX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    /*P0.6 CAN RX*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIO0, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_6, AF10_CAN);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_7, AF10_CAN);
}
/*******************************************************************************
 ��������: void CAN_init(void)
 ��������: CAN��ʼ������,����2.0BЭ��
 �����ı�:  Can������      BTR0    	BTR1
            1Mbps 		  0x05      0x14
            800Kbps 	  0x05      0x16
            666Kbps 	  0x85 	    0xB6
            500Kbps 	  0x05      0x1C
            400Kbps 	  0x05	    0xFA
            250Kbps 	  0x0B 	    0x1C
            200Kbps 	  0x05 	    0xFA
            125Kbps 	  0x17      0x1C
            100Kbps 	  0x1D      0x1C
            83.33Kbps 	0x17      0x6F
            80Kbps 		  0x97      0xFF
            66.66Kbps 	0x1D      0x6F
            50Kbps 		  0x3B      0x1C
            40Kbps 		  0xAF      0xFF
 �������: ��
 �������: ��
 ����ֵ:   ��
 ����˵��:
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/5/3      V1.0           HuangMG            ����
*******************************************************************************/
void CAN_init(void)
{
    CAN_InitTypeDef CAN_InitStruct;
    CAN_StructInit(&CAN_InitStruct);
    /*Can������:500Kbps*/
    CAN_InitStruct.CAN_Btr0 = 0x05;
    CAN_InitStruct.CAN_Btr1 = 0x1C;
    CAN_InitStruct.CAN_DMAEn = ENABLE;               /*DMA��������*/
    CAN_InitStruct.CAN_WorkMode = CAN_SELFTEST_MODE; /*CAN�����շ�����ģʽ*/
	  CAN_InitStruct.CAN_ErrWarThre = 50;              /*�����뾯������ֵΪ50*/
    CAN_InitStruct.IRQEna = CAN_IE_RFIFONOEMPTY | CAN_IE_TXDONE | CAN_IE_WERR;
    CAN_Init(CAN, &CAN_InitStruct);
    /*�����˲��� ID:0x5A ,��־֡��BYTE1:0x81,BYTE:0x23*/
    SFF_ID_Filter(0X5A, 0x00, 0x0, 0x0, 0x81, 0x00, 0x23, 0x00);
}

/*******************************************************************************
 �������ƣ�    void CAN_DMA_RX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
 ����������    CAN_DMA���ݽ��պ���
 ���������    ide��0��׼/1��չ֡
              frame_number����Ҫ���յ�֡����
              frame_size��ÿ֡�������ݳ��ȣ�0-8 BYTE�����ݣ�����ID�ȡ�
              memaddr���������ݻ�������ַ
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/5/4      V1.0           HuangMG            ����
 *******************************************************************************/
void CAN_DMA_RX(u8 ide , u8 frame_number,u8 frame_size ,  u8* memaddr)
{ 
	  u8 len = 0 , rgsr = 0;
	  DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
	  if(ide)/*��չ֡*/
		{
		  len = 5+frame_size;
		}else{ /*��׼֡*/
			len = 3+frame_size;
		}
    DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;             /* DMA ��������ж�ʹ�� */
    DMA_InitStruct.DMA_DIR = PERI2MEMORY;             /* �������ڴ� */
    DMA_InitStruct.DMA_CIRC = DISABLE;                /* DMA����ģʽ��ѭ��ģʽ������Ч */
    DMA_InitStruct.DMA_PINC = ENABLE;                 /* �����ַÿ�����Ƿ����,����Ч */
    DMA_InitStruct.DMA_MINC = ENABLE;                 /* �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ��������ڵ�ַһ��������,����Ч */
    DMA_InitStruct.DMA_PBTW = DMA_BYTE_TRANS;         /* �������λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_MBTW = DMA_BYTE_TRANS;         /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
    DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN;   /* CAN DMA����ʹ�ܣ�����Ч */
    DMA_InitStruct.DMA_TIMES = len;                   /* DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    DMA_InitStruct.DMA_ROUND = frame_number;          /* DMA ͨ�� x �������� 1~255 */
    DMA_InitStruct.DMA_CPAR = (u32)&CAN_TXRX0;        /* DMA ͨ�� x �����ַ */
    DMA_InitStruct.DMA_CMAR = (u32)memaddr;           /* DMA ͨ�� x �ڴ��ַ */
    DMA_Init(DMA_CH3, &DMA_InitStruct);
/*******************�������DMA���ݰ��˱�־λ����ֹ��������ݴ���DMA����*****************/
    rgsr =CAN_GetIRQFlag();                           /*��ȡ�ж�״̬�Ĵ���*/
    rgsr = rgsr;                                      /*��ֹ���뱨����*/
/************************************************************************************/		
    DMA_CHx_EN(DMA_CH3,ENABLE);                       /*ʹ��DMA_CH3ͨ��*/
};

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    ����������
 ���������    mode:0,��֧��������;1,֧��������;
 ���������    ��
 �� �� ֵ��    0��û���κΰ�������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/6/8      V1.0           HuangMG            ����
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
