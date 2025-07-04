 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_dma.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DMA������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
 
#ifndef __lks32mc07x_dma_H
#define __lks32mc07x_dma_H

#include "lks32mc07x_lib.h"

/** 
 *@brief DMAͨ����ؼĴ���������
 */
typedef struct
{
    __IO uint32_t CCR;  /**< DMA ͨ�����üĴ���*/
    __IO uint32_t REN;/**< DMA ����ʹ�ܼĴ���*/
    __IO uint32_t CTMS;/**< DMA����������üĴ���*/
    __IO uint32_t SADR;/**< DMA Դ��ַ���üĴ���*/
    __IO uint32_t DADR;/**< DMA Ŀ�ĵ�ַ����*/
} DMA_RegTypeDef;

/** 
 *@brief DMA����״̬�Ĵ���������
 */
typedef struct
{
    __IO u32 CTRL;/**< DMA ���ƼĴ���*/
    __IO u32 IE;/**< DMA �ж�ʹ�ܼĴ���*/
    __IO u32 IF;/**< DMA �жϱ�־�Ĵ���*/
} DMA_MODULERegTypeDef;

/**
 * DAMͨ��0 1 2 3�ṹ�����ַ����
 */
#define DMA_CHN0 ((DMA_RegTypeDef *) 0x40011200)
#define DMA_CHN1 ((DMA_RegTypeDef *) (DMA0_BASE + 0x20))
#define DMA_CHN2 ((DMA_RegTypeDef *) (DMA0_BASE + 0x40))
#define DMA_CHN3 ((DMA_RegTypeDef *) (DMA0_BASE + 0x60))
#define DMA_TOP ((DMA_MODULERegTypeDef *)(DMA0_BASE + 0x80))


/**
 * DAM����ʹ��Դ����
 */
typedef union
{
    u32 R;
    struct
    {
        u16 ADC0_EN         :1 ;    // BIT0
        u16 ADC1_EN         :1 ;    // BIT1
        u16                 :1 ;    // BIT2
        u16 DSP_EN          :1 ;    // BIT3
        u16 TIMER0_EN       :1 ;    // BIT4
        u16 TIMER1_EN       :1 ;    // BIT5
        u16 TIMER2_EN       :1 ;    // BIT6
        u16 TIMER3_EN       :1 ;    // BIT7
        u16                 :1 ;    // BIT8
        u16                 :1 ;    // BIT9
        u16 SPI_RX_EN       :1 ;    // BIT10
        u16 SPI_TX_EN       :1 ;    // BIT11
        u16 MCPWM_RE0_EN    :1 ;    // BIT12
        u16 MCPWM_RE1_EN    :1 ;    // BIT13
        u16 I2C0_RX_EN      :1 ;    // BIT14
        u16 I2C0_TX_EN      :1 ;    // BIT15
        u16 UART0_RX_EN     :1 ;    // BIT16
        u16 UART0_TX_EN     :1 ;    // BIT17
        u16 UART1_RX_EN     :1 ;    // BIT18
        u16 UART1_TX_EN     :1 ;    // BIT19
        u16                 :1 ;    // BIT20
        u16                 :1 ;    // BIT21
        u16                 :1 ;    // BIT22
        u16                 :1 ;    // BIT23
        u16 HALL0_EN        :1 ;    // BIT24
        u16 SIF_EN          :1 ;    // BIT25
        u16 CMP_EN          :1 ;    // BIT26
        u16                 :1 ;    // BIT27
        u16                 :1 ;    // BIT28
        u16 GPIO_EN         :1 ;    // BIT29
        u16                 :1 ;    // BIT30
        u16 SW_TRIG_EN      :1 ;    // BIT31
    } B               ;   
} stru_DMA_REN;

/** 
 *@brief DMA��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
    u8  IRQ_EN          ;
    u8  SBTW            ;   // Դ��ַ����λ�� 0: Byte 1: Halfword 2: Word
    u8  DBTW            ;   // Ŀ�ĵ�ַ����λ��  0: Byte  1: Halfword  2: Word
    u8  SINC            ;   // Դ��ַ����ʹ��
    u8  DINC            ;   // Ŀ�ĵ�ַ����ʹ��
    u8  CIRC            ;   // ѭ��ģʽʹ��
    u8  RMODE           ;   // ���ִ���ʹ�ܣ����DMA ������ɴ���
    u8  EN              ;   // ͨ��ʹ��
    u8  TMS             ;   // DMA ͨ�� x ���ݰ��˴�����
    u32 SADR            ;   // DMA ͨ�� x Դ��ַ
    u32 DADR            ;   // DMA ͨ�� x Ŀ�ĵ�ַ
    stru_DMA_REN REN    ;   // DMA ����ʹ��
} DMA_InitTypeDef;


#define DMA_TCIE                 BIT0       /* ��������ж�ʹ�ܣ�����Ч */   
#define DMA_TEIE                 BIT1       /* �����ж�ʹ�ܣ�����Ч */

#define PERI2MEMORY              0          /* �������ڴ� */
#define MEMORY2PERI              1          /* �ڴ������� */

#define DMA_BYTE_TRANS           0          /* ����λ�� 0:byte */
#define DMA_HALFWORD_TRANS       1          /* ����λ�� 1:half-word */
#define DMA_WORD_TRANS           2          /* ����λ�� 2:word */ 

/** 
 *@brief DMA���˴���Դλ�궨��
 */
#define DMA_REQ_SW BIT31        /*  SW        ����*/
#define DMA_REQ_GPIO BIT29      /*  GPIO      ����*/  
#define DMA_REQ_CMP BIT26       /*  CMP       ����*/
#define DMA_REQ_SIF BIT25       /*  HALL1     ����*/
#define DMA_REQ_HALL BIT24      /*  HALL0     ����*/  
#define DMA_REQ_UART1_TX BIT1   /*  UART1_TX  ����*/
#define DMA_REQ_UART1_RX BIT1   /*  UART1_RX  ����*/
#define DMA_REQ_UART0_TX BIT1   /*  UART0_TX  ����*/
#define DMA_REQ_UART0_RX BIT1   /*  UART0_RX  ����*/
#define DMA_REQ_I2C0_TX BIT15   /*  I2C0_TX   ����*/
#define DMA_REQ_I2C0_RX BIT14   /*  I2C0_RX   ����*/
#define DMA_REQ_MCPWM1 BIT13    /*  MCPWM1    ����*/
#define DMA_REQ_MCPWM0 BIT12    /*  MCPWM0    ����*/
#define DMA_REQ_SPI_TX BIT11    /*  SPI1_TX   ����*/
#define DMA_REQ_SPI_RX BIT10    /*  SPI1_RX   ����*/
#define DMA_REQ_TIMER4 BIT7     /*  TIMER4    ����*/
#define DMA_REQ_TIMER3 BIT6     /*  TIMER3    ����*/
#define DMA_REQ_TIMER2 BIT5     /*  TIMER2    ����*/
#define DMA_REQ_TIMER1 BIT4     /*  TIMER1    ����*/
#define DMA_REQ_TIMER0 BIT3     /*  TIMER0    ����*/
#define DMA_REQ_DSP BIT2        /*  ADC2      ����*/
#define DMA_REQ_ADC1 BIT1       /*  ADC1      ����*/
#define DMA_REQ_ADC0 BIT0       /*  ADC0      ����*/

/** 
 *@brief DMA�жϱ�־λMASK
 */
#define DMA_IF_CH0 BIT0
#define DMA_IF_CH1 BIT1
#define DMA_IF_CH2 BIT2
#define DMA_IF_CH3 BIT3

/** 
 *@brief DMA�ж�ʹ��λMASK
 */
#define DMA_IE_CH0 BIT0
#define DMA_IE_CH1 BIT1
#define DMA_IE_CH2 BIT2
#define DMA_IE_CH3 BIT3


void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct);
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct);
u32 DMA_GetIRQFlag(u32 DMA_IF_x);
void DMA_ClearIRQFlag(u32 DMA_IF_x);
u32 DMA_GetFlag(u32 DMA_IF_x);


void DMA_Enable(void);
void DMA_Disable(void);
void DMA_SetAddr(DMA_RegTypeDef *DMA_CHx, u32 addr1, u32 addr2);
void DMA_SetLength(DMA_RegTypeDef *DMA_CHx, u32 len);
void DMA_Trigger(DMA_RegTypeDef *DMA_CHx);
void DMA_SetRequest(DMA_RegTypeDef *DMA_CHx, u32 DMA_REQ_x);
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN);

#endif /*__lks32mc08x_dma_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/

