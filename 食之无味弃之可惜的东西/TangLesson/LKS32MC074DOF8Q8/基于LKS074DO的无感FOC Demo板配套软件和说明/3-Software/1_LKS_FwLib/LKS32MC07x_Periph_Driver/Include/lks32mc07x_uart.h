/**
 * @file 
 * @copyright (C)2021, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_uart.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� UART����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��23��   <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
#ifndef __lks32mc07x_UART_H
#define __lks32mc07x_UART_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
#include "basic.h"

/** ������żУ������ö�ٳ�Ա*/ 
typedef enum
{
   UART_Parity_NO   = 0x00,
   UART_Parity_EVEN = 0x01,
   UART_Parity_ODD  = 0x02
}UART_ParityMode;

/** 
 *@brief UART�Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t CTRL;     /**<UART���ƼĴ���*/
    __IO uint32_t DIVH;     /**<UART���������ø��ֽڼĴ���*/
    __IO uint32_t DIVL;     /**<UART���������õ��ֽڼĴ���*/
    __IO uint32_t BUFF;     /**<UART�շ�����Ĵ���*/
    __IO uint32_t ADR ;     /**<485ͨ�ŵ�ַƥ��Ĵ���*/
    __IO uint32_t STT ;     /**<UART״̬�Ĵ���*/
    __IO uint32_t RE  ;     /**<UART DMA ����ʹ�ܼĴ���*/
    __IO uint32_t IE  ;     /**<UART�ж�ʹ�ܼĴ���*/
    __IO uint32_t IF  ;     /**<UART�жϱ�־�Ĵ���*/
    __IO uint32_t IOC ;     /**<UART IO��תʹ��*/
}UART_TypeDef;

/**
 * ����0�ṹ�����ַ����
 */
#define UART0            ((UART_TypeDef *) UART0_BASE)

/**
 * ����1�ṹ�����ַ����
 */
#define UART1            ((UART_TypeDef *) UART1_BASE)

typedef struct
{
   uint32_t BaudRate;           /**<������*/
   /**@brief���ݳ���
    * @see UART_WORDLENGTH_8b
    * @see UART_WORDLENGTH_9b
    */
   uint8_t WordLength;         
    /**@briefֹͣλ����
    * @see UART_STOPBITS_1b
    * @see UART_STOPBITS_2b
    */
   uint8_t StopBits;           
   /**@brief�������ݴ�С������
    * @see UART_FIRSTSEND_LSB
    * @see UART_FIRSTSEND_MSB
    */
   uint8_t FirstSend;          
   /**@brief��żУ��
    * @see UART_ParityMode
    */
   UART_ParityMode ParityMode;  

   uint8_t MultiDropEna;       /**< ʹ��Multi-drop,0:Disable 1:Enable*/
   uint8_t Bit9Value;          /**< Multi-drop Masterģʽʱ,��9������λֵ*/
   uint8_t IOFlipEna;          /**< ʹ��IO��ת,0:Disable 1:Enable*/

   uint16_t Match485Addr;      /**< ����485ͨ��ʱ��ƥ���ַ*/
   uint8_t IRQEna;             /**< �ж�ʹ�ܼĴ���*/
   uint8_t DMARE;              /**< DMA ����ʹ��*/   
	 uint8_t RXD_INV;            /**< ���յ�ƽȡ�� */
	 uint8_t TXD_INV;            /**< ���͵�ƽȡ�� */	
}UART_InitTypeDef;

#define UART_WORDLENGTH_8b                  0      /**<8bit����λ*/
#define UART_WORDLENGTH_9b                  1      /**<9bit����λ*/

#define UART_STOPBITS_1b                    0       /**<��ֹͣλ*/
#define UART_STOPBITS_2b                    1       /**<1bitֹͣλ*/

#define UART_FIRSTSEND_LSB                  0       /**<�Ͷ��ȷ���*/
#define UART_FIRSTSEND_MSB                  1       /**<�߶��ȷ���*/

/*�ж�ʹ�ܶ���*/
#define UART_IRQEna_SendOver                BIT0            /**<ʹ�ܷ�������ж�*/
#define UART_IRQEna_RcvOver                 BIT1            /**<ʹ�ܽ�������ж�*/
#define UART_IRQEna_SendBuffEmpty           BIT2            /**<ʹ�ܷ��ͻ��������ж�*/
#define UART_IRQEna_StopError               BIT3            /**<ʹ��ֹͣλ����*/
#define UART_IRQEna_CheckError              BIT4            /**<ʹ��У�����*/
#define UART_IRQEna_TX_OV                   BIT5            /**<���ͻ���������ж�ʹ��*/
#define UART_IRQEna_RX_OV                   BIT6            /**<���ջ���������ж�ʹ��*/
#define UART_IRQEna_LBD                     BIT7            /**<LIN break character ����ж�ʹ��*/
#define UART_IRQEna_IDLE                    BIT8            /**<����֡�ж�ʹ��*/

#define TX_DMA_RE               BIT0            /*�������DMA����ʹ��*/
#define RX_DMA_RE               BIT1            /*�������DMA����ʹ��*/
#define TX_BUF_DMA_RE           BIT2            /*���ͻ�������DMA����ʹ��*/


/*�жϱ�־����*/
#define UART_IF_SendOver                    BIT0            /**<��������жϱ�־*/
#define UART_IF_RcvOver                     BIT1            /**<��������жϱ�־*/
#define UART_IF_SendBufEmpty                BIT2            /**<���ͻ��������жϱ�־*/
#define UART_IF_StopError                   BIT3            /**<ֹͣλ�����жϱ�־*/
#define UART_IF_CheckError                  BIT4            /**<У������жϱ�־ */
#define UART_IF_TX_OV                       BIT5            /**<���ͻ���������жϱ�־*/
#define UART_IF_RX_OV                       BIT6            /**<���ջ���������жϱ�־*/
#define UART_IF_LBD                         BIT7            /**<LIN break character ����жϱ�־*/
#define UART_IF_IDLE                        BIT8            /**<����֡�жϱ�־*/


void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);

void UART_SendData(UART_TypeDef *UARTx, uint32_t n);
uint32_t UART_ReadData(UART_TypeDef *UARTx);

void UART_SendnData(UART_TypeDef *UARTx, char *sendData, uint32_t len);
void UART_RecvnData(UART_TypeDef *UARTx, char *recvData, uint32_t maxlen);


void UART_ClearIRQFlag(UART_TypeDef *UARTx, uint32_t tempFlag);

uint32_t UART_GetIRQFlag(UART_TypeDef *UARTx , uint32_t tempFlag);

uint8_t UART_SendAddr_485(UART_TypeDef *UARTx, uint8_t n);
uint8_t UART_SendData_485(UART_TypeDef *UARTx, uint8_t n);

#endif /*__lks32mc07x_UART_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
