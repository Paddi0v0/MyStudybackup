 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_opa.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� OPA����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��06��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
#ifndef __LKS32MC08x_OPA_H
#define __LKS32MC08x_OPA_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

typedef enum
{
   OPA0 = 0, /**< OPA0ģ��*/
   OPA1 = 1, /**< OPA1ģ��*/
   OPA2 = 2, /**< OPA2ģ��*/
   OPA3 = 3  /**< OPA3ģ��*/
}enumOPA;

/** 
 *@brief OPA��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
       /** 
     * @brief �˷űջ����� \n
     * @see PGA_GAIN_20   \n  
     * @see PGA_GAIN_9P5  \n
     * @see PGA_GAIN_6    \n
     * @see PGA_GAIN_4P25           
     */  
   uint32_t OPA_Gain;      
   uint32_t OPA_CLEna;      /**<�˷űջ�ʹ��:Enable,Disable*/
}OPA_InitTypeDef;

/* ------------------------------PGA������ض��� ------------------------------ */
#define PGA_GAIN_20                    0x00                   /**< ��������200:10.2 */
#define PGA_GAIN_9P5                   0x01                   /**< ��������190:20.2 */
#define PGA_GAIN_6                     0x02                   /**< ��������180:30.4 */
#define PGA_GAIN_4P25                  0x03                   /**< ��������170:40.4 */                                                                                 

void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct);
void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct);
void OPA_OUT(enumOPA opa, FuncState state);

#endif /*__lks32mc08x_OPA_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
