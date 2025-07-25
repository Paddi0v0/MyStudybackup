/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： hardware_config.h
 * 文件标识：
 * 内容摘要： 硬件相关文件
 * 其它说明： 无
 * 当前版本： V 1.0
 * 作    者： Howlet
 * 完成日期： 2020年8月20日
 *
 * 修改记录1：
 * 修改日期： 2020年8月20日
 * 版 本 号： V 2.0
 * 修 改 人： Howlet
 * 修改内容： 创建
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion -------------------*/
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "lks32mc08x_lib.h"
#include "hardware_init.h"
#include "MC_Parameter.h"
#include "Bemf_Check.h"

/*电源掉电监测阈值*/
#define POWER_4V5   0x00    /*电源掉电监测阈值为4.5V*/       
#define POWER_4V2   0x01    /*电源掉电监测阈值为4.2V*/ 
#define POWER_3V9   0x02    /*电源掉电监测阈值为3.9V*/ 
#define POWER_3V6   0x03    /*电源掉电监测阈值为3.6V*/ 



#define P_HIGH__N_HIGH                   0                 /* 预驱预动极性设置 上管高电平有效，下管高电平有效 */
#define P_HIGH__N_LOW                    1                 /* 预驱预动极性设置 上管高电平有效，下管低电平有效 */

#define  CHIP_PART_NUMBER              LKS32MC081          /* 芯片型号选择，选择不正确将影响芯片模块的初始化 */

#if ((CHIP_PART_NUMBER == LKS32MC084D)||(CHIP_PART_NUMBER == LKS32MC086D)||(CHIP_PART_NUMBER == LKS32MC087D)) 
#define  MCPWM_SWAP_FUNCTION           1                  /* 芯片内置预驱标志位*/
#define  PRE_DRIVER_POLARITY           P_HIGH__N_HIGH     /* 预驱预动极性设置 上管高电平有效，下管高电平有效 */
#else
#define  PRE_DRIVER_POLARITY           P_HIGH__N_LOW      /* 预驱预动极性设置 上管高电平有效，下管低电平有效 */
#endif

/* ----------------------PWM 频率及死区定义----------------------------------- */
#define MCU_MCLK                       (96000000LL)       /* PWM模块运行主频 */
#define PWM_MCLK                       ((u32)MCU_MCLK)    /* PWM模块运行主频 */
#define PWM_PRSC                       ((u8)0)            /* PWM模块运行预分频器 */
#define PWM_FREQ                       ((u16)16000)       /* PWM斩波频率 */


/* 电机控制PWM 周期计数器值 */
#define PWM_PERIOD                     ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1))))
/* PFC控制PWM 周期计数器值 */
#define PFC_PERIOD                     ((u16) (PWM_MCLK / (u32)(2 * PFC_FREQ *(PWM_PRSC+1))))

#define DEADTIME_NS                    ((u16)1200)       /* 死区时间 */
#define DEADTIME                       (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)
  
#define DEADTIMECOMPVOLTAGE            (u16)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)  

/* ------------------------------单电阻采样时间设置--------------------------- */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define ADC_COV_TIME                   (500)  /* 预留的ADC转换时间，单位：ns*/
#define SAMP_STABLE_TIME_1SHUNT        (1200) /* 单电阻采样，信号稳定时间设置| 单位 nS */
/* 单电阻采样信号稳定时间设置，主要取决于电流上升沿振铃时间 */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS+ADC_COV_TIME))/1000000000uL)
/* 单电阻采样移相采样点时间设置 */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS))/1000000000uL)  
#endif
   
/* ------------------------------三电阻采样时间设置--------------------------- */
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
#define ADC_COV_TIME                   (u16)(300)  /* 预留的ADC转换时间，单位：ns*/
#define SAMP_STABLE_TIME_3SHUNT        (1500 + DEADTIME_NS) /* 三电阻采样，信号稳定时间设置| 单位 nS */
/* 单电阻采样信号稳定时间设置，主要取决于电流上升沿振铃时间 */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT/2)/1000000000uL)
/* 单电阻采样移相采样点时间设置 */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT)/1000000000uL)

#define SAMP_NORMAL_TIME               (u16)(SAMP_STABLE_TIME - ((unsigned long long)PWM_MCLK * (unsigned long long)(ADC_COV_TIME))/1000000000uL)
  
#define SAMP_WAIT_TIME                 (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_3SHUNT-ADC_COV_TIME))/1000000000uL)

#endif    
                                                                                 
/* --------------------------------ADC通道号定义------------------------------ */
#define ADC0_CHANNEL_OPA0              ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1              ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2              ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3              ADC_CHANNEL_3

#define ADC1_CHANNEL_OPA0              ADC_CHANNEL_0
#define ADC1_CHANNEL_OPA1              ADC_CHANNEL_1
#define ADC1_CHANNEL_OPA2              ADC_CHANNEL_2
#define ADC1_CHANNEL_OPA3              ADC_CHANNEL_3

/* ADC相电流采样时序，硬件相关 ------------------------------------------------ */
/* Porting Application Notice 注意采样序列 ------------------------------------ */
#define ADC0_CURRETN_A_CHANNEL         (ADC0_CHANNEL_OPA2)  //A相 = U相
#define ADC0_CURRETN_B_CHANNEL         (ADC0_CHANNEL_OPA1)  //B相 = V相
#define ADC0_CURRETN_C_CHANNEL         (ADC0_CHANNEL_OPA0)  //C相 = W相

#define ADC_1SHUNT_CURR_CH             (ADC0_CHANNEL_OPA3)  /* 单电阻采样电流通道 */

#define ADC_BUS_VOL_CHANNEL            (ADC_CHANNEL_12)     /* 母线电压ADC采样通道 */
#define M0_ADC_BUS_CURR_CH             (ADC_CHANNEL_14)     /* 母线电流ADC采样通道 */

#define ADC0_TEMP_CHANNEL              (ADC_CHANNEL_7)      /* 温度检测 */

#define ADC0_VSP_CHANNEL               (ADC_CHANNEL_9)      /*电位器VSP检测通道*/

#define BEMF_CH_A                      ADC_CHANNEL_17       /* A相反电势检测ADC通道号 */
#define BEMF_CH_B                      ADC_CHANNEL_16       /* B相反电势检测ADC通道号 */
#define BEMF_CH_C                      ADC_CHANNEL_15       /* C相反电势检测ADC通道号 */

/* 电流内环相电流采样，ADC通道采样结果宏定义 */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define GET_ADC0_DATA(value)           {value.nData0 = (s16)ADC0_DAT0;}
#define GET_ADC1_DATA(value)           {value.nData0 = (s16)ADC0_DAT2;}
#else
#define GET_ADC0_DATA(value)           {value.nData0 = (s16)ADC0_DAT0;}
#define GET_ADC1_DATA(value)           {value.nData0 = (s16)ADC0_DAT1;}
#endif

/* 母线电压ADC通道采样结果宏定义 */
#define GET_BUS_VOL_ADC_RESULT         (ADC0_DAT4)

#define ADC_STATE_RESET()              {ADC0_CFG |= BIT2;}   /* ADC0 状态机复位,用以极限情况下确定ADC工作状态 */
#define ADC_SOFTWARE_TRIG_ONLY()       {ADC0_TRIG = 0;}      /* ADC设置为仅软件触发 */


/* ------------------------------PGA操作相关定义 ------------------------------ */                                                                                
#define OPA0_GIAN                      (PGA_GAIN_20)
#define OPA1_GIAN                      (PGA_GAIN_20 << 2)
#define OPA2_GIAN                      (PGA_GAIN_20 << 4)
#define OPA3_GIAN                      (PGA_GAIN_20 << 6)

/* ------------------------------DAC操作相关定义 ------------------------------ */
#define DAC_RANGE_1V2                  1                   /* DAC 1.2V量程 */
#define DAC_RANGE_3V0                  0                   /* DAC 3.0V量程 */
#define DAC_RANGE_4V85                 2                   /* DAC 4.85V量程 */

/* HALL自学习设置 ------------------------------------------------------------- */
#define HALL_LEARN_CURRENT             3000      /* HALL自学习电流定义 */

/* ------------------------------编译器选项------------------------------------ */
#define RUN_IN_RAM_FUNC  __attribute__ ((used, section ("ram3functions")))

/* ------------------------------硬件调试接口---------------------------------- */
#define DEBUG_SIG_OUT_HIGH()           {GPIO0_PDO |= BIT0;}
#define DEBUG_SIG_OUT_LOW()            {GPIO0_PDO &= ~BIT0;}
#define DEBUG_SIG_OUT_TANGO()          {GPIO0_PDO ^= BIT0;}

/* ---------------------------FOC 控制 常量定义 ------------------------------- */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT) 
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* 下桥电阻采样时,SampTime = 三相中最大占空比相-中间占空比相, 此处为满足ADC可靠采样的最小的SampTime值 */       \
    0,                                                                                                             \
                                                                                                                   \
    /* 下桥电阻采样时,需要保证ADC有正确采样窗口，判断两相PWM占空之间的差值是否满足采样条件                      */ \
    0,                                                                                                             \
                                                                                                                   \
    0,                                     /* 三电阻采样移位时间 */                                                \
    0,                                     /* 三电阻采样等待时间 */                                                \
    SAMP_STABLE_TIME,                      /* 单电阻采样，信号上升沿时间 */                                        \
    SAMP_SHIFT_TIME,                       /* 单电阻采样移相时间 */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM周期值 */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* 死区补偿值 */                                                        \
}
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT) 
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* 下桥电阻采样时,SampTime = 三相中最大占空比相-中间占空比相, 此处为满足ADC可靠采样的最小的SampTime值 */       \
    0,                                                                                                             \
                                                                                                                   \
    /* 下桥电阻采样时,需要保证ADC有正确采样窗口，判断两相PWM占空之间的差值是否满足采样条件                      */ \
    0,                                                                                                             \
                                                                                                                   \
    0,                                     /* 三电阻采样移位时间 */                                                \
    0,                                     /* 三电阻采样等待时间 */                                                \
    0,                                     /* 单电阻采样，信号上升沿时间 */                                        \
    0,                                     /* 单电阻采样移相时间 */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM周期值 */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* 死区补偿值 */                                                        \
}
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* 下桥电阻采样时,三相中最大占空比相,满足ADC可靠采样的最小的PWM值 */                                           \
    (PWM_PERIOD - SAMP_STABLE_TIME),                                                                               \
                                                                                                                   \
    /* 下桥电阻采样时,需要保证ADC有正确采样窗口，判断两相PWM占空之间的差值是否满足采样条件 */                      \
    SAMP_SHIFT_TIME,                                                                                               \
                                                                                                                   \
    (SAMP_NORMAL_TIME- PWM_PERIOD),        /* 三电阻采样移位时间 */                                                \
    SAMP_WAIT_TIME,                        /* 三电阻采样等待时间 */                                                \
    0,                                     /* 单电阻采样，信号上升沿时间 */                                        \
    0,                                     /* 单电阻采样移相时间 */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM周期值 */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* 死区补偿值 */                                                        \
}
#endif
#endif
#endif



#endif  /* __HARDWARE_CONFIG_H_ */

 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
