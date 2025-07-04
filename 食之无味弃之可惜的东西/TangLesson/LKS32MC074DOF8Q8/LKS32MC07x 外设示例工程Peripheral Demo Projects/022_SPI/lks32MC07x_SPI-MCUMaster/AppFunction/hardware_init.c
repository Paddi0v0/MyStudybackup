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
//#include "lks32mc07x_tim.h"
#include "lks32mc07x_lib.h"
#define  CNT0_PRE_DRIVER_POLARITY           P_HIGH__N_HIGH
#define  CNT1_PRE_DRIVER_POLARITY           P_HIGH__N_LOW


void SoftDelay(u32 cnt);
void SPI_init(void);
void DMA_init(void);
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
	  FLASH_CFG |= 0x00080000;          /* enable prefetch */
	  IWDG_DISABLE();                   /* �رտ��Ź�*/
   
    GPIO_init();                      /* GPIO��ʼ�� */
    SPI_init();

    SoftDelay(100);                   /* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */
    __enable_irq();                   /* �������ж� */
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
    SPI_InitStruct.TRANS_MODE = SPI_DMA_DISABLE;         // ѡ��SPI���Ʒ�ʽ����MCU�������ݵ�SPI
    SPI_InitStruct.Trig = 0;                      //�ڲ��Զ���������
    SPI_InitStruct.ENDIAN = SPI_FIRSTSEND_LSB;   //���ֽ��ȷ��� LSB
    SPI_InitStruct.CPHA = 0;                      //��һ����Ϊ��������ʱ��
    SPI_InitStruct.CPOL = 0;                      //CLKĬ�ϸߵ�ƽ
    SPI_InitStruct.BaudRate = 30;                 //�����ٶ�96MHZ/(2*(31+1))= 1.50MHZ
    SPI_InitStruct.ByteLength = 8;                //��������ֽ�
    SPI_InitStruct.IRQEna = DISABLE;              //�ر�SPI���ж�
    SPI_Init(SPI0, &SPI_InitStruct);               //SPI��ʼ������
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

    SPI0_TX_DATA = byte; //��������

    while (!(SPI0_IE & BIT2))
    {
    };                   /*�ȴ��������*/
    rdata = SPI0_RX_DATA; //��������
    SPI0_IE |= BIT2;      //���SPI������ɱ�־λ
    return rdata;        //���ؽ�������
}
volatile u32 dma_hall_cnt;

void DMA_init(void)
{
    DMA_InitTypeDef dma_init = {0};
    
    dma_init.SBTW  = 2                      ;   // Դ��ַ����λ�� 0: Byte 1: Halfword 2: Word
    dma_init.DBTW  = 2                      ;   // Ŀ�ĵ�ַ����λ��  0: Byte  1: Halfword  2: Word
    dma_init.SINC  = DISABLE                ;   // Դ��ַ����ʹ��
    dma_init.DINC  = DISABLE                ;   // Ŀ�ĵ�ַ����ʹ��
    dma_init.CIRC  = DISABLE                ;   // ѭ��ģʽʹ��
    dma_init.RMODE = DISABLE                ;   // ���ִ���ʹ�ܣ����DMA ������ɴ���
    dma_init.EN    = ENABLE                 ;   // ͨ��ʹ��
    dma_init.TMS   = 1                      ;   // DMA ͨ�� x ���ݰ��˴�����
    dma_init.SADR  = (u32)&HALL->CNT              ;   // DMA ͨ�� x Դ��ַ
    dma_init.DADR  = (u32)&dma_hall_cnt     ;   // DMA ͨ�� x Ŀ�ĵ�ַ
    dma_init.REN.B.MCPWM_RE0_EN = ENABLE    ;   // DMA ����ʹ��
    DMA_Init(DMA_CHN0,&dma_init);
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
		
		GPIO_StructInit(&GPIO_InitStruct);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //����Ϊ���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_3;
    GPIO_Init(GPIO2, &GPIO_InitStruct);
//    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_1, AF5_SPI);  //GPIO2.1����SPI_clk����
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_10, AF5_SPI); //GPIO2.10����SPI_DO����
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_3, AF5_SPI);  //GPIO2.3����SPI_CS����
	
	  GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;            //����Ϊ����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIO1, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO1, GPIO_PinSource_0, AF5_SPI); //GPIO1.0����SPI_DI����
	
	  GPIO_StructInit(&GPIO_InitStruct);
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;            //����Ϊ����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_14, AF5_SPI); //GPIO1.0����SPI_CLK����

}





