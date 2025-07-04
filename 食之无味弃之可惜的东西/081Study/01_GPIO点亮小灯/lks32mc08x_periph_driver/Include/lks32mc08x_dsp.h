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
    s32 Dividend;  /**< ������ */
    s16 Divisor;   /**< ����   */
    s32 Quotient;  /**< ��     */
    s16 Remainder; /**< ����   */
} stru_DiviComponents;

/**
 *@brief ���Ǻ�������ṹ����
 */
typedef struct
{
    s16 Theta;       /**< �Ƕ�-32768~32767��Ӧ-(��)~��    */
    s16 SinValue;    /**< ����õ�sinֵ                 */
    s16 CosValue;    /**< ����õ�cosֵ                 */
    s16 ArctanX;     /**< Arctan�����ӦXֵ arctan =Y/X */
    s16 ArctanY;     /**< Arctan�����ӦYֵ arctan =Y/X */
    s16 ArctanValue; /**< ����õ�arctanֵ              */
} stru_CordicComponents;

typedef enum
{
    ARCTAN = 0, /**< 0����arctan*/
    SINCOS = 1  /**< 1����sin cos*/
} Arc_or_SinCFuction;

void DSP_Cmd(FuncState state);                                                 /* DSPʹ�ܺ͹ر� */
void DSP_CalcDivision(stru_DiviComponents *stru_Divi);                         /* DSP�������� */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise); /* DSP���Ǻ����������� */
uint32_t DSP_GetSqrt(uint32_t Data);                                           /* DSPȡ�ÿ�����*/
s16 DSP_CalcSprt(s16 X, s16 Y);                                                /* DSP���㿪����*/

void DSP_Init(void);
s16 DSP_arctan(s16 x, s16 y);
s32 DSP_div(s32 a, s32 b);
s16 DSP_sin(u16 angle);
s16 DSP_cos(u16 angle);
u16 DSP_rms(s16 a, s16 b);
s16 DSP_mod(s32 x, s16 y);
u16 DSP_sqrt(u32 val);

// ����DSP_CALCϵ�еĽӿڣ��Խ��Ͳ������ݴ�����ʱ�����
#define DSP_CALC_SIN(sin, angle)       \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_SC = BIT2 | BIT1;          \
        DSP_THETA = (u16)angle;        \
        sin = DSP_SIN;                 \
        __set_PRIMASK(flg);            \
    }
#define DSP_CALC_COS(cos, angle)       \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_SC = BIT2 | BIT1;          \
        DSP_THETA = (u16)angle;        \
        cos = DSP_COS;                 \
        __set_PRIMASK(flg);            \
    }
#define DSP_CALC_RMS(rms, a, b)        \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_DID = a;                   \
        DSP_DIS = b;                   \
        rms = DSP_REM;                 \
        __set_PRIMASK(flg);            \
    }

#define DSP_CALC_ARCTAN(arctan, x, y)  \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_SC = BIT1;                 \
        DSP_Y = y;                     \
        arctan = DSP_ARCTAN;           \
        __set_PRIMASK(flg);            \
    }
#define DSP_CALC_DIV(c, a, b)           \
    {                                   \
        int32_t _flg = __get_PRIMASK(); \
        __disable_irq();                \
        DSP_DID = (a);                  \
        DSP_DIS = (b);                  \
        c = DSP_QUO;                    \
        __set_PRIMASK(_flg);            \
    }
#define DSP_CALC_MOD(mod, x, y)        \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_SC = BIT1;                 \
        DSP_X = x;                     \
        DSP_Y = y;                     \
        mod = DSP_MOD;                 \
        __set_PRIMASK(flg);            \
    }
#define DSP_CALC_SQRT(sqrt, val)       \
    {                                  \
        int32_t flg = __get_PRIMASK(); \
        __disable_irq();               \
        DSP_RAD = val;                 \
        sqrt = DSP_SQRT;               \
        __set_PRIMASK(flg);            \
    }

#endif

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
