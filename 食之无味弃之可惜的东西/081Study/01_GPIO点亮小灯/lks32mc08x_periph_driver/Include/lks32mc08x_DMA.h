 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_dma.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DMA������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2020��08��1�� <td>1.0     <td>Howlet     <td>����
 * </table>
 */
#ifndef __LKS32MC08x_DMA_H
#define __LKS32MC08x_DMA_H

#include "lks32mc08x_lib.h"

/** 
 *@brief DMA��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    /** 
     * @brief DMA �ж�ʹ��
     * @see DMA_TCIE    
     * @see DMA_TEIE      
     */  
    uint8_t DMA_IRQ_EN;               /**< DMA �ж�ʹ�� */
    /** 
     * @brief DMA���䷽��
     * @see PERI2MEMORY  �������ڴ����  
     * @see MEMORY2PERI  �ڴ����������     
     */   
    uint8_t DMA_DIR;               
    uint8_t DMA_CIRC;                 /**< DMA����ģʽ��ѭ��ģʽ�� ENABLE:ʹ�� �� DISABLE:ʧ��*/
    uint8_t DMA_PINC;                 /**< �����ַÿ�����Ƿ����,ENABLE:ʹ�� �� DISABLE:ʧ��*/
    uint8_t DMA_MINC;                 /**< �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ���, ENABLE:ʹ�� �� DISABLE:ʧ��*/
    /** 
     * @brief �������λ��
     * @see DMA_BYTE_TRANS     
     * @see DMA_HALFWORD_TRANS      
     * @see DMA_WORD_TRANS
     */   
    uint8_t DMA_PBTW;                 /* �������λ�� 0:byte, 1:half-word, 2:word */
    /** 
     * @brief �ڴ����λ��
     * @see DMA_BYTE_TRANS     
     * @see DMA_HALFWORD_TRANS      
     * @see DMA_WORD_TRANS
     */   
    uint8_t DMA_MBTW;                 /* �ڴ����λ�� 0:byte, 1:half-word, 2:word */
     /** 
     * @brief ͨ�� x ����Ӳ�� DMA ����ʹ��
     * @see DMA_CH0_ADC0_REQ_EN     ��DMAͨ��0ʹ�ã�  
     * @see DMA_CH0_TIMER0_REQ_EN   ��DMAͨ��0ʹ�ã�
     * @see DMA_CH0_TIMER1_REQ_EN   ��DMAͨ��0ʹ�ã� 
     * @see DMA_CH1_SPI_RX_REQ_EN   ��DMAͨ��1ʹ�ã�
     * @see DMA_CH1_MCPWM_REQ_EN    ��DMAͨ��1ʹ�ã�
     * @see DMA_CH1_TIMER2_REQ_EN   ��DMAͨ��1ʹ�ã�
     * @see DMA_CH2_UART0_REQ_EN    ��DMAͨ��2ʹ�ã�
     * @see DMA_CH2_SPI_TX_REQ_EN   ��DMAͨ��2ʹ�ã�
     * @see DMA_CH2_TIMER3_REQ_EN   ��DMAͨ��2ʹ�ã�  
     * @see DMA_CH3_UART1_REQ_EN    ��DMAͨ��3ʹ�ã� 
     * @see DMA_CH3_CAN_REQ_EN      ��DMAͨ��3ʹ�ã�
     * @see DMA_CH3_I2C_REQ_EN      ��DMAͨ��3ʹ�ã� 
     */    
    uint8_t DMA_REQ_EN;            
    uint16_t DMA_TIMES;               /**< DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 */
    uint8_t DMA_ROUND;                /**< DMA ͨ�� x �������� 1~255 */
    uint32_t DMA_CPAR;                /**< DMA ͨ�� x �����ַ */
    uint32_t DMA_CMAR;                /**< DMA ͨ�� x �ڴ��ַ */

} DMA_InitTypeDef;

/**
 * DAMͨ��0�ṹ�����ַ����
 */
#ifndef DMA_CH0
#define DMA_CH0             ((DMA_RegTypeDef *) DMA_BASE)
#endif
/**
 * DAMͨ��1�ṹ�����ַ����
 */
#ifndef DMA_CH1
#define DMA_CH1             ((DMA_RegTypeDef *) (DMA_BASE+0x10))
#endif
/**
 * DAMͨ��2�ṹ�����ַ����
 */
#ifndef DMA_CH2
#define DMA_CH2             ((DMA_RegTypeDef *) (DMA_BASE+0x20))
#endif
/**
 * DAMͨ��3�ṹ�����ַ����
 */
#ifndef DMA_CH3
#define DMA_CH3             ((DMA_RegTypeDef *) (DMA_BASE+0x30))
#endif

/** 
 *@brief DMA�Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t DMA_CCR;  /**< DMA ͨ�����üĴ���*/
    __IO uint32_t DMA_CTMS; /**< DMA ���ƼĴ���*/
    __IO uint32_t DMA_CPAR; /**< DMA �����ַ�Ĵ���*/
    __IO uint32_t DMA_CMAR; /**< DMA �ڴ��ַ�Ĵ���*/
} DMA_RegTypeDef;

#define DMA_TCIE                 BIT0       /**< DMA�ж�ʹ�ܶ��壬��������ж�ʹ�ܣ�����Ч */   
#define DMA_TEIE                 BIT1       /**< DMA�ж�ʹ�ܶ��壬�����ж�ʹ�ܣ�����Ч */

#define PERI2MEMORY              0          /**< DMA���˷����壬�������ڴ� */
#define MEMORY2PERI              1          /**< DMA���˷����壬�ڴ������� */

#define DMA_BYTE_TRANS           0          /**< DMA��������λ���壬����λ�� 0:byte */
#define DMA_HALFWORD_TRANS       1          /**< DMA��������λ���壬����λ�� 1:half-word */
#define DMA_WORD_TRANS           2          /**< DMA��������λ���壬����λ�� 2:word */ 

#define DMA_CH0_ADC0_REQ_EN      BIT0       /**< ����DMA����Դ���壬 Channel_0 ADC0 DMA����ʹ�� */
#define DMA_CH0_TIMER0_REQ_EN    BIT1       /**< ����DMA����Դ���壬 Channel_0 TIMER0 DMA����ʹ�� */
#define DMA_CH0_TIMER1_REQ_EN    BIT2       /**< ����DMA����Դ���壬 Channel_0 TIMER1 DMA����ʹ�� */ 
#define DMA_CH1_SPI_RX_REQ_EN    BIT0       /**< ����DMA����Դ���壬 Channel_1 SPI_RX DMA����ʹ�� */
#define DMA_CH1_MCPWM_REQ_EN     BIT1       /**< ����DMA����Դ���壬 Channel_1 MCPWM DMA����ʹ�� */
#define DMA_CH1_TIMER2_REQ_EN    BIT2       /**< ����DMA����Դ���壬 Channel_1 TIMER2 DMA����ʹ�� */
#define DMA_CH2_UART0_REQ_EN     BIT0       /**< ����DMA����Դ���壬 Channel_2 UART0 DMA����ʹ�� */
#define DMA_CH2_SPI_TX_REQ_EN    BIT1       /**< ����DMA����Դ���壬 Channel_2 SPI_TX DMA����ʹ�� */
#define DMA_CH2_TIMER3_REQ_EN    BIT2       /**< ����DMA����Դ���壬 Channel_2 TIMER3 DMA����ʹ�� */ 
#define DMA_CH3_UART1_REQ_EN     BIT0       /**< ����DMA����Դ���壬 Channel_3 UART1 DMA����ʹ�� */
#define DMA_CH3_CAN_REQ_EN       BIT1       /**< ����DMA����Դ���壬 Channel_3 SPI_CAN DMA����ʹ�� */
#define DMA_CH3_I2C_REQ_EN       BIT2       /**< ����DMA����Դ���壬 Channel_3 I2X DMA����ʹ�� */

#define DMA_CH0_FIF  BIT0    /**< DMA�жϱ�־���壬ͨ�� 0 ����жϱ�־*/
#define DMA_CH1_FIF  BIT1    /**< DMA�жϱ�־���壬ͨ�� 1 ����жϱ�־*/
#define DMA_CH2_FIF	 BIT2    /**< DMA�жϱ�־���壬ͨ�� 2 ����жϱ�־*/
#define DMA_CH3_FIF  BIT3    /**< DMA�жϱ�־���壬ͨ�� 3 ����жϱ�־*/
#define DMA_CH0_EIF  BIT4    /**< DMA�жϱ�־���壬ͨ�� 0 �����жϱ�־*/
#define DMA_CH1_EIF  BIT5    /**< DMA�жϱ�־���壬ͨ�� 1 �����жϱ�־*/
#define DMA_CH2_EIF  BIT6    /**< DMA�жϱ�־���壬ͨ�� 2 �����жϱ�־*/
#define DMA_CH3_EIF  BIT7    /**< DMA�жϱ�־���壬ͨ�� 3 �����жϱ�־*/

void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct);
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct);
uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if);
void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag);
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN);
#endif
