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
// #include "lks32mc07x_tim.h"
#include "lks32mc07x_lib.h"
#define CNT0_PRE_DRIVER_POLARITY P_HIGH__N_HIGH
#define CNT1_PRE_DRIVER_POLARITY P_HIGH__N_LOW

void SoftDelay(u32 cnt);
void SPI_init(void);
void DMA_init(void);
void GPIO0_Pin14Init(void);
void DMA_init(void);
extern uint16_t Tx_data_tbl[];
extern uint16_t Rx_data_tbl[];
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

    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
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
    __disable_irq(); /* �ر��ж� �ж��ܿ��� */
    SYS_WR_PROTECT = 0x7a83;
    FLASH_CFG |= 0x00080000; /* enable prefetch */
//    SYS_SFT_RST = 0xffffffff;
    SYS_SFT_RST = 0;
    IWDG_DISABLE();          /* �رտ��Ź�*/
    SYS_ModuleClockCmd(SYS_Module_GPIO, ENABLE);
    
    
    DMA_init();
    SPI_init();
    GPIO_init();
    
    
    SoftDelay(100); /* ��ʱ�ȴ�Ӳ����ʼ���ȶ� */
    
    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_SetPriority(DMA0_IRQn, 0);
    NVIC_EnableIRQ(SPI0_IRQn);
    NVIC_SetPriority(SPI0_IRQn, 0);

    __enable_irq(); /* �������ж� */
}
/*******************************************************************************
 �������ƣ�    void DMA_init(void)
 ����������    DMA��ʼ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ʣ�����ж�DMA_UART�Ĵ��䷽��
               ��DMA_DIR����DMA_UART�Ĵ��䷽��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/02/22      V1.0             ywx                ����
 *******************************************************************************/
void DMA_init(void)
{
    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.IRQ_EN = ENABLE;
    DMA_InitStruct.CIRC = DISABLE; // �ر�ѭ��ģʽ
    DMA_InitStruct.SINC = ENABLE;  // Դ��ַ����ʹ��
    DMA_InitStruct.DINC = DISABLE; // Ŀ�ĵ�ַ����ʧ��
    DMA_InitStruct.SBTW = DMA_BYTE_TRANS;
    DMA_InitStruct.DBTW = DMA_BYTE_TRANS;
    DMA_InitStruct.REN.B.SPI_TX_EN = ENABLE;
    DMA_InitStruct.TMS = 8;
    DMA_InitStruct.RMODE = ENABLE;
    DMA_InitStruct.SADR = (u32)Tx_data_tbl;
    DMA_InitStruct.DADR = (u32)&SPI0_TX_DATA;
    DMA_InitStruct.EN = ENABLE;
    DMA_Init(DMA_CHN1, &DMA_InitStruct);
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

    SPI_StructInit(&SPI_InitStruct);            // SPI�ṹ���ʼ��
    SPI_InitStruct.Duplex = SPI_Half_Tx;        // ˫��ģʽ����
    SPI_InitStruct.Mode = SPI_Master;           // master����ģʽ
    SPI_InitStruct.EN = ENABLE;                 // ʹ��SPIģ��
    SPI_InitStruct.TRANS_MODE = SPI_DMA_ENABLE; // ѡ��SPI���Ʒ�ʽ����MCU�������ݵ�SPI
    SPI_InitStruct.Trig = 0;                    // �ڲ��Զ���������
    SPI_InitStruct.ENDIAN = SPI_FIRSTSEND_LSB;  // ���ֽ��ȷ��� LSB
    SPI_InitStruct.CPHA = 0;                    // ��һ����Ϊ��������ʱ��
    SPI_InitStruct.CPOL = 0;                    // CLKĬ�ϸߵ�ƽ
    SPI_InitStruct.BaudRate = 10;               // �����ٶ�96MHZ/(2*(31+1))= 1.50MHZ
    SPI_InitStruct.ByteLength = 8;              // ��������ֽ�
    SPI_InitStruct.IRQEna = 0xf0;               // �ر�SPI���ж�
    SPI_Init(SPI0, &SPI_InitStruct);            // SPI��ʼ������
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
    SYS_WR_PROTECT = 0x7a83;  /* ���ϵͳ�Ĵ���д���� */
    SYS_AFE_REG5 = BIT15;     /* BIT15:PLLPDN ��PLL */
    SoftDelay(100);           /* ��ʱ100us, �ȴ�PLL�ȶ� 21.4.17*/
    SYS_CLK_CFG = 0x000001ff; /* BIT8:0: CLK_HS,1:PLL  | BIT[7:0]CLK_DIV  | 1ff��Ӧ96Mʱ�� */
    SYS_WR_PROTECT = 0;       /* �ر�ϵͳ�Ĵ���д����*/
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
    while (nSize--)
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
    // SPI_DO P2.10
    // SPI_CLK P0.14
    // SPI_DO P2.3
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // ����Ϊ���ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
    GPIO_SetBits(GPIO2,GPIO_Pin_3);
    GPIO_Init(GPIO2, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO2, GPIO_PinSource_10, AF5_SPI); // GPIO2.10����SPI_DO����


    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // ����Ϊ����ģʽ
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIO0, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIO0, GPIO_PinSource_14, AF5_SPI); // GPIO1.0����SPI_CLK����

}
