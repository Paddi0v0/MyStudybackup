#ifndef _PubData_DriverInstance_H_
#define _PubData_DriverInstance_H_

#include "basic.h"
#include "project_config.h"
#include "PubData_HD_MD.h"
#include "PubData_MD_MA.h"
#include "PubData_MA_UA.h"
#include "MC_configure_FB_data.h"
#include "MC_Parameter_M0.h"


										// ����FB���ò����ṹ������
#define MAX_CFG_PARA_ID					(PLANTFORM_MAX_CFG_PARA_ID)

										// ֧�ֵ�PWMͨ�����ݣ�����֧��������Ԫ��Ŀ
#define MAX_DRV_MODULE_USED				(PLANTFORM_DRV_MODULE_NUM)


//define PWM_MODULE_DYNAMIC_CONFIG

typedef struct {
    u16 nuCur;
    s16 nCurShftNum;
    u16 nuVol;
    s16 nVolShftNum;
    u16 nuFreq;
    s16 nFreqShftNum;
    u16 nuSpeed;
    s16 nSpeedShftNum;
    u32 nuPower;
    s16 nPowerShftNum;
    u16 nuTorque;
    s16 nTorqueShftNum;
    u16 nuAngle;
    s16 nAngleShftNum;
    u16 nuVdc;
    s16 nVdcShftNum;
} STR_TransCoefElement,*PSTR_TransCoefElement;


typedef struct {
		STR_TransCoefElement mUserToApp;		//USR2APP ����ת��
		STR_TransCoefElement mUserToCore;		//USR2CORE ����ת��
		STR_TransCoefElement mCoreToApp;		//CORE2APP ����ת��
		STR_TransCoefElement mAppToCore;		//APP2CORE ����ת��
} STR_TransCoef,*PSTR_TransCoef;


//////////////////////////////////////////////////////////////////////
////////////////******Motor Drive Layer para define*****//////////////
//////////////////////////////////////////////////////////////////////
typedef struct {
		s8  bMotorType;           /* 0:im,1:spm,2:ipm,3...*/
		s8  bPolePairs;           /* ����ż����� */
		s16 nRatedCur;            /* ����� ��λ��0.1A */
		s32 wRatedFreq;           /* �Ƶ�� ��λ��0.01Hz*/
		s32 wMotorRs;             /* ������� ��λ��0.0001ŷķ */
		s32 wMotorLd;             /* ���D���� ��λ��uH */
		s32 wMotorLq;             /* ���Q���� ��λ��uH */
		s32 wFluxConst;           /* ����������� 0.00001WB */
		/////////////////////////////////////////////////////////////////////////////////////////////////
		s16 nRatedUdc;            /* ���ѹ ��λ��0.1V */
		u16 nPWMFreq;             /* PWM��Ƶ,hz */
		//HD  Ӳ��������
		s32 wAdcSupplyVol;			/* ADC�ο���ѹ ��λ��0.000001����  max */
		s32 wRShunt;				/* ��ѹ���� ��λ��0.001��ŷķ*/
		s32 wAmpGain;				/* �Ŵ����Ŵ��� ��λ��0.000001�� */ 
		s32 wVolShuntRatio;			/* ĸ�ߵ�ѹ������ѹ�� 0.000001��*/ 
		s32 wBemfShuntRatio;		/* �����Ƶ�ѹ������ѹ�� 0.000001��*/
} STR_FocObjPar,*PSTR_FocObjPar;

typedef struct {
		u16 nuMinPwmSampTime3Shunt;    	/* ���ŵ������ʱ,���������ռ�ձ���,����ADC�ɿ���������С��PWMֵ */
		u16 nuSampStableTime3Shunt;     /* ���ŵ������ʱ,��Ҫ��֤ADC����ȷ�������ڣ��ж�����PWMռ��֮��Ĳ�ֵ�Ƿ������������ */
		s16 nShiftSampTime3Shunt;      	/* �����������λʱ�� */
		s16 nSampWaitTime3Shunt;       	/* ����������ȴ�ʱ�� */
		s16 nSampStableTime1Shunt;     	/* ������������ź�������ʱ�� */
		u16 nuShiftSampTime1Shunt;      /* �������������ʱ�� */
		u16 nuPwmPrd;              		/* PWM����ֵ */
		u16 nDeadTime;            		/* ����ֵ */
		u8 bSampType;					/* shunt type */
		u8 bSVPWM_Type;					/* svpwm type 0 ---- 7Seg, 1----- 5 Seg*/
} STR_FocSvpwmPar,*PSTR_FocSvpwmPar;

typedef struct {
		u16		nCurrLoopIniKP;        /* ������KP��ʼֵ, �������ݵ����е��������˲��� */
		u16		nCurrLoopIniKI;        /* ������KI��ʼֵ, �������ݵ����е��������˲��� */
		s16		nVqMax;					//Q���ѹ���ֵ	1
		s16		nVqMin;					//Q���ѹ��С 1
		s16		nVdMax;					//d���ѹ���ֵ	1
		s16		nVdMin;					//d���ѹ��С 1
		s16		nWeakenCurMax;			//������ŵ���	1

		u16		nuObsMode;//obs slvc1 or slvc2��or vf,,or vf+Slvc
		s32		wCloseFreq;				//�����϶�����Ƶ��	0.01hz
		u32		wMinRunFreq;			/* ��С����Ƶ��		0.01hz	*/
		u32		wCurrentClosedFreq;     /* �����ջ�Ƶ��		0.01hz	Ԥ��*/
		u16		nThetaFilTime;      	/* �Ƕ��˲�ϵ��		Q10*/
		
		s16		nPLLKp0;				//PLL_Kp ������Kp0				1
		s16		nPLLKi0;				//PLL_Kp ������Ki0				1
		s16	  	nPLLFreq0;				//PLLƵ�ʵ�		0.1hz			1
		s16		nPLLKp1;				//PLL_Kp ������Kp1				1
		s16		nPLLKi1;				//PLL_Kp ������Ki1				1
		s16	 	nPLLFreq1;				//PLLƵ�ʵ�1	0.1hz			1

		s16 	nVFStartValue;			//VF������ѹֵ		0.01V  			��ɾ��
		s16 	nVfVolRamp;				//��ѹrampֵ 		0.01V/s			��ɾ��
		s16 	nVF2CloseDelayTime;		//����������ʱʱ��					��ɾ��

		s32     wIPD_PlusWidthSet;		//IPD���
		s32     wIPD_IdleWaitSet;		//IPD����
		u16     nuIPD_Comp;				//IPD�����Ƕ�
} STR_FocObsPar,*PSTR_FocObsPar;

typedef struct {	
	STR_FocObjPar		mObjPar;		//���ƶ������
	STR_FocSvpwmPar		mSvpwmPar;		//SVPWM����
	STR_FocObsPar		mObsPar;		//�۲�������
}STR_MotorFocParDef,*PSTR_MotorFocParDef;


typedef struct {
  	STR_PubDatHDMD mHdToMd;
	STR_PubDatMDHD mMdToHd;
	STR_PubDatMDMA mMdToMa;
	STR_PubDatMAMD mMaToMd;
	STR_PubDatUAMA mUaToMa;
	STR_PubDatMAUA mMaToUa;
} STR_GlobalDatPackage,*PSTR_GlobalDatPackage;	//���ݽ�����

typedef struct {

	u8						m_bPWM_Id;			//ʵ�ʵ�PWM����˿ںţ�0  ------ EPWM0

	/*********************************************************************************/
	// MA2MD FOC����
	PSTR_MotorFocParDef		m_pMotorFocPar;

	/*********************************************************************************/
	// ��Ӳ������ýṹ��ָ��,���ò�������ƶ���󶨣�const��������
	PSTR_DrvCfgPara			m_pDrvCfgPara; 

	/*********************************************************************************/
	// Trans Coef for application, Used by all layer
	 STR_TransCoef			mTransCoef;
	/*********************************************************************************/

	/*********************************************************************************/
	// Definiation about global data structure  interface between MA and MD
	STR_GlobalDatPackage	mGlobalDatPackage;
	/*********************************************************************************/
 
}STR_ControlObject,*PSTR_ControlObject;


typedef struct
{
    u16 nTimeBaseFlg;        /* Timer �жϱ�ǣ�ȡֵ 0��1 */
    u8  bPWM_UpdateFlg;      /* PWM���ڸ��±�־��һ�μ��Ϊһ��PWM���� */
    u8  bTimeCnt1ms;         /* 1mS������ */
    u16 nTimeCnt10ms;        /* 10mS������ */
    u16 nTimeCnt500ms;       /* 500mS������ */
    const char* sVersion;    /* ����汾�� */
} STR_TaskScheduler;

extern void InitMotorDriveLayer(void);
extern void BondingMotorDrvObject(void);
extern void InitMotorAppLayer(void);
extern void InitUsrAppLayer(void);
extern bool checkParaCfgValid(void);

#endif // _PubData_DriverInstance_H_
/////////////////////

