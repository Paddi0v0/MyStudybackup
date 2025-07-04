/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_uart.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� UART����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��05��21�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */


#ifndef __LKS32MC08x_UART_H
#define __LKS32MC08x_UART_H

#include "lks32mc08x_lib.h"

/** ������żУ������ö�ٳ�Ա*/ 
typedef enum
{
   UART_Parity_NO = 0x00,  /**<����żУ��*/
   UART_Parity_EVEN = 0x01,/**<żУ��*/
   UART_Parity_ODD = 0x02  /**<��У��*/
} UART_ParityMode;

/** 
 *@brief UART�Ĵ����ṹ����
 */
typedef struct
{
   __IO uint32_t CTRL; /**<UART���ƼĴ���*/
   __IO uint32_t DIVH; /**<UART���������ø��ֽڼĴ���*/
   __IO uint32_t DIVL; /**<UART���������õ��ֽڼĴ���*/
   __IO uint32_t BUFF; /**<UART�շ�����Ĵ���*/
   __IO uint32_t ADR;  /**<485ͨ�ŵ�ַƥ��Ĵ���*/
   __IO uint32_t STT;  /**<UART״̬�Ĵ���*/
   __IO uint32_t IE;   /**<UART�ж�ʹ�ܼĴ���*/
   __IO uint32_t IF;   /**<UART�жϱ�־�Ĵ���*/
   __IO uint32_t INV;  /**<UART IO��תʹ��*/
} UART_TypeDef;

/** 
 *@brief UART��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
   uint32_t BaudRate;          /**<������*/
   uint8_t WordLength;         /**<���ݳ���   @see UART_WORDLENGTH_8b @see UART_WORDLENGTH_7b*/
   uint8_t StopBits;           /**<ֹͣλ���� @see UART_STOPBITS_1b   @see UART_STOPBITS_2b*/
   uint8_t FirstSend;          /**<�������ݴ�С������ @see UART_FIRSTSEND_LSB @see UART_FIRSTSEND_MSB*/
   UART_ParityMode ParityMode; /**<��żУ��,@see UART_ParityMode*/

   uint8_t MultiDropEna;       /**<ʹ��Multi-drop,0:Disable 1:Enable*/
   uint8_t Bit9Value;          /**<Multi-drop Masterģʽʱ,��9������λֵ ��Ϊһ�㲻�ڳ�ʼ����ʹ��*/

   uint16_t Match485Addr;      /**<����485ͨ��ʱ��ƥ���ַ*/
   /**@brief�ж�ʹ�ܼĴ��� 
    * @see UART_IRQEna_SendOver
    * @see UART_IRQEna_RcvOver
    * @see UART_IRQEna_SendBuffEmpty
    * @see UART_IRQEna_StopError
    * @see UART_IRQEna_CheckError
    */
   uint8_t IRQEna;             
   uint8_t RXD_INV;            /**<���յ�ƽȡ�� ENABLEȡ����DISABLE����*/
   uint8_t TXD_INV;            /**<���͵�ƽȡ�� ENABLEȡ����DISABLE����*/
} UART_InitTypeDef;
/**
 * ����0�ṹ�����ַ����
 */
#ifndef UART0
#define UART0               ((UART_TypeDef *) UART0_BASE)
#endif
/**
 * ����1�ṹ�����ַ����
 */
#ifndef UART1
#define UART1               ((UART_TypeDef *) UART1_BASE)
#endif

/**
 * ϵͳʱ�ӣ�UARTʹ��ϵͳʱ�ӷ�Ƶ��
 */
#ifndef UART_MCU_MCLK
#define UART_MCU_MCLK (96000000LL) 
#endif

#define UART_WORDLENGTH_8b 0  /**<8bit����λ*/
#define UART_WORDLENGTH_7b 1  /**<7bit����λ*/

#define UART_STOPBITS_1b 0    /**<��ֹͣλ*/
#define UART_STOPBITS_2b 1    /**<1bitֹͣλ*/

#define UART_FIRSTSEND_LSB 0  /**<�Ͷ��ȷ���*/
#define UART_FIRSTSEND_MSB 1  /**<�߶��ȷ���*/

/*�ж�ʹ�ܶ���*/
#define UART_IRQEna_SendOver BIT0      /**<ʹ�ܷ�������ж�*/
#define UART_IRQEna_RcvOver BIT1       /**<ʹ�ܽ�������ж�*/
#define UART_IRQEna_SendBuffEmpty BIT2 /**<ʹ�ܷ��ͻ��������ж�*/
#define UART_IRQEna_StopError BIT3     /**<ʹ��ֹͣλ����*/
#define UART_IRQEna_CheckError BIT4    /**<ʹ��У�����*/

/*DMA����ʹ�ܶ���*/
#define UART_IRQEna_TX_DMA_RE BIT5     /**<�������DMA����ʹ��*/
#define UART_IRQEna_RX_DMA_RE BIT6     /**<�������DMA����ʹ��*/
#define UART_IRQEna_TX_BUF_DMA_RE BIT7 /**<���ͻ�������DMA����ʹ��*/

/* �жϱ�־����*/
#define UART_IF_SendOver BIT0     /**<��������ж�*/
#define UART_IF_RcvOver BIT1      /**<��������ж�*/
#define UART_IF_SendBufEmpty BIT2 /**<���ͻ��������ж�*/
#define UART_IF_StopError BIT3    /**<ֹͣλ����*/
#define UART_IF_CheckError BIT4   /**<У����� */

void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
void UART_StructInit(UART_InitTypeDef *UART_InitStruct);

void UART_SendData(UART_TypeDef *UARTx, uint32_t n);
uint32_t UART_ReadData(UART_TypeDef *UARTx);

uint8_t UART_SendAddr_485(UART_TypeDef *UARTx, uint8_t n);
uint8_t UART_SendData_485(UART_TypeDef *UARTx, uint8_t n);

uint32_t UART_GetIRQFlag(UART_TypeDef *UARTx, uint32_t tempFlag);
void UART_ClearIRQFlag(UART_TypeDef *UARTx, uint32_t tempFlag);



#endif /*__lks32mc08x_UART_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
