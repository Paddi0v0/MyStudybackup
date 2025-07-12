/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_config.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��8��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2020��8��20��
 * �� �� �ţ� V 2.0
 * �� �� �ˣ� Howlet
 * �޸����ݣ� ����
 *
 *******************************************************************************/

/*------------------------------prevent recursive inclusion -------------------*/
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "lks32mc08x_lib.h"
#include "hardware_init.h"
#include "MC_Parameter.h"

/*��Դ��������ֵ*/
#define POWER_4V5   0x00    /*��Դ��������ֵΪ4.5V*/       
#define POWER_4V2   0x01    /*��Դ��������ֵΪ4.2V*/ 
#define POWER_3V9   0x02    /*��Դ��������ֵΪ3.9V*/ 
#define POWER_3V6   0x03    /*��Դ��������ֵΪ3.6V*/ 



#define P_HIGH__N_HIGH                   0                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#define P_HIGH__N_LOW                    1                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */

#define  CHIP_PART_NUMBER              LKS32MC081          /* оƬ�ͺ�ѡ��ѡ����ȷ��Ӱ��оƬģ��ĳ�ʼ�� */

#if ((CHIP_PART_NUMBER == LKS32MC084D)||(CHIP_PART_NUMBER == LKS32MC086D)||(CHIP_PART_NUMBER == LKS32MC087D)) 
#define  MCPWM_SWAP_FUNCTION           1                  /* оƬ����Ԥ����־λ*/
#define  PRE_DRIVER_POLARITY           P_HIGH__N_HIGH     /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#else
#define  PRE_DRIVER_POLARITY           P_HIGH__N_LOW      /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */
#endif

/* ----------------------PWM Ƶ�ʼ���������----------------------------------- */
#define MCU_MCLK                       (96000000LL)       /* PWMģ��������Ƶ */
#define PWM_MCLK                       ((u32)MCU_MCLK)    /* PWMģ��������Ƶ */
#define PWM_PRSC                       ((u8)0)            /* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ                       ((u16)16000)       /* PWMն��Ƶ�� */


/* �������PWM ���ڼ�����ֵ */
#define PWM_PERIOD                     ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1))))
/* PFC����PWM ���ڼ�����ֵ */
#define PFC_PERIOD                     ((u16) (PWM_MCLK / (u32)(2 * PFC_FREQ *(PWM_PRSC+1))))

#define DEADTIME_NS                    ((u16)1200)       /* ����ʱ�� */
#define DEADTIME                       (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)
  
#define DEADTIMECOMPVOLTAGE            (u16)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)  

/* ------------------------------���������ʱ������--------------------------- */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define ADC_COV_TIME                   (500)  /* Ԥ����ADCת��ʱ�䣬��λ��ns*/
#define SAMP_STABLE_TIME_1SHUNT        (1200) /* ������������ź��ȶ�ʱ������| ��λ nS */
/* ����������ź��ȶ�ʱ�����ã���Ҫȡ���ڵ�������������ʱ�� */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS+ADC_COV_TIME))/1000000000uL)
/* ������������������ʱ������ */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS))/1000000000uL)  
#endif
   
/* ------------------------------���������ʱ������--------------------------- */
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
#define ADC_COV_TIME                   (u16)(300)  /* Ԥ����ADCת��ʱ�䣬��λ��ns*/
#define SAMP_STABLE_TIME_3SHUNT        (1500 + DEADTIME_NS) /* ������������ź��ȶ�ʱ������| ��λ nS */
/* ����������ź��ȶ�ʱ�����ã���Ҫȡ���ڵ�������������ʱ�� */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT/2)/1000000000uL)
/* ������������������ʱ������ */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT)/1000000000uL)

#define SAMP_NORMAL_TIME               (u16)(SAMP_STABLE_TIME - ((unsigned long long)PWM_MCLK * (unsigned long long)(ADC_COV_TIME))/1000000000uL)
  
#define SAMP_WAIT_TIME                 (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_3SHUNT-ADC_COV_TIME))/1000000000uL)

#endif    
                                                                                 
/* --------------------------------ADCͨ���Ŷ���------------------------------ */
#define ADC0_CHANNEL_OPA0              ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1              ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2              ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3              ADC_CHANNEL_3

#define ADC1_CHANNEL_OPA0              ADC_CHANNEL_0
#define ADC1_CHANNEL_OPA1              ADC_CHANNEL_1
#define ADC1_CHANNEL_OPA2              ADC_CHANNEL_2
#define ADC1_CHANNEL_OPA3              ADC_CHANNEL_3

/* ADC���������ʱ��Ӳ����� ------------------------------------------------ */
/* Porting Application Notice ע��������� ------------------------------------ */
#define ADC0_CURRETN_A_CHANNEL         (ADC0_CHANNEL_OPA2)
#define ADC0_CURRETN_B_CHANNEL         (ADC0_CHANNEL_OPA1)
#define ADC0_CURRETN_C_CHANNEL         (ADC0_CHANNEL_OPA0)

#define ADC_1SHUNT_CURR_CH             (ADC0_CHANNEL_OPA3)  /* �������������ͨ�� */

#define ADC_BUS_VOL_CHANNEL            (ADC_CHANNEL_12)     /* ĸ�ߵ�ѹADC����ͨ�� */
#define M0_ADC_BUS_CURR_CH             (ADC_CHANNEL_14)     /* ĸ�ߵ���ADC����ͨ�� */

#define ADC0_TEMP_CHANNEL              (ADC_CHANNEL_7)      /* �¶ȼ�� */

#define ADC0_VSP_CHANNEL               (ADC_CHANNEL_9)      /*��λ��VSP���ͨ��*/

#define BEMF_CH_A                      ADC_CHANNEL_17       /* A�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_B                      ADC_CHANNEL_16       /* B�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_C                      ADC_CHANNEL_15       /* C�෴���Ƽ��ADCͨ���� */

/* �����ڻ������������ADCͨ����������궨�� */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define GET_ADC0_DATA(value)           {value.nData0 = (s16)ADC0_DAT0;}
#define GET_ADC1_DATA(value)           {value.nData0 = (s16)ADC0_DAT2;}
#else
#define GET_ADC0_DATA(value)           {value.nData0 = (s16)ADC0_DAT0;}
#define GET_ADC1_DATA(value)           {value.nData0 = (s16)ADC0_DAT1;}
#endif

/* ĸ�ߵ�ѹADCͨ����������궨�� */
#define GET_BUS_VOL_ADC_RESULT         (ADC0_DAT4)

#define ADC_STATE_RESET()              {ADC0_CFG |= BIT2;}   /* ADC0 ״̬����λ,���Լ��������ȷ��ADC����״̬ */
#define ADC_SOFTWARE_TRIG_ONLY()       {ADC0_TRIG = 0;}      /* ADC����Ϊ��������� */


/* ------------------------------PGA������ض��� ------------------------------ */                                                                                
#define OPA0_GIAN                      (PGA_GAIN_20)
#define OPA1_GIAN                      (PGA_GAIN_20 << 2)
#define OPA2_GIAN                      (PGA_GAIN_20 << 4)
#define OPA3_GIAN                      (PGA_GAIN_20 << 6)

/* ------------------------------DAC������ض��� ------------------------------ */
#define DAC_RANGE_1V2                  1                   /* DAC 1.2V���� */
#define DAC_RANGE_3V0                  0                   /* DAC 3.0V���� */
#define DAC_RANGE_4V85                 2                   /* DAC 4.85V���� */

/* HALL��ѧϰ���� ------------------------------------------------------------- */
#define HALL_LEARN_CURRENT             3000      /* HALL��ѧϰ�������� */

/* ------------------------------������ѡ��------------------------------------ */
#define RUN_IN_RAM_FUNC  __attribute__ ((used, section ("ram3functions")))

/* ------------------------------Ӳ�����Խӿ�---------------------------------- */
#define DEBUG_SIG_OUT_HIGH()           {GPIO0_PDO |= BIT0;}
#define DEBUG_SIG_OUT_LOW()            {GPIO0_PDO &= ~BIT0;}
#define DEBUG_SIG_OUT_TANGO()          {GPIO0_PDO ^= BIT0;}

/* ---------------------------FOC ���� �������� ------------------------------- */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT) 
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* ���ŵ������ʱ,SampTime = ���������ռ�ձ���-�м�ռ�ձ���, �˴�Ϊ����ADC�ɿ���������С��SampTimeֵ */       \
    0,                                                                                                             \
                                                                                                                   \
    /* ���ŵ������ʱ,��Ҫ��֤ADC����ȷ�������ڣ��ж�����PWMռ��֮��Ĳ�ֵ�Ƿ������������                      */ \
    0,                                                                                                             \
                                                                                                                   \
    0,                                     /* �����������λʱ�� */                                                \
    0,                                     /* ����������ȴ�ʱ�� */                                                \
    SAMP_STABLE_TIME,                      /* ������������ź�������ʱ�� */                                        \
    SAMP_SHIFT_TIME,                       /* �������������ʱ�� */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM����ֵ */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* ��������ֵ */                                                        \
}
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT) 
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* ���ŵ������ʱ,SampTime = ���������ռ�ձ���-�м�ռ�ձ���, �˴�Ϊ����ADC�ɿ���������С��SampTimeֵ */       \
    0,                                                                                                             \
                                                                                                                   \
    /* ���ŵ������ʱ,��Ҫ��֤ADC����ȷ�������ڣ��ж�����PWMռ��֮��Ĳ�ֵ�Ƿ������������                      */ \
    0,                                                                                                             \
                                                                                                                   \
    0,                                     /* �����������λʱ�� */                                                \
    0,                                     /* ����������ȴ�ʱ�� */                                                \
    0,                                     /* ������������ź�������ʱ�� */                                        \
    0,                                     /* �������������ʱ�� */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM����ֵ */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* ��������ֵ */                                                        \
}
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
#define FOC_CONST_PARAMETER                                                                                        \
{                                                                                                                  \
    /* ���ŵ������ʱ,���������ռ�ձ���,����ADC�ɿ���������С��PWMֵ */                                           \
    (PWM_PERIOD - SAMP_STABLE_TIME),                                                                               \
                                                                                                                   \
    /* ���ŵ������ʱ,��Ҫ��֤ADC����ȷ�������ڣ��ж�����PWMռ��֮��Ĳ�ֵ�Ƿ������������ */                      \
    SAMP_SHIFT_TIME,                                                                                               \
                                                                                                                   \
    (SAMP_NORMAL_TIME- PWM_PERIOD),        /* �����������λʱ�� */                                                \
    SAMP_WAIT_TIME,                        /* ����������ȴ�ʱ�� */                                                \
    0,                                     /* ������������ź�������ʱ�� */                                        \
    0,                                     /* �������������ʱ�� */                                                \
                                                                                                                   \
    PWM_PERIOD,                            /* PWM����ֵ */                                                         \
    DEADTIMECOMPVOLTAGE,                   /* ��������ֵ */                                                        \
}
#endif
#endif
#endif



#endif  /* __HARDWARE_CONFIG_H_ */

 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
