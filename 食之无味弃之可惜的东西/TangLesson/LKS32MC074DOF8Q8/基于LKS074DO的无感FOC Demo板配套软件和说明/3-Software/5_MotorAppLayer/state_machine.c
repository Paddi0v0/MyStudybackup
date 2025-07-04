/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� state_machine.c
 * �ļ���ʶ��
 * ����ժҪ�� state machine��״̬������״̬���������ã��Լ��ӿ�����ά��
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��08��19��
 *
 *******************************************************************************/
#include "state_machine.h"
#include "MA_Static_Variable.h"
#include "Time_Process.h"
#include "MC_Math.h"
#include "string.h"

/*******************************************************************************/
//ȫ�ֱ���������


/*******************************************************************************/
//�ⲿ����������

extern void MotorFaultMain(STR_MACtrProc *pObj);					//	������Ͽ��ƺ���
extern bool InitDriverMain(STR_MACtrProc *pObj);					//	������ʼ��������
extern bool PowerOnOffDriverMain(STR_MACtrProc *pObj, bool t_bOn);	//	���ʰ����µ紦����
extern bool StopMotorMain(STR_MACtrProc *pObj);						//	���stop״̬������
extern void ChargeDriverMain(STR_MACtrProc *pObj);					//	���ʰ�Ԥ��纯��
extern void MotorTrakingMain(STR_MACtrProc *pObj);					//	˳����⺯�� 
extern void MotorTracking2Run(STR_MACtrProc *pObj);					//	���˳���������к���
extern void MotorTracking2Brake(STR_MACtrProc *pObj);				//	���˳�������ƶ�����
extern void MotorIPDMain(STR_MACtrProc *pObj);						//	�����ʼλ�ü�⺯��
extern void MotorAlignMain(STR_MACtrProc *pObj);					//	������ຯ��
extern void MotorCloseMain(STR_MACtrProc *pObj);					//	���run״̬�������
extern void MotorBreakMain(STR_MACtrProc *pObj);					//	����ƶ����ƺ���
extern void MotorFaultMain(STR_MACtrProc *pObj);					//	������Ͽ��ƺ���
extern bool MotorInitIPDMain(STR_MACtrProc *pObj);					//	�����ʼλ�ü���ʼ������
extern void updateMAInfo2UALocal(STR_MACtrProc *pObj);				//	MA��Ϣ��UA����º���


extern	void ActionMainRelayOn(void);								//	�̵����պϺ���
extern  void ActionMainRelayOff(void);								//	�̵����Ͽ�����
extern  void setInitAcrParaReq(STR_MACtrProc *pObj);				//	���õ�������ʼ��������

extern	void BrakeStateCheckInit(u32 t_CurrentSqrt,pSTR_OnTheFlyDetectMA this);
																	//	�ƶ�״̬����ʼ������


/*******************************************************************************/
//�ֲ���̬����������

/*******************************************************************************/
//�ֲ���̬����������


/*******************************************************************************/
/*****************************end***********************************************/

/*******************************************************************************
 �������ƣ�  GetUA2MACmd(STR_MACtrProc *pObj)
 ����������    ��ȡ������п�����
 ���˼·��    1.��UA to MA �����ݽṹ�л�ȡ���п�����
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    ������е�ǰ������
 �� �� ֵ��    ������е�ǰ������
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
ENUM_MotorCmdWord GetUA2MACmd(STR_MACtrProc *pObj)
{
	return pObj->mUaToMa.eAppCmd;
}
/*******************************************************************************
 �������ƣ�  static bool	IsMotorRunCmd(STR_MACtrProc *pObj)
 ����������    �жϵ���Ƿ�����
 ���˼·��    1.�ж����б�־��UA���·���������
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    bool  True������������� False�� ���δ����
 �� �� ֵ��    bool  True������������� False�� ���δ����
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsMotorRunCmd(STR_MACtrProc *pObj)
{
			// m_bUAMOEʹ����m_bMC_RunFlg��Чʱ������������
	return (pObj->m_SMACtrlBit.m_bMC_RunFlg == 1) && (pObj->m_SMACtrlBit.m_bUAMOE == 1);
}
/*******************************************************************************
 �������ƣ�  static bool	IsPOSTFinish(STR_MACtrProc *pObj)
 ����������    �жϵ����λ�Ƿ����
 ���˼·��    1.�ж����б�־��UA���·���������
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    bool  True��post��� False�� postδ���
 �� �� ֵ��    bool  True��post��� False�� postδ���
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsPOSTFinish(STR_MACtrProc *pObj)
{
	return (pObj->m_SMACtrlBit.m_bPostOK == 1);				//	TRUE : �ϵ� ���
}
/*******************************************************************************
 �������ƣ�  static bool WaitPowerOnFinish(STR_MACtrProc *pObj)
 ����������    �ж��ϵ��Ƿ����
 ���˼·��    1.�ж�vdcֵ �Ƿ���ڵ���Ƿѹ�ָ�ֵ
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    bool  True��post��� False�� postδ���
 �� �� ֵ��    bool  True��post��� False�� postδ���
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool WaitPowerOnFinish(STR_MACtrProc *pObj)
{
	s16 t_nVdc;
	PSTR_DrvCfgPara			t_pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara; 
	STR_TransCoefElement 	*pAppToCore	= &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
	
	//�жϵ�ǰ��ѹ�Ƿ���ڵ�ѹ�����ָ�ֵ
	t_nVdc = App2CoreVoltTrans(pAppToCore,t_pDrvCfgPara->mS_FBDcUnit0.m_wDCUnderVoltageRecover);
	return (pObj->mMdToMa.nVdc >= t_nVdc);
}

/*******************************************************************************
 �������ƣ�  static bool IsDriveChargeFinish(STR_MACtrProc *pObj)
 ����������    �ж�Ԥ����Ƿ����
 ���˼·��    1.�ж�Ԥ��������־
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    bool  True��Ԥ������ False�� Ԥ���δ���
 �� �� ֵ��    bool  True��Ԥ������ False�� Ԥ���δ���
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool IsDriveChargeFinish(STR_MACtrProc *pObj)
{
	return (pObj->m_pAppCommData->bChargeEndFlag == 4);		//	TRUE : Ԥ������ ���
}
// Check if calib finish
#if 0
static bool IsCalibDriveFinish(STR_MACtrProc *pObj)
{
	return (pObj->m_SMACtrlBit.m_bCalibFinish);
}
#endif
/*******************************************************************************
 �������ƣ�  static bool	IsFaultExists(STR_MACtrProc *pObj)
 ����������    �жϹ����Ƿ����
 ���˼·��    1.�жϹ��ϱ�־
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ��
 ���������    bool  True�����ϴ��� False�� ���ϲ�����
 �� �� ֵ��    bool  True�����ϴ��� False�� ���ϲ�����
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsFaultExists(STR_MACtrProc *pObj)
{
					// m_bUAFault 	: UA����ϱ�־
					// mg_UFault.R	: MA�������Ϣ
	return ((pObj->mg_UFault.R!= 0) || (pObj->m_SMACtrlBit.m_bUAFault == 1));
}

// motor info function
/*******************************************************************************
 �������ƣ�  static void	SetMotorInfoSubStatus(STR_MACtrProc *pObj, ENU_MOTOR_INFO_STATE status)
 ����������    �趨��һ��״̬
 ���˼·��    
 �����ı�    ��
 ���������    	STR_MACtrProc *pObj ------�ṹ��ָ�� 
 				ENU_MOTOR_INFO_STATE status ------�趨��״̬
 ���������    ��
 �� �� ֵ��    ��
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static void	SetMotorInfoSubStatus(STR_MACtrProc *pObj, ENU_MOTOR_INFO_STATE status)
{
	pObj->m_eMotorInfoState	= status;
}
/*******************************************************************************
 �������ƣ�  static ENU_MOTOR_INFO_STATE	GetMotorInfoSubStatus(STR_MACtrProc *pObj)
 ����������   ��ȡ����趨��״̬
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ENU_MOTOR_INFO_STATE status ------�趨��״̬
 �� �� ֵ��    ENU_MOTOR_INFO_STATE status ------�趨��״̬
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static ENU_MOTOR_INFO_STATE	GetMotorInfoSubStatus(STR_MACtrProc *pObj)
{
	return pObj->m_eMotorInfoState;
}
/*******************************************************************************
 �������ƣ�  static bool	IsNeedTracking(STR_MACtrProc *pObj)
 ����������   �Ƿ���Ҫ˳���
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True��˳���ʹ�� 
 					False��˳���ر�
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsNeedTracking(STR_MACtrProc *pObj)
{
	return pObj->m_SMACtrlBit.m_bNeedTracking;		//	TRUE : ��Ҫ˳�����
}
/*******************************************************************************
 �������ƣ�  bool	IsNeedIPD(STR_MACtrProc *pObj)
 ����������   �Ƿ���Ҫ����ע��
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True������ע��(IPD���)ʹ�� 
 					False������ע��(IPD���)�ر�
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
bool	IsNeedIPD(STR_MACtrProc *pObj)
{
	return pObj->m_SMACtrlBit.m_bNeedIPD;			//	TRUE : ��ҪIPD
}
/*******************************************************************************
 �������ƣ�  static bool	IsIPDFinish(STR_MACtrProc *pObj)
 ����������   ���������Ƿ����
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True������ע��(IPD���)��� 
 					False������ע��(IPD���)δ���
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsIPDFinish(STR_MACtrProc *pObj)
{
	return pObj->m_SMACtrlBit.m_bIPDFinish;			//	TRUE : IPD���� ���
}

// motor Align function
/*******************************************************************************
 �������ƣ�  static bool	IsNeedAllign(STR_MACtrProc *pObj)
 ����������   �Ƿ���Ҫ����
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True����Ҫ���� 
 					False������Ҫ����
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsNeedAllign(STR_MACtrProc *pObj)
{
	return pObj->m_SMACtrlBit.m_bNeedAlign;			//	TRUE : ��ҪALLIGN
}
/*******************************************************************************
 �������ƣ�  static bool	IsAllignFinish(STR_MACtrProc *pObj)
 ����������   �����Ƿ����
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True��������� 
 					False������δ���
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsAllignFinish(STR_MACtrProc *pObj)
{
	return pObj->m_SMACtrlBit.m_bAlignFinish;		//	TRUE : ALLIGN���� ���
}

// motor brake function
/*******************************************************************************
 �������ƣ�  static bool	IsCanRestore2Run(STR_MACtrProc *pObj)
 ����������   �Ƿ�����������
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True�����Իָ����� 
 					False�����ָܻ�����
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsCanRestore2Run(STR_MACtrProc *pObj)
{
				// �������Ӧ�Ĵ�������Ƿ���Ҫ�ظ��ָ����еĴ���
				// ���� TRUE ��ʾ���Իָ�����
	#if (DRV0_STOP_MODE == 1)
		return TRUE;
	#else
		return FALSE;
	#endif
}

// Fault handle
/*******************************************************************************
 �������ƣ�  static bool	IsFaultClrCmd(STR_MACtrProc *pObj)
 ����������   �����Ƿ�����
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True���й�������ָ��
 					False��û�й�������ָ��
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool	IsFaultClrCmd(STR_MACtrProc *pObj)
{
					//UA�㲻���ڱ����ҷ��ͱ����������ʱ��MA��ɵõ������������
	return (pObj->m_SMACtrlBit.m_bUAFaultClr == 1) && (pObj->m_SMACtrlBit.m_bUAFault == 0);

}
/*******************************************************************************
 �������ƣ�  static bool IsBreakInClrReq(STR_MACtrProc *pObj)
 ����������   �Ƿ���BREAK_IN�������
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    	True����BREAK_IN������� 
 				False��û��BREAK_IN������� 
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static bool IsBreakInClrReq(STR_MACtrProc *pObj)
{
	return (pObj->m_SMACtrlBit.m_bBreakInClrReq == 1);		//	TRUE : BREAK_IN �������
}
/*******************************************************************************
 �������ƣ� static void ClrBreakInClrReq(STR_MACtrProc *pObj)
 ����������   ����ɲ����������
 ���˼·��    
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� 
 ���������    ��
 �� �� ֵ��    bool True���������� False������û������
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static void ClrBreakInClrReq(STR_MACtrProc *pObj)
{
	pObj->m_SMACtrlBit.m_bBreakInClrReq = 0;				//	��� BreakIn�������
}


static bool IsBreakInFault(STR_MACtrProc *pObj)
{
	return (pObj->m_SMACtrlBit.m_bBreakInFault == 1);		//	�Ƿ����BreakIn�����ź� ?
}

static void ClrBreakInFault(STR_MACtrProc *pObj)
{
	pObj->m_SMACtrlBit.m_bBreakInFault = 0;					//	���BreakIn�����źű�־
}
/*******************************************************************************
 �������ƣ� static void UARunCmd2MA(STR_MACtrProc *pObj,bool dirRev)
 ����������   UA�����������MA��
 ���˼·��    1.UA�����������������MA��
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� bool dirRev ------�����Ƿ��෴
 ���������    ��
 �� �� ֵ��    ��
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static void UARunCmd2MA(STR_MACtrProc *pObj,bool dirRev)
{
	pObj->m_SMACtrlBit.m_bUAMOE			= 1;				//	ʹ��MOE��־
	pObj->m_SMACtrlBit.m_bMC_RunFlg		= 1;				//	ʹ��run�����־
	pObj->m_SMACtrlBit.m_bUAFaultClr	= 0;				//	UA���������־��0
	if(dirRev)
	{
		pObj->m_SMACtrlBit.m_bUAReverse	= 1;				//	���õ����ת���б�־
	}
	else
	{
		pObj->m_SMACtrlBit.m_bUAReverse	= 0;				//	ά�ֵ����ǰ�趨����ת����
	}

	pObj->m_eStopMode					= pObj->mUaToMa.eBrakeMode;		// ���� ���STOPģʽ
}

/*******************************************************************************
 �������ƣ� static void parseUACmdAndSetpoint(STR_MACtrProc *pObj)
 ����������   ����UA������趨MA�����ָ��
 ���˼·��    1.
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� bool dirRev ------�����Ƿ��෴
 ���������    ��
 �� �� ֵ��    ��
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static void parseUACmdAndSetpoint(STR_MACtrProc *pObj)
{
	STR_PubDatUAMA *pUa2Ma = &pObj->mUaToMa;
	
	#if (DRV0_CLOSE_LOOP != CURRENT_LOOP)
	PSTR_DrvCfgPara pDrvCfgPara;
	pDrvCfgPara		= pObj->mg_pCtrlObj->m_pDrvCfgPara;						// ���ò��� ָ��
	#endif
	
	switch (pUa2Ma->eAppCmd)												// ����UA��Ŀ�������
	{
		case E_CMD_MOTOR_STOP_MOE_OFF:		// �ر�MOEֹͣ���
		{
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;						// ���run��־
			pObj->m_SMACtrlBit.m_bUAMOE			= 0;						// ���MOEʹ�ܱ�־
			pObj->m_eStopMode					= E_STOP_MOE_OFF;			// ����ֹͣģʽΪ�ر�MOE
			break;
		}
		case E_CMD_MOTOR_STOP:				// ʹ��MOEֹͣ���
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 1;						// ����MOEʹ�ܱ�־
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;						// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;		// ����ֹͣģʽΪѡ�����Ӧ�ƶ�ģʽ				
			break;
		}

		#if (DRV0_CLOSE_LOOP == SPEED_LOOP)	//������ٶȻ�ģʽ
		case E_CMD_MOTOR_SPEED_RUN:			//���е����ת������Ϊ�趨����
		{		
			pObj->m_nLoopMode					= SPEED_LOOP;			
			UARunCmd2MA(pObj, FALSE);									// ����UA�㷢�͸�MA��Ŀ�������
			break;
		}

		case E_CMD_MOTOR_SPEED_RUN_REV:		//���е����ת������Ϊ�趨�����෴����
		{			
			pObj->m_nLoopMode					= SPEED_LOOP;			
			UARunCmd2MA(pObj, TRUE);									// ����UA�㷢�͸�MA��Ŀ�������ٶȷ���ȡ��
			break;
		}
		#elif (DRV0_CLOSE_LOOP == CURRENT_LOOP)		//�����������ģʽ

		case E_CMD_MOTOR_TORQUE_RUN:		//���е����ת������Ϊ�趨����
		{
			pObj->m_nLoopMode					= CURRENT_LOOP;			
			UARunCmd2MA(pObj, FALSE);									// ����UA�㷢�͸�MA��Ŀ�������
			break;
		}

		case E_CMD_MOTOR_TORQUE_RUN_REV:	//���е����ת������Ϊ�趨�����෴����
		{
			pObj->m_nLoopMode					= CURRENT_LOOP;			
			UARunCmd2MA(pObj, TRUE);									// ����UA�㷢�͸�MA��Ŀ�������ٶȷ���ȡ��
			break;
		}

		#elif (DRV0_CLOSE_LOOP == POWER_LOOP)
		case E_CMD_MOTOR_POWER_RUN:			//���е����ת������Ϊ�趨����
		{
			pObj->m_nLoopMode					= POWER_LOOP;
			UARunCmd2MA(pObj, FALSE);									// ����UA�㷢�͸�MA��Ŀ�������
			break;
		}

		case E_CMD_MOTOR_POWER_RUN_REV:		//���е����ת������Ϊ�趨�����෴����
		{
			pObj->m_nLoopMode					= POWER_LOOP;			
			UARunCmd2MA(pObj, TRUE);									// ����UA�㷢�͸�MA��Ŀ�������ٶȷ���ȡ��
			break;
		}
		#endif

		#if (DRV0_STOP_MODE == 0)
		case E_CMD_MOTOR_FAULT_DIMOE:		// UA����FAULTֹͣ���,MOE�ر�
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 0;					// ���MOEʹ�ܱ�־
			pObj->m_SMACtrlBit.m_bUAFault		= 1;					// ����UA������Ч��־
			pObj->m_SMACtrlBit.m_bUAFaultClr	= 0;					// UA���������־��0
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;					// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;
			break;
		}
		#else
		case E_CMD_MOTOR_FAULT_ENMOE:		// UA����FAULTֹͣ���,MOEʹ��
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 1;					// ����MOEʹ�ܱ�־
			pObj->m_SMACtrlBit.m_bUAFault		= 1;					// ����UA������Ч��־
			pObj->m_SMACtrlBit.m_bUAFaultClr	= 0;					// UA���ϱ�־��0
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;					// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;	// ����ֹͣģʽΪѡ�����Ӧ�ƶ�ģʽ				
			break;
		}
		#endif

		#if (DRV0_PWM_ENABLE_STOP == FALSE)	
		case E_CMD_MOTOR_FAULT_CLR_DIMOE:	// ��������� MOE��ʹ��
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 0;					// ���MOEʹ�ܱ�־
			pObj->m_SMACtrlBit.m_bUAFault		= 0;					// ����UA������Ч��־
			pObj->m_SMACtrlBit.m_bUAFaultClr	= 1;					// ����UA���������־
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;					// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;				
			break;
		}
		#else
		case E_CMD_MOTOR_FAULT_CLR_ENMOE:	// ������� MOEʹ��
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 1;					// ʹ��MOE��Ч��־
			pObj->m_SMACtrlBit.m_bUAFault		= 0;					// ����UA������Ч��־
			pObj->m_SMACtrlBit.m_bUAFaultClr	= 1;					// ����UA���������־
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;					// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;				
			break;
		}
		#endif
		
		default:
		{
			pObj->m_SMACtrlBit.m_bUAMOE			= 0;					// ���MOEʹ�ܱ�־
			pObj->m_SMACtrlBit.m_bUAFault		= 0;					// ����UA������Ч��־
			pObj->m_SMACtrlBit.m_bUAFaultClr	= 1;					// ����UA���������־
			pObj->m_SMACtrlBit.m_bMC_RunFlg		= 0;					// ���run��־
			pObj->m_eStopMode					= pUa2Ma->eBrakeMode;				
			break;
		}
	}

	// ������ֵ��־
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)	
		pObj->m_pAppCommData->bLoopLmtEnable	= pDrvCfgPara->mS_GlobalCfg.m_buPowerLmtInSpeedmode;
	#endif

	#if (DRV0_CLOSE_LOOP == POWER_LOOP)	
		pObj->m_pAppCommData->bLoopLmtEnable	= pDrvCfgPara->mS_GlobalCfg.m_buSpdLmtInPowermode;
	#endif
}
/*******************************************************************************
 �������ƣ� static void parseMD2MAInfor(STR_MACtrProc *pObj)
 ����������   MD���������MA��
 ���˼·��    1.UA�����������������MA��
 �����ı�    ��
 ���������    STR_MACtrProc *pObj ------�ṹ��ָ�� bool dirRev ------�����Ƿ��෴
 ���������    ��
 �� �� ֵ��    ��
 ���������:   
 ����˵����    ��
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.10.09     V1.0           Tonghua Li           �޸�
 ********************************************************************/
static void parseMD2MAInfor(STR_MACtrProc *pObj)
{
	STR_PubDatMDMA*	pMd2Ma = &pObj->mMdToMa;
	PSTR_DrvCfgPara pDrvCfgPara		= pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_TransCoefElement *pCoreToApp= &(pObj->mg_pCtrlObj->mTransCoef.mCoreToApp);

	s32 t_wTmp32;

	pObj->m_pMotorSpeed->wSpeedEst	= pMd2Ma->wFreq;		//��ȡMD��Ƶ�ʷ���

	//	�����趨���ٶ��˲�ʱ������ٶ��˲� 
	//  �˲�ʱ��ϵ�� nSpeedFiltTime ��StateInit()�г�ʼ��
	#if 0
	s16 t_nfreqFilt;
	t_nfreqFilt = 1024 - pObj->m_pMotorSpeed->nSpeedFiltTime;
	t_wTmp32 = (s32)pObj->m_pMotorSpeed->wSpeedfbk * t_nfreqFilt + (s32)pObj->m_pMotorSpeed->wSpeedEst * pObj->m_pMotorSpeed->nSpeedFiltTime;
	pObj->m_pMotorSpeed->wSpeedfbk = t_wTmp32 >> 10;
	#else
	t_wTmp32								= pObj->m_pMotorSpeed->wSpeedEst - (pObj->m_pMotorSpeed->wSpeedfbkFilter >>10);
	pObj->m_pMotorSpeed->wSpeedfbkFilter	+= t_wTmp32 * pObj->m_pMotorSpeed->nSpeedFiltTime;
	pObj->m_pMotorSpeed->wSpeedfbk			= (pObj->m_pMotorSpeed->wSpeedfbkFilter >> 10);
	#endif
	
	//pObj->m_pMotorSpeed->wSpeedfbk = pObj->m_pMotorSpeed->wSpeedEst;

	// ���ʼ���
	// �������ݲ���Ҫʱ�����ε�ǰ���룬��ԼFLASH�ռ�
	// �����˲�ʱ��Ϊ10����ǰ��·���ڣ�1ms����ʱ���˲�ʱ��Ϊ10ms
	// m_nPower001WFactor			= 1024/(1+10);  Q10���ݸ�ʽ
	
	t_wTmp32 = Core2AppPowerTrans(pCoreToApp, pMd2Ma->nPowerDec);
	t_wTmp32				= t_wTmp32 - (pObj->m_wPower001WFil >> 10);
	pObj->m_wPower001WFil	+= ((s64)t_wTmp32 * pObj->m_nPower001WFactor);


	// ʹ�ܹ��ʼ���У����ʱ�����й���У��
	
	#if (POWER_CALC_CALIB_FUNCTION_ENABLE == FUNCTION_ON)
	s64 t_lTmp;
	t_wTmp32				= (pObj->m_wPower001WFil >> 10);

	t_lTmp = ((s64)pDrvCfgPara->mS_FBPwrLoop.m_wPowerFacA2 * t_wTmp32 * t_wTmp32)/100;
	t_lTmp += ((s64)pDrvCfgPara->mS_FBPwrLoop.m_wPowerFacA1 * t_wTmp32);
	t_lTmp += pDrvCfgPara->mS_FBPwrLoop.m_wPowerFacA0;	

	// Pcal = m_wPowerFacA0 + m_wPowerFacA1 * Power + m_wPowerFacA2 * Power^2
	// m_wPowerFacAx   Q12���ݸ�ʽ
	pObj->m_wPower001W		= (s32)(t_lTmp>>12);

	#else
	
	pObj->m_wPower001W		= (pObj->m_wPower001WFil >> 10);
	#endif
}

/*******************************************************************************
 �������ƣ�    static void parseFaultDetection(STR_MACtrProc *pObj)
 ����������    ���� ���ϼ��ģ�� �� UA ģ��ֹͣ��ʽ���������stop mode
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����		
 			   ��UA���ڹ�����Ϣ
 			   		���ùر�MOEֹͣʱ�����ϼ��ģ�鼴��Ϊ�����ȼ�Ҳ���ܸ���ֹͣģʽ
 			   		����������ʽֹͣʱ ���ϼ��ģ��Ϊ�����ȼ�ʱ�����ϼ��ģ��ֹͣģʽ����
 			   		
 			   ��UA�����ڹ�����Ϣ
 			   		ֹͣģʽ�ɹ��ϼ��ģ�����
 			   		
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void parseFaultDetection(STR_MACtrProc *pObj)
{
	if(pObj->mg_UFault.R != 0)										//	���ڱ����ȹ�����Ϣ
	{
		if(pObj->m_SMACtrlBit.m_bUAFault == 1)						//  ����UA�����	
		{
			if(pObj->m_eStopMode != E_STOP_MOE_OFF)					//  ֹͣģʽ��ΪMOE�ر�ģʽʱ
			{
				if(pObj->mg_eFaultPrio == E_FAULT_PRIO_HIGH)		//  ����ģʽΪ�����ȼ�ʱ
				{
					pObj->m_eStopMode	= pObj->mg_eFaultStopMode;	//  ���ո����ȼ�����ֹͣ
				}
			}
		}
		else
		{
			pObj->m_eStopMode				= pObj->mg_eFaultStopMode;
		}		
	}

	if(pObj->mg_UFault.B.HardCurretError == 1)					// ����Ӳ����������
	{
		pObj->m_SMACtrlBit.m_bBreakInFault		= 1;			// ����Ӳ��������־
	}
	else
	{															// û��Ӳ����������ʱ,
		if(pObj->m_SMACtrlBit.m_bBreakInFault == 1)				// �������Ӳ��������־�������ж�
	{
			pObj->m_SMACtrlBit.m_bBreakInClrReq = 1;			// ����Ӳ�����������־���������й����������
		}
	}
}

/*******************************************************************************
 �������ƣ�    void handleMAStateMachine(STR_MACtrProc *pObj)
 ����������    ϵͳ״̬�� ���״̬��״̬�л�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void handleMAStateMachine(STR_MACtrProc *pObj)
{
	memset(&pObj->mMaToUa, 0, sizeof(STR_PubDatMAUA));
	memset(&pObj->mMaToMd, 0, sizeof(STR_PubDatMAMD));	

	pObj->mg_pCtrlObj->mGlobalDatPackage.mMdToMa.gWrFlag = 1;		//	д��������
	
		//copy��˽�б���ռ䣬��ֹ����ʱ�������ݱ�����
	memcpy(&pObj->mMdToMa, &pObj->mg_pCtrlObj->mGlobalDatPackage.mMdToMa, sizeof(STR_PubDatMDMA));

	pObj->mg_pCtrlObj->mGlobalDatPackage.mMdToMa.gWrFlag = 0;		//  �ر�д����

	if(pObj->mg_pCtrlObj->mGlobalDatPackage.mUaToMa.gWrFlag == 0)	//  UA��MA�������ݲ���д״̬����ǰ��Ϣ����
	{
		memcpy(&pObj->mUaToMa, &pObj->mg_pCtrlObj->mGlobalDatPackage.mUaToMa, sizeof(STR_PubDatUAMA));
		//copy��˽�б���ռ䣬��ֹ����ʱ�������ݱ�����
	}

	parseUACmdAndSetpoint(pObj);				//	��copy�����ص�mUaToMa�н��� UA��������
	parseMD2MAInfor(pObj);						//	��copy�����ص�mMdToMa�н��� MD��Ϣ��״̬
	parseFaultDetection(pObj);					//	���� ������Ԫ�ı�����Ϣ��״̬
	
	if(IsFaultExists(pObj))						//	���ڱ���?
	{		
		if(pObj->m_SMACtrlBit.m_bMotorStopped)	// �����ֹͣ���� E_DRIVER_FAULT ״̬
		{
			pObj->m_eNextSysState	= E_DRIVER_FAULT;
		}
		else									// ���δֹͣ���� E_DRIVER_READY_STOP ״̬�����ƻ�ȴ����ֹͣ
		{
			pObj->m_eNextSysState	= E_DRIVER_READY_STOP;
		}
	}

	pObj->m_eSysState = pObj->m_eNextSysState;	//״̬������
	
    switch (pObj->m_eSysState)
   	{
		case E_DRIVER_INIT:						/* �ϵ��ʼ״̬ */
    	{
			if(InitDriverMain(pObj))			// ��ʼ������������������Ϣ
			{
				if(WaitPowerOnFinish(pObj))		// �ϵ����?
				{
					// ����ʵ����ӱ�Ҫ�Ĵ��� ��̵������ϵ�
					//��ҪУ��ʱ����У��
					//pObj->m_eNextSysState = E_DRIVER_CALIB;

					//����STOP״̬
					setInitAcrParaReq(pObj);	// ���õ�����������ʼ�������־
					pObj->m_eNextSysState = E_DRIVER_STOP;
				}
			}
		}
		break;

		case E_DRIVER_STOP:						/* ֹͣSTOP״̬ */
		{
			if(StopMotorMain(pObj))				// ֹͣSTOP״̬����������ʼ�����ʱ����TRUE
			{
				if( IsMotorRunCmd(pObj))		// ����run����?
				{	// Ԥ���ʹ��ʱ������Ԥ��紦��
					// Ԥ���δʹ��ʱ
						// ˳���   ʹ��  ----->  ˳��紦��
						//        δʹ��  ----->  IPD�ж�   ------ ʹ��   ����IPD
						//                                  ------ δʹ�� ���ж��ദ��
					#if (DRV0_CHARGE_ENABLE==FUNCTION_ON)
					pObj->m_eNextSysState = E_DRIVER_CHARGE;
					#else
						#if (DRV0_DIR_CHECK_ENABLE==FUNCTION_ON)		
						SetMotorInfoSubStatus(pObj, MOTOR_INFO_TRACKING);
						pObj->m_eNextSysState = E_DRIVER_MOTOR_INFO;
						#elif (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)
						SetMotorInfoSubStatus(pObj, MOTOR_INFO_IPD);
						pObj->m_eNextSysState = E_DRIVER_MOTOR_INFO;
						#else
						pObj->m_eNextSysState = E_DRIVER_READY_RUN;
						#endif

					#endif
				}
				else
				{
                    setInitAcrParaReq(pObj);	// ���õ�����������ʼ�������־
				}
			}
		}
		break;

		#if (DRV0_CHARGE_ENABLE==FUNCTION_ON)
											/* �����ʹ��Ԥ��繦��ʱ�ű���Ԥ���״̬*/
		case E_DRIVER_CHARGE:
		{	
			ChargeDriverMain(pObj);			//	Ԥ��紦����
											//	Ԥ���ʱ������Ϊ0ʱ���൱��û��Ԥ��繦��
			if( IsMotorRunCmd(pObj))
			{
				if(IsDriveChargeFinish(pObj))
				{						
					// ˳���   ʹ��  ----->  ˳��紦��
					//        δʹ��  ----->  IPD�ж�   ------ ʹ��   ����IPD
					//                                  ------ δʹ�� ���ж��ദ��
					#if (DRV0_DIR_CHECK_ENABLE==FUNCTION_ON)		
					SetMotorInfoSubStatus(pObj, MOTOR_INFO_TRACKING);
					pObj->m_eNextSysState = E_DRIVER_MOTOR_INFO;
					#elif (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)
					SetMotorInfoSubStatus(pObj, MOTOR_INFO_IPD);
					pObj->m_eNextSysState = E_DRIVER_MOTOR_INFO;
					#else
					pObj->m_eNextSysState = E_DRIVER_READY_RUN;
					#endif					
				}
			}
			else
			{
				pObj->m_eNextSysState = E_DRIVER_STOP;
			}
		}
		break;
		#endif

		case E_DRIVER_MOTOR_INFO:					/* �����Ϣ���״̬ */
		{
			switch (GetMotorInfoSubStatus(pObj))
			{
				// ��������ԪʱԤ����
				#if (DRV0_DIR_CHECK_ENABLE==FUNCTION_ON)	
				case MOTOR_INFO_TRACKING:			//	˳����ж�
				{
					if(IsNeedTracking(pObj))		//	��Ҫ˳���?
					{
						MotorTrakingMain(pObj);		//	˳��紦��

						if(pObj->m_eTrackingResult == TRACKING_2_RUN)			//	˳�����Ϊ����״̬
						{							
							SetMotorInfoSubStatus(pObj, MOTOR_INFO_TRACKING_RUN);
						}
						else if(pObj->m_eTrackingResult == TRACKING_2_BRAKE)	//	˳�����Ϊ�ƶ�״̬
						{
							BrakeStateCheckInit(pObj->mMdToMa.nMotorCur,pObj->m_pOnTheFlyDetect);
							SetMotorInfoSubStatus(pObj, MOTOR_INFO_TRACKING_BRAKE);
						}
					}
					else
					{
						// ��������ԪʱԤ���� IPD�ж�   ------ ʹ��   ����IPD
						//  		 					------ δʹ�� ���ж��ദ��
						#if (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)					
						SetMotorInfoSubStatus(pObj, MOTOR_INFO_IPD);
						#else
						pObj->m_eNextSysState = E_DRIVER_READY_RUN;
						#endif
					}
				}
				break;

				case MOTOR_INFO_TRACKING_RUN:		//	˳�����Ϊ����״̬
				{
					// handle motor run
					MotorTracking2Run(pObj);		//	˳����л�ΪRUN�Ĵ���			
					pObj->m_eNextSysState = E_DRIVER_RUN;
				}
				break;

				case MOTOR_INFO_TRACKING_BRAKE:		//	˳�����Ϊ�ƶ�״̬
				{
					// handle motor brake
					MotorTracking2Brake(pObj);		//  �ƶ�ֹͣ�������
					if(pObj->m_SMACtrlBit.m_bMotorStopped)		//	�����ֹͣ?
					{
						// ��������ԪʱԤ���� IPD�ж�   ------ ʹ��   ����IPD
						//  		 					------ δʹ�� ���ж��ദ��
						#if (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)					
						SetMotorInfoSubStatus(pObj, MOTOR_INFO_IPD);
						#else
						pObj->m_eNextSysState = E_DRIVER_READY_RUN;
						#endif
					}					
				}
				break;
				#endif

				#if (DRV0_POS_SEEK_ENABLE==FUNCTION_ON)
				case MOTOR_INFO_IPD:		//����� ��ʼλ�ü��״̬IPD
				{
					if(IsNeedIPD(pObj))			//	��ҪIPD���?
					{
						SetMotorInfoSubStatus(pObj, MOTOR_INFO_IPD_RESULT);	//	����IPD����ȴ�״̬
					}
					else
					{
						pObj->m_eNextSysState = E_DRIVER_READY_RUN;
					}
				}
				break;

				case MOTOR_INFO_IPD_RESULT:		//	IPD����ȴ�״̬
				{
					MotorIPDMain(pObj);			// 	IPD��⺯��
					if(IsIPDFinish(pObj))		//	IPD������?
					{
						pObj->m_eNextSysState = E_DRIVER_READY_RUN;
					}						
				}
				
				#endif

				default:
				{
					//SetMotorInfoSubStatus(pObj, MOTOR_INFO_TRACKING);
				}
				break;
			
			}

			if( !IsMotorRunCmd(pObj))			//	STOP����ʱ������STOP״̬��ֹͣ��ǰ�����м��
			{
				setInitAcrParaReq(pObj);
				pObj->m_eNextSysState = E_DRIVER_STOP;
			}			
		}
		break;
			

		case E_DRIVER_READY_RUN:				//	���׼������״̬���������
		{
			#if (DRV0_ALLIGN_CUR_ENABLE==FUNCTION_ON)	
			if(IsNeedAllign(pObj))
			{
				MotorAlignMain(pObj);			//	���ദ����
				if(IsAllignFinish(pObj))		//	����������������RUN����״̬
				{
					pObj->m_eNextSysState = E_DRIVER_RUN;						
				}
			}
			else
			{									//	����Ҫ�������ʱ��ֱ�ӽ���RUN����״̬
				pObj->m_eNextSysState = E_DRIVER_RUN;
			}
			#else
			pObj->m_eNextSysState = E_DRIVER_RUN;
			#endif

			if( !IsMotorRunCmd(pObj))			//	STOP����ʱ������STOP״̬��ֹͣ��ǰ״̬������
			{
				setInitAcrParaReq(pObj);		//	���õ�������ʼ�������־
				pObj->m_eNextSysState = E_DRIVER_STOP;
			}
		}
		break;
		

		case E_DRIVER_RUN:						//	���RUN����״̬
		{
			MotorCloseMain(pObj);				//	���RUN���п��ƺ���
			if( !IsMotorRunCmd(pObj) || (pObj->m_SMACtrlBit.m_bExceptionEnable == 1))
			{
				//	���յ�ͣ��������������쳣��־��λ
				SetTimeOut_Counter(pObj->m_nSetTimeLeftCnt, pObj->m_nSetTimeLeftPrd);	//	����ͣ���ȴ�ʱ��
				BrakeStateCheckInit(pObj->mMdToMa.nMotorCur,pObj->m_pOnTheFlyDetect);	//	�ƶ�����ʼ������
				pObj->m_eNextSysState = E_DRIVER_READY_STOP;							//	����׼��ֹͣ״̬��
			}

		}
		break;

		case E_DRIVER_READY_STOP:				//	���ֹͣ����״̬
		{
			MotorBreakMain(pObj);						//	����ƶ�������
			if(pObj->m_SMACtrlBit.m_bMotorStopped)		//	����Ƿ��Ѿ�ֹͣ?
			{
				setInitAcrParaReq(pObj);				//	���õ�����������ʼ�����־
				pObj->m_eNextSysState = E_DRIVER_STOP;	//	����STOP״̬
			}
			else
			{
				if(IsMotorRunCmd(pObj))					//	���δֹͣ�����յ�RUN��־
				{
					if(IsCanRestore2Run(pObj))			//	�Ƿ�����ɻָ�����?
					{
						pObj->m_eNextSysState = E_DRIVER_RUN;	//	�ɻָ�������ΪRUN״̬
					}
				}
			}				
		}
		break;

		case E_DRIVER_FAULT:					//	����״̬
		{
			MotorFaultMain(pObj);						//	����������
			if(WaitPowerOnFinish(pObj))					//	��ǰӲ�����Ƿ��ϵ����?
			{
				if(!IsFaultExists(pObj))				//	�ϵ�������Ҳ����ڱ�����Ϣ
				{
					if(IsFaultClrCmd(pObj))				//	�Ƿ���ڱ��������־
					{
						if(IsPOSTFinish(pObj))			//	�Ƿ��ϵ���� ?
						{
							setInitAcrParaReq(pObj);	//	�ϵ���ɺ����õ�����������ʼ�����־��������STOP״̬
							pObj->m_eNextSysState = E_DRIVER_STOP;
						}
						else
						{								//	�ϵ�δ��ɺ󣬽����ϵ��ʼ״̬�������������
							pObj->m_eNextSysState = E_DRIVER_INIT;
						}
					}
				}
				else
				{
					if(IsBreakInClrReq(pObj))			//	����Ӳ������״̬�����־��Ӳ��������־���ʱ��������BreakInClr����
					{
						if(IsBreakInFault(pObj))		//	��ǰ����breakIn���ϱ�־?
						{
							if(!getBreakInClrResultMD2MA(&pObj->mMdToMa))
							{			//	��ǰ����breakIn���ϱ�־����breakIn�������δ���
								setClrBreakInFaultFCmdMA2MD(&pObj->mMaToMd);	//	���������־��������������������MD��
						}
						else
							{			//	��ǰ����breakIn���ϱ�־����breakIn������������
								ClrBreakInFault(pObj);							//	���breakIn���ϱ�־
								ClrBreakInClrReq(pObj);							//	���Ӳ������״̬�����־
								setDefaultCmdMA2MD(&pObj->mMaToMd);				//	����Ĭ�ϲ��������MD
							}
						}
						else
						{				//	��ǰ������breakIn���ϱ�־	���ദ��
							ClrBreakInClrReq(pObj);								//	���Ӳ������״̬�����־
							setDefaultCmdMA2MD(&pObj->mMaToMd);					//	����Ĭ�ϲ��������MD				
						}
					}
				}
			}
		}
		break;

		default:
		{
		pObj->m_eNextSysState	= E_DRIVER_FAULT;
		}
		break;
	}

	// Update local data zone of MA2UA
	// some SR bit  of MA2UA is updated in state task.
	updateMAInfo2UALocal(pObj);
	
	// Update data swap zone for MA2MD and MA2UA
	//	����MA2MD��Ϣ����������
	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.gWrFlag = 1;
	memcpy(&pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd,&pObj->mMaToMd,sizeof(STR_PubDatMAMD));
	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToMd.gWrFlag = 0;
	
	//	����MA2UA��Ϣ����������
	if(pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToUa.SR.Bit.locked == 0)
	{
		memcpy(&pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToUa,&pObj->mMaToUa,sizeof(STR_PubDatMAUA));
	}
}


//************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



