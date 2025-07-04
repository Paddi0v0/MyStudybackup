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

#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "project_mcu.h"
#include "project_config.h"
#include "MC_Parameter_M0.h"
#include "PubData_DriverInstance.h"


/*��Դ��������ֵ*/
#define POWER_4V5   0x00    /*��Դ��������ֵΪ4.5V*/       
#define POWER_4V2   0x01    /*��Դ��������ֵΪ4.2V*/ 
#define POWER_3V9   0x02    /*��Դ��������ֵΪ3.9V*/ 
#define POWER_3V6   0x03    /*��Դ��������ֵΪ3.6V*/ 



#define LKS32MC070   0x01    /*оƬ�ͺ�*/ 
#define LKS32MC071   0x02    /*оƬ�ͺ�*/ 
#define LKS32MC072   0x03    /*оƬ�ͺ�*/ 


#define P_HIGH__N_HIGH                   0                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#define P_HIGH__N_LOW                    1                 /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */

#define  CHIP_PART_NUMBER              LKS32MC074D         /* оƬ�ͺ�ѡ��ѡ����ȷ��Ӱ��оƬģ��ĳ�ʼ�� */

#if ((CHIP_PART_NUMBER == LKS32MC074D)||(CHIP_PART_NUMBER == LKS32MC076D)||(CHIP_PART_NUMBER == LKS32MC077D)) 
#define  MCPWM_SWAP_FUNCTION           1
#define  PRE_DRIVER_POLARITY           P_HIGH__N_HIGH     /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܸߵ�ƽ��Ч */
#else
#define  PRE_DRIVER_POLARITY           P_HIGH__N_LOW      /* Ԥ��Ԥ���������� �Ϲܸߵ�ƽ��Ч���¹ܵ͵�ƽ��Ч */
#endif

/* ----------------------Ӧ��PWM ģ�鶨��----------------------------------- */



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
#define ADC0_CURRETN_A_CHANNEL         (ADC0_CHANNEL_OPA0)//2
#define ADC0_CURRETN_B_CHANNEL         (ADC0_CHANNEL_OPA2)//1
#define ADC0_CURRETN_C_CHANNEL         (ADC0_CHANNEL_OPA1)

#define ADC_1SHUNT_CURR_CH             (ADC0_CHANNEL_OPA3)  /* �������������ͨ�� */

#define ADC_BUS_VOL_CHANNEL            (ADC_CHANNEL_8)      /* ĸ�ߵ�ѹADC����ͨ�� */
#define M0_ADC_BUS_CURR_CH             (ADC0_CHANNEL_OPA3)  /* ĸ�ߵ���ADC����ͨ�� */

#define ADC0_TEMP_CHANNEL              (ADC_CHANNEL_7)      /* �¶ȼ�� */
#define ADC0_VSP_CHANNEL               (ADC_CHANNEL_9)      /*��λ��VSP���ͨ��*/


#define BEMF_CH_A                      ADC_CHANNEL_13       /* A�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_B                      ADC_CHANNEL_12       /* B�෴���Ƽ��ADCͨ���� */
#define BEMF_CH_C                      ADC_CHANNEL_11       /* C�෴���Ƽ��ADCͨ���� */

/* �����ڻ������������ADCͨ����������궨�� */
#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
//M0 ��һ���������ݣ��ڶ�����������
#define GET_ADC0_DATA_M0					(ADC0_DAT0)   
#define GET_ADC1_DATA_M0					(ADC0_DAT2)
#else
#define GET_ADC0_DATA_M0					(ADC0_DAT0)
#define GET_ADC1_DATA_M0					(ADC0_DAT1)
#endif



/* ĸ�ߵ�ѹADCͨ����������궨�� */
#define GET_M0_BUS_VOL_ADC_RESULT			(ADC0_DAT4)

#define GET_M0_TEMP_DATA_A					(ADC0_DAT6)
#define GET_M0_BEMF_DATA_A					(ADC0_DAT8)
#define GET_M0_BEMF_DATA_B					(ADC0_DAT9)
#define GET_M0_BEMF_DATA_C					(ADC0_DAT10)




#define ADC_STATE_RESET()              {ADC0_CFG |= BIT11; ADC1_CFG |= BIT11;}   /* ADC0 ״̬����λ,���Լ��������ȷ��ADC����״̬ */
#define ADC0_STATE_RESET()              {ADC0_CFG |= BIT11; }   /* ADC0 ״̬����λ,���Լ��������ȷ��ADC����״̬ */
#define ADC1_STATE_RESET()              {ADC1_CFG |= BIT11; }  

#define ADC_SOFTWARE_TRIG_ONLY()       {ADC0_TRIG = 0;}      /* ADC����Ϊ��������� */

/* ------------------------------PGA������ض��� ------------------------------ */
//#define PGA_GAIN_20                    0                   /* ��������200:10 */
//#define PGA_GAIN_9P5                   1                   /* ��������190:20 */
//#define PGA_GAIN_6                     2                   /* ��������180:30 */
//#define PGA_GAIN_4P25                  3                   /* ��������170:40 */

#endif  /* __HARDWARE_CONFIG_H_ */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

