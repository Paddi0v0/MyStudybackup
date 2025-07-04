/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Main.c
 * �ļ���ʶ��
 * ����ժҪ�� ����������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/
#include "hardware_config.h"
#include "global_variable.h"
#include "lks32mc07x_lib.h"

void spi_txdata(uint8_t *addr,uint32_t len);
void spidelaysend(void);
void Hardware_init(void);
void Spi_master_DMA(UINT8 *spi_txdma_data, UINT8 *spi_rxdma_data, u8 spi_len);
/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ע�� SPICSʹ�����ʵ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 SPI_SendByte(u8 byte);
volatile uint8_t Tx_data_tbl[4096]; // ���ݷ��ͻ�����
u16 SPISendStartFlag = 0;
int main(void)
{
    Hardware_init(); /* Ӳ����ʼ�� */
    int i,j=0;
    for (;;)
    {
        // ��ʼ������Ϊȫ0
        for (i = 0; i < 4096; i++)
        {
            Tx_data_tbl[i] = (uint8_t) 0;
        }
        if(j<4096)
        {
            j++;
        }
        else
        {
            j=0;
        }
        // Ϊ�˷�����ԣ���ȫ0����������һ���� 0xff
        Tx_data_tbl[j] = 0xff;
        // ��������
        spi_txdata((uint8_t*)Tx_data_tbl,4096);
        // �ȴ����ݷ������
        spidelaysend();
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
