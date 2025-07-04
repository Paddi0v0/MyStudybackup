///*******************************************************************************
// * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
// *
// * �ļ����ƣ� MA_Static_Variable.h
// * �ļ���ʶ��
// * ����ժҪ�� ȫ�ֱ��������ļ�
// * ����˵���� ��
// * ��ǰ�汾�� V 1.0
// * ��    �ߣ� Tonghua Li
// * ������ڣ� 2022��8��19��
// *
// *******************************************************************************/

#ifndef __MA_STATIC_VARIABLE__
#define __MA_STATIC_VARIABLE__

#include "basic.h"
#include "MC_Math.h"

#include "fault_detection.h"
#include "state_machine.h"

#include "PowerCalculation.h" 
#include "PubData_MA_UA.h" 
#include "PubData_MD_MA.h" 
#include "PubData_DriverInstance.h" 
#include "PID_regulators.h" 


// ����֧�ֵ�MA���ƶ�������������ƽ̨������Ԫ������Ӧ
#define MA_OBJ_MAX_DEFINE	(PLANTFORM_DRV_MODULE_NUM)

typedef struct 
{
    s32 wSvcMinFreq;                //�����϶�Ƶ��

    u8  bChargeFlag;                //Ԥ����־λ
    u8  bChargeEndFlag;             //Ԥ�����ɱ�־λ
	s16 nChargePwmWidth;            //Ԥ�������

    s32 wDirTrackFreq;              //�������ջ�Ƶ��
    s32 wDirEbreakFreq;             //���ɲ��Ƶ��
    s16 nDCurFilFactor;             //�������ٵ���ֵ
    s16 nQCurFilFactor;             //�������ٵ���ֵ
    s32 wSpeedRef;                  //�ٶȸ���ֵ
    
    s16 nLoopCntr;                  //��·����
    s16 nLoopPeriod;                //��·����
    u8  bLoopLmtEnable;				//��·�������ʹ��

    u8  bOpenRunFlag;               //�������б�־λ
    s32 wThetaErr;                  //����͸����Ƕ����
    s16 nMatchCnt;                  //����Ƕ�ƥ�����

    s16 nPowerFistFlag;             //��һ�ν��빦�ʻ���־λ

    s16 nTempADC;                   //NTC����ADֵ
    
    u16 nMotorRunDirPre;            //�ϴε��ת��
}STR_CommData,*PSTR_CommData;


typedef struct
{
    s16 nChargeTime;			//���ʱ�䣬��1msΪ��λ
    s16 nChargePeriod;			//ÿ�����趨ʱ�䣬��1msΪ��λ
    
    s16 nAlignTime;        		//��λʱ�䣬��1msΪ��λ
    s16 nAlignLowPeriod;        //��λ�͵����趨ʱ�䣬��1msΪ��λ
    s16 nAlignAllPeriod;        //��λ���趨ʱ�䣬��1msΪ��λ
    
    s16 nDirCheckTime;     		//˳���ʱ�䣬��1msΪ��λ
    s16 nDirCheckPeriod;		//˳������趨ʱ�䣬��1msΪ��λ 
    
    s16 nLoopDelyTime;			//�ٶȻ��ӳ�ʱ�䣬��1msΪ��λ
    s16 nLoopDelyPeriod;		//�ٶȻ��ӳ��趨ʱ�䣬��1msΪ��λ
    
    s16 nStopDelayTime;			//���ֹͣ�ӳ�ʱ�䣬��1msΪ��λ
    s16 nStopDelayTimePeriod;	//���ֹͣ�ӳ��趨ʱ�䣬��1msΪ��λ
    
    s16 nStopDelayCntr;    		//���ֹͣ�˲�ʱ�䣬�������жϵ��ֹͣ��ʱ�䣬��1msΪ��λ
    s16 nStopDelayPeriod;		//���ֹͣ�˲��趨ʱ�䣬�������жϵ��ֹͣ��ʱ�䣬��1msΪ��λ
    
    s16 nOpenRunCntr;			//�����бջ�ʱ�䣬��1msΪ��λ
    s16 nOpenRunPeriod;			//�����бջ��趨ʱ�䣬��1msΪ��λ
    
}STR_Time_t, *PSTR_Time_t;


typedef struct {
	s32 wSpeedSet;						/* �ٶ��趨 		core���ݸ�ʽ */
	s32 wSpeedRef;						/* �ٶȸ���ֵ 		core���ݸ�ʽ */
	
	s32 wSpeedfbk;						/* �ٶȷ���ֵ 		core���ݸ�ʽ */	
	s32 wSpeedfbkFilter;				/* �ٶȷ���ֵ�˲�	core���ݸ�ʽ */
	
	s32 wSpeedEst;						/* �۲������ٶ���� core���ݸ�ʽ */
	u16 nSpeedFiltTime;					/* �ٶ��˲�ʱ��ϵ��	*/
	
	STR_RampGen32	STR_SpdRampGen;		// �ٶ�ָ��б�¹滮���ݽṹ

	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)
	stru_SpeedPIRegulator mSpeedPI;		/* �ٶȻ� PI */
	#endif

	s32 wSpeedRampACCStep;				/*�ٶ����¼���ֵ Q16 core���ݸ�ʽ */
	s32 wSpeedRampDECStep;				/*�ٶ����¼���ֵ Q16 core���ݸ�ʽ */

	s32 wPowerLimitSpeedChangeSet;		/*�޹���ת�ٱ仯ֵ*/
	s32 wPowerLimitSpeedChangeSetInc;	/*�޹���ת�ٱ仯ֵ���� Q16*/
	s32 wPowerLimitValue;				/*�޹��ʴ�С*/
	s16 nPowerLimitCntr;				/*�޹��ʼ������*/
	s16 nPowerLimitPeriod;				/*�޹��ʼ�������*/
} STR_MechQuantity, *PSTR_MechQuantity;

typedef struct
{
    s32 nCurrentAccStep;   /* �������Ӳ��� */
    s32 nCurrentDecStep;   /* ������С���� */   
    s32 nStartCurSet;      /* ���������趨 */
    s32 nStartCurRef;      /* ���������ο� */   
    s32 wStartCurRamp;     /* ��������ֵ */
  
    s32 wOpen2CloseFreq;   /* �����бջ�Ƶ�� */
    s16 nFreqAccStep;      /* Ƶ�ʼ��ٲ��� */
    s16 nFreqDecStep;      /* Ƶ�ʼ��ٲ��� */
    s32 wRampFreqRef;      /* Ƶ������ֵ */
} STR_OpenForceRun, *pSTR_OpenForceRun; /* �Ƕȿ���ǿ�Ͻṹ�� */

typedef struct
{
	u16 m_bMC_RunFlg		:1;					/* �������ָ�� */
	u16 m_bMotorDirtion		:1;					/* �������Ƶĵ�����з��� */
	u16 m_bEnableMOEInStop	:1;					/* ���ֹͣ��MOE�Ƿ����*/
	u16 m_bCalibFinish		:1;					/* ������*/
	
	u16 m_bNeedTracking		:1;					/* �Ƿ���Ҫtracking*/
	u16 m_bTrackingFinish	:1;					/* tracking����*/
	u16 m_bNeedIPD			:1;					/* �Ƿ���ҪIPD*/
	u16 m_bIPDFinish		:1;					/* IPD��ɱ�־*/

	u16 m_bNeedAlign		:1;					/* �Ƿ���Ҫ����*/
	u16 m_bAlignFinish		:1;					/* ������ɱ�־*/	
	u16 m_bMotorStopped		:1;					/* ���ֹͣ��ɱ�־*/
	u16 m_bPostOK			:1;					/* ���POST��ɱ�־*/

	u16 m_bUAFault			:1; 				/* UA��������*/
	u16 m_bUAFaultClr		:1; 				/* UA�����������*/
	u16 m_bUAReverse		:1; 				/* UA����ȡ��*/
	u16 m_bUAMOE			:1;					/* UA MOE�Ƿ�ʹ��*/

	u16 m_bObParaInitDone	:1; 				/* ������ʼ����ɱ�־*/
	u16 m_bAcrInitDone		:1; 				/* FOC ������ʼ����ɱ�־*/
	u16 m_bSVPWMInitDone	:1; 				/* SVPWM ������ʼ����ɱ�־*/
	u16 m_bFaultInitDone	:1;					/* Ԥ��*/

	u16 m_bObParaUpdate		:1;					/* ����������ɱ�־*/
	u16 m_bObParaReq		:1;					/* �������������־*/
	u16 m_bAcrParaUpdate	:1;					/* ����������ɱ�־*/
	u16 m_bAcrParaReq		:1;					/* �������������־*/

	u16 m_bSVPWMParaUpdate	:1;					/* ����������ɱ�־*/
	u16 m_bSVPWMParaReq		:1;					/* �������������־*/

	u16 m_bFaultInitReq		:1;					/* FAULT�����Ҫ��ʼ��*/

	u16 m_bBreakInFault		:1;					/* BreakIn Fault ���ڱ�־*/
	
	u16 m_bBreakInClrReq	:1;					/* BreakIn Clr �����־*/

	u16 m_bIPDInitReq		:1;					/* IPD Init 	�����־*/
	u16 m_bIPDInitDone		:1;					/* IPD Done 	�����־*/
	
	u16 m_bExceptionEnable	:1;					/* �쳣���� ʹ��*/
	
}STR_MACtrBit,*pSTR_MACtrBit;		/* ���Ʊ�־λ�ṹ�� */



typedef struct
{
	/*---------------------------------------------------------------------------------------------*/
	// �󶨵Ŀ��ƶ���
	u8						mg_nMACtrlProcID;			/* ���ƶ��� ID		*/
	PSTR_ControlObject		mg_pCtrlObj;				/* ���ƶ���			*/
	
	/*---------------------------------------------------------------------------------------------*/
	// FAULT ��⽻����Ϣ
	STR_FaultStatus			mg_UFault;					/* ���ϱ�־ 		*/
	ENUM_MotorBrakeMode		mg_eFaultStopMode;			/* ����ʱֹͣģʽ 	*/
	ENU_FaultPrio			mg_eFaultPrio;				/* ���ϼ���			*/

	/*---------------------------------------------------------------------------------------------*/
	// �ڲ�������Ϣ�洢��ˢ�»�����,��ʡRAMʱ����ֱ�Ӳ���mg_pCtrlObj�󶨵����ݽ�����
	STR_PubDatMDMA 			mMdToMa;					/* MD��MA ״̬����Ϣ ���ر�����	*/
	STR_PubDatMAMD 			mMaToMd;					/* MA��MD ״̬����Ϣ ���ر�����	*/
	
	STR_PubDatUAMA 			mUaToMa;					/* UA��MA ״̬����Ϣ ���ر�����	*/
	STR_PubDatMAUA			mMaToUa;					/* MA��UA ״̬����Ϣ ���ر�����	*/
	
	/*---------------------------------------------------------------------------------------------*/
	// �ڲ����Ʊ���	(˽��)
    volatile u16			m_nSys_TimerMA;				/* MA���ڼ���Cnt */
	
	STR_MACtrBit			m_SMACtrlBit;				/* ���������ǰ״̬�Ĵ���������������UA��FAULTģ���Ӱ��*/	
	ENUM_MotorBrakeMode		m_eStopMode;				// ���ֹͣģʽ
	u8						m_nLoopMode;				/* ��·״̬���ٶȡ����ʡ�������ѡ��ON���ܸ��� */
	
    ENU_DRIVER_STATUS		m_eSysState;				/* ��ǰ�������״̬�� */
	ENU_DRIVER_STATUS		m_eNextSysState;			/* �´ε������״̬�� */
	
    u16						m_nSetTimeLeftCnt;			/* ״̬���ȴ�ʱ������� */
	u16						m_nSetTimeLeftPrd;			/* ״̬���ȴ�ʱ������ */

	s16						m_nInvTemp;					// ����ģ���¶ȣ�������Ҫѡ��
	
	s16						m_nBreakDuty;				// �ƶ�PWM���

	s32						m_wPower001W;				// ����ʵʱֵ��0.01W,ȡ��MD��������ݣ�1���ٶȻ�ˢ��
	s64						m_wPower001WFil;			// �����˲�ֵ��0.01W,ȡ��MD��������ݺ��˲���1���ٶȻ�ˢ��
	s16 					m_nPower001WFactor;			// �����˲�ϵ�����˲�ʱ��10���ٶȻ�����
	

	// motor info ���������ò���
	ENU_MOTOR_INFO_STATE	m_eMotorInfoState;			/* �����Ϣ��� MOTOR INFO ״̬*/
	ENU_TRAKING_RESULT		m_eTrackingResult;			/* ˳����� tracking���*/

	
	/*---------------------------------------------------------------------------------------------*/
	// �����ܿ����ݿռ� (˽��)
	PSTR_CommData			m_pAppCommData;				/* �������̿��ƽṹ�� */
	PSTR_MechQuantity		m_pMotorSpeed;				/* ת��ʸ���ṹ�壬core */
	
	pSTR_OnTheFlyDetectMA	m_pOnTheFlyDetect;			/* ˳��������ṹ�� */

	#if (DRV0_UNDVOLT_PROT_ENABLE == 1)
	pSTR_MotorExceptionMA	m_pMotorException;			/* ˳��������ṹ�� */
	#endif
	
    PSTR_Time_t            	m_pTime;					/*��λʱ��� ʱ���ʱ*/
	
	PSTR_RampGen32			m_pSTR_CurRampGen;			/*����б�¹滮�ṹ��*/

	#if ((DRV0_CLOSE_LOOP == POWER_LOOP) || ((DRV0_POWER_LIMIT_STATUS == TRUE) && (DRV0_CLOSE_LOOP == SPEED_LOOP)))
	pSTR_PowerDef			m_pPower;					//  ���ʼ���ṹ��
	#endif
	
	/*---------------------------------------------------------------------------------------------*/
	// Fault ���ݿռ� (˽��)
	STR_FaultStatus			*m_pFaults;					// ������־ָ��
	
	STR_FaultVariable		*m_pFaultValue;				// ���������ṹ��ָ��
	s16						t_nFlagCheck;				// ����������ڱ���
	STR_CurrentAmplitude	*m_pstruCurrentAmplitude;	// ������ֵ�ṹ��ָ��
	STR_VectorUVW			*m_pstruImaxCurrent;		// �������ֵ�ṹ��ָ��
	
	STR_FaultTime			*m_pstruFaultVoltTime;		// ��ѹ���ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultEmptyTime;		// ��ת���ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultStallTime;		// ��ת���ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultCurrentTime;	// �������ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultTempTime;		// �¶ȼ��ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultPhaseTime;		// ȱ����ʱ��ṹ��ָ��
	STR_FaultTime			*m_pstruFaultStartTime;		// �������ʱ��ṹ��ָ��

	/*---------------------------------------------------------------------------------------------*/
	s16						m_nQCur_Reference;			/* Q��������ο����� */
	s16						m_nDCur_Reference;			/* D��������ο����� */
	s16						m_nQCurrentSet;				/* Q������趨ֵ */
	s16						m_nDCurrentSet;				/* D������趨ֵ */	
	s32						m_wDCurFilter;				/* D������˲��м�ֵ */	
	s32						m_wQCurFilter;				/* Q������˲��м�ֵ */	
	
}STR_MACtrProc, *PSTR_MACtrProc;	/* MA���ݽṹ�� */


typedef  struct
{
	STR_Time_t         		m_struTime;
	STR_CommData			m_struAppCommData;			/* �������̿��ƽṹ�� */
	STR_MechQuantity		m_struMotorSpeed;			/* ת��ʸ���ṹ�壬core */
	//stru_PIRegulator		m_SpeedPI;					/* �ٶȻ� */

	#if ((DRV0_CLOSE_LOOP == POWER_LOOP) || ((DRV0_POWER_LIMIT_STATUS == TRUE) && (DRV0_CLOSE_LOOP == SPEED_LOOP)))	
	STR_PowerDef			m_struPower;				/* ���빦�ʼ��� */
	#endif

	STR_OnTheFlyDetectMA	m_OnTheFlyDetect;			//	˳�����ṹ��

	#if(DRV0_UNDVOLT_PROT_ENABLE == 1)
	STR_MotorExceptionMA	m_MotorException;
	#endif

	STR_RampGen32			m_STR_CurRampGen;			//	ALLIGN�������ָ��б�¹滮�ṹ��

	//
	STR_CurrentAmplitude	m_struCurrentAmplitude;		// ������ֵ�ṹ��
	STR_VectorUVW			m_struImaxCurrent;			// ���������ṹ��
	STR_FaultVariable 		m_stru_FaultValue;			// ���������ṹ��
	STR_FaultStatus			m_stru_Faults;				// ������־�ṹ��

	STR_FaultTime			m_struFaultVoltTime;		// ��ѹ���ʱ��ṹ��
	STR_FaultTime 			m_struFaultEmptyTime;		// ��ת���ʱ��ṹ��
	STR_FaultTime 			m_struFaultStallTime;		// ��ת���ʱ��ṹ��
	STR_FaultTime 			m_struFaultCurrentTime;		// �������ʱ��ṹ��
	STR_FaultTime 			m_struFaultTempTime;		// �¶ȼ��ʱ��ṹ��
	STR_FaultTime 			m_struFaultPhaseTime;		// ȱ����ʱ��ṹ��
	STR_FaultTime 			m_struFaultStartTime;		// �������ʱ��ṹ��
}STR_MASubBlock;

#endif
///* ********************** (C) COPYRIGHT LINKO SEMICONDUCTOR ******************** */
///* ------------------------------END OF FILE------------------------------------ */
