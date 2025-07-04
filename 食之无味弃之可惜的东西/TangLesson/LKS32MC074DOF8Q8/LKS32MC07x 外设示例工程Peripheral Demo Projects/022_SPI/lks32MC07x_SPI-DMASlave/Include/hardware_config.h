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

#include "lks32mc07x.h"
#include "lks32mc07x_MCPWM.h"
#include "lks32mc07x_sys.h"
#include "lks32mc07x_adc.h"
#include "lks32mc07x_MCPWM.h"
#include "lks32mc07x_flash.h"
#include "lks32mc07x_Gpio.h"
#include "lks32mc07x_cmp.h"
#include "lks32mc07x_uart.h"
#include "lks32mc07x_hall.h"
#include "lks32mc07x_tim.h"
#include "hardware_init.h"
#include "lks32mc07x_NVR.h"
#include "lks32mc07x_tmp.h"
#include "MC_Parameter.h"

#define P_HIGH__N_HIGH                   0                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#define P_HIGH__N_LOW                    1                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */

#define  CHIP_PART_NUMBER              LKS32MC071         /* оƬ�ͺ�ѡ��ѡ����ȷ��Ӱ��оƬģ��ĳ�ʼ�� */

#if ((CHIP_PART_NUMBER == LKS32MC074D)||(CHIP_PART_NUMBER == LKS32MC076D)||(CHIP_PART_NUMBER == LKS32MC077D)) 
#define  MCPWM_SWAP_FUNCTION           1
     /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#else
#define  PRE_DRIVER_POLARITY           P_HIGH__N_LOW      /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */
#endif

  
#define DEADTIMECOMPVOLTAGE            (u16)(DEADTIME_NS/(1000000000.0/PWM_FREQ)*MAX_MODULE_VALUE)  

/* ------------------------------���������ʱ������--------------------------- */
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
#define ADC_COV_TIME                   (200)  /* Ԥ����ADCת��ʱ�䣬��λ��ns*/
#define SAMP_STABLE_TIME_1SHUNT        (1000) /* ������������ź��ȶ�ʱ������| ��λ nS */
/* ����������ź��ȶ�ʱ�����ã���Ҫȡ���ڵ�������������ʱ�� */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS+ADC_COV_TIME))/1000000000uL)
/* ������������������ʱ������ */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_1SHUNT+DEADTIME_NS))/1000000000uL)  
#endif
   
/* ------------------------------���������ʱ������--------------------------- */
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
#define ADC_COV_TIME                   (u16)(300)  /* Ԥ����ADCת��ʱ�䣬��λ��ns*/
#define SAMP_STABLE_TIME_3SHUNT        (2200 + DEADTIME_NS) /* ������������ź��ȶ�ʱ������| ��λ nS */
/* ����������ź��ȶ�ʱ�����ã���Ҫȡ���ڵ�������������ʱ�� */
#define SAMP_STABLE_TIME               (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT/2)/1000000000uL)
/* ������������������ʱ������ */
#define SAMP_SHIFT_TIME                (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)SAMP_STABLE_TIME_3SHUNT)/1000000000uL)

#define SAMP_NORMAL_TIME               (u16)(SAMP_STABLE_TIME - ((unsigned long long)PWM_MCLK * (unsigned long long)(ADC_COV_TIME))/1000000000uL)
  
#define SAMP_WAIT_TIME                 (u16)(((unsigned long long)PWM_MCLK * (unsigned long long)(SAMP_STABLE_TIME_3SHUNT-ADC_COV_TIME))/1000000000uL)

#endif    
                                                                                 

#endif  /* __HARDWARE_CONFIG_H_ */

 
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
 
