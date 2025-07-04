 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_opa.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� OPA����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */ 
#ifndef __LKS32MC07x_OPA_H
#define __LKS32MC07x_OPA_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc07x_lib.h"

typedef enum
{
   OPA0 = 0,   /**< OPA0ģ��*/
   OPA1 = 1,   /**< OPA1ģ��*/
   OPA2 = 2,   /**< OPA2ģ��*/
   OPA3 = 3    /**< OPA3ģ��*/ 
}enumOPA;

/** 
 *@brief OPA��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
	       /** 
     * @brief �˷�ƫ�õ������� \n
     * @see PGA_IT_1   \n  
     * @see PGA_IT_1P2  \n
     * @see PGA_IT_1P5    \n
     * @see PGA_IT_DISABLE           
     */ 
	 uint32_t OPA_IT;					/**< �˷�ƫ�õ�������*/
	       /** 
     * @brief �˷űջ����� \n
     * @see PGA_GAIN_32   \n  
     * @see PGA_GAIN_16  \n
     * @see PGA_GAIN_8    \n
     * @see PGA_GAIN_4           
     */ 
   uint32_t OPA_Gain;       /**< �˷űջ�����*/
   uint32_t OPA_CLEna;      /**< �˷űջ�ʹ��:Enable,Disable*/
}OPA_InitTypeDef;

/* ------------------------------PGA������ض��� ------------------------------ */
#define PGA_GAIN_32                    0x00                   /**< ��������320k:10k */
#define PGA_GAIN_16                    0x01                   /**< ��������160k:10k */
#define PGA_GAIN_8                     0x02                   /**< ��������80k:10k */
#define PGA_GAIN_4                     0x03                   /**< ��������40k:10k */                                                                                 

#define PGA_IT_1                       0x00         			/**< opaƫ�õ�������x1*/        
#define PGA_IT_1P2                     0x01        			/**< opaƫ�õ�������x1.2*/         
#define PGA_IT_1P5                     0x02        			/**< opaƫ�õ�������x1.5*/         
#define PGA_IT_DISABLE                 0x03        			/**< opaƫ�õ������ڽ�ֹ*/                                                                                    

void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct);	/*opa��ʼ��*/
void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct);	/*opa�ṹ���ʼ��*/
void OPA_OUT(enumOPA opa, FuncState state);	/*opa�����IO����*/

#endif /*__lks32mc07x_OPA_H */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
