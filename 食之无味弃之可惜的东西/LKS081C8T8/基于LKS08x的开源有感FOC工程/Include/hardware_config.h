#ifndef __HARDWARE_CONFIG_H_
#define __HARDWARE_CONFIG_H_

#include "lks32mc08x.h"
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x_sys.h"
#include "lks32mc08x_adc.h"
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x_flash.h"
#include "lks32mc08x_Gpio.h"
#include "lks32mc08x_cmp.h"
#include "lks32mc08x_uart.h"
#include "lks32mc08x_hall.h"
#include "lks32mc08x_tim.h"
#include "lks32mc08x_NVR.h"

/* ==============================PWM Ƶ�ʼ���������=========================== */

#define MCU_MCLK							((u32)96000000uL) 										/* PWMģ��������Ƶ */
#define PWM_MCLK							((u32)MCU_MCLK) 										/* PWMģ��������Ƶ */
#define PWM_PRSC 							((u8)0) 												/* PWMģ������Ԥ��Ƶ�� */
#define PWM_FREQ 							((u16)15625) 											/* PWMն��Ƶ�� */

#define PWM_PERIOD         					((u16) (PWM_MCLK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1)))) 	/* PWM ���ڼ�����ֵ */

#define DEADTIME_NS        					((u16)1300)												/* ����ʱ�� */
#define DEADTIME           					(u16)(((unsigned long long)PWM_MCLK * (unsigned long long)DEADTIME_NS)/1000000000uL)
 
/* ******************************��������˿ڶ���****************************************** */

#define BREAK_INPUT_MASK                  	(GPIO0_PDI & BIT3)  									/* ɲ���ź����� */ 

#define BREAK_INPUT                       	(BIT1)    												/* ɲ���ź� */

/* ******************************ADCͨ���Ŷ���****************************************** */

#define ADC0_CHANNEL_OPA0            		ADC_CHANNEL_0
#define ADC0_CHANNEL_OPA1            		ADC_CHANNEL_1
#define ADC0_CHANNEL_OPA2            		ADC_CHANNEL_2
#define ADC0_CHANNEL_OPA3            		ADC_CHANNEL_3

#define ADC0_CURRETN_A_CHANNEL       		(ADC0_CHANNEL_OPA0) 
#define ADC0_CURRETN_B_CHANNEL       		(ADC0_CHANNEL_OPA1)
#define ADC0_CURRETN_C_CHANNEL       		(ADC0_CHANNEL_OPA2)

#define ADC0_BUS_CUR_CHANNEL         		(ADC0_CHANNEL_OPA3) 

//#define PGA_GAIN_20                    		0                   							/* ��������200:10 */
//#define PGA_GAIN_9P5                   		1                   							/* ��������190:20 */
//#define PGA_GAIN_6                     		2                   							/* ��������180:30 */
//#define PGA_GAIN_4P25                  		3                   							/* ��������170:40 */
                                                                                  
#define OPA0_GIAN                      		(PGA_GAIN_20)
#define OPA1_GIAN                      		(PGA_GAIN_20 << 2)
#define OPA2_GIAN                      		(PGA_GAIN_20 << 4)
#define OPA3_GIAN                      		(PGA_GAIN_9P5 << 6)

/* ----------------------- ��һ�β���˳��MSK -------------------------------- */

#define ADC0_CUR_A_1ST_MSK      			(u16)(ADC0_CURRETN_A_CHANNEL) 					/* ADC�Ȳ���A����� */
#define ADC0_CUR_B_1ST_MSK      			(u16)(ADC0_CURRETN_B_CHANNEL) 					/* ADC�Ȳ���B����� */
#define ADC0_CUR_C_1ST_MSK      			(u16)(ADC0_CURRETN_C_CHANNEL) 					/* ADC�Ȳ���B����� */

#define ADC1_CUR_A_1ST_MSK      			(u16)(ADC0_CURRETN_A_CHANNEL<<8) 				/* ADC�Ȳ���B����� */
#define ADC1_CUR_C_1ST_MSK      			(u16)(ADC0_CURRETN_C_CHANNEL<<8) 				/* ADC�Ȳ���C����� */
#define ADC1_CUR_B_1ST_MSK      			(u16)(ADC0_CURRETN_B_CHANNEL<<8) 				/* ADC�Ȳ���B����� */

/* ----------------------- �����β���˳��MSK -------------------------------- */

#define ADC0_3th_MSK            			(u16)(ADC_CHANNEL_8) 							/* ADC����ĸ�ߵ�ѹ */
#define ADC1_3th_MSK            			(u16)(ADC0_BUS_CUR_CHANNEL<<8)  				/* ADC����ĸ�ߵ��� */

/* ----------------------- ���Ĵβ���˳��MSK -------------------------------- */

#define ADC0_4th_MSK            			(u16)(ADC_CHANNEL_11) 							/* B�෴�綯�� */
#define ADC1_4th_MSK            			(u16)(ADC_CHANNEL_9<<8) 						/* ת�ѵ�ѹ */  

#define ADC_STATE_RESET()                 	{ADC0_CFG |= BIT2;}

/* ĸ�ߵ�ѹADCͨ����������궨�� */
#define GET_BUS_VOL_ADC_RESULT         		(ADC0_DAT4)

/* ת�ѵ�ѹADCͨ����������궨�� */
#define GET_SPEED_ADC_RESULT           		(ADC0_DAT7) 

/* ĸ�ߵ���ADCͨ����������궨�� */
#define GET_BUS_ADC_RESULT             		(ADC0_DAT5)

/* оƬ�¶�ADCͨ����������궨�� */
#define GET_TEMPERTURE_ADC             		(ADC0_DAT11 >> 4)

/* ******************************PGA�������������****************************************** */

#define BUS_PGA_GAIN                      	9.05   											/* ĸ�߷Ŵ��� */
#define RESIS_NUMBER						1												/* ĸ��ȡ��������� */
#define RESISTANCE	                    	0.003 											/* ����ȡ��������ֵ */
#define SHUNT_RESISTOR                    	(RESISTANCE / RESIS_NUMBER) 					/* ȡ��������ֵ */
#define MAX_BUS_CURRENT_SETTINT           	(u16)16 										/* ����������� */
#define ABS_CURRENT_SETTINT               	(u16)12 										/* ABS���� */

#define CURRENT_ADC_PER_A                 	(u16)(SHUNT_RESISTOR*BUS_PGA_GAIN*32752/3.6) 	/* ÿ������ADCֵ */
#define CURRENT_ADC_PER_1_8A              	(u16)(CURRENT_ADC_PER_A >> 3) 					/* ÿ�˷�֮һ������ADCֵ */
#define CURRENT_LIM_VALUE                 	(u16)(MAX_BUS_CURRENT_SETTINT*SHUNT_RESISTOR*BUS_PGA_GAIN*32752/3.6) 	/* ������ADCֵ */
#define ABS_CURRENT_VALUE                 	(u16)(ABS_CURRENT_SETTINT*SHUNT_RESISTOR*BUS_PGA_GAIN*32752/3.6)  		/* ��С����ADCֵ */
 
#define PHASE_MAX_CURRENT                  	(u16)(15000 / RESIS_NUMBER)						/* ������ߵ��� */

/* ------------------------------DAC������ض��� ------------------------------ */
#define DAC_RANGE_1V2                  		1                   							/* DAC 1.2V���� */
#define DAC_RANGE_3V0                  		0                   							/* DAC 3.0V���� */
#define DAC_RANGE_4V85                 		2                   							/* DAC 4.85V���� */

/* ******************************Ƿѹ  ��ѹ �趨���� ****************************************** */

#define VOLTAGE_SHUNT_RATIO                 (1.0/(40+1.0))									/* ��ѹ�� */

#define VOLATAGE_0_2V                       (u16)(1 * VOLTAGE_SHUNT_RATIO/18*32752) 
#define VOLATAGE_0_5V                       (u16)(1 * VOLTAGE_SHUNT_RATIO/7.2*32752)  

#define LOW_VOLATAGE_THD                    (u16)(32 * VOLTAGE_SHUNT_RATIO/3.6*32752)

/////////////////////////////      48V������ѹ��غ궨��    ////////////////////////////////

#define VOLATAGE_42V                        (u16)(41.8 * VOLTAGE_SHUNT_RATIO/3.6*32752)  
#define VOLATAGE_60V                      	(u16)(60 * VOLTAGE_SHUNT_RATIO/3.6*32752)
#define VOLATAGE_62V                      	(u16)(62 * VOLTAGE_SHUNT_RATIO/3.6*32752)

#define VOLATAGE_RAMP                  		15  											/* ��ѹ�˲�б�ʿ��� 0.3V/S */


/* ****************************** BLDC������� ****************************************** */

		/* ��ǰӲ��ʹ�õĹ����Ƚ�����0���Ƚ���0��1���Ƚ���1 */
		
#define CMP_CHOICE                 			0 
#define PWM_FAIL_IO   						0x1500											/* Ĭ��������ƽ���� */

#if (CMP_CHOICE == 0) 
#define PWM_FAIL_CMPEN   					0x0091											/* FALL������Դʹ�� */
#else
#define PWM_FAIL_CMPEN   					0x00a2											/* FALL������Դʹ�� */
#endif

#define PWM_FAIL_CMPDIS   					0x0081											/* FALL������Դ�ر� */
#define MCPWM_FAIL_IO_CMPEN       	  		(PWM_FAIL_IO | PWM_FAIL_CMPEN)
#define MCPWM_FAIL_IO_CMPDIS       	  		(PWM_FAIL_IO | PWM_FAIL_CMPDIS)

#define PWM_01   							0x4141											/* ������������ */
#define PWM_23   							0x4141											/* ������������ */

#define H_PWM_L_LOW   						0x04											/* �Ϲ�ն���¹ܹر� */
#define H_PWM_L_PWM   						0x00											/* �Ϲ��¹ܻ������ */
#define H_LOW_L_HIG   						0x1c											/* �Ϲܹر��¹��ø� */
#define H_LOW_L_PWM   						0x08											/* �Ϲܹر��¹�ն�� */
#define H_LOW_L_LOW   						0x0c											/* �Ϲ��¹�ͬʱ�ر� */
#define H_HIG_L_LOW   						0x2c											/* �Ϲ��ø��¹ܹر� */

#define A_H_PWM_L_LOW   					(H_PWM_L_LOW << 0)
#define A_H_PWM_L_PWM   					(H_PWM_L_PWM << 0)
#define A_H_LOW_L_HIG   					(H_LOW_L_HIG << 0)
#define A_H_LOW_L_PWM   					(H_LOW_L_PWM << 0)
#define A_H_LOW_L_LOW   					(H_LOW_L_LOW << 0)
#define A_H_HIG_L_LOW   					(H_HIG_L_LOW << 0)

#define B_H_PWM_L_LOW   					(H_PWM_L_LOW << 8)
#define B_H_PWM_L_PWM   					(H_PWM_L_PWM << 8)
#define B_H_LOW_L_HIG   					(H_LOW_L_HIG << 8)
#define B_H_LOW_L_PWM   					(H_LOW_L_PWM << 8)
#define B_H_LOW_L_LOW   					(H_LOW_L_LOW << 8)
#define B_H_HIG_L_LOW   					(H_HIG_L_LOW << 8)

#define C_H_PWM_L_LOW   					(H_PWM_L_LOW << 0)
#define C_H_PWM_L_PWM   					(H_PWM_L_PWM << 0)
#define C_H_LOW_L_HIG   					(H_LOW_L_HIG << 0)
#define C_H_LOW_L_PWM   					(H_LOW_L_PWM << 0)
#define C_H_LOW_L_LOW   					(H_LOW_L_LOW << 0)
#define C_H_HIG_L_LOW   					(H_HIG_L_LOW << 0)

#define EN_HPWM_FUNC()                    	{MCPWM_PRT = 0x0000DEAD;MCPWM_IO01 = PWM_01;MCPWM_IO23 = PWM_23;MCPWM_PRT = 0x0000CAFE;}

											/* 1_step A�����գ�B��PWM C��ON ������½��ع����¼� */
#define A_OFF__B_PWM__C_ON()      			{MCPWM_IO01 = PWM_01 | (A_H_LOW_L_LOW | B_H_PWM_L_LOW); MCPWM_IO23 = PWM_23 | C_H_LOW_L_HIG;} 	

											/* 2_step A��ON��B��PWM C������ ����������ع����¼� */
#define A_ON__B_PWM__C_OFF()      			{MCPWM_IO01 = PWM_01 | (A_H_LOW_L_HIG | B_H_PWM_L_LOW); MCPWM_IO23 = PWM_23 | C_H_LOW_L_LOW;}

											/* 3_step A��ON��B������ C��PWM ������½��ع����¼� */
#define A_ON__B_OFF__C_PWM()      			{MCPWM_IO01 = PWM_01 | (A_H_LOW_L_HIG | B_H_LOW_L_LOW); MCPWM_IO23 = PWM_23 | C_H_PWM_L_LOW;}

											/* 4_step A�����գ�B��ON C��PWM  ����������ع����¼�*/
#define A_OFF__B_ON__C_PWM()      			{MCPWM_IO01 = PWM_01 | (A_H_LOW_L_LOW | B_H_LOW_L_HIG); MCPWM_IO23 = PWM_23 | C_H_PWM_L_LOW;}

											/* 5_step A��PWM��B��ON C������  ������½��ع����¼�*/
#define A_PWM__B_ON__C_OFF()      			{MCPWM_IO01 = PWM_01 | (A_H_PWM_L_LOW | B_H_LOW_L_HIG); MCPWM_IO23 = PWM_23 | C_H_LOW_L_LOW;}

											/* 6_step A��PWM��B������ C ON����������ع����¼� */
#define A_PWM__B_OFF__C_ON()      			{MCPWM_IO01 = PWM_01 | (A_H_PWM_L_LOW | B_H_LOW_L_LOW); MCPWM_IO23 = PWM_23 | C_H_LOW_L_HIG;}


#define FAIL0_ERROR          				0x10  											/* FALL0�������� */
#define FAIL1_ERROR          				0x20  											/* FALL1�������� */

#endif
 
 
