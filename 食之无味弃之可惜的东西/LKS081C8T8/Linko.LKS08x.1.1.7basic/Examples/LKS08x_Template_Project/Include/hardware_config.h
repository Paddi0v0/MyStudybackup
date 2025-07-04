/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� hardware_config.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2018��1��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2018��1��20��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
 
#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_
#define MCU_MCLK	((u32)96000000uL) /* MCU������Ƶ */
/* ==============================PWM Ƶ�ʼ���������=========================== */
#define PWM_MCLK	((u32)96000000uL) /* PWMģ��������Ƶ */
#define PWM_PRSC ((u8)0) /* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ ((u16)15000) /* PWMն��Ƶ�� */
/* PWM ���ڼ�����ֵ */
#define PWM_PERIOD         ((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1))))
#define PWM_TIME_2S        (u16)(2*PWM_FREQ)
#define PWM_TIME_500MS     (u16)(500*PWM_FREQ/1000)
#define PWM_TIME_500US     (u16)(PWM_FREQ/500)
#define PWM_TIME_500uS     (u16)(PWM_FREQ/1000/2)
#define PWM_TIME_1MS       (u16)(PWM_FREQ/1000)
#define PWM_TIME_2MS       (u16)(2*PWM_FREQ/1000)
#define PWM_TIME_4MS       (u16)(4*PWM_FREQ/1000)
#define PWM_TIME_5MS       (u16)(5*PWM_FREQ/1000)
#define PWM_TIME_10MS      (u16)(20*PWM_FREQ/1000)
#define PWM_TIME_20MS      (u16)(40*PWM_FREQ/1000)

#define TIME_2S_BASE_5MS   ((u16)400)
#define TIME_1S6_BASE_5MS  ((u16)130)
#define TIME_0S5_BASE_5MS  ((u16)20)

#define HALL_BLOCK_TIME_S  (u8)(2)
#define HALL_TIMER_TH      (u32)(PWM_MCLK * HALL_BLOCK_TIME_S)
#define SAMPLING_FREQ      ((u16)PWM_FREQ)   // Resolution: 1Hz

#define MAX_MODULE_VALUE   19600 /* �����Ʊ� */

#define DEADTIME_NS        ((u16)1000)/* ����ʱ�� */
#define DEADTIME           (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)

#define DEADTIMECOMPVOLTAGE (u16)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)	//385(1us/((1/15k)*1000000)us)*18907

 

/* ******************************ADCͨ���Ŷ���****************************************** */

#define ADC0_CHANNEL_OPA0            ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1            ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2            ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3            ADC_CHANNEL_3

#define ADC1_CHANNEL_OPA0            ADC_CHANNEL_0
#define ADC1_CHANNEL_OPA1            ADC_CHANNEL_1
#define ADC1_CHANNEL_OPA2            ADC_CHANNEL_2
#define ADC1_CHANNEL_OPA3            ADC_CHANNEL_3


#endif
 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
