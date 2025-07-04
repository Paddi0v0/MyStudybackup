/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� parameter.h
 * �ļ���ʶ��
 * ����ժҪ�� parameter config
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Lizy, Howlet Li
 * ������ڣ� 2022��5��18��
 *******************************************************************************/
 
/*------------------------------prevent recursive inclusion -------------------*/ 
#ifndef __PARAMETER0_H
#define __PARAMETER0_H

#include "basic.h"
#include "MC_Type.h"
#include "MC_GlobalDef_M0.h"


/* -----------------------------Hardware Parameter---------------------------- */
#define ADC_SUPPLY_VOLTAGE_M0             (3.6)              /* ��λ: V  ADC��׼��ѹ��2.4����1.2 */
//˫���������Χ:101.25A
//����40Kŷ 320/(5+20*2)=7.11111111111   ��ǰ40Kŷķ

//�����������Χ:99.0A
//����1Kŷ 80/(10+1)=7.2727272            ��ǰ1Kŷķ //  17.5439 14.5454545


#define AMPLIFICATION_GAIN_M0			(3.98)             /*�˷ŷŴ��� 18.905*/  //  3.98  3.6/(14.5454545 * 0.05)= 4.95
#define RSHUNT_M0						(0.05)                /* ��λ: ��  ����������ֵ */
#if 1
#define VOLTAGE_SHUNT_RATIO_M0			(1.0/(20.0 * 2 + 1.0)) /* ĸ�ߵ�ѹ��ѹ�� */
#define BEMF_SHUNT_RATIO_M0				(1.0/(20.0 * 2 + 1.0)) /* �����Ƶ�ѹ��ѹ�� */
#else

#define VOLTAGE_SHUNT_RATIO_M0			(1.0/(14.3 + 1.0)) /* ĸ�ߵ�ѹ��ѹ�� */
#define BEMF_SHUNT_RATIO_M0				(1.0/(14.3 + 1.0)) /* �����Ƶ�ѹ��ѹ�� */


#endif

/* ------------------------------Rated Parameter------------------------------ */
#define U_MAX_FREQ_M0						(1200.0)				/* ��λ:Hz,  ���������е�Ƶ�� */

/* ------------------------------Motor Parameter------------------------------ */
#if 0
#define U_MOTOR_PP_M0						(7.0)			/* ��������� */
#define U_MOTOR_RS_M0						(1.537)		/* ��λ: �� �������� */
#define U_MOTOR_LD_M0						(2269.0)		/* ��λ: uH ���d���� (343.893)*/
#define U_MOTOR_LQ_M0						(2500.0)		/* ��λ: uH ���q���� (234.4485)*/

/* ����������� ���㹫ʽ��Vpp/2/sqrt(3)/(2*PI)/f������VppΪ��ѹ���ֵ��fΪ��Ƶ�� 
   �˲�����Ӱ��˳����������ٶȼ�⣬�Ƕȹ��㲻ʹ��Щ���� */
#define U_MOTOR_FLUX_CONST_M0				(0.0687364598)  
#else

#define U_MOTOR_PP_M0                     (5.0)             /* ��������� */
#define U_MOTOR_RS_M0                     (0.454273)             /* ��λ: �� �������� */
#define U_MOTOR_LD_M0                     (234.4485)//(8000.0)          /* ��λ: uH ���d���� */
#define U_MOTOR_LQ_M0                     (355.893)//(10000.0)         /* ��λ: uH ���q���� */

/* ����������� ���㹫ʽ��Vpp/2/sqrt(3)/(2*PI)/f������VppΪ��ѹ���ֵ��fΪ��Ƶ�� 
   �˲�����Ӱ��˳����������ٶȼ�⣬�Ƕȹ��㲻ʹ��Щ���� */
#define U_MOTOR_FLUX_CONST_M0             (0.005874687)   


#endif



/* ----------------------IPD ת�ӳ�ʼλ�ü�� ����ע��ʱ������---------------- */
#define U_START_ANGLE_COMP_M0				(0)				/* ��λ:�� ��ʼλ�ü�ⲹ���Ƕ� */

#define IPD_PLUS_TIME_SETTING_M0			(30)			/* ����ע��ʱ�������� ��λus */
#define IPD_WAIT_TIME_SETTING_M0			(500)			/* ������еȴ�ʱ�������� ��λus ���һ��PWM����*/

#define IPD_PLUS_TIME_WIDTH_M0            (u32)(IPD_PLUS_TIME_SETTING_M0*(MCU_MCLK_M0))   /* ����ע��ʱ�������� ��λclk */
#define IPD_PLUS_WAIT_TIME_M0             (u32)(IPD_WAIT_TIME_SETTING_M0*(MCU_MCLK_M0))   /* ������еȴ�ʱ�������� ��λclk */


/* -------------------------------- ˳��������------------------------------ */
#define SPEED_TRACK_DELAYTIME_M0			(500)			/* ��λ: ms ˳�����ʱ��  */
#define SPEED_TRACK_ON_FREQ_THH_M0			(50.0)			/* ��λ: Hz ˳���бջ�Ƶ�� */
#define EBRK_ON_FREQ_THH_M0					(20.0)			/* ��λ: Hz ���ɲ��Ƶ�� */

#define MOTOR_STOP_CUR_THD_M0				(0.3)			/* ��λ: A  ���ֹͣ��������ֵ   */
#define MOTOR_STOP_CUR_DIF_THD_M0			(0.3)			/* ��λ: A  ���ֹͣ����������ֵ */

#define STOP_TIME_M0						(200)			/* ��λ��ms ���ֹͣ����˲�ʱ�䣬����ʵ�ʸ����޸� */
#define STOP_DELAY_TIME_M0					(200)			/* ��λ: ms ���ֹͣ���ӳ�ʱ�䣬����ʵ�ʸ����޸ġ��޸����ݣ�������ж�Ϊֹͣ����ת���ͼӴ��ӳ�ʱ�� */


#define BEMF_ANG_COMP_M0					(0)				/* ��λ���� �����Ƽ��ǶȲ���������ʵ�ʸ����޸� */
#define BEMF_TRACK_MIN_VOLT_M0				(200)			/* ��λ: 0.01V �����Ƽ����С��ѹ������ʵ�ʸ����޸ġ��޸����ݣ�������ж�Ϊֹͣ����ת���ͼӴ��ӳ�ʱ�� */


/*------------------------------------Ԥ��λ����---------------------------------*/
#define ALIGN_ANGLE_M0						(0.0)			/* ��λ:�� Ԥ��λ�Ƕ� */
#define U_START_CUR_SET_F_M0				(1)			/* ��λ: A ��һ�ζ�λ���� */
#define U_START_CUR_SET_S_M0				(2)			/* ��λ: A �ڶ��ζ�λ���� */
#define DC_HOLDTIME_TIME_LENTH_M0			(300)			/* ��λ: ms ��һ�ζ�λʱ�� */
#define DC_HOLDTIME_TIME_LENTH_STAGE1_M0	(300)			/* ��λ: ms �ڶ��ζ�λʱ�� */
#define DC_ALIGN_TOTAL_LENTH_M0            \
       (DC_HOLDTIME_TIME_LENTH_M0 + DC_HOLDTIME_TIME_LENTH_STAGE1_M0)/* ��λ��ʱ�� */

#define ALIGN_CURRENT_ACC_M0				(20)			/* ��λ: A/s  ��λ�������ٵ���ֵ  ��ʼλ�ü��ʹ�ܺ�������ֵ�����ܳ���30���������ݻ������ */
#define ALIGN_CURRENT_DEC_M0				(20)			/* ��λ: A/s  ��λ�������ٵ���ֵ  ��ʼλ�ü��ʹ�ܺ�������ֵ�����ܳ���30���������ݻ������ */


/*------------------------------------VF��������---------------------------------*/
															/* ��ǰδ�ã�Ԥ�� */
#define VF_START_VALUE_M0					(1)				/* VF������ѹֵ VF_START_VALUE/VF_VOLT_ACC = 800/50 = 16 DC_ALIGN_TOTAL_LENTH */   
#define VF_OPEN2CLOSE_TIME_M0				(1)				/* VF����������ʱʱ�� */   //20
#define VF_VOLT_ACC_M0						(1)				/* VF��ѹ���ٵ���ֵ*/
#define VF_VOLT_DEC_M0						(1)				/* VF��ѹ���ٵ���ֵ*/

/*---------------------------------��������------------------------------------*/
#define OPEN_ANGLE_TAG_FREQ_M0				(30.0)			/* ��λ��Hz �����϶�����Ƶ�� */
#define FREQ_ACC_M0							(20.0)			/* ��λ��Hz/s �����϶�Ƶ�ʼ��ٵ���ֵ */
#define FREQ_DEC_M0							(20.0)			/* ��λ��Hz/s �����϶�Ƶ�ʼ��ٵ���ֵ */
  
#define MATCH_TIME_M0						(5)				/* ����͸�������ƥ�����, ��ǰδ�ã�Ԥ�� */

#define MIN_RUN_FREQ_M0						(2.0)			/* ��λ��Hz ����ٶ� */
#define CLOSE2OPEN_FREQ_M0					(2.0)			/* ��λ��Hz �л�Ϊ�����϶�Ƶ�� */
#define CURRENTLOOP_CLOSE_FREQ_M0			(20.0)			/* ��λ��Hz �������ջ�Ƶ�� */

/*---------------------- -----�����ջ��л����ɲ���------------------------------*/
#define OPEN2CLOSE_RUN_COV_TIME_M0 			(30)			/* �����ջ��л�����ʱ�䣺��λ��mS 30*/
#define OPEN2CLOSE_RUN_CURRENT_RAMP_M0		(0.1)			/* �����ջ��л������ڣ�D,Q������仯б�ʡ���λ��A/ms */
															/* ��ǰδ�ã�Ԥ�� */

#define ID_START_M0							(0.5)			/* ��λ:A, D������趨ֵ */
#define IQ_START_M0							(0.5)			/* ��λ:A, Q������趨ֵ */

#define U_UNDVOLT_PROT_ENABLE_M0			(0)
#define U_UNDVOLT_START_M0					(16)			/* ��λ��V Ƿѹ�ָ��趨ֵ */     /* AC175 */
#define U_UNDVOLT_STOP_M0					(20)			/* ��λ��V Ƿѹ�ָ��趨ֵ */     /* AC175 */
#define U_UNDVOLT_RECOVER_M0				(22)			/* ��λ��V Ƿѹ�ָ��趨ֵ */     /* AC160 */
#define IQ_BRAKE_SET_M0						(1)			/* ��λ��A IqRef��Iq�ƶ�����ֵ */

#define SLVC_MIN_VOLTAGE_M0					(300)				// 0.01V
#define SLVC_MIN_VOLTAGE_SPD_M0				(MIN_RUN_FREQ_M0)			//	10.00Hz

/* ------------------------------�۲���PLL PI����----------------------------- */
#define PLL_KP_GAIN_M0						(16 * 32)		/* PLL_Kp ������Kp 50 16  ����32��*/
#define PLL_KI_GAIN_M0						(4 * 256)		/* PLL_Ki ������Ki 10  4  ����256��*/
#define PLL_K_FREQ0_M0						(30.0)			/* ��λ: PLL_K ������Ƶ��0 */

#define PLL_KP_RUN_M0						(20 * 32)		/* ��λ: PLL_Kp ������Kp1 */
#define PLL_KI_RUN_M0						(4 * 256)		/* ��λ: PLL_Ki ������Ki1 */
#define PLL_K_FREQ1_M0						(30.0)			/* ��λ: PLL_K ������Ƶ��1 */


#define THETA_FIL_TIME_M0					(2.0)			/* ��λ:ms, �˲�ʱ�� */
#define SPEED_FIL_TIME_M0					(6.0)			/* ��λ:ms, �˲�ʱ�� */


/*----------------------------------����������---------------------------------*/
#define IQ_SET_M0							(0.5)			/* ��λ��A IqRef��Iq����ֵ */
 
#define VQMAX_M0							(0.2)			/* Q������������   Udc�������ֵ��ĿǰΪ��3.6*41=147.6V*/
#define VQMIN_M0							(-0.2)			/* Q����С������� ĸ�ߵ�ѹ24V����˶���Ҫ�ӽ����� ��Ϊ����Ϊ0.2*/

#define VDMAX_M0							(0.2)			/* D������������*/
#define VDMIN_M0							(-0.2)			/* D����С�������*/

#define P_CURRENT_KP_M0						(0)			/* ������Kp��ʵ�����õ�Kp��������ֵ�͵��������������յ�Kp */
															/* 0 --- �Զ��������    ��0 --- �趨ֵ*/
#define P_CURRENT_KI_M0						(3000)			/* ������Ki��ʵ�����õ�Kp��������ֵ�͵��������������յ�Ki */

#define P_CUURENT_KP_RUN_M0					(1000)			/* ����ʱ�ĵ�����Kp*/ /* ��ǰδ�ã������ڶ�̬����PI���� */
#define P_CUURENT_KI_RUN_M0					(3000)			/* ����ʱ�ĵ�����Ki*/ /* ��ǰδ�ã������ڶ�̬����PI���� */

#define AUTO_FW_LIM_M0						(0.00)			/* �Զ�����D��������� A*/

#define D_CURRENT_REF_FIL_TIME_M0			(2.0)			/* ��λ��ms D�����ָ���˲�ʱ�䳣�� */
#define Q_CURRENT_REF_FIL_TIME_M0			(2.0)			/* ��λ��ms Q�����ָ���˲�ʱ�䳣�� */


/*----------------------------------�ٶȻ�����-------------------------------*/
#define POWER_LIMIT_VALUE_M0				(3.0)			/* ��λ��W   ���ƹ��ʵĴ�С */
#define POWER_LIMIT_TIME_M0					(5)				/* ��λ���ٶȻ����ڣ�  �޹��ʼ������� */
#define POWER_LIMIT_SPEED_M0				(100)			/* ��λ��Hz  �޹���ת�ٸ���������ʵ��Ӧ�������ã� */

#define SPEED_LOOP_CNTR_M0					(0)				/* ��λ��ms  �ٶȻ�·�������� */

#define STATE04_WAITE_TIME_M0				(100)			/* Unit: ms �ٶȱ�����ʼ��ʱ��100--10 */ /* ��ǰδ�ã�Ԥ�� */ 

#define P_ASR_KP_M0							(800)			/* �ٶȻ�Kp */
#define P_ASR_KI_M0							(300)			/* �ٶȻ�Ki */

#define IQMAX_M0							(0.5)			/* ��λ:A, �ٶȻ�������ֵ */
#define IQMIN_M0							(-0.5)			/* ��λ:A, �ٶȻ������Сֵ */

#define SPEED_RUN_ACC_M0					(50.00)			/* ��λ Hz/s �ٶȼ��ٵ���ֵ */
#define SPEED_RUN_DEC_M0					(50.00)			/* ��λ Hz/s �ٶȼ��ٵ���ֵ */

/*------------------------------------���ʻ�����-------------------------------*/
#define SPEED_LIMIT_VALUE_M0				(150.0)            /* ��λ��Hz  ����ת�ٵĴ�С */
#define SPEED_LIMIT_TIME_M0					(5)                /* ��λ��ms  ���ʻ����ڣ� ��ת�ټ������� */
#define POWER_LOOP_CNTR_M0					(1)                /* ��λ��ms  ���ʻ�·�������� */

#define SPEED_LIMIT_POWER_VALUE_M0			(3.00)               /* ��λ��W   ��ת�ٹ��ʸ��� */
#define POWER_SET_M0						(3.00)               /* ��λ��W  ���ʸ���ֵ */

                                                                  
#define POWER_KP_M0							(1000)             /* ���ʻ�Kp */
#define POWER_KI_M0							(500)              /* ���ʻ�Ki */

#define POWER_IQMAX_M0						(2.0)              /* ��λ:A, ���ʻ�������ֵ */
#define POWER_IQMIN_M0						(-2.0)             /* ��λ:A, ���ʻ������Сֵ */
                                                                   
#define POWER_RUN_ACC_M0					(0.1)              /* ��λ w ���ʼ��ٵ���ֵ ע��POWER_RUN_ACC��POWER_RUN_DEC����̫С�������ϲ������ã�ʵ�ʳ����еļӼ���ֵ������С��0. */
#define POWER_RUN_DEC_M0					(0.1)              /* ��λ w ���ʼ��ٵ���ֵ */

#define POWER_FAC_A0_M0						(0.0)				/* ����У��ϵ�� */
#define POWER_FAC_A1_M0						(1.0)				/* ����У��ϵ�� */
#define POWER_FAC_A2_M0						(0.0)				/* ����У��ϵ�� */

/*------------------------------------FaultDetection---------------------------*/
/* ���������� */
#define I_PH_HARD_OC_FAULT_CMP_VOLT_M0		(400)    /* ��λ��mV Ӳ�����������Ƚϵ�ѹֵ */
#define I_PH_OVERCURRENT_FAULT_M0			(3.5)				/* ��λ��A �����������趨ֵ */
#define I_PH_OVERCURRENT_FAULT_TIMES_M0		(5)		  /* ��λ��������������� */

#define CURRENT_FAULT_RECOVER_TIME_M0		(1000)	  /* ��λ��ms  �����ָ�ʱ�� */

///* ��Ƿѹ������ */
#define U_OVERVOLTAGE_FAULT_M0				(30)				/* ��λ��V ��ѹ����趨ֵ */
#define U_OVERVOLTAGE_RECOVER_M0			(26)				/* ��λ��V ��ѹ�ָ��趨ֵ */
#define U_UNDERVOLTAGE_FAULT_M0				(12)				/* ��λ��V Ƿѹ����趨ֵ */
#define U_UNDERVOLTAGE_RECOVER_M0			(18)				/* ��λ��V Ƿѹ�ָ��趨ֵ */

#define U_OVER_VOLT_DETECT_TIME_M0 			(1000)				/* ��ѹ�������ʱ�� 1ms*/
#define U_UNDE_VOLT_DETECT_TIME_M0 			(1000)				/* ��ѹ�������ʱ�� 1ms*/
#define VOLT_FAULT_RECOVER_TIME_M0 			(1000)				/* ��Ƿѹ�ָ�ʱ�� 1ms*/

#define U_OV_VOLT_PROTECT_ENABLE_M0 		(1)					/* ��ѹ��������ʹ�� */
#define U_UND_VOLT_PROTECT_ENABLE_M0 		(1)					/* ��ѹ��������ʹ�� */


///* ��ˮ��ת���� */
#define I_PH_EMPTY_FAULT_M0					(0.3)				/* ��λ��A ��ת�������趨ֵ */
#define SPEED_EMPTY_FAULT_M0				(50.0)				/* ��λ��Hz ��ת���ת���趨ֵ  */

#define EMPTY_FAULT_DETECT_TIME_M0       	(2000)				/* ��λ��ms  ��ˮ��ת���ʱ�� */
#define EMPTY_FAULT_RECOVER_TIME_M0			(2000)				/* ��λ��ms  ��ˮ��ת�ָ�ʱ�� */
#define EMPTY_PROTECTION_ENABLE_M0			(0)					/* ��ˮ��ת����ʹ�� */



///* �¶ȼ����� */
/* �¶ȴ���TEMP_MAX_ALLOWED_M0		(100.0)	��߹����¶� �����ж�Ϊ���¹���; */
/* �¶�ֵС��TEMP_MIN_ALLOWED_M0		(10.0)	��͹����¶ȣ����ж�ΪNTC��·  */
#define TEMP_BASED_M0						(25.0)				/* ��λ���� ��׼�¶� */
#define TEMP_BASED_VOLT_M0					(2.0)				/* ��λ��V  ��׼��ѹ */
#define TEMP_B_FACTOR_M0					(25.0)				/* ��׼�¶� */

#define TEMP_MAX_ALLOWED_M0					(10.0)				/* ��߹����¶� */
#define TEMP_MIN_ALLOWED_M0					(100.0)				/* ��͹����¶� */
#define PH_CUR_ALLOWED_MAXTEMP_M0			(1.5)				/* ��߹����¶�������� */
#define TEMP_HYP_ALLOWED_M0					(10.0)				/* �ͻ��¶� */

#define TEMP_PROTECT_ENABLE_M0				(0)					/* �¶ȱ���ʹ��  128*/

#define TEMP_FAULT_DETECT_TIME_M0			(1000)				/* ��λ��1ms  ���¼��ʱ�� */
#define TEMP_FAULT_RECOVER_TIME_M0			(2000)              /* ��λ��ms  ���»ָ�ʱ�� */
//                                                                  
///* ��ת������ */
/* ���������ֵ���ڴ˲��� CUR_STALL_MAX_FAULT_M0	(1.8)	��λ��A  ��ת������ƫ�����ֵ ����ת */
#define SPEED_STALL_DELAY_TIME_M0			(500.0)				/* ��λ��1ms ��ת����ӳ�ʱ�� */

#define CUR_STALL_MAX_FAULT_M0				(1.8)				/* ��λ��A  ��ת������ƫ�����ֵ */
#define CUR_STALL_MIN_FAULT_M0				(1.2)				/* ��λ��Hz ��ת����˳�������Сֵ */

#define I_PH_STALL_FAULT_M0					(0.01)				/* ��λ��A ��ת�������趨ֵ */
#define SPEED_FAC_STALL_FAULT_M0			(4.0)				/* ��λ���� ��ת���ת��ƫ��� */

#define IQ_STALL_FAULT_M0					(0.2)				/* ��λ��A ��ת�������趨ֵ */ /*δ�ã�Ԥ��*/

#define STALL_FAULT_DETECT_TIME_M0       	(2000)				/* ��λ��1ms  ��ת���ʱ�� */
#define STALL_FAULT_RECOVER_TIME_M0			(2000)				/* ��λ��1ms  ��ת�ָ�ʱ�� */

#define STALL_PROTECTION_ENABLE_M0			(1)					/* ��ת����ʹ�� */


//  �����쳣���
/* ���������ֵ���ڴ˲��� I_PH_STALL_FAULT_M0	(0.01)	��λ��A ��ת�������趨ֵ �������쳣 */
#define START_DETECT_CLOSED_TIME_M0			(3000)				/* ��λ��1ms  �ջ��쳣���ʱ�� */

#define START_DETECT_PERIOD_M0				(2000)				/* ��λ��1ms  �����쳣���ʱ�� */
#define START_FAULT_RECOVER_TIME_M0			(1000)				/* ��λ��ms  ���������ָ�ʱ�� */
#define START_PROTECTION_ENABLE_M0			(0)					/* �����쳣����ʹ�� 512*/



///* ȱ������� */
/* ���������ֵС�ڴ˲��� I_PHASE_LOSS_FAULT_M0 (6) ��ȱ�� */
#define I_PHASE_LOSS_FAULT_M0             	(6)				/* ��λ��A  ȱ��������趨ֵ */
#define LOSS_PH_FAULT_DETECT_TIME_M0		(400)				/* ��λ��ms ȱ���������趨ֵ */
#define LOSS_PH_FAULT_RECOVER_TIME_M0		(2000)				/* ��λ��ms  ȱ��ָ�ʱ�� */
#define LOSS_PH_PROTECTION_ENABLE_M0		(0)					/* ȱ���쳣����ʹ��  64*/

#endif  /* __PARAMETER0_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* -----------------------------------END OF FILE------------------------------- */

