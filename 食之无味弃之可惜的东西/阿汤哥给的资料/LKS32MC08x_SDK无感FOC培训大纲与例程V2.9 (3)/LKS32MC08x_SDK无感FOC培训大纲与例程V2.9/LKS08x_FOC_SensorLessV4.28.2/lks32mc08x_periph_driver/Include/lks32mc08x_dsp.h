 /**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_dsp.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DSP����ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2018��07��25�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */

#ifndef __LKS32MC08x_DSP_H
#define __LKS32MC08x_DSP_H


/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/** 
 *@brief ��������ṹ����
 */
typedef struct
{
	  s32 Dividend;     /**< ������ */
	  s16 Divisor;      /**< ����   */
      s32 Quotient;     /**< ��     */
      s16 Remainder;    /**< ����   */
} stru_DiviComponents;

/** 
 *@brief ���Ǻ�������ṹ����
 */
typedef struct
{
	  s16 Theta;          /**< �Ƕ�-32768~32767��Ӧ-(��)~��    */
      s16 SinValue;       /**< ����õ�sinֵ                 */
      s16 CosValue;       /**< ����õ�cosֵ                 */
	  s16 ArctanX;        /**< Arctan�����ӦXֵ arctan =Y/X */
	  s16 ArctanY;        /**< Arctan�����ӦYֵ arctan =Y/X */
	  s16 ArctanValue;    /**< ����õ�arctanֵ              */
} stru_CordicComponents;

typedef enum 
{
	ARCTAN = 0,  /**< 0����arctan*/
    SINCOS = 1   /**< 1����sin cos*/
} Arc_or_SinCFuction;

void DSP_Cmd(FuncState state);                          /* DSPʹ�ܺ͹ر� */
void DSP_CalcDivision(stru_DiviComponents *stru_Divi);  /* DSP�������� */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise);  /* DSP���Ǻ����������� */
uint32_t DSP_GetSqrt(uint32_t Data); /* DSPȡ�ÿ�����*/
s16 DSP_CalcSprt(s16 X,s16 Y);  /* DSP���㿪����*/

#endif 



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
