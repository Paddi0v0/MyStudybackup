#ifndef __FUNCTION_CFG_PARAM_H
#define __FUNCTION_CFG_PARAM_H

/* ****************************** �������� ****************************************** */

#define ROTOR_SPEED_FACTOR_1US         			(s32)(1048576)						 	/* 1��ʱ�ӽ����ٶ�ֵ���ۣ�360�ȶ�Ӧ65536 */
/* ����ٱ���ֵ����Ӧһ��Hall����ֻ����1��ʱ�����ڵ���� */
/* ���ٶȼ���ϵ��,Ϊ������ٶȵı�����: һ��PWM�����ڽǶ��ۼ�ֵ = ROTOR_SPEED_FACTOR/Hall�仯����, ����Hall PLL����͵�ǰת���ٶ�ֵ���� */
#define ROTOR_SPEED_FACTOR             			(s32)(ROTOR_SPEED_FACTOR_1US*(1000000/PWM_FREQ))

#define ROTOR_SPEED_FACTOR_PER_UNIT    			(s16)(MCU_MCLK/PWM_FREQ)              	/* PWM���ڶ�Ӧ��ʱ�����ڣ����ٶ�ϵ����ϵ */
#define ROTOR_FACTOR_REDUCE_PER_UNIT   			(s16)(ROTOR_SPEED_FACTOR_PER_UNIT)    	/* ����ʱ, ���ٶ�ϵ�������ϵ */

#define MAX_SPEED_DPP                  			(ROTOR_SPEED_FACTOR/4000)			 	/* ϵͳ���Ƶ����Dppֵ */

/* ****************************** ���ݴ洢 ****************************************** */

#define HALL_LEARN_ADDR                			0x7800 							/* Hallѧϰ�洢���ַ */

#define HALL0                 					2
#define HALL1                 					6
#define HALL2                 					4
#define HALL3                 					5
#define HALL4                 					1
#define HALL5                 					3								 /* Hall��λ */

#define MOTORTYPE0             					1								 /* ����������� */

#define HALLTYPE              					120								 /* ����Ƕ� */
#define VERSION0              					0
#define VERSION1              					3
#define VERSION2              					0
#define VERSION3              					1								 /* ����汾 */
#define BEMFCOE               					0
#define HALL_OFFSET           					1600
#if (SPEED_CLOSED_LOOP == 1) 								 					 /* �ٶȻ�ѡ�� */
#define NUL1                  					1								 /* �ٶȱջ� */								
#else
#define NUL1                  					0								 /* �ٶȿ��� */								
#endif
#define NUL11              						0								 /* ��������ʹ�� */

/* ****************************** ת�Ѵ��� ****************************************** */

#define HAND_BASE                 				(20.0/(20+10.0)) 									/* ת�ѷ�ѹ�� */

#define HBSPD_BASE		    					(u16)(32767 / 3.6 * 1.1 * HAND_BASE)				/* ת��������ѹ��ֵ */	
#define HBSPD_MAX_EFFECTIVE		    			(u16)(32767 / 3.6 * 3.25 * HAND_BASE)				/* ת�������Ч��ѹ */	

#define HBSPD_EFFECTIVE_AMPLITUDE		    	(HBSPD_MAX_EFFECTIVE - HBSPD_BASE)					/* ת����ЧADC��ֵ */	

		/* ��λ����ٶ�ռ�ձ� 32760Ϊ����Ķת�� ����32760������������� ���34660 */
#define FIRST_GEAR                 				(32000 * 128 / HBSPD_EFFECTIVE_AMPLITUDE + 1) 		/* һ���ٶ� ���34660 */
#define SECOND_GEAR                 			(32000 * 128 / HBSPD_EFFECTIVE_AMPLITUDE + 1) 		/* �����ٶ� ���34660 */
#define THIRD_GEAR                 				(32000 * 128 / HBSPD_EFFECTIVE_AMPLITUDE + 1) 		/* �����ٶ� ���34660 */
#define BACK_UP_GEAR                 			(3200 * 128 / HBSPD_EFFECTIVE_AMPLITUDE + 1) 		/* �������ٶ� ���34660 */

#define HBSPD_LOW		    					(u16)(32767 / 3.6 * 0.3 * HAND_BASE)				/* ת�ѵ�ѹ�жϷ�ֵ */	
#define HBSPD_OVERHIGH 	    					(u16)(32767 / 3.6 * 3.95 * HAND_BASE)				/* ת�ѹ�ѹ�жϷ�ֵ */	

/* ****************************** �Ӽ��ٴ��� ****************************************** */

#define POWER_UP_STEP_STRONG           			12    							/* Ӳ�������ٲ��� */

/* ****************************** ��ת �˿����� ****************************************** */

#define TIME_1S6_BASE  							((u16)160)						/* ��תʱ����� */

#define KEYIN_FILTER_TIME             			60  							/* ����˿������˲�ʱ�� */  
#define KEYIN_FILTER_TIME1            			-60  							/* ����˿������˲�ʱ�� */

/* ****************************** ƫ�ò��� ****************************************** */

#define PHASE_OFFSET_MAX        				2000
#define PHASE_OFFSET_MIN        				-2000
#define ADC_GET_OFFSET_CNT      				512
#define TIME_5S_BASE_20MS       				250u

#endif /* __CONTROL_PARAM_H */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

























