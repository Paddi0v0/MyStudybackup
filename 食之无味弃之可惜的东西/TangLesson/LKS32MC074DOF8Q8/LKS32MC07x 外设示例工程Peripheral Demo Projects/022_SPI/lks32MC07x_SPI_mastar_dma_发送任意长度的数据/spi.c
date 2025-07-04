#include "LKS32MC07x_spi.h"       
#include "LKS32MC07x_dma.h"

#define SPI_CS_0() GPIO2->BRR = BIT3;
#define SPI_CS_1() GPIO2->BSRR = BIT3;

void spidelaysend(void);
void spi_txdata(uint8_t *addr,uint32_t len);
uint32_t txaddr;
uint32_t txlen; // ʣ�෢�����ݳ���
volatile uint32_t spisendstate = 0; // 0 ���� 1���ڷ�������
uint32_t sendflg = 0;// �������ͱ�־
void spi_txdata(uint8_t *addr,uint32_t len)
{
    spidelaysend();
    SPI_CS_0();
    spisendstate = 1;
    txaddr = ((uint32_t)addr) ;
    txlen = len;
    
    SPI0_IE |= 0x03;
    DMA_CHx_EN(DMA_CHN1, DISABLE);
    DMA_CHN1->SADR = txaddr;
    if(txlen > 255)
    {
        txaddr += 255;
        txlen -= 255;
        DMA_CHN1->CTMS = 255;
    }
    else
    {
        DMA_CHN1->CTMS = txlen;
        txlen = 0;
    }
    DMA_CHx_EN(DMA_CHN1, ENABLE);
    SPI0_TX_DATA = 0x01; // ����DMA���ˣ�����д�벻�����ݲ������SPI��������
}
void spidelaysend(void)
{
    while(spisendstate == 1);
}

/*******************************************************************************
 �������ƣ�    void DMA_IRQHandler(void)
 ����������    DMA�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void DMA0_IRQHandler(void)
{
    if (DMA0_IF & BIT1) // DMAͨ��1����жϱ�־
	{
		DMA0_IF = BIT1;
        if(txlen == 0)
        {
            DMA_CHN1->SADR = 0; // �������
        }
        else
        {
            if(txlen > 255)
            {
                DMA_CHN1->CCR &= ~1;
                DMA_CHN1->SADR = txaddr;
                DMA_CHN1->CTMS = 255;
                DMA_CHN1->CCR |= 1;
                sendflg = 1;
                txlen -= 255;
                txaddr += 255;
            }
            else
            {
                DMA_CHN1->CCR &= ~1;
                DMA_CHN1->SADR = txaddr;
                DMA_CHN1->CTMS = txlen;
                DMA_CHN1->CCR |= 1;
                txlen = 0;
                sendflg = 1;
            }
        }
	}
    
}
void SPI0_IRQHandler(void)
{
    SPI0->IE |= 7;
    if(sendflg != 0)
    {
        SPI0->TX_DATA=0xff;// ����DMA���ˣ�����д�벻�����ݲ������sPI��������
        sendflg = 0;
    }
    else
    {
        SPI_CS_1();
        spisendstate = 0;
    }
}
