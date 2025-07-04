/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ�����lks32mc07x8x_spi.h
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
 
#ifndef __lks32mc07x_SPI_H
#define __lks32mc07x_SPI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
#include "basic.h"



typedef enum
{
   SPI_Master = 0x01,/* Master Mode */
   SPI_Slave  = 0x00 /* Slave Mode */
}SPI_Mode;

typedef enum
{
   SPI_Full    = 0x0,/* ����ȫ˫��ģʽ */
	 SPI_Half_Tx = 0x2,/* ������˫��ģʽ ������ */
   SPI_Half_Rx = 0x3 /* ������˫��ģʽ ������ */
}SPI_Duplex;

/** 
 *@brief SPI�Ĵ����ṹ����
 */
typedef struct
{
   __IO uint32_t CFG;      /**< SPI���ƼĴ��� */
   __IO uint32_t IE;       /**< SPI�жϼĴ��� */
   __IO uint32_t BAUD;     /**< SPI�����ʿ��ƼĴ��� */
	 __IO uint32_t TX_DATA;  /**< SPI�������ݼĴ��� */
   __IO uint32_t RX_DATA;  /**< SPI�������ݼĴ��� */
   __IO uint32_t SIZE;     /**< SPI�������ݳ��ȼĴ��� */
}SPI_TypeDef;

#define SPI0            ((SPI_TypeDef *) SPI0_BASE)

typedef struct
{
	 SPI_Duplex Duplex;    /**< ���ݴ���ģʽѡ��ȫ˫��0x00, ��˫��������0x10, ��˫��������0x11 ģʽѡ��  */
   uint8_t    CS;        /**< ���豸��Ƭѡ�ź���Դ��0����Ϊ0��1����Դ�����豸 */
   SPI_Mode   Mode;      /**< ����ģʽѡ��0����ģʽ��1����ģʽ */
	 uint8_t    CPHA;      /**< ��λѡ��0��Ӧʱ�Ӵ�Ĭ�ϵ�ƽ����һ���������Ϊ��������ʱ�̣�1��Ӧ��������ʱ�� */
   uint8_t    CPOL;      /**< ����ѡ��0��ӦĬ��ʱ�ӵ�ƽΪ�͵�ƽ��1��ӦĬ��ʱ��Ϊ�ߵ�ƽ */
   uint8_t    ENDIAN;    /**< ����˳��ѡ��0��ӦMSB��λ�ȴ��䣻1��ӦLSB��λ�ȴ��� */
   uint8_t    EN;        /**< ģ��ʹ�ܣ�1 ����ģ�� */

   uint8_t    IRQEna;    /**< �ж�ʹ�� */
   uint8_t    Trig;      /**< ���䴥����ʽѡ�� 1���ⲿ������0�ڲ��Զ�ִ�У�����ģʽ��Ч�� */

   uint8_t    TRANS_MODE;/**< ���ݰ��Ʒ�ʽѡ�� 0��DMA���ˣ�1MCU���� */
   uint16_t   BaudRate;  /**< ���������� ȡֵ��Χ3-4095 */

   uint8_t    ByteLength;/**< ���ݳ��� */
}SPI_InitTypeDef;

#define SPI_FIRSTSEND_LSB 1    /**< ��λ�ȴ� */
#define SPI_FIRSTSEND_MSB 0    /**< ��λ�ȴ� */

#define SPI_DMA_ENABLE     0   /**< ʹ��DMA���� */
#define SPI_DMA_DISABLE    1   /**< ��ֹDMA���� */

/*�ж�ʹ�ܶ���*/
#define SPI_IRQEna_Enable              BIT7            /**<�ж�ʹ�ܶ��壬SPI�ж�ʹ�� */
#define SPI_IRQEna_TranDone            BIT6            /**<�ж�ʹ�ܶ��壬��������ж� */
#define SPI_IRQEna_SSErr               BIT5            /**<�ж�ʹ�ܶ��壬�����쳣�¼��ж� */
#define SPI_IRQEna_DataOver            BIT4            /**<�ж�ʹ�ܶ��壬��������¼��ж� */

/*�жϱ�־����*/
#define SPI_IF_TranDone                BIT2            /**<�жϱ�־���壬��������жϱ�־λ */
#define SPI_IF_SSErr                   BIT1            /**<�жϱ�־���壬�����쳣�жϱ�־λ */
#define SPI_IF_DataOver                BIT0            /**<�жϱ�־���壬��������жϱ�־λ */

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct);

void    SPI_SendData(SPI_TypeDef *SPIx, uint8_t n);
uint8_t SPI_ReadData(SPI_TypeDef *SPIx);

uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag);
void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag);

#endif /*__lks32mc07x_SPI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
