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
    __disable_irq();                /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;        /* ʹ��ϵͳ�Ĵ���д����*/
    FLASH_CFG |= 0x00080000;        /* FLASHԤȡ����ʹ��*/
    GPIO_init();                    /* GPIO��ʼ��*/
    SPI_init();                     /* SPI��ʼ��*/
    SoftDelay(100);                 /* �ȴ�Ӳ����ʼ�����*/	
	  NVIC_EnableIRQ(SPI0_IRQn);      /* ʹ��SPI�ж�*/
    NVIC_SetPriority(SPI0_IRQn, 1); /* SPI�ж����ȼ�����*/
	  SYS_WR_PROTECT = 0x0;           /* �ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
    __enable_irq();                 /* �����ж� */
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
    /*SPI��ʼ��*/
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //����Ϊ���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_3;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_1, AF5_SPI);  //GPIO2.1����SPI_clk����
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_10, AF5_SPI); //GPIO2.10����SPI_DO����
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_3, AF5_SPI);  //GPIO2.3����SPI_CS����
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;            //����Ϊ����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF5_SPI); //GPIO1.0����SPI_DI����
}

/*******************************************************************************
 �������ƣ�    void SPI_Init(void)
 ����������    SPIӲ�����ֳ�ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/31      V1.0           HMG               ����
 *******************************************************************************/
void SPI_init(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    SPI_StructInit(&SPI_InitStruct);              // SPI�ṹ���ʼ��
    SPI_InitStruct.Duplex = SPI_Full;             //˫��ģʽ����
    SPI_InitStruct.Mode = SPI_Master;             //master����ģʽ
    SPI_InitStruct.EN = ENABLE;                   //ʹ��SPIģ��
    SPI_InitStruct.DMA = SPI_DMA_DISABLE;         // ѡ��SPI���Ʒ�ʽ����MCU�������ݵ�SPI
    SPI_InitStruct.Trig = 0;                      //�ڲ��Զ���������
    SPI_InitStruct.DataOrder = SPI_FIRSTSEND_MSB; //���ֽ��ȷ��� MSB
    SPI_InitStruct.CPHA = 0;                      //��һ����Ϊ��������ʱ��
    SPI_InitStruct.CPOL = 0;                      //CLKĬ�ϸߵ�ƽ
    SPI_InitStruct.BaudRate = 30;                 //�����ٶ�96MHZ/(2*(30+1))= 1.548MHZ
    SPI_InitStruct.ByteLength = 0;                //��������ֽ�
    SPI_InitStruct.IRQEna = DISABLE;              //�ر�SPI���ж�
    SPI_Init(SPI, &SPI_InitStruct);               //SPI��ʼ������
}

/****************************************************************************************************
 �������ƣ�    void SPI_SendByte(u8 byte)
 ����������    SPI���ݷ�������պ���
 ���������    byte:Ҫ���͵�����
             
 ���������    ��
 �� �� ֵ��    1�ֽڽ�������
 ����˵����    ÿ�η������SPI_SIZEӲ���Զ����㣬ÿ�η���ʱ��Ҫ��SPI_SIZE��1.
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/31      V1.0           HMG               ����
 ****************************************************************************************************/
u8 SPI_SendByte(u8 byte)
{
    u8 rdata = 0;

    SPI_SIZE = 1;       //SPI�����ֽڣ�MCU��ʽ��ֻ��д��1��
    SPI_TX_DATA = byte; //��������
    while (!(SPI_IE & BIT2))
    {
    };                   /*�ȴ��������*/
    rdata = SPI_RX_DATA; //��������
    SPI_IE |= BIT2;      //���SPI������ɱ�־λ
    return rdata;        //���ؽ�������
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



