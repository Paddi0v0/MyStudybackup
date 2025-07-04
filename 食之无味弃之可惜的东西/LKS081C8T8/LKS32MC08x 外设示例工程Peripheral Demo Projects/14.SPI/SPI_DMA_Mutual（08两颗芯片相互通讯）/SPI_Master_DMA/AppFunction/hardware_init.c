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
	  NVIC_EnableIRQ(DMA_IRQn);       /* ʹ��SPI�ж�*/
    NVIC_SetPriority(DMA_IRQn, 1);  /* SPI�ж����ȼ�����*/
    SYS_WR_PROTECT = 0x0;           /* �ر�ϵͳ�Ĵ���д����*/
 #if POWER_MODE	
    SYS_VolSelModuleEnableIRQ(MCU_POWER_5v0);/*MCU��Դ�ж�ʹ�ܺ���*/ 
	#else
		SYS_VolSelModuleEnableIRQ(MCU_POWER_3v3);/*MCU��Դ�ж�ʹ�ܺ���*/ 
 #endif	
	  __enable_irq();                 /* �����ж� */
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
    SPI_InitStruct.DMA = SPI_DMA_ENABLE;          // ѡ��SPI���Ʒ�ʽ����DMA�������ݵ�SPI
    SPI_InitStruct.Trig = 0;                      //�ڲ��Զ���������
    SPI_InitStruct.DataOrder = SPI_FIRSTSEND_LSB; //���ֽ��ȷ��� MSB
    SPI_InitStruct.CPHA = 1;                      //��һ����Ϊ��������ʱ��
    SPI_InitStruct.CPOL = 0;                      //CLKĬ�ϸߵ�ƽ
    SPI_InitStruct.BaudRate = 30;                 //�����ٶ�96MHZ/(2*(30+1))= 1.548MHZ
    SPI_InitStruct.ByteLength = 1;                //��������ֽ�
    SPI_InitStruct.IRQEna = DISABLE;              //�ر�SPI���ж�
    SPI_Init(SPI, &SPI_InitStruct);               //SPI��ʼ������
	  					
    
}

/****************************************************************************************************
 �������ƣ�    void Spi_master_DMA(UINT8 *spi_txdma_data ��UINT8 *spi_rxdma_data, u32 spi_txdma_len)
 ����������    SPI���ݷ�������պ���
 ���������    spi_txdma_data�����������ڴ��ַ
              spi_rxdma_data�����������ڴ��ַ
              spi_txdma_len : �����ֽ���
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ��ʵ��SPI����Ϊȫ˫��������SPIһ�η���len���ݣ�ͬʱ��������Ҳ��len����
              ������Ҫȷ�������뷢�ͻ�������Сһ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2021/8/31      V1.0           HMG               ����
 ****************************************************************************************************/
void Spi_master_DMA(UINT8 *spi_txdma_data, UINT8 *spi_rxdma_data, u8 spi_len)
{
	  DMA_CTRL = 0x0000;                                         //�ر�DMAʹ��	
	  spi_len = spi_len+1;
    SPI_SIZE = spi_len;                                      //SPIһ�ַ���spi_txdma_len�ֽ�
    DMA_CTMS2 = (((spi_len) << 16) & 0x00ff0000) | 0x00000001; //����1�֣�ÿ��1���ֽ�
    DMA_CPAR2 = (u32)&SPI_TX_DATA;                             //SPI_TX_DATA
    DMA_CMAR2 = (u32)spi_txdma_data;                           //�ڴ��ַ

    DMA_CCR2 = 0x2091; //BIT0:ͨ��2ʹ��,BIT4�����䷽���������ڴ�,BIT7���ڴ�ڶ��ֵ�ַ�ڵ�һ���ϵ�����
                       //BIT8~BIT9 = 0b00:�������λ��byte��BIT10~BIT11 = 0b00:�ڴ����λ��byte
                       //BIT12~BIT14 = 0b010:ѡ��SPI_TX��ΪDMA_CH2�����ź�

    DMA_CTMS1 = (((spi_len) << 16) & 0x00ff0000) | 0x00000001; //����1�֣�ÿ��һ���ֽ�
    DMA_CPAR1 = (u32)&SPI_RX_DATA;                             //SPI_RX_DATA
    DMA_CMAR1 = (u32)spi_rxdma_data;                           //�ڴ��ַ

    DMA_CCR1 = 0x1081; //BIT0:ͨ��2ʹ��,BIT1:��������ж�ʹ��,BIT7���ڴ�ڶ��ֵ�ַ�ڵ�һ���ϵ���
                       //BIT8~BIT9 = 0b00:�������λ��byte��BIT10~BIT11 = 0b00:�ڴ����λ��byte
                       //BIT12~BIT14 = 0b001:ѡ��SPI_RX��ΪDMA_CH1�����ź�
	  DMA_CTRL = 0x0001; //DMAʹ��	

		SPI_TX_DATA = 0x00000000; //����SPI������ɱ�־λ��λ��ʹSPI_DMA���ƣ�
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



