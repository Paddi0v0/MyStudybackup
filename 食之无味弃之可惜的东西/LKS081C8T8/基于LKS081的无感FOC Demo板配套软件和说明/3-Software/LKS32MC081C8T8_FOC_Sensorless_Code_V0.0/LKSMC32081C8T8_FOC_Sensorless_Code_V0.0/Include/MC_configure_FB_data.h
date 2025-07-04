/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� config_data.h
 * �ļ���ʶ��
 * ����ժҪ�� config data
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��6��28��
 *
 *******************************************************************************/
 
/*------------------------------prevent recursive inclusion -------------------*/ 
#ifndef __MC_CONFIG_FB_DATA_H
#define __MC_CONFIG_FB_DATA_H

#include "basic.h"
#include "MC_Type.h"

//////////////////////////////////////////////
//�ڲ����������������ݣ������޸�
#define DOWNLOAD_WRITE_REQ1				(0x43)
#define DOWNLOAD_WRITE_REQ2				(0x54)
#define DOWNLOAD_WRITE_REQ3				(0x59)


#define DOWNLOAD_READ_ACK_INVLAD		(0x00)
#define DOWNLOAD_READ_ACK1				(0x44)
#define DOWNLOAD_READ_ACK2				(0x53)
#define DOWNLOAD_READ_ACK_DOING			(0x64)
#define DOWNLOAD_READ_ACKOK				(0x65)
//////////////////////////////////////////////

/*---------------------------------FB����------------------------------------*/
// Function Block ID definition, 
// Don't modify FB ID, which is matched with PC configure tool.
// FB descriptor should be matched with PC configure tool
#define ID_FB_BOARD							(0)
#define ID_FB_MOTOR							(1)
#define ID_FB_SLVCCFG						(2)
#define ID_FB_CURLOOP						(3)
#define ID_FB_SPDLOOP						(4)
#define ID_FB_PWRLOOP						(5)
#define ID_FB_DIRCHECK						(6)
#define ID_FB_IPD							(7)
#define ID_FB_ALLIGN						(8)
#define ID_FB_VF							(9)
#define ID_FB_DCUNIT						(10)
#define ID_FB_CURSENSOR						(11)
#define ID_FB_TEMPER						(12)
#define ID_FB_EMPTY_STALL					(13)
#define ID_FB_ACUNIT						(14)

#define ID_FB_GLOBAL						(0xFE)


#if 0
typedef struct
{
	u8		FBID;
	u8		FBMode;
	u16		FBSize;
}stru_FB_PRE_DEF;
#endif



/////////////////////Ӧ������/////////////////////////
#define NOT_APPLICABLE                 (0)               /* δ���� */


///////////////////////////////////////////////////////////////////////////////
////////////////////Global config para   //////////////////////////////////////
typedef const struct
{   /* ȫ������ */
    s16	  m_nMCUClkFreq;            	/* MCU CLOCK��Ƶ					*/
    s16	  m_nPWMClkFreq;              	/* PWMģ�� CLOCK Ƶ��				*/
	
	u16   m_nuPWMFreq;              	/* PWM��� �ز� Ƶ��				*/
	u16	  m_nuPWMPrsc;              	/* PWMģ��Ԥ��Ƶ					*/
	
	u16   m_nuSpdloopFreq;				/* �ٶȻ�ִ��Ƶ��					*/
	s16   m_nOverModuFactor;            /* ��ѹ����ϵ��						*/
	
	u16	  m_nuDeadZone;              	/* ����ʱ��  ns						*/
	u8	  m_bSampType;              	/* ����������ʽ 1 --- ������.....	*/
	u8	  m_bSVPWMType;					/* SVPWM��ʽ 0 -- 7Seg  1 --- 5Seg 	*/
	u8    m_bPWMUsedID;					/* ѡ�õ�PWM��AD�˿� 0--- PWM0  	*/
	u8	  m_bOPAValue;					/* ��������OPAѡ��					*/	
    u8	  m_buBemfDirEnable;      		/* ʹ�ܷ�����˳�����				*/
	u8    m_buChargeEnale;				/* ʹ��Ԥ��繦��					*/
	
    u8	  m_buIPDEnable;      			/* ʹ��IPD							*/	
    u8	  m_buDirCheckEnable;      		/* ʹ��˳�����					*/
	u8	  m_buAllignEnable;      		/* ʹ�ܶ���							*/	
    u8	  m_buVFStartEnable;      		/* ʹ��VF����						*/
	
	u8	  m_buACDetectEnable;      		/* ʹ��AC���						*/	
	u8	  m_buCtrLoopmode;      		/* ����ģʽ 0 �ٶȻ� 1-				*/
	u8	  m_buPowerLmtInSpeedmode;		/* �ٶȻ� ���ƹ���					*/	
	u8	  m_buSpdLmtInPowermode;		/* ���ʻ� �����ٶ�					*/
	
	u8	  m_buWeaenFluxEnable;			/* ���ſ���ʹ��						*/	
	u8	  m_buStopPWMEnable;			/* ֹͣʱPWMʹ�� 					*/
	u8	  m_buFaultAutoClr;				/* �����Զ����ʹ��					*/

	u8	  m_buStopMode;					/* ֹͣ��ʽ							*/
	
	u16   m_nuADCCaliTimes;				/* ADУ������						*/	
	u16   m_nuADCOffsetError;			/* ADУ������ƫ��					*/	
	u16   m_nuChargeTime;				/* ���ʱ��							*/
	u16   m_nuFaultClrPrd;				/* �����������						*/
	
} STR_GlobalCfg,*PSTR_GlobalCfg;

#define GLOBAL_CFG_TAB_M(ID)											\
{  		/* ȫ������ ���� */												\
	(s16)(MCU_MCLK_M##ID),				/* MCU CLOCK��ƵMHZ		*/		\
	(s16)(PWM_MCLK_M##ID),				/* PWM ģ��CLOCK MHZ	*/		\
	(u16)PWM_FREQ_M##ID,				/* PWM��� �ز� Ƶ�� 	*/		\
	(u16)PWM_PRSC_M##ID,				/* PWMģ��Ԥ��Ƶ		*/		\
	(u16)SPD_LOOP_FREQUENCY_M##ID,		/* �ٶȻ�ִ��Ƶ��		*/		\
	(u16)OVER_MODU_FACTOR_M##ID,		/* ��ѹ����ϵ��			*/		\
	(u16)DEADTIME_NS_M##ID,				/* ����ʱ��  ns			*/		\
	(u8)CURRENT_SAMPLE_TYPE_M##ID,		/* ����������ʽ			*/		\
	(u8)SVPWM_TYPE_M##ID,				/* SVPWM��ʽ			*/		\
																		\
	(u8)PWM_USED_ID_M##ID,				/* ѡ�õ�PWM��AD�˿�	*/		\
	(u8)OPA_SELECT_M##ID,				/* ѡ�õ�OPA�Ŵ�ѡ��	*/		\
	(u8)DIR_EMF_CHECK_ENABLE_M##ID,		/* ʹ�ܷ�����˳�����	*/		\
	(u8)CHARGE_STATUS_M##ID,			/* ʹ��Ԥ��繦��		*/		\
																		\
	(u8)SEEK_POSITION_STATUS_M##ID,		/* ʹ��IPD				*/		\
	(u8)DIR_CHECK_STATUS_M##ID,			/* ʹ��˳�����		*/		\
	(u8)ALLIGN_STATUS_M##ID,			/* ʹ�ܶ���				*/		\
	(u8)VF_START_STATUS_M##ID,			/* ʹ��VF����			*/		\
																		\
	(u8)AC_SENOR_PROTECT_STATUS_M##ID,	/* ʹ��AC���			*/		\
	(u8)CLOSE_LOOP_M##ID,				/* ����ģʽ				*/		\
	(u8)POWER_LIMIT_STATUS_M##ID,		/* �ٶȻ� ���ƹ���		*/		\
	(u8)SPPED_LIMIT_STATUS_M##ID,		/* ���ʻ� �����ٶ�		*/		\
																		\
	(u8)WEAKEN_FLUX_STATUS_M##ID,		/* ���ſ���ʹ��			*/		\
	(u8)PWM_ENABLE_STOP_M##ID,			/* ֹͣʱPWMʹ��		*/		\
	(u8)FAULT_AUTO_CLR_M##ID,			/* �����Զ����ʹ��		*/		\
	(u8)STOP_MODE_M##ID,				/* ֹͣ��ʽ				*/		\
																		\
	(u16)CALIB_SAMPLES_M##ID,			/* ADУ������			*/		\
	(u16)ADC_OFFSET_ERROR_M##ID,		/* ADУ������ƫ��		*/		\
																		\
	(u16)CHARGE_TIME_M##ID,				/* ���ʱ��				*/		\
	(u16)FAULT_CLR_PRIOD_M##ID			/* �����������			*/		\
}





//////////////////////////////////////////////////////////////////////
////////////////////Board para   /////////////////////////////////////

typedef const struct
{   /* ���ʰ�Ӳ����·���������ò��� */
    s16 m_nRatedUdc;            		/* �ֱ��ĸ�ߵ�ѹ  0.1V	*/
    s16 m_nMaxUdc;              		/* ���ֱ��ĸ�ߵ�ѹ 		*/			// 0
    s32 m_nMaxCur;              		/* �������� ��λ��0.1A 	*/			// 0
  
	//HD  Ӳ��������
    s32	  m_wAdcSupplyVol;      		/* ADC�ο���ѹ 	��λ��0.001���� 	*/ 
    s32	  m_wRShunt;            		/* ��ѹ���� 	��λ��0.001��ŷķ	*/
    s32	  m_wAmpGain;           		/* �Ŵ����Ŵ��� ��λ��0.000001��	*/ 
    s32	  m_wVolShuntRatio;     		/* ĸ�ߵ�ѹ������ѹ�� 	0.000001�� 	*/ 
    s32	  m_wBemfShuntRatio;    		/* �����Ƶ�ѹ������ѹ�� 0.000001�� 	*/
} STR_BoardParameter,*PSTR_BoardParameter;//stru_BoardParameterDef, *pStru_BoardParameterDef;

#define BOARD_PARAMETER_TAB_M(ID)									\
{  		/* ���ʰ�Ӳ����·���Բ����� */								\
	(s16)(NOT_APPLICABLE),			/* �ֱ��ĸ�ߵ�ѹ 0.1V		*/	\
	(s16)NOT_APPLICABLE,				/* ���ֱ��ĸ�ߵ�ѹ 			*/	\
	(s32)NOT_APPLICABLE,				/* �������� ��λ��A			*/	\
	(s32)(1000*1000*ADC_SUPPLY_VOLTAGE_M##ID),	/* ADC�ο���ѹ  ��λ��0.000001���� */	\
	(s32)(1000*1000*RSHUNT_M##ID),				/* ��ѹ����     ��λ��0.001��ŷķ*/	\
	(s32)(1000*1000*AMPLIFICATION_GAIN_M##ID),	/* �Ŵ����Ŵ��� ��λ��0.000001�� */	\
	(s32)(1000*1000*VOLTAGE_SHUNT_RATIO_M##ID),	/* ĸ�ߵ�ѹ������ѹ�� 	0.000001��  */	\
	(s32)(1000*1000*BEMF_SHUNT_RATIO_M##ID)		/* �����Ƶ�ѹ������ѹ�� 0.000001��  */	\
}

//////////////////////////////////////////////////////////////////////
////////////////////Motor para   /////////////////////////////////////

typedef const struct
{   /* ��������ṹ�� */
    s8  m_bMotorType;           		/* 0:im,1:spm,2:ipm,3...*/
	s8  m_bPolePairs;					/* ����ż����� */
	u16  m_nuReserved;	
	
    s32		m_wRatedFreq;				/* �Ƶ�� ��λ��0.01Hz*/
    s32		m_nRatedCur;				/* ����� ��λ��0.1A */
    s32		m_wMotorRs;					/* ������� ��λ��0.0001ŷķ */
    s32		m_wMotorLd;					/* ���D���� ��λ��uH */
    s32		m_wMotorLq;             	/* ���Q���� ��λ��uH */
    s32		m_wFluxConst;           	/* ����������� WB */	
}STR_MotorParameter, *PSTR_MotorParameter;

#define MOTOR_PARAMETER_TAB_M(ID)									\
{																	\
	NOT_APPLICABLE,					/* 0:im,1:spm,2:ipm,3...*/		\
	U_MOTOR_PP_M##ID,				/* ����ż����� */				\
	NOT_APPLICABLE,					/* Ԥ��       */				\
	(s32)(100*U_MAX_FREQ_M##ID),			/* �Ƶ�� ��λ��0.01Hz*/		\
	(s32)(NOT_APPLICABLE),					/* ����� ��λ��0.1A */		\
	(s32)(10000*U_MOTOR_RS_M##ID),			/* ������� ��λ��0.0001ŷķ */	\
	(s32)U_MOTOR_LD_M##ID,					/* ���D���� ��λ��uH */		\
	(s32)U_MOTOR_LQ_M##ID,					/* ���Q���� ��λ��uH */		\
	(s32)(100000*U_MOTOR_FLUX_CONST_M##ID),	/* ����������� WB */			\
}

//////////////////////////////////////////////////////////////////////
////////////////////SLVC0 para   /////////////////////////////////////

typedef struct
{	
	s32		m_wCloseFreq;						//�����϶�����Ƶ��				1 0.01Hz
	s32		m_wOpenFreq;						//��������Ƶ��					1 0.01Hz
	s32		m_wMinRunFreq;						//�������Ƶ��					1 0.01Hz
	s32		m_wCurCloseFreq;					//�����ջ�Ƶ��					1 0.01Hz
	s32		m_wFreq_Acc;						//�����϶�Ƶ�ʼ��ٵ���ֵ		1 0.01Hz
	s32		m_wFreq_Dec;						//�����϶�Ƶ�ʼ��ٵ���ֵ		1
	s32		m_wDQCurRatioOpen2Close;			//���ջ��л�ʱDQ��仯��		1 0.001A/s
	u16		MatchTimes2Close;					//�ջ�ʱ����͵���ƥ�����		0
	u16		TransTime2Close;					//�����ջ�����ʱ��				0

	s32		m_wImax;							//���������					1 0.001A
	s32		m_wIdStartSet;						//D������趨					1 0.001A
	s32		m_wIqStartSet;						//Q������趨					1 0.001A	
	s32		m_wIqBrake;							//Q���ƶ������趨				1 0.001A
	s32		m_wBusVoltStart;					//ĸ�ߵ�ѹ������ѹֵ			1
	s32		m_wBusVoltEnd;						//ĸ�ߵ�ѹ��������ֵ			1
	s32		m_wBusVoltRestore;					//ĸ�ߵ�ѹ�����ָ�ֵ			1
	u8		bVoltageProtectionEnable;			//��ѹ����ģʽ����				1
	u8		StopModeInVoltProtect;				//��ѹ����ģʽʱ���ֹͣ��ʽ	1
	s16		m_VFDeadZoneComp;					//VF������������ϵ��			1 0~32767
	
	s32		m_wThetaFilTime;					//�Ƕ��˲�ʱ��					1 0.1ms
	s32		m_wSpeedFilTime;					//�ٶ��˲�ʱ��					1 0.1ms
	s16		nPLLKp0;							//PLL_Kp ������Kp0				1
	s16		nPLLKi0;							//PLL_Kp ������Ki0				1
	s16		nPLLFreq0;							//PLLƵ�ʵ�0					1
	s16		nPLLKp1;							//PLL_Kp ������Kp1				1
	s16		nPLLKi1;							//PLL_Kp ������Ki1				1
	s16		nPLLFreq1;							//PLLƵ�ʵ�1					1
	u16		Reseved1;							//Ԥ��							0
	u16		Reseved2;							//Ԥ��							0
}STR_FB_SLVC_CFG0;

#define SLVC0_PARAMETER_TAB_M(ID)										\
{																		\
	(s32)(100*OPEN_ANGLE_TAG_FREQ_M##ID),		/* �����϶�����Ƶ��*/	\
	(s32)(100*CLOSE2OPEN_FREQ_M##ID),			/* ��������Ƶ�� */		\
	(s32)(100*MIN_RUN_FREQ_M##ID),				/* �������Ƶ�� */		\
	(s32)(100*CURRENTLOOP_CLOSE_FREQ_M##ID),	/* �����ջ�Ƶ�� */		\
	(s32)(100*FREQ_ACC_M##ID),					/* ����Ƶ�ʼ��ٵ���ֵ*/	\
	(s32)(100*FREQ_DEC_M##ID),					/* ����Ƶ�ʼ��ٵ���ֵ*/	\
	(s32)(1000*OPEN2CLOSE_RUN_CURRENT_RAMP_M##ID),	/* DQ��仯��*/			\
	(u16)MATCH_TIME_M##ID,						/* ƥ�����*/			\
	(u16)OPEN2CLOSE_RUN_COV_TIME_M##ID,			/* ����ʱ��*/			\
	(s32)(1000*IQMAX_M##ID),					/* ��������� */		\
	(s32)(1000*ID_START_M##ID),					/* D������趨 */		\
	(s32)(1000*IQ_START_M##ID),					/* Q������趨 */		\
	(s32)(1000*IQ_BRAKE_SET_M##ID),				/* Q���ƶ����� */		\
	(s32)(100* U_UNDVOLT_START_M##ID),			/* ��ѹ������ѹֵ */	\
	(s32)(100* U_UNDVOLT_STOP_M##ID),			/* ��ѹ��������ֵ */	\
	(s32)(100* U_UNDVOLT_RECOVER_M##ID),		/* ��ѹ�����ָ�ֵ */	\
	(u8)U_UNDVOLT_PROT_ENABLE_M##ID,			/* ��ѹ����ģʽ���� */	\
	(u8)NOT_APPLICABLE,							/* ��ѹ����ֹͣ��ʽ*/	\
	(s16)VF_DEADZONE_COMP_FACTOR_M##ID,			/* VF������������ϵ��*/	\
	(s32)(10*THETA_FIL_TIME_M##ID),				/* �Ƕ��˲�ʱ�� */		\
	(s32)(10*SPEED_FIL_TIME_M##ID),				/* �ٶ��˲�ʱ�� */		\
	(s16)PLL_KP_GAIN_M##ID,						/* PLL_Kp0*/			\
	(s16)PLL_KI_GAIN_M##ID,						/* PLL_Ki0*/			\
	(s16)(10* PLL_K_FREQ0_M##ID),				/* PLLƵ�ʵ�0 0.1Hz*/	\
	(s16)PLL_KP_RUN_M##ID,						/* PLL_Kp1*/			\
	(s16)PLL_KI_RUN_M##ID,						/* PLL_Ki1*/			\
	(s16)(10* PLL_K_FREQ1_M##ID),				/* PLLƵ�ʵ�1 0.1Hz*/	\
	(u16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(u16)NOT_APPLICABLE							/* Ԥ��	0*/				\
}

//////////////////////////////////////////////////////////////////////
////////////////////Cur loop para   /////////////////////////////////////

typedef struct
{	
	s32		m_wIqSet;							//Q������趨ֵ	1		0.001A
	s16		nVqMax;								//Q���ѹ���ֵ	1
	s16 	nVqMin;								//Q���ѹ��С 1
	s16 	nVdMax;								//d���ѹ���ֵ	1
	s16 	nVdMin;								//d���ѹ��С 1
	u16		Kp;									//Kp	1
	u16		Ki;									//Ki	1
	s16		nWeakenCurMax;						//������ŵ���	1		0.01A
	s16		Reserved;							//����	0
	s32		m_wDCurFilTimeSet;					//����ָ���˲�ʱ��	1	0.1ms
	s32		m_wQCurFilTimeSet;					//����ָ���˲�ʱ��	1	0.1ms
}STR_FB_CUR_LOOP;


#define CURLOOP_PARAMETER_TAB_M(ID)										\
{																		\
	(s32)(1000*IQ_SET_M##ID),					/*Q������趨ֵ*/		\
	(s16)(32767.0 * VQMAX_M##ID),				/*Q���ѹ���ֵ*/		\
	(s16)(32767.0 * VQMIN_M##ID),				/*Q���ѹ��Сֵ*/		\
	(s16)(32767.0 * VDMAX_M##ID),				/*D���ѹ���ֵ*/		\
	(s16)(32767.0 * VDMIN_M##ID),				/*D���ѹ��Сֵ*/		\
	(u16)P_CURRENT_KP_M##ID,					/*Kp			*/		\
	(u16)P_CURRENT_KI_M##ID,					/*Ki			*/		\
	(u16)(100*AUTO_FW_LIM_M##ID),				/*������ŵ���	*/		\
	(u16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)(10*D_CURRENT_REF_FIL_TIME_M##ID),		/* ����ָ���˲�ʱ��*/	\
	(s32)(10*Q_CURRENT_REF_FIL_TIME_M##ID)		/* ����ָ���˲�ʱ��*/	\
}

#define CURLOOP_PARAMETER_TAB_RSVD_M(ID)								\
{																		\
	(s32)NOT_APPLICABLE,						/*Q������趨ֵ*/		\
	(s16)NOT_APPLICABLE,						/*Q���ѹ���ֵ*/		\
	(s16)NOT_APPLICABLE,						/*Q���ѹ��Сֵ*/		\
	(s16)NOT_APPLICABLE,						/*D���ѹ���ֵ*/		\
	(s16)NOT_APPLICABLE,						/*D���ѹ��Сֵ*/		\
	(u16)NOT_APPLICABLE,						/*Kp			*/		\
	(u16)NOT_APPLICABLE,						/*Ki			*/		\
	(u16)NOT_APPLICABLE,						/*������ŵ���	*/		\
	(u16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)NOT_APPLICABLE,						/* �������ٵ���ֵ*/		\
	(s32)NOT_APPLICABLE							/* �������ٵ���ֵ*/		\
}


//////////////////////////////////////////////////////////////////////
////////////////////Speed loop para   /////////////////////////////////////

typedef struct
{	
	s32		m_wPowerLmtValue;					//���ƹ��ʴ�С		1	0.01W
	s16		nPowerLmtCalcCycle;					//���ƹ��ʼ�������	1
	s16		Reserved;							//����	0
	s32		m_wPowerLmtSpdValue;				//�޹���ת�ٸ���	1	0.01Hz
	s16		nSpeedLoopCycle;					//�ٶȻ���������	1
	s16		nState04WaitTime;					//�ٶȱ�����ʼ��ʱ��	1
	u16		PASRKp;								//�ٶȻ�Kp	1
	u16		PASRKi;								//�ٶȻ�Ki	1
	s32		m_wIqMax;							//�ٶȻ�������ֵ	1	0.001A
	s32		m_wIqMin;							//�ٶȻ������Сֵ	1	0.001A
	s32		m_wSpdRunAcc;						//�ٶȼ��ٵ���ֵ	1	0.01Hz
	s32		m_wSpdRunDec;						//�ٶȼ��ٵ���ֵ	1	0.01Hz	
}STR_FB_SPD_LOOP;

#define SPDLOOP_PARAMETER_TAB_M(ID)										\
{																		\
	(s32)(100*POWER_LIMIT_VALUE_M##ID),			/*���ƹ��ʴ�С*/		\
	(s16)POWER_LIMIT_TIME_M##ID,				/*���ƹ��ʼ�������*/	\
	(s16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)(100*POWER_LIMIT_SPEED_M##ID),			/*�޹���ת�ٸ���*/		\
	(s16)SPEED_LOOP_CNTR_M##ID,					/*�ٶȻ���������*/		\
	(s16)STATE04_WAITE_TIME_M##ID,				/*�ٶȱ�����ʼ��ʱ��*/	\
	(u16)P_ASR_KP_M##ID,						/*�ٶȻ�Kp		*/		\
	(u16)P_ASR_KI_M##ID,						/*�ٶȻ�Ki		*/		\
	(s32)(1000*IQMAX_M##ID),					/*�ٶȻ�������ֵ*/	\
	(s32)(1000*IQMIN_M##ID),					/*�ٶȻ������Сֵ*/	\
	(s32)(100*SPEED_RUN_ACC_M##ID),				/*�ٶȼ��ٵ���ֵ*/		\
	(s32)(100*SPEED_RUN_DEC_M##ID)				/*�ٶȼ��ٵ���ֵ*/		\
}

#define SPDLOOP_PARAMETER_TAB_RSVD_M(ID)										\
{																		\
	(s16)NOT_APPLICABLE,						/*���ƹ��ʴ�С*/		\
	(s16)NOT_APPLICABLE,						/*���ƹ��ʼ�������*/	\
	(s16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)NOT_APPLICABLE,						/*�޹���ת�ٸ���*/		\
	(s16)NOT_APPLICABLE,						/*�ٶȻ���������*/		\
	(s16)NOT_APPLICABLE,						/*�ٶȱ�����ʼ��ʱ��*/	\
	(u16)NOT_APPLICABLE,						/*�ٶȻ�Kp		*/		\
	(u16)NOT_APPLICABLE,						/*�ٶȻ�Ki		*/		\
	(s32)NOT_APPLICABLE,						/*�ٶȻ�������ֵ*/	\
	(s32)NOT_APPLICABLE,						/*�ٶȻ������Сֵ*/	\
	(s32)NOT_APPLICABLE,						/*�ٶȼ��ٵ���ֵ*/		\
	(s32)NOT_APPLICABLE							/*�ٶȼ��ٵ���ֵ*/		\
}


//////////////////////////////////////////////////////////////////////
////////////////////Power loop para   /////////////////////////////////////


typedef struct
{	
	
	s32		m_wSpdLmtValue;						//����ת�ٴ�С	1			0.01Hz
	s16		nPLoopSpdLmtCalcCycle;				//���ʻ���ת�ټ�������	1
	s16		nPLoopCalcCycle;					//���ʻ���������	1
	s32		m_wLmdSpdPowerSetting;				//��ת�ٹ��ʸ���	1		0.01W
	s32		m_wPowerSetting;					//���ʸ���	1				0.01W	
	u16		nPowerKp;							//���ʻ�Kp	1
	u16 	nPowerKi;							//���ʻ�Ki	1
	s32		m_wPLoopIqMax;						//���ʻ����Iq���ֵ	1
	s32		m_wPLoopIqMin;						//���ʻ����Iq��Сֵ	1
	s32		m_wPowerRunAcc;						//���ʼ��ٵ���ֵ	1
	s32		m_wPowerRunDec;						//���ʼ��ٵ���ֵ	1
		
	s32		m_wPowerFacA0;						//���ʼ�������ϵ��a0	1
	s32		m_wPowerFacA1;						//���ʼ�������ϵ��a1	1
	s32		m_wPowerFacA2;						//���ʼ�������ϵ��a2	1
		
}STR_FB_POWER_LOOP;

#define PWRLOOP_PARAMETER_TAB_M(ID)										\
{																		\
	(s32)(100*SPEED_LIMIT_VALUE_M##ID),			/*����ת�ٴ�С*/		\
	(s16)SPEED_LIMIT_TIME_M##ID,				/*��ת�ټ�������*/		\
	(s16)POWER_LOOP_CNTR_M##ID,					/*���ʻ���������*/		\
	(s32)(100*SPEED_LIMIT_POWER_VALUE_M##ID),	/*��ת�ٹ��ʸ���*/		\
	(s32)(100*POWER_SET_M##ID),					/*���ʸ���*/			\
	(u16)POWER_KP_M##ID,						/*���ʻ�Kp		*/		\
	(u16)POWER_KI_M##ID,						/*���ʻ�Ki		*/		\
	(s32)(1000*POWER_IQMAX_M##ID),				/*���ʻ����Iq���ֵ*/	\
	(s32)(1000*POWER_IQMIN_M##ID),				/*���ʻ����Iq��Сֵ*/	\
	(s32)(100*POWER_RUN_ACC_M##ID),				/*���ʼ��ٵ���ֵ*/		\
	(s32)(100*POWER_RUN_DEC_M##ID),				/*���ʼ��ٵ���ֵ*/		\
	(s32)(4096*POWER_FAC_A0_M##ID),				/*���ʼ�������ϵ��a0*/	\
	(s32)(4096*POWER_FAC_A1_M##ID),				/*���ʼ�������ϵ��a1*/	\
	(s32)(4096*POWER_FAC_A2_M##ID)				/*���ʼ�������ϵ��a2*/	\
}

#define PWRLOOP_PARAMETER_TAB_RSVD_M(ID)								\
{																		\
	(s32)NOT_APPLICABLE,						/*����ת�ٴ�С*/		\
	(s16)NOT_APPLICABLE,						/*��ת�ټ�������*/		\
	(s16)NOT_APPLICABLE,						/*���ʻ���������*/		\
	(s32)NOT_APPLICABLE,						/*��ת�ٹ��ʸ���*/		\
	(s32)NOT_APPLICABLE,						/*���ʸ���*/			\
	(u16)NOT_APPLICABLE,						/*���ʻ�Kp		*/		\
	(u16)NOT_APPLICABLE,						/*���ʻ�Ki		*/		\
	(s32)NOT_APPLICABLE,						/*���ʻ����Iq���ֵ*/	\
	(s32)NOT_APPLICABLE,						/*���ʻ����Iq��Сֵ*/	\
	(s32)NOT_APPLICABLE,						/*���ʼ��ٵ���ֵ*/		\
	(s32)NOT_APPLICABLE,						/*���ʼ��ٵ���ֵ*/		\
	(s32)NOT_APPLICABLE,						/*���ʼ�������ϵ��a0*/	\
	(s32)NOT_APPLICABLE,						/*���ʼ�������ϵ��a1*/	\
	(s32)NOT_APPLICABLE							/*���ʼ�������ϵ��a2*/	\
}


//////////////////////////////////////////////////////////////////////
////////////////////˳��� para   /////////////////////////////////////


typedef struct
{
	s16		nSpdTrackDelayTime;					//˳�����ʱ��	1
	s16		Reserved;							//		0
	s32		m_wSpdTrackOnFreq;					//˳���бջ�Ƶ��	1		0.01Hz
	s32		m_wBrakeFreqAntiDirec;				//���ɲ��Ƶ��		1		0.01Hz
	s32		m_wMotorStopCurrentThd;				//���ͣת���������ֵ	1	0.001A
	s32		m_wMotorStopCurDifThd;				//���ͣת������ֵ�����ֵ	1
	s16		nMotorStopTime;						//���ֹͣ����˲�ʱ��	1
	s16		nMotorStopDelayTime;				//���ֹͣ���ӳ�ʱ��	1
	s16		nBemfAngComp;						//�����ƽǶȲ��� ��		1
	s16		nBemfTrackMinVolt;					//�����Ƽ����С��ѹ	1
}STR_FB_DIR_CHECK;

#define DIR_CHECK_PARAMETER_TAB_M(ID)									\
{																		\
	(s16)SPEED_TRACK_DELAYTIME_M##ID,			/*˳�����ʱ��*/		\
	(s16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)(100*SPEED_TRACK_ON_FREQ_THH_M##ID),	/*˳���бջ�Ƶ��*/		\
	(s32)(100*EBRK_ON_FREQ_THH_M##ID),			/*���ɲ��Ƶ��  */		\
	(s32)(1000*MOTOR_STOP_CUR_THD_M##ID),		/*ͣת���������ֵ*/	\
	(s32)(1000*MOTOR_STOP_CUR_DIF_THD_M##ID),	/*ͣת������ֵ�����ֵ*/\
	(s16)STOP_TIME_M##ID,						/*ֹͣ����˲�ʱ��*/	\
	(s16)STOP_DELAY_TIME_M##ID,					/*ֹͣ���ӳ�ʱ��*/		\
	(s16)BEMF_ANG_COMP_M##ID,					/*�����ƽǶȲ��� ��*/	\
	(s16)BEMF_TRACK_MIN_VOLT_M##ID				/*�����Ƽ����С��ѹ*/	\
}

#define DIR_CHECK_PARAMETER_TAB_RSVD_M(ID)								\
{																		\
	(s16)NOT_APPLICABLE,						/*˳�����ʱ��*/		\
	(s16)NOT_APPLICABLE,						/* Ԥ��	0*/				\
	(s32)NOT_APPLICABLE,						/*˳���бջ�Ƶ��*/		\
	(s32)NOT_APPLICABLE,						/*���ɲ��Ƶ��  */		\
	(s32)NOT_APPLICABLE,						/*ͣת���������ֵ*/	\
	(s32)NOT_APPLICABLE,						/*ͣת������ֵ�����ֵ*/\
	(s16)NOT_APPLICABLE,						/*ֹͣ����˲�ʱ��*/	\
	(s16)NOT_APPLICABLE,						/*ֹͣ���ӳ�ʱ��*/		\
	(s16)NOT_APPLICABLE,						/*�����ƽǶȲ��� ��*/	\
	(s16)NOT_APPLICABLE							/*�����Ƽ����С��ѹ*/	\
}



//////////////////////////////////////////////////////////////////////
////////////////////IPD para   /////////////////////////////////////

typedef struct
{
	u32		m_wuInitAngComp;					//��ʼλ�ü�ⲹ���Ƕ�	1		65536 ---360
	u16		nuIPDPlusTime;						//����ע��ʱ��������	1
	u16		nuIPDWaitTime;						//������еȴ�ʱ��������	1
}STR_FB_IPD;

#define IPD_PARAMETER_TAB_M(ID)												\
{																			\
	(u32)(182*U_START_ANGLE_COMP_M##ID),		/*��ʼλ�ü�ⲹ���Ƕ�*/	\
	(u16)IPD_PLUS_TIME_SETTING_M##ID,			/*ע��ʱ��������*/		\
	(u16)IPD_WAIT_TIME_SETTING_M##ID,			/*���еȴ�ʱ��������*/	\
}

#define IPD_PARAMETER_TAB_RSVD_M(ID)										\
{																			\
	(u32)NOT_APPLICABLE,						/*��ʼλ�ü�ⲹ���Ƕ�*/	\
	(u16)NOT_APPLICABLE,						/*ע��ʱ��������*/		\
	(u16)NOT_APPLICABLE,						/*���еȴ�ʱ��������*/	\
}


//////////////////////////////////////////////////////////////////////
////////////////////Allign para   /////////////////////////////////////

typedef struct
{
	u32		m_wuAlignAngle;						//Ԥ��λ�Ƕ�	1
	s32		m_wPlat1Cur;						//��һ�ζ�λ����	1	0.001A
	s32		m_wPlat2Cur;						//�ڶ��ζ�λ����	1	0.001A
	s16		nPlat1Time;							//��һ�ζ�λʱ��	1
	s16		nPlat2Time;							//�ڶ��ζ�λʱ��	1
	s32		nAlignTotalTime;					//��λ��ʱ��	1
	s32		m_wAlignCurAcc;						//��λ�������ٵ���ֵ	1	0.001A/s
	s32		m_wAlignCurDec;						//��λ�������ٵ���ֵ	1	0.001A/s	
}STR_FB_ALLIGN;

#define ALLIGN_PARAMETER_TAB_M(ID)											\
{																			\
	(u32)(182*ALIGN_ANGLE_M##ID),				/*Ԥ��λ�Ƕ�     */			\
	(s32)(1000*U_START_CUR_SET_F_M##ID),		/*��һ�ζ�λ���� */			\
	(s32)(1000*U_START_CUR_SET_S_M##ID),		/*�ڶ��ζ�λ���� */			\
	(s16)DC_HOLDTIME_TIME_LENTH_M##ID,			/*��һ�ζ�λʱ�� */			\
	(s16)DC_HOLDTIME_TIME_LENTH_STAGE1_M##ID,	/*�ڶ��ζ�λʱ�� */			\
	(s32)DC_ALIGN_TOTAL_LENTH_M##ID,			/*��λ��ʱ��	 */			\
	(s32)(1000*ALIGN_CURRENT_ACC_M##ID),		/*�������ٵ���ֵ */			\
	(s32)(1000*ALIGN_CURRENT_DEC_M##ID)			/*�������ٵ���ֵ */			\
}

#define ALLIGN_PARAMETER_TAB_RSVD_M(ID)										\
{																			\
	(u32)NOT_APPLICABLE,						/*Ԥ��λ�Ƕ�     */			\
	(s32)NOT_APPLICABLE,						/*��һ�ζ�λ���� */			\
	(s32)NOT_APPLICABLE,						/*�ڶ��ζ�λ���� */			\
	(s16)NOT_APPLICABLE,						/*��һ�ζ�λʱ�� */			\
	(s16)NOT_APPLICABLE,						/*�ڶ��ζ�λʱ�� */			\
	(s32)NOT_APPLICABLE,						/*��λ��ʱ��	 */			\
	(s32)NOT_APPLICABLE,						/*�������ٵ���ֵ */			\
	(s32)NOT_APPLICABLE							/*�������ٵ���ֵ */			\
}


//////////////////////////////////////////////////////////////////////
////////////////////VF para   /////////////////////////////////////

typedef struct
{	
	s16		nVFStartValue;						//VF������ѹֵ	0.01V		1
	u16		nVF2CloseDelayTime;					//����������ʱʱ��	1
	s16		nVFVoltAcc;							//VF��ѹ���ٵ���ֵ	0.01V/s 1
	s16		nVFVoltDec;							//VF��ѹ���ٵ���ֵ	0.01V/s 1	
}STR_FB_VF;

#define VF_START_PARAMETER_TAB_M(ID)										\
{																			\
	(s16)(100*VF_START_VALUE_M##ID),			/*VF������ѹֵ 0.01V   */			\
	(u16)VF_OPEN2CLOSE_TIME_M##ID,				/*�������ջ���ʱʱ�� */		\
	(s16)(100*VF_VOLT_ACC_M##ID),				/*VF��ѹ���ٵ���ֵ 0.01V/s */		\
	(s16)(100*VF_VOLT_DEC_M##ID)				/*VF��ѹ���ٵ���ֵ 0.01V/s */		\
}

#define VF_START_PARAMETER_TAB_RSVD_M(ID)									\
{																			\
	(s16)NOT_APPLICABLE,						/*VF������ѹֵ   */			\
	(u16)NOT_APPLICABLE,						/*�������ջ���ʱʱ�� */		\
	(s16)NOT_APPLICABLE,						/*VF��ѹ���ٵ���ֵ */		\
	(s16)NOT_APPLICABLE							/*VF��ѹ���ٵ���ֵ */		\
}


//////////////////////////////////////////////////////////////////////
////////////////////DC UNIT para   /////////////////////////////////////

typedef struct
{	
	s32		m_wDCOverVoltage;					//	��ѹ������ֵ	1
	s32		m_wDCOverVoltageRecover;			//	��ѹ�ָ���ѹֵ	1
	s32		m_wDCUnderVoltage;					//  ��ѹ������ֵ	1
	s32		m_wDCUnderVoltageRecover;			//	��ѹ�����ָ���ѹֵ	1
	u16		nOvVoltageDetectTime;				//	��ѹ���ʱ��	1
	u16		nUndVoltageDetectTime;				//	Ƿѹ���ʱ��	1
	u16		nVoltageRecoverTime;				//  ��ѹ�ָ������ȴ�ʱ��	1
	s8		bOvVoltProtectEnbale;				//	ʹ�ܹ�ѹ����	1
	s8		bUndVoltProtectEnbale;				//	ʹ�ܵ�ѹ����	1		
}STR_FB_DC_UNIT0;

#define DC_UNIT0_PARAMETER_TAB_M(ID)										\
{																			\
	(s32)(100*U_OVERVOLTAGE_FAULT_M##ID),		/*��ѹ������ֵ		*/		\
	(s32)(100*U_OVERVOLTAGE_RECOVER_M##ID),		/*��ѹ�ָ���ѹֵ	*/		\
	(s32)(100*U_UNDERVOLTAGE_FAULT_M##ID),		/*��ѹ������ֵ		*/		\
	(s32)(100*U_UNDERVOLTAGE_RECOVER_M##ID),	/*��ѹ�����ָ���ѹֵ*/		\
	(u16)U_OVER_VOLT_DETECT_TIME_M##ID,			/*��ѹ���ʱ��		*/		\
	(u16)U_UNDE_VOLT_DETECT_TIME_M##ID,			/*Ƿѹ���ʱ��		*/		\
	(u16)VOLT_FAULT_RECOVER_TIME_M##ID,			/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u8)U_OV_VOLT_PROTECT_ENABLE_M##ID,			/*ʹ�ܹ�ѹ����		*/		\
	(u8)U_UND_VOLT_PROTECT_ENABLE_M##ID			/*ʹ�ܵ�ѹ����		*/		\
}

#define DC_UNIT0_PARAMETER_TAB_RSVD_M(ID)									\
{																			\
	(s32)NOT_APPLICABLE,						/*��ѹ������ֵ		*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ�ָ���ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ������ֵ		*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ�����ָ���ѹֵ*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*Ƿѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u8)NOT_APPLICABLE,							/*ʹ�ܹ�ѹ����		*/		\
	(u8)NOT_APPLICABLE							/*ʹ�ܵ�ѹ����		*/		\
}


//DC_UNIT1 Ԥ��������Ӧ�ã���Ҫ��Ӧ���ò���
typedef struct
{	
	s32		m_wDCOverVoltage;					//	��ѹ������ֵ	1
	s32		m_wDCOverVoltageRecover;			//	��ѹ�ָ���ѹֵ	1
	s32		m_wDCUnderVoltage;					//  ��ѹ������ֵ	1
	s32		m_wDCUnderVoltageRecover;			//	��ѹ�����ָ���ѹֵ	1
	u16		nOvVoltageDetectTime;				//	��ѹ���ʱ��	1
	u16		nUndVoltageDetectTime;				//	Ƿѹ���ʱ��	1
	u16		nVoltageRecoverTime;				//  ��ѹ�ָ������ȴ�ʱ��	1
	s8		bOvVoltProtectEnbale;				//	ʹ�ܹ�ѹ����	1
	s8		bUndVoltProtectEnbale;				//	ʹ�ܵ�ѹ����	1
	
	s32		m_wUndVoltChargeVolt;				//	��ѹ���Ƶ�ѹֵ	1
	s32		m_wUndVoltChargeStopVolt;			//	��ѹ����ֹͣ��ѹֵ	1
	s32		m_wUndVoltChargeRecoverVolt;		//	�˳���ѹ���Ƶ�ѹֵ	1
	s32		m_wOvVoltChargeVolt;				//	��ѹ���Ƶ�ѹֵ	1
	s32		m_wOvVoltChargeStopVolt;			//	��ѹ����ֹͣ��ѹֵ	1
	s32		m_wOvVoltChargeRecoverVolt;			//	�˳���ѹ���Ƶ�ѹֵ	1
}STR_FB_DC_UNIT1;

#define DC_UNIT1_PARAMETER_TAB_M(ID)										\
{																			\
	(s32)(100*U_OVERVOLTAGE_FAULT_M##ID),		/*��ѹ������ֵ		*/		\
	(s32)(100*U_OVERVOLTAGE_RECOVER_M##ID),		/*��ѹ�ָ���ѹֵ	*/		\
	(s32)(100*U_UNDERVOLTAGE_FAULT_M##ID),		/*��ѹ������ֵ		*/		\
	(s32)(100*U_UNDERVOLTAGE_RECOVER_M##ID),	/*��ѹ�����ָ���ѹֵ*/		\
	(u16)U_OVER_VOLT_DETECT_TIME_M##ID,			/*��ѹ���ʱ��		*/		\
	(u16)U_UNDE_VOLT_DETECT_TIME_M##ID,			/*Ƿѹ���ʱ��		*/		\
	(u16)VOLT_FAULT_RECOVER_TIME_M##ID,			/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u16)U_OV_VOLT_PROTECT_ENABLE_M##ID,		/*ʹ�ܹ�ѹ����		*/		\
	(u16)U_UND_VOLT_PROTECT_ENABLE_M##ID��		/*ʹ�ܵ�ѹ����		*/		\
																			\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ƶ�ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ����ֵֹͣ	*/		\
	(s32)NOT_APPLICABLE,						/*�˳���ѹ���Ƶ�ѹֵ*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ƶ�ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ����ֹͣ��ѹֵ*/		\
	(s32)NOT_APPLICABLE							/*�˳���ѹ���Ƶ�ѹֵ*/		\
}

#define DC_UNIT1_PARAMETER_TAB_RSVD_M(ID)									\
{																			\
	(s32)NOT_APPLICABLE,						/*��ѹ������ֵ		*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ�ָ���ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ������ֵ		*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ�����ָ���ѹֵ*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*Ƿѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u16)NOT_APPLICABLE,						/*ʹ�ܹ�ѹ����		*/		\
	(u16)NOT_APPLICABLE,						/*ʹ�ܵ�ѹ����		*/		\
																			\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ƶ�ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ����ֵֹͣ	*/		\
	(s32)NOT_APPLICABLE,						/*�˳���ѹ���Ƶ�ѹֵ*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ƶ�ѹֵ	*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ����ֹͣ��ѹֵ*/		\
	(s32)NOT_APPLICABLE							/*�˳���ѹ���Ƶ�ѹֵ*/		\
}


//////////////////////////////////////////////////////////////////////
////////////////////CUR SENSOR UNIT para   /////////////////////////////////////

typedef struct
{	
	s16		nHardOvCurVolt;						//	Ӳ�����������Ƚϵ�ѹֵ	1
	s16		nReserved;							//	Ԥ��	1
	s32		m_wSoftOvCur;						//	���������������ֵ	1	0.001A
	u16		nSoftOvCurTimes;					//  ���������������	1
	u16		nOvCurRecoverTime;					//	�����ָ�ʱ��		1
	u16 	nCurSampADConvTime;					//	��������ת��ʱ��	1
	u16 	nCurSampStableTime;					//	���������ȶ�ʱ��	1	
}STR_FB_CUR_SENSE_UNIT;

#define CUR_SENSOR_PARA_TAB_M(ID)											\
{																			\
	(s16)I_PH_HARD_OC_FAULT_CMP_VOLT_M##ID,		/*Ӳ�����������Ƚϵ�ѹֵ*/	\
	(s16)NOT_APPLICABLE,						/*Ԥ��				*/		\
	(s32)(1000*I_PH_OVERCURRENT_FAULT_M##ID),	/*���������������ֵ*/		\
	(u16)I_PH_OVERCURRENT_FAULT_TIMES_M##ID,	/*���������������	*/		\
	(u16)CURRENT_FAULT_RECOVER_TIME_M##ID,		/*�����ָ�ʱ��		*/		\
	(u16)ADC_COV_TIME_M##ID,					/*��������ת��ʱ��	*/		\
	(u16)SAMP_STABLE_TIME_SHUNT_M##ID			/*���������ȶ�ʱ��	*/		\
}

//////////////////////////////////////////////////////////////////////
////////////////////TEMP SENSOR UNIT para   /////////////////////////////////////

typedef struct
{
	s32		m_wVotBaseTemp;						//	��׼�¶�	1
	s32		m_wVotBaseVolt;						//	��׼VOT��ѹ 1
	s32		m_wVotSlope;						//	б��	1
	s32		m_wMaxAllowedWorkTemp;				//  ������߹����¶�	1
	s32		m_wMinAllowedWorkTemp;				//	������͹����¶�	1
	s32		m_wMaxCurInMaxTemp;					//	����¶��������	1
	u8		HysterTemp;							//	�¶ȱ����ͻ��¶�	1
	u8		bOvTempProtectEnable;				//	ʹ�ܹ��±���	1
	u16		nDetectionTime;						//	���ϼ��ʱ��	
	u16		nFaultRecoverTime;					//	���ϻָ�ʱ��	
	u16		Reserved;							//		0	
}STR_FB_TEMPER_VOT_UNIT;


typedef struct
{
	s32		m_wNTCBaseTemp;						//	NTC��׼�¶� 1
	s32		m_wNTCBaseVolt;						//	NTC��ѹ���׼��ѹ	1
	s32		m_wNTCBFactor;						//	NTC�¶�ϵ��B	1
	s32		m_wMaxAllowedWorkTemp;				//	������߹����¶�	1
	s32		m_wMinAllowedWorkTemp;				//	������͹����¶�	1
	s32		m_wMaxCurInMaxTemp;					//	����¶��������	1	
	u8		HysterTemp;							//	�¶ȱ����ͻ��¶�	1
	u8		bOvTempProtectEnable;				//	ʹ�ܹ��±���	1
	u16		nDetectionTime;						//	���ϼ��ʱ��	
	u16		nFaultRecoverTime;					//	���ϻָ�ʱ��
	u16		nreserved;							//	���ϻָ�ʱ��
}STR_FB_TEMPER_NTC_UNIT;

#define TEMPER_NTC_PARA_TAB_M(ID)											\
{																			\
	(s32)TEMP_BASED_M##ID,						/*��׼�¶�			*/		\
	(s32)TEMP_BASED_VOLT_M##ID,					/*NTC��ѹ���׼��ѹ */		\
	(s32)TEMP_B_FACTOR_M##ID,					/*NTC�¶�ϵ��B		*/		\
	(s32)TEMP_MAX_ALLOWED_M##ID,				/*������߹����¶�	*/		\
	(s32)TEMP_MIN_ALLOWED_M##ID,				/*������͹����¶�	*/		\
	(s32)PH_CUR_ALLOWED_MAXTEMP_M##ID,			/*����¶��������	*/		\
	(u8)TEMP_HYP_ALLOWED_M##ID,					/*�¶ȱ����ͻ��¶�	*/		\
	(u8)TEMP_PROTECT_ENABLE_M##ID,				/*ʹ�ܹ��±���		*/		\
	(u16)TEMP_FAULT_DETECT_TIME_M##ID,			/*���ϼ��ʱ��		*/		\
	(u16)TEMP_FAULT_RECOVER_TIME_M##ID,		/*���ϻָ�ʱ��		*/		\
	(u16)NOT_APPLICABLE							/*���ϻָ�ʱ��		*/		\
}

#define TEMPER_NTC_PARA_TAB_RSVD_M(ID)										\
{																			\
	(s32)NOT_APPLICABLE,						/*��׼�¶�			*/		\
	(s32)NOT_APPLICABLE,						/*NTC��ѹ���׼��ѹ */		\
	(s32)NOT_APPLICABLE,						/*NTC�¶�ϵ��B		*/		\
	(s32)NOT_APPLICABLE,						/*������߹����¶�	*/		\
	(s32)NOT_APPLICABLE,						/*����¶��������	*/		\
	(s32)NOT_APPLICABLE,						/*������͹����¶�	*/		\
	(u8)NOT_APPLICABLE,							/*ʹ�ܹ��±���		*/		\
	(u8)NOT_APPLICABLE,							/*�¶ȱ����ͻ��¶�	*/		\
	(u16)NOT_APPLICABLE,						/*���ϼ��ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*���ϻָ�ʱ��		*/		\
	(u16)NOT_APPLICABLE							/*���ϻָ�ʱ��		*/		\
}


typedef struct
{	
	s32		m_wStallPHCurrent;					//	��ת��������			1 0.001A
	s32		m_wStallCurMax;						//	��ת���ת�����ֵ		1 0.001A
	s32		m_wStallCurMin;						//	��ת���ת����Сֵ		1 0.001A	
	s32		m_wStallSpeedFactor;				//  ��ת���ת���趨ֵ		1 IQ8
	s32		m_wStallIqCurrent;					//  ��ת���Iq�趨ֵԤ��	0												
		
	u16		nStallDelaytime;					//	��ת�����ʱʱ��	1
	u16		nStallDetectTime;					//	��ת���ϼ��ʱ��	1
	u16		nStallRecoverTime;					//	��ת���ϻָ�ʱ��	1
	u16		nStallRecvd;						//	Ԥ��				0

	s32		m_wEmptyCurrent;					//	��ת������		1
	s32		m_wEmptySpeed;						//	��ת���ת��		1
	u16		nEmptyDetectTime;					//	��ת���ϼ��ʱ��	1
	u16		nEmptyRecoverTime;					//	��ת���ϻָ�ʱ��	1

	s32		m_wLosePhaseCurrent;				//  ȱ���������趨ֵ	1	
	u16 	nLossPHDetectTime;					//	ȱ����ϼ��ʱ��	1
	u16 	nLossPHRecoverTime;					//	ȱ����ϻָ�ʱ��	1

	u16 	nStartFaultDetectTime;				//	�������ϼ��ʱ��	1
	u16 	nStartClosedDetectTime;				//	�����ջ����ʱ��	1	
	u16 	nStartFaultRecoverTime;				//	�������ϻָ�ʱ��	1
	u16 	nReserved;							//	Ԥ��		1

	u8		bEmptyProtectEnable;				//	ʹ�ܿ�ת����		1
	u8		bStallProtectEnable;				//	ʹ�ܶ�ת����		1
	u8		bStartProtectEnable;				//	ʹ����������		1
	u8		bLossPHProtectEnable;				//	ʹ��ȱ�ౣ��		1
	
}STR_FB_EMPTY_STALL;

#define EMPTY_STALL_PARA_TAB_M(ID)											\
{																			\
	(s32)(1000*I_PH_STALL_FAULT_M##ID),			/*��ת��������	*/		\
	(s32)(1000*CUR_STALL_MAX_FAULT_M##ID),		/*��ת���������ֵ*/		\
	(s32)(1000*CUR_STALL_MIN_FAULT_M##ID),		/*��ת��������Сֵ*/		\
	(s32)(256*SPEED_FAC_STALL_FAULT_M##ID),		/*��ת���ת��ϵ���趨ֵ*/	\
	(s32)(1000*IQ_STALL_FAULT_M##ID),			/*��ת���Iq�趨ֵ	*/		\
	(u16)SPEED_STALL_DELAY_TIME_M##ID,			/*��ת�����ʱʱ��	*/		\
	(u16)STALL_FAULT_DETECT_TIME_M##ID,			/*��ת���ϼ��ʱ��	*/		\
	(u16)STALL_FAULT_RECOVER_TIME_M##ID,		/*��ת���ϻָ�ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*��תԤ��			*/		\
	(s32)(1000*I_PH_EMPTY_FAULT_M##ID),			/*��ת������		*/		\
	(s32)(100*SPEED_EMPTY_FAULT_M##ID),			/*��ת���ת��		*/		\
	(u16)EMPTY_FAULT_DETECT_TIME_M##ID,			/*��ת���ϼ��ʱ��	*/		\
	(u16)EMPTY_FAULT_RECOVER_TIME_M##ID,		/*��ת���ϻָ�ʱ��	*/		\
	(s32)(1000*I_PHASE_LOSS_FAULT_M##ID),		/*ȱ���������趨ֵ*/		\
	(u16)LOSS_PH_FAULT_DETECT_TIME_M##ID,		/*ȱ����ϼ��ʱ��	*/		\
	(u16)LOSS_PH_FAULT_RECOVER_TIME_M##ID,		/*ȱ����ϻָ�ʱ��	*/		\
	(u16)START_DETECT_PERIOD_M##ID,				/*�������ϼ��ʱ��	*/		\
	(u16)START_DETECT_CLOSED_TIME_M##ID,		/*�����ջ����ʱ��	*/		\
	(u16)START_FAULT_RECOVER_TIME_M##ID,		/*�������ϻָ�ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*Ԥ��				*/		\
	(u8)EMPTY_PROTECTION_ENABLE_M##ID,			/*ʹ�ܿ�ת����		*/		\
	(u8)STALL_PROTECTION_ENABLE_M##ID,			/*ʹ�ܶ�ת����		*/		\
	(u8)START_PROTECTION_ENABLE_M##ID,			/*ʹ����������		*/		\
	(u8)LOSS_PH_PROTECTION_ENABLE_M##ID			/*ʹ��ȱ�ౣ��		*/		\
}

#define EMPTY_STALL_PARA_TAB_RSVD_M(ID)										\
{																			\
	(s32)NOT_APPLICABLE,						/*��ת��������	*/		\
	(s32)NOT_APPLICABLE,						/*��ת���ת�����ֵ*/		\
	(s32)NOT_APPLICABLE,						/*��ת���ת����Сֵ*/		\
	(s32)NOT_APPLICABLE,						/*��ת���ת���趨ֵ*/		\
	(s32)NOT_APPLICABLE,						/*��ת���Iq�趨ֵ	*/		\
	(u16)NOT_APPLICABLE,						/*��ת�����ʱʱ��	*/		\
	(u16)NOT_APPLICABLE							/*��ת���ϼ��ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*��ת���ϻָ�ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*��תԤ��			*/		\
	(s32)NOT_APPLICABLE,						/*��ת������		*/		\
	(s32)NOT_APPLICABLE,						/*��ת���ת��		*/		\
	(u16)NOT_APPLICABLE,						/*��ת���ϼ��ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*��ת���ϻָ�ʱ��	*/		\
	(s32)NOT_APPLICABLE,						/*ȱ���������趨ֵ*/		\
	(u16)NOT_APPLICABLE,						/*ȱ����ϼ��ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*ȱ����ϻָ�ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*�������ϼ��ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*�����ջ����ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*�������ϻָ�ʱ��	*/		\
	(u16)NOT_APPLICABLE,						/*ʹ����������		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ�ܿ�ת����		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ�ܶ�ת����		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ����������		*/		\
	(u8)NOT_APPLICABLE							/*ʹ��ȱ�ౣ��		*/		\
}




//////////////////////////////////////////////////////////////////////
////////////////////AC SENSOR UNIT para   /////////////////////////////////////

typedef struct
{
	s32		m_wACUndVoltage;					//	��Сֵ(��ѹ�������ֵ)	1
	s32		m_wACOvVoltage;						//	���ֵ(��ѹ�������ֵ)	1
	s32		m_wAcRateVoltage;					//	�ֵ	1
	s32		m_wACVolShuntRatio;					//	��ѹ���Ŵ���	1
	s32		m_wAdcSupplyVol;					//	AD�ο���ѹ	1
	u16		nACRateFreq;						//  AC�Ƶ��	1
	s8		bACFreqDectEnable;					//	ʹ��Ƶ�ʼ��	1
	s8		HysterFreq;							//	����Ƶ��ƫ�Χ	1	
}STR_FB_AC_UNIT0;

#define AC_UNIT0_PARA_TAB_M(ID)												\
{																			\
	(s32)NOT_APPLICABLE,						/*��Сֵ			*/		\
	(s32)NOT_APPLICABLE,						/*���ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*�ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ŵ���	*/		\
	(s32)NOT_APPLICABLE,						/*AD�ο���ѹ		*/		\
	(u16)NOT_APPLICABLE,						/*AC�Ƶ��		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ��Ƶ�ʼ��		*/		\
	(u8)NOT_APPLICABLE							/*����Ƶ��ƫ�Χ	*/		\
}

#define AC_UNIT0_PARA_TAB_RSVD_M(ID)										\
{																			\
	(s32)NOT_APPLICABLE,						/*��Сֵ			*/		\
	(s32)NOT_APPLICABLE,						/*���ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*�ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ŵ���	*/		\
	(s32)NOT_APPLICABLE,						/*AD�ο���ѹ		*/		\
	(u16)NOT_APPLICABLE,						/*AC�Ƶ��		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ��Ƶ�ʼ��		*/		\
	(u8)NOT_APPLICABLE							/*����Ƶ��ƫ�Χ	*/		\
}

typedef struct
{
	s32		m_wACUndVoltage;						//	��Сֵ(��ѹ�������ֵ)	1
	s32		m_wACOvVoltage;						//	���ֵ(��ѹ�������ֵ)	1
	s32		m_wAcRateVoltage;						//	�ֵ	1
	s32		m_wACVolShuntRatio;					//	��ѹ���Ŵ���	1
	s32		m_wAdcSupplyVol;						//	AD�ο���ѹ	1
	u16		nACRateFreq;						//  AC�Ƶ��	1
	s8		bACFreqDectEnable;					//	ʹ��Ƶ�ʼ��	1
	s8		HysterFreq;							//	����Ƶ��ƫ�Χ	1	
	
	u16		nACOvVoltTime;						//	��ѹ���ʱ��	1
	u16		nACUndVoltTime;						//	��ѹ���ʱ��	1
	u16		nACVoltRecoverTime;					//	��ѹ�ָ������ȴ�ʱ��	1
	s8		bACOvVoltDectEnable;				//  ʹ�ܹ�ѹ����	1
	s8		bACUndVoltDectEnable;				//	ʹ�ܵ�ѹ����	1
}STR_FB_AC_UNIT1;

#define AC_UNIT1_PARA_TAB_M(ID)												\
{																			\
	(s32)NOT_APPLICABLE,						/*��Сֵ			*/		\
	(s32)NOT_APPLICABLE,						/*���ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*�ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ŵ���	*/		\
	(s32)NOT_APPLICABLE,						/*AD�ο���ѹ		*/		\
	(u16)NOT_APPLICABLE,						/*AC�Ƶ��		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ��Ƶ�ʼ��		*/		\
	(u8)NOT_APPLICABLE							/*����Ƶ��ƫ�Χ	*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u8)NOT_APPLICABLE,							/*ʹ�ܹ�ѹ����		*/		\
	(u8)NOT_APPLICABLE							/*ʹ�ܵ�ѹ����		*/		\
}

#define AC_UNIT1_PARA_TAB_RSVD_M(ID)										\
{																			\
	(s32)NOT_APPLICABLE,						/*��Сֵ			*/		\
	(s32)NOT_APPLICABLE,						/*���ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*�ֵ			*/		\
	(s32)NOT_APPLICABLE,						/*��ѹ���Ŵ���	*/		\
	(s32)NOT_APPLICABLE,						/*AD�ο���ѹ		*/		\
	(u16)NOT_APPLICABLE,						/*AC�Ƶ��		*/		\
	(u8)NOT_APPLICABLE,							/*ʹ��Ƶ�ʼ��		*/		\
	(u8)NOT_APPLICABLE							/*����Ƶ��ƫ�Χ	*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ���ʱ��		*/		\
	(u16)NOT_APPLICABLE,						/*��ѹ�ָ������ȴ�ʱ��*/	\
	(u8)NOT_APPLICABLE,							/*ʹ�ܹ�ѹ����		*/		\
	(u8)NOT_APPLICABLE							/*ʹ�ܵ�ѹ����		*/		\
}



//////////////////////////////////////////////////////////////////////

#define DRV_CFG_GLOBAL_PARA_TAB									\
{																\
	(u8)PARA_CFG_HEADER,			/* �ļ�ͷ			*/		\
	(u8)PARA_CFG_MAIN_VERSION,		/* ���汾			*/		\
	(u8)PARA_CFG_VERSION,			/* �汾				*/		\
	(u8)PARA_CFG_SUB_VERSION,		/* �Ӱ汾			*/		\
	(u8)0x00,						/* ��С��			*/		\
	(u8)NOT_APPLICABLE,				/* Ԥ��				*/		\
	(u8)PARA_CFG_LENGTH,			/* ����				*/		\
	(u8)PARA_CFG_NUMBER				/* ���������		*/		\
}

////////////////////FB block package /////////////////////////////////////
typedef struct
{
	u8 header;						// �ļ�ͷ
	u8 mainVersion;					// ���汾
	u8 version;						// �汾
	u8 subversion;					// �Ӱ汾
	u8 dataFormat;					// ��С��
	u8 reserved;					// Ԥ��
	u8 length;						// ����
	u8 objNumber;					// ���������	
}STR_DrvCfgGlobal, *PSTR_DrvCfgGlobal;


typedef const struct
{
	STR_GlobalCfg			mS_GlobalCfg;
	STR_BoardParameter		mS_Boardpara;
	STR_MotorParameter		mS_Motorpara;
	STR_FB_SLVC_CFG0		mS_FBSlvcCfg0;
	STR_FB_CUR_LOOP			mS_FBCurLoop;
	STR_FB_SPD_LOOP			mS_FBSpdLoop;
	STR_FB_POWER_LOOP		mS_FBPwrLoop;
	STR_FB_DIR_CHECK		mS_FBDirCheck;
	STR_FB_IPD				mS_FBIPD;
	STR_FB_ALLIGN			mS_FBAllign;
	STR_FB_VF				mS_FBVF;
	STR_FB_DC_UNIT0			mS_FBDcUnit0;
	STR_FB_CUR_SENSE_UNIT	mS_FBCurSense;
	STR_FB_TEMPER_NTC_UNIT	mS_FBTempNTC;
	STR_FB_EMPTY_STALL		mS_FBEmptyStall;
}STR_DrvCfgPara, *PSTR_DrvCfgPara;

#define DRV_CFG_PARA_TAB_M(ID)											\
{																		\
	GLOBAL_CFG_TAB_M(ID),												\
	BOARD_PARAMETER_TAB_M(ID),											\
	MOTOR_PARAMETER_TAB_M(ID),											\
	SLVC0_PARAMETER_TAB_M(ID),											\
	CURLOOP_PARAMETER_TAB_M(ID),										\
	SPDLOOP_PARAMETER_TAB_M(ID),										\
	PWRLOOP_PARAMETER_TAB_M(ID),										\
	DIR_CHECK_PARAMETER_TAB_M(ID),										\
	IPD_PARAMETER_TAB_M(ID),											\
	ALLIGN_PARAMETER_TAB_M(ID),											\
	VF_START_PARAMETER_TAB_RSVD_M(ID),									\
	DC_UNIT0_PARAMETER_TAB_M(ID),										\
	CUR_SENSOR_PARA_TAB_M(ID),											\
	TEMPER_NTC_PARA_TAB_RSVD_M(ID),										\
	EMPTY_STALL_PARA_TAB_M(ID)											\
}

/*---------------------------------------------------------------------------*/

#endif

