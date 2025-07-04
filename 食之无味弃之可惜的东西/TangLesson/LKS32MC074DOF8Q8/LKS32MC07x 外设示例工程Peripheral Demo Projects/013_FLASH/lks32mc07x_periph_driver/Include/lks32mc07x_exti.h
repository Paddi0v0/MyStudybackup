 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_exti.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� EXTI�ⲿ�ж�����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��28�� <td>1.0       <td>cakezhang        <td>����
 * </table>
 */
#ifndef __lks32mc07x_EXTI_H
#define __lks32mc07x_EXTI_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x.h"
#include "basic.h"
#include "lks32mc07x_gpio.h"

/** 
 *@brief EXTI�Ĵ����ṹ����
 */
typedef struct  
{
   __IO uint32_t CR0;
   __IO uint32_t CR1;
   __IO uint32_t IE;
   __IO uint32_t IF;   
}EXTI_TypeDef;


 /**
 * EXTI�ṹ�����ַ����
 */
#ifndef EXTI
#define EXTI             ((EXTI_TypeDef *) EXTI_BASE)
#endif

 /**
 * EXTI�����źŶ���
 */
typedef enum
{
   EXTI_Trigger_None    = 0x00, /**< ������*/
   EXTI_Trigger_Negedge = 0x01, /**< �½��ش���*/
   EXTI_Trigger_Posedge = 0x02, /**< �����ش���*/
   EXTI_Trigger_Edge    = 0x03  /**< �����ء��½��ؾ�����*/
} EXTIMode_TypeDef;




uint32_t EXTI_ReadIFFlag(EXTI_TypeDef* EXTIx);
void EXTI_ResetIF(EXTI_TypeDef* EXTIx, uint32_t temp);

void EXTI_Trigger_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint16_t EXTI_Trigger);
void EXTI_ClearIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource);
u8 EXTI_GetIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource);

#endif /*__lks32mc07x_EXTI_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
