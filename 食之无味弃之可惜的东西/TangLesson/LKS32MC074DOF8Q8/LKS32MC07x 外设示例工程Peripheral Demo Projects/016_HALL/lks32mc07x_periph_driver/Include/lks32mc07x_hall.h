/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_hall.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� HALL����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��    <td>1.0  <td>Olive Wang     <td>����
 * </table>
 */
#ifndef __lks32mc07x_HALL_H
#define __lks32mc07x_HALL_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
#include "basic.h"

/** 
 *@brief HALL�Ĵ����ṹ����
 */
typedef struct
{
    __IO uint32_t CFG;			/**< HALL ģ�����üĴ��� */
    __IO uint32_t INFO;     /**< HALL ģ����Ϣ�Ĵ��� */
    __IO uint32_t WIDTH;    /**< HALL ��ȼ���ֵ�Ĵ���*/
    __IO uint32_t TH;       /**< HALL ģ�����������ֵ�Ĵ���*/
    __IO uint32_t CNT;      /**< HALL �����Ĵ���*/
}HALL_TypeDef;
/**
 * HALL�ṹ�����ַ����
 */
#ifndef HALL
#define HALL     ((HALL_TypeDef *) HALL0_BASE)
#endif
/** 
 *@brief HALL��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{   
  	uint32_t CountTH;  								/**< HALL����������ֵ����ЧλΪ24λ*/
    uint16_t FilterLen;               /**< �˲�����,0��Ӧ����1,1023��Ӧ����1024�˲�����*/
    uint8_t ClockDivision;            /**< ��Ƶ 0~3:/1 /2 /4 /8*/
    uint8_t Filter75_Ena;             /**< ʹ�ܵ�һ��7/5�˲�,�ߵ�ƽ��Ч,ENABLE��ʹ�� DISABLE��ʧ��*/
    uint8_t HALL_Ena;                 /**< ʹ��HALL,�ߵ�ƽ��Ч,ENABLE��ʹ�� DISABLE��ʧ��*/
    uint8_t Capture_IRQ_Ena;          /**< HALL�źű仯�ж�ʹ��,�ߵ�ƽ��Ч,ENABLE��ʹ�� DISABLE��ʧ��*/
    uint8_t OverFlow_IRQ_Ena;         /**< HALL����������ж�ʹ��,�ߵ�ƽ��Ч,ENABLE��ʹ�� DISABLE��ʧ��*/          
    uint8_t softIE;                   /**< ����ж�ʹ�� ,ENABLE��ʹ�� DISABLE��ʧ��*/
}HALL_InitTypeDef;

#define HALL_CLK_DIV1 ((uint32_t)0x00)  /**< HALLʱ��Ϊϵͳ��ʱ��1��Ƶ*/
#define HALL_CLK_DIV2 ((uint32_t)0x01)		/**< HALLʱ��Ϊϵͳ��ʱ��2��Ƶ*/
#define HALL_CLK_DIV4 ((uint32_t)0x02)		/**< HALLʱ��Ϊϵͳ��ʱ��4��Ƶ*/
#define HALL_CLK_DIV8 ((uint32_t)0x03)		/**< HALLʱ��Ϊϵͳ��ʱ��8��Ƶ*/
  
#define HALL_CAPTURE_EVENT  ((uint32_t)0x00010000)  /**< HALL�ж�ʹ�ܺͱ�־���壬HALL�źű仯�ж�*/
#define HALL_OVERFLOW_EVENT ((uint32_t)0x00020000)  /**< HALL�ж�ʹ�ܺͱ�־���壬HALL��������ж�*/

void HALL_StructInit(HALL_InitTypeDef* HALL_InitStruct);/*HALL�ṹ���ʼ��*/
void HALL_Init(HALL_InitTypeDef* HALL_InitStruct);/*HALL��ʼ��*/
void HALL_Cmd(FuncState state);/*HALLʹ��*/

uint32_t HALL_GetFilterValue(void);/*��ȡ�˲���HALLֵ*/
uint32_t HALL_GetCaptureValue(void);/*��ȡδ�˲���HALLֵ*/
uint32_t HALL_GetCount(void);/*��ȡHALL����ֵ*/

uint32_t HALL_IsCaptureEvent(void);/*�ж��Ƿ񲶻��¼�*/
uint32_t HALL_IsOverFlowEvent(void);/*�ж��Ƿ�����¼�*/
uint32_t HALL_GetFlag(uint32_t tempFlag);/*ȡ��HALL�¼���־*/

void HALL_Clear_IRQ(uint32_t tempFlag);/*���HALL�жϱ�־λ*/
uint32_t HALL_GetIRQFlag(uint32_t tempFlag);/*��ȡHALL�жϱ�־λ*/

void HALL_SoftTrigger(void);/*�������HALL�źű仯�ж�*/

#endif /*__lks32mc07x_HALL_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
