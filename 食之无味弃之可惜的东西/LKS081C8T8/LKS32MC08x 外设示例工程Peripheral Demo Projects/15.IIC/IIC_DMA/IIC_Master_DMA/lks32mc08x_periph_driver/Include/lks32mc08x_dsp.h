/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief 文件名称： lks32mc08x_dsp.h\n
 * 文件标识： 无 \n
 * 内容摘要： DSP驱动头文件 \n
 * 其它说明： 无 \n
 *@par 修改日志:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2018年07月25日 <td>1.0     <td>Howlet      <td>创建
 * </table>
 */

#ifndef __LKS32MC08x_DSP_H
#define __LKS32MC08x_DSP_H

/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/**
 *@brief 除法运算结构体句柄
 */
typedef struct
{
    s32 Dividend;  /**< 被除数 */
    s16 Divisor;   /**< 除数   */
    s32 Quotient;  /**< 商     */
    s16 Remainder; /**< 余数   */
} stru_DiviComponents;

/**
 *@brief 三角函数运算结构体句柄
 */
typedef struct
{
    s16 Theta;       /**< 角度-32768~32767对应-(π)~π    */
    s16 SinValue;    /**< 计算得到sin值                 */
    s16 CosValue;    /**< 计算得到cos值                 */
    s16 ArctanX;     /**< Arctan计算对应X值 arctan =Y/X */
    s16 ArctanY;     /**< Arctan计算对应Y值 arctan =Y/X */
    s16 ArctanValue; /**< 计算得到arctan值              */
} stru_CordicComponents;

typedef enum
{
    ARCTAN = 0, /**< 0计算arctan*/
    SINCOS = 1  /**< 1计算sin cos*/
} Arc_or_SinCFuction;

void DSP_Cmd(FuncState state);                                                 /* DSP使能和关闭 */
void DSP_CalcDivision(stru_DiviComponents *stru_Divi);                         /* DSP除法运算 */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise); /* DSP三角函数运算运算 */
uint32_t DSP_GetSqrt(uint32_t Data);                                           /* DSP取得开方根*/
s16 DSP_CalcSprt(s16 X, s16 Y);                                                /* DSP计算开方根*/

void DSP_Init(void);
s16 DSP_arctan(s16 x, s16 y);
s32 DSP_div(s32 a, s32 b);
s16 DSP_sin(u16 angle);
s16 DSP_cos(u16 angle);
u16 DSP_rms(s16 a, s16 b);
s16 DSP_mod(s32 x, s16 y);
u16 DSP_sqrt(u32 val);

// 增加DSP_CALC系列的接口，以降低参数传递带来的时间损耗
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
