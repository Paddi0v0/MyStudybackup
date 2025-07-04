/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc08x_spi.h
 * �ļ���ʶ��
 * ����ժҪ�� SPI����ͷ�ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� 
 * ������ڣ� 
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#ifndef __lks32mc08x_SPI_H
#define __lks32mc08x_SPI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x.h"
#include "basic.h"

typedef enum
{
   SPI_Master = 0x01, /* SPI��ģʽ */
   SPI_Slave  = 0x00  /* SPI��ģʽ */
}SPI_Mode;

typedef enum
{
   SPI_Full    = 0x0, /* SPIȫ˫�� */
	 SPI_Half_Tx = 0x2, /* SPI��˫�������� */
   SPI_Half_Rx = 0x3  /* SPI��˫�������� */
}SPI_Duplex;

typedef struct
{
   __IO uint32_t CFG;     /* SPI���ƼĴ��� */
   __IO uint32_t IE;      /* SPI�жϼĴ��� */
   __IO uint32_t DIV;     /* SPI�����ʿ��ƼĴ��� */
	 __IO uint32_t TX_DATA; /* SPI�������ݼĴ��� */
   __IO uint32_t RX_DATA; /* SPI�������ݼĴ��� */
   __IO uint32_t SIZE;    /* SPI�������ݳ��ȼĴ��� */
}SPI_TypeDef;

#ifndef   SPI
#define   SPI      ((SPI_TypeDef *) SPI0_BASE)
#endif

typedef struct
{
	 SPI_Duplex Duplex;    /* ȫ˫������˫������ģʽѡ��0��ȫ˫����2����˫�������ͣ�3����˫�������� */
   uint8_t    SS;        /* ���豸��Ƭѡ�ź���Դ��0����Ϊ0��1����Դ�����豸 */
   SPI_Mode   Mode;      /* ����ģʽѡ��0����ģʽ��1����ģʽ */
   uint8_t    CPHA;      /* ��λѡ��0��Ӧ0��1��Ӧ1 */
   uint8_t    CPOL;      /* ����ѡ��0��Ӧ0��1��Ӧ1 */
   uint8_t    DataOrder; /* ����˳��0����λ�ȴ���1����λ�ȴ� */
   uint8_t    EN;        /* SPIģ��ʹ�ܣ�0���رգ�1������ */

   uint8_t    IRQEna;    /* SPI�ж�ʹ�ܣ�0���رգ�1������ */
   uint8_t    Trig;      /* ���䴥��ѡ��0���ڲ��Զ�ִ�У�����ģʽ��Ч����1���ⲿ���� */

	 uint8_t    DMA;       /* SPI���ݰ��˷�ʽ��0��DMA���ˣ�1��MCU���� */
   uint8_t    BaudRate;  /* ���������� */

   uint8_t    ByteLength;/* SPI�������ݳ��ȣ�CPU����ֻ��д1 */
}SPI_InitTypeDef;

#define SPI_FIRSTSEND_LSB 1    /* ��λ�ȴ� */
#define SPI_FIRSTSEND_MSB 0    /* ��λ�ȴ� */

#define SPI_DMA_ENABLE     0      /* ʹ��DMA���� */
#define SPI_DMA_DISABLE    1      /* ��ֹDMA���� */

/*�ж�ʹ�ܶ���*/
#define SPI_IRQEna_Enable              BIT7            /* SPI�ж�ʹ�� */
#define SPI_IRQEna_TranDone            BIT6            /* ��������ж� */
#define SPI_IRQEna_SSErr               BIT5            /* �����쳣�¼��ж� */
#define SPI_IRQEna_DataOver            BIT4            /* ��������¼��ж� */

/*�жϱ�־����*/
#define SPI_IF_TranDone                BIT2            /* ��������жϱ�־λ */
#define SPI_IF_SSErr                   BIT1            /* �����쳣�жϱ�־λ */
#define SPI_IF_DataOver                BIT0            /* ��������жϱ�־λ */

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag);
void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag);

void    SPI_SendData(SPI_TypeDef *SPIx, uint8_t n);
uint8_t SPI_ReadData(SPI_TypeDef *SPIx);
#endif /*__lks32mc08x_SPI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
