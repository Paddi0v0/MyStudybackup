/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_tmp.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� оƬ�����¶ȴ���������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��3��9��  <td>1.0     <td>Zhu Jie     <td>�޸�
 * </table>
 */
#ifndef _LKS32MC07x_TMP_H_
#define _LKS32MC07x_TMP_H_

#include "lks32mc07x.h"
#include "basic.h"

/** 
 *@brief оƬ�����¶ȴ�������ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
   u16 nCofA;      /**< �¶�ϵ��A����ԴRead_Trim(0x00000398)���ֵ*/
   u16 nOffsetB;   /**< �¶�ϵ��ƫ�ã���ԴRead_Trim(0x0000039C)���ֵ*/
} Stru_TempertureCof_Def;


extern void TempSensor_Init(void);
extern s16 GetCurrentTemperature(s16 ADC_value);

#endif /*_CONNIE_TMP_H_ */



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
