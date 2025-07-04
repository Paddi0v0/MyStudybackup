/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_dac.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DAC������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��3��22�� <td>1.0     <td>Olive Wang    <td>����
 * </table>
 */ 
#ifndef _LKS32MC07X_DAC_H_
#define _LKS32MC07X_DAC_H_
/* Includes ------------------------------------------------------------------*/

#include "lks32mc07x_lib.h"

typedef struct
{
	    /**
     * @brief DAC�����λ��1��1.2V��0��4.85V
     * @see DAC_RANGE_1V2
     * @see DAC_RANGE_4V85
     */
	uint8_t DAC_GAIN;   /**< DAC�����λ��1:1.2V��0:4.85V  */	             
	uint8_t DACOUT_EN;  /**< DAC���ʹ�ܣ�DISABLE����ʹ�ܣ�ENABLE��ʹ�� */
	int8_t  DAC_STEP;   /**< ����ֵ�� 3bit �з���������Χ-4~3*/
	uint8_t TIG_CH_EN;  /**< UTIMER0/1/2/3��������ʹ��,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/

} DAC_InitTypeDef;

/* ------------------------------DAC������ض��� ------------------------------ */
#define DAC_RANGE_4V85    0      	/**< DAC 4.85V���� */
#define DAC_RANGE_1V2     1      	/**< DAC 1.2V���� */
 
#define DAC_Channel_0    0			  	/**< DAC ͨ��0 */
#define DAC_Channel_1    1			 		/**< DAC ͨ��1*/

#define  DAC0_UTIMER0_TRG_EN    BIT0  /**< TIMER0 �����¼����� DAC0 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC0_UTIMER1_TRG_EN    BIT1	 /**< TIMER1 �����¼����� DAC0 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC0_UTIMER2_TRG_EN    BIT2  /**< TIMER2 �����¼����� DAC0 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC0_UTIMER3_TRG_EN    BIT3  /**< TIMER3 �����¼����� DAC0 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 

#define  DAC1_UTIMER0_TRG_EN    BIT8  /**< TIMER0 �����¼����� DAC1 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC1_UTIMER1_TRG_EN    BIT9  /**< TIMER1 �����¼����� DAC1 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC1_UTIMER2_TRG_EN    BIT10 /**< TIMER2 �����¼����� DAC1 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 
#define  DAC1_UTIMER3_TRG_EN    BIT11 /**< TIMER3 �����¼����� DAC1 ����ʹ�ܣ�����Ч,DISABLE����ʹ�ܣ�ENABLE��ʹ��*/ 

void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct);										/* DAC�ṹ���ʼ�� */
void DAC_Init(uint8_t DAC_Channel,DAC_InitTypeDef* DAC_InitStruct);      /* DAC��ʼ�� */
void DAC_OutputValue(uint8_t DAC_Channel,uint32_t DACValue);             /* DAC�����ֵ����--������ */
void DAC_OutputVoltage(uint8_t DAC_Channel,uint16_t DACVoltage);         /* DAC���ģ������ֵ����--ģ���� */
void DAC_Cmd(uint8_t DAC_Channel,FuncState state);												/* DACͨ��ʱ��ʹ��*/

#endif /*__lks32mc07x_OPA_H */
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/



