/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� parameter.h
 * �ļ���ʶ��
 * ����ժҪ�� parameter config
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Andrew kong, Howlet Li
 * ������ڣ� 2020��8��18��
 *
 *******************************************************************************/
 
/*------------------------------prevent recursive inclusion -------------------*/ 
#ifndef __MC_GLOBAL_DEF_M0_H
#define __MC_GLOBAL_DEF_M0_H

#include "basic.h"
#include "MC_Type.h"
#include "project_config.h"


//���±���걻�޸�ʱ����Ҫ���±������?
//ȫ��������Ϣ���޸�ʱ��ͬ����Ҫ���±��롣

/* -----------------------------Hardware Parameter---------------------------- */

/* ----------------------PWM Ƶ�ʼ���������----------------------------------- */
#define MCU_MCLK_M0							(MCU_MCLK_USED)       	/* PWMģ��������Ƶ */
#define PWM_MCLK_M0							((u32)MCU_MCLK_M0)		/* PWMģ��������Ƶ */
#define PWM_PRSC_M0							(0)						/* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ_M0							((u16)16000)			/* PWMն��Ƶ�� */

#define CURRENT_SAMPLE_TYPE_M0				(DRV0_CUR_SAMP_TYPE)	/* ����������ʽѡ�� */
#define ROTOR_SENSOR_TYPE_M0				ROTOR_SENSORLESS		/* ���λ�ô���������ѡ�� */

#define SVPWM_TYPE_M0						(0)						/* 0 --- 7seg  1--- 5seg */
#define PWM_USED_ID_M0						(DRV0_PWM_ID)
#define OPA_SELECT_M0						(0)						/*	����Ŵ��� ѡ�� 0--1--2--3*/

#define OVER_MODU_FACTOR_M0					((s16)21845)			/*��λ: Q15, ������ϵ��*/



#define CUR_LOOP_FREQUENCY_M0				((u16)16000)		/*��λ: HZ,��ǰδ�ã�Ԥ��*/
#define OBSVR_LOOP_FREQUENCY_M0				((u16)16000)		/*��λ: HZ,��ǰδ�ã�Ԥ��*/
#define SPD_LOOP_FREQUENCY_M0				((u16)1000)			/*��λ: HZ*/


/* �������PWM ���ڼ�����ֵ */
#if (PWM_PRSC_M0 == 0)
#define PWM_PERIOD_M0                        ((u16) (PWM_MCLK_M0 * 1000000 / (u32)(2 * PWM_FREQ_M0 *(PWM_PRSC_M0+1))))
#elif (PWM_PRSC_M0 == 1)
#define PWM_PERIOD_M0                        ((u16) (PWM_MCLK_M0 * 1000000 / (u32)(2 * PWM_FREQ_M0 *(2))))
#elif (PWM_PRSC_M0 == 2)
#define PWM_PERIOD_M0                        ((u16) (PWM_MCLK_M0 * 1000000 / (u32)(2 * PWM_FREQ_M0 *(4))))
#elif (PWM_PRSC_M0 == 3)
#define PWM_PERIOD_M0                        ((u16) (PWM_MCLK_M0 * 1000000 / (u32)(2 * PWM_FREQ_M0 *(8))))
#else
#error "Don't support PWM_PRSC_M"
#endif

/*Motor0 Deadtime config*/
#define DEADTIME_NS_M0                       ((u16)1200)                              /* ����ʱ��500--1000 */
#define DEADTIME_M0                          (u16)(((unsigned long long)PWM_MCLK_M0 * (unsigned long long)DEADTIME_NS_M0)/1000uL)

#define ADC_COV_TIME_M0						(200)   /* Ԥ����ADCת��ʱ�䣬��λ��500--200ns*/
#define SAMP_STABLE_TIME_SHUNT_M0			(1500)  /* 1000--1800������������ź��ȶ�ʱ������| ��λ nS */

#define VF_DEADZONE_COMP_FACTOR_M0			(0)		// Q15��ʽ: 0~32767 	0----- ��������ʱ���Զ�����

/* ----------------------˳�������� ----------------------------------------------------------------------- */
#if (DRV0_BEMF_CHECK_EABLE == FUNCTION_ON)
#define DIR_EMF_CHECK_ENABLE_M0				(TRUE)   		// ������˳����� TRUE = ʹ��, FALSE = �ر�
#else
#define DIR_EMF_CHECK_ENABLE_M0				(FALSE)
#endif

/* ----------------------IPD ת�ӳ�ʼλ�ü�� ����ע��ʱ������--------------------------------------------- */
#define SEEK_POSITION_STATUS_M0				(FALSE)			// ��ʼλ�ü��״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��

/* ----------------------˳������Ƿ�����---------------------------------------------------------------- */
#define DIR_CHECK_STATUS_M0					(FALSE)			// ˳�����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��

/* ----------------------allign �����Ƿ�����--------------------------------------------------------------- */
#define ALLIGN_STATUS_M0					(TRUE)			// ALLIGN����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��

/* ----------------------VF �����Ƿ�����------------------------------------------------------------------- */
#define VF_START_STATUS_M0					(FALSE)			// VF ����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��, 
															// ��ǰδ�ã�Ԥ��

/* ----------------------AC��⼰�����Ƿ�����-------------------------------------------------------------- */
#define AC_SENOR_PROTECT_STATUS_M0			(FALSE)			// AC��⼰����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��
															// ��ǰδ�ã�Ԥ��  */

/* ----------------------NTC��⼰�����Ƿ�����-------------------------------------------------------------- */
#define NTC_SENOR_PROTECT_STATUS_M0			(FALSE)			// NTC��⼰����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��

/* ----------------------Ԥ����Ƿ�����--------------------------------------------------------------------- */
#define CHARGE_STATUS_M0					(FALSE)			// CHARGE����״̬ TRUE Ϊʹ��, FALSE Ϊ��ʹ��


/* ------------------------------ADCУ׼��ز�������-------------------------------------------------------- */
#define CALIB_SAMPLES_M0					(512)			// ADCƫ��У׼�����������޸�
#define ADC_OFFSET_ERROR_M0					(3500)			// ADCƫ�������ֵ�������޸�

/* ----------------------------Ԥ���Ծٵ���Ԥ������------------------------------------------------------- */
#define CHARGE_TIME_M0						(30)			// ÿ��Ԥ���ʱ�䣬����ʵ��Ӳ�������޸�

/*-----------------------------------��·ѡ��----------------------------------------------------------------*/

#define CLOSE_LOOP_M0						(SPEED_LOOP)	// ��·ѡ��


/*----------------------------------�ٶȻ�����---------------------------------------------------------------*/
#define POWER_LIMIT_STATUS_M0				(FALSE)			// �޹���״̬��TRUE = ʹ�ܣ�FALSE = ��ʹ��


/*------------------------------------���ʻ�����-------------------------------------------------------------*/
#define SPPED_LIMIT_STATUS_M0				(FALSE)			// ��ת��״̬��TRUE = ʹ�ܣ�FALSE = ��ʹ��

/*------------------------------------���ſ���---------------------------------------------------------------*/
#define WEAKEN_FLUX_STATUS_M0				(FALSE)			// ���ſ���״̬��TRUE = ʹ�ܣ�FALSE = ��ʹ��

/*------------------------------------PWMʹ�ܿ���------------------------------------------------------------*/
#define PWM_ENABLE_STOP_M0					(FALSE)			// PWM״̬ʹ�ܣ�TRUE = ʹ�ܣ�FALSE = ��ʹ��
#define STOP_MODE_M0						(0)				// ֹͣ��ʽ��0 = �ر�MOE��1-- �����  2--- �̽�


#define FAULT_AUTO_CLR_M0					(FALSE)			// �����Զ����ʹ��
#define FAULT_CLR_PRIOD_M0					(2000)			// ����������� ms


#endif  /* __MC_GLOBAL_DEF_M0_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* -----------------------------------END OF FILE------------------------------- */

