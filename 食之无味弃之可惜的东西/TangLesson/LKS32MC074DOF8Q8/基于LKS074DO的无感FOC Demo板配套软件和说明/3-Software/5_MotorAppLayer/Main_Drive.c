/*******************************************************************************
 * ��Ȩ���� (C)2019, Bright Power Semiconductor Co.ltd
 * �ļ����ƣ� Main_Drive.c
 * �ļ���ʶ��
 * ����ժҪ�� ״̬�������� 
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��8��19��
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "MA_Static_Variable.h"
#include "MC_Math.h"
#include "MC_type.h"
#include "Time_Process.h"

//////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************/
//ȫ�ֱ���������


extern bool MinQSpdFilReachHi[MA_OBJ_MAX_DEFINE];
/*******************************************************************************/
//�ⲿ����������

extern void FaultInit(STR_MACtrProc *pObj);														//	�����������ʼ��
extern void FaultInitTask(STR_MACtrProc *pObj);													//	�����������ʼ������
extern void FaultRecoverInit(STR_MACtrProc *pObj);
extern void FaultRecover(STR_MACtrProc *pObj);
extern void FaultCurrentCheck(STR_VectorUVW *pCurrent,STR_MACtrProc *pObj);
extern void updateObParaMALayer(PSTR_ControlObject pCtrlObj);
extern void updateAcrParaMALayer(PSTR_ControlObject pCtrlObj);
extern void updateSVPWMParaMALayer(PSTR_ControlObject pCtrlObj);

extern void PowerLoopInit(STR_MACtrProc *pObj);
extern void PowerLimitCalc(STR_MechQuantity *pstruMotorSpeed, s32 PowerValue);
extern void SpeedLimitCalc(s32 wSpeedFbk, pSTR_PowerDef pstruPower);
extern void PowerLoopReg(pSTR_PowerDef pstru_Power);
extern void BrakeStateCheck(u32 t_CurrentSqrt,pSTR_OnTheFlyDetectMA this);

extern bool getRunStatus(u8 t_bObjId);
extern s16	AutoFieldWeakReg(u8 t_bObjId);
extern void InitFieldWeakenID(u8 t_bObjId);
extern bool motorStopOrExceptionHandle(u8 RunStop,pSTR_MotorExceptionMA this,s16 *PReturnCurSet);
extern void InitDQCurRefGen(u8 t_bObjId, s16 dCurSet,s16 qCurSet, s16 TransTime);
extern void SetDCurRefInitValue(u8 t_bObjId, s16 dCurref);
extern s16 getDCurRefGen(u8 t_bObjId);
extern void QCurRefGenValue(u8 t_bObjId, s64 *pValue,u8 QFormat,s32 spdSet);
extern void SpeedPIAutoTunning(u8 t_bObjId,s16 KpFactor, s16 Ton, s32 *pKp, s32 *pKi);
extern void InitDQCurRefGenWithWeakenFlux(u8 t_bObjId, s16 dCurSet, s16 qCurSet,s16 TransTime);
extern s16 getDCurRefGenWithEnWeakenFlux(u8 t_bObjId);
extern s16 getDCurRefGenWithWeakenFlux(u8 t_bObjId);

extern bool	IsNeedIPD(STR_MACtrProc *pObj);
extern void setOBElectAngleEst(u8 t_bObjId, u32 theta);

extern bool getIfSpdReachHigh(u8 t_bObjId);
extern void MinMotorVoltSpd2Core(u8 tObjId);

/*******************************************************************************/
//�ֲ���̬����������
static void CurrentLoopRef_filter(STR_MACtrProc *pObj);				//	�����ο��˲�������
#if (DRV0_CLOSE_LOOP==SPEED_LOOP)
static s16 SpeedLoopReg(STR_MechQuantity *this);
#endif


static void StateInit(STR_MACtrProc *pObj);
static void StateCharge(STR_MACtrProc *pObj);
static void StateDirCheck(STR_MACtrProc *pObj);
static void StateAlign(STR_MACtrProc *pObj);
static void StateRun(STR_MACtrProc *pObj);
static void StateFault(STR_MACtrProc *pObj);
static void StateBrake(STR_MACtrProc *pObj);

/*******************************************************************************/
//�ֲ���̬����������


/*******************************************************************************/
/*****************************end***********************************************/
/*******************************************************************************
 �������ƣ�    static void handleIfMotorSpdZero(STR_MACtrProc *pObj)
 ����������    ������Ƿ�ֹͣ
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    m_bMotorStopped ��־			   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void handleIfMotorSpdZero(STR_MACtrProc *pObj)
{
	PSTR_DrvCfgPara			t_pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	STR_TransCoefElement 	*pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);

	s32 t_wMinSpeed	= App2CoreFreqTrans(pAppToCore,t_pDrvCfgPara->mS_FBSlvcCfg0.m_wMinRunFreq);

	if (ABS(pObj->m_pMotorSpeed->wSpeedfbk) <= t_wMinSpeed)			//	����ٶ�  < ��С�����ٶ� ?
	{
		pObj->m_SMACtrlBit.m_bMotorStopped = TRUE;					//	С��ʱ������ֹͣ��־
	}
	else
	{
		pObj->m_SMACtrlBit.m_bMotorStopped = FALSE;					//	����ʱ������ֹͣ��־
	}	
}
/*******************************************************************************
 �������ƣ�    static void handleCurSetpointClr(STR_MACtrProc *pObj)
 ����������    �������ƶ���ָ����0
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
	#if ((DRV0_STOP_MODE != 0))									//	��MOE�ر�ͣ��
	static void handleCurSetpointClr(STR_MACtrProc *pObj)
	{
	#if (DRV0_CLOSE_LOOP==SPEED_LOOP)
		pObj->m_pMotorSpeed->mSpeedPI.wIntegral = 0;			//	�ٶȻ�����0
		#endif
		pObj->m_nQCurrentSet					= 0;			//	Iq�趨ֵ��0
		pObj->m_nQCur_Reference					= 0;			//	Iq�˲�ֵ��0
	}
	#endif

/*******************************************************************************
 �������ƣ�    static void handleCurSetpoint(STR_MACtrProc *pObj,s16 temp16)
 ����������    ���õ������ƶ���ָ��
 ���������    pObjָ�룬temp16
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
	#if(DRV0_UNDVOLT_PROT_ENABLE == 1)							//	��ѹ����ʹ��
	static void handleCurSetpoint(STR_MACtrProc *pObj,s16 temp16)
	{
		#if (DRV0_CLOSE_LOOP == SPEED_LOOP)		
		pObj->m_pMotorSpeed->mSpeedPI.wIntegral = (s64)temp16 << 22;	//	�ٶȻ����趨
		#endif
		pObj->m_nQCurrentSet					= temp16;		//	Iq�趨ֵ�趨
		pObj->m_nQCur_Reference					= temp16;		//	Iq�˲�ֵ�趨
	}
	#endif

/*******************************************************************************
 �������ƣ�    static void handleSpdSetpoint(STR_MACtrProc *pObj)
 ����������    ����ת�ٿ��ƶ���ָ��
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void handleSpdSetpoint(STR_MACtrProc *pObj)
{
	pObj->m_pMotorSpeed->wSpeedfbk		= pObj->m_pMotorSpeed->wSpeedEst;
	pObj->m_pMotorSpeed->wSpeedfbkFilter	= pObj->m_pMotorSpeed->wSpeedEst << 10;
	pObj->m_pMotorSpeed->wSpeedRef		= pObj->m_pMotorSpeed->wSpeedfbk;
	pObj->m_pMotorSpeed->STR_SpdRampGen.wRampOut = pObj->m_pMotorSpeed->wSpeedfbk;
}

/*******************************************************************************
 �������ƣ�    static void HandleSpdSetpointRamp(STR_MACtrProc *pObj)
 ����������    ����ת�ٿ��ƶ���ָ��ramp����
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void HandleSpdSetpointRamp(STR_MACtrProc *pObj)
{
	STR_TransCoefElement *pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	s32 t_wTemp;
	s32 t_wTemp1;

	//	�����趨���ٶȻ����ڼ���б������
	
	t_wTemp1	= (s32)16777 * (pDrvCfgPara->mS_FBSpdLoop.nSpeedLoopCycle+1);	
	if(ABS(pObj->m_pMotorSpeed->wSpeedRef) < App2CoreFreqTrans(pAppToCore,pObj->m_pAppCommData->wSvcMinFreq) )
	{
		//	�ٶ�ָ��С��SVC��СƵ��(�����϶���ֹƵ��)ʱ���ÿ������ٶ�/���ٶ�
		t_wTemp = App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wFreq_Acc);		
		pObj->m_pMotorSpeed->STR_SpdRampGen.wRampIncrease	= ((s64)t_wTemp * t_wTemp1) >> 8;

		t_wTemp = App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wFreq_Dec);
		pObj->m_pMotorSpeed->STR_SpdRampGen.wRampDecrease	= ((s64)t_wTemp * t_wTemp1) >> 8;
		
	}
	else
	{
		//	�ٶ�ָ�����SVC��СƵ��(�����϶���ֹƵ��)ʱ���ñջ����ٶ�/���ٶȣ����ٶȻ� ���ٶ�/���ٶ�
		t_wTemp = App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wSpdRunAcc);
		pObj->m_pMotorSpeed->STR_SpdRampGen.wRampIncrease	= ((s64)t_wTemp * t_wTemp1) >> 8;
		
		t_wTemp = App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wSpdRunDec);
		pObj->m_pMotorSpeed->STR_SpdRampGen.wRampDecrease	= ((s64)t_wTemp * t_wTemp1) >> 8;
	}


	//	����б������ָ��
	pObj->m_pMotorSpeed->STR_SpdRampGen.wRampIn			= pObj->m_pMotorSpeed->wSpeedSet;
	ramp32GenCalc(&pObj->m_pMotorSpeed->STR_SpdRampGen);		//	б���ٶȹ滮
}

/*******************************************************************************
 �������ƣ�    static void HandleSpdSetpointCmd(STR_MACtrProc *pObj)
 ����������    ����ת�ٿ��ƶ���ָ���
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    wSpeedSet		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
#if (DRV0_CLOSE_LOOP!=POWER_LOOP)
static void HandleSpdSetpointCmd(STR_MACtrProc *pObj)
{
	//	�ٶ��趨ֵ���������趨������
	//	Ĭ��CWʱ���ٶ�ָ��Ϊ��ֵ
	pObj->m_pMotorSpeed->wSpeedSet = ABS(pObj->m_pMotorSpeed->wSpeedSet);
	if(pObj->m_SMACtrlBit.m_bMotorDirtion == CCW)
	{
		pObj->m_pMotorSpeed->wSpeedSet = -pObj->m_pMotorSpeed->wSpeedSet;
	}

	//	����������ʾ��ʽʱ�������޸Ĵ���
	//	��ʵ������ת�ٶ�ָ�����ǰ����֧������ת����(�������ò���)��
}
#endif

// ���Ϊ���ű۵�ͨ��ȣ�0ʱ���ű�ȫͨ
static void SetPWMBrakeDuty(STR_MACtrProc *pObj, s16 duty)
{
	pObj->m_nBreakDuty = duty;				//����PWM��ȡ�ͨ��PWM��ȿ���ɲ���ٶ�ʱ�����MCPWM����
}

static s16 getPWMBrakeDuty(STR_MACtrProc *pObj)
{
	return pObj->m_nBreakDuty;
}


bool IsEnableMoeInIDLEStatus(STR_MACtrProc *pObj)		//stop״̬�Ƿ�ʹ��MOE
{
	return pObj->m_SMACtrlBit.m_bEnableMOEInStop;
}

bool IsBusVoltageLarge(STR_MACtrProc *pObj)
{
	return FALSE;
}

/*******************************************************************************
 �������ƣ�    static void SetDefaultMA2MD(STR_MACtrProc *pObj)
 ����������    ���ÿ��ƶ���MA2MDĬ�ϴ���
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void SetDefaultMA2MD(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD *pMA2MD	= &pObj->mMaToMd;

	if(pObj->m_SMACtrlBit.m_bUAMOE == 1)				//	MOE��Ч��־λΪ1 ?
	{
		SetPWMCmdMA2MD(pMA2MD, E_PWM_ON);				//	����MOEʹ�ܲ����������������MD�����HD��������ʵ��MOE����
	}
	else
	{
		SetPWMCmdMA2MD(pMA2MD, E_PWM_OFF);				//	���ò����������������MD�����HD��������ʵ��MOE����
	}
	
	SetBrakePWMDutyMA2MD(pMA2MD, getPWMBrakeDuty(pObj));	//	�����ƶ���ȵ�MD�㣬��MD�����HD��������ʵ��PWM�������
	
	SetMoeBlankTimeMA2MD(pMA2MD,5);							//	������Ч���ʱ�䣬MD��������
	SetMotorCmdMA2MD(pMA2MD, E_MOTOR_RESET);				//	����MD��״̬������ͨ�����ݽ�����ֵ����ʵ��
}

/*******************************************************************************
 �������ƣ�    static void SetFaultMA2MD(STR_MACtrProc *pObj)
 ����������    ���ÿ��ƶ���MA2MDĬ��FAULT����
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void SetFaultMA2MD(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD *pMA2MD	= &pObj->mMaToMd;
	
	if(pObj->m_SMACtrlBit.m_bUAMOE == 1)				//	MOE��Ч��־λΪ1?	--- FAULT״̬�Ƿ�ʹ��MOE?
	{
		SetPWMCmdMA2MD(pMA2MD, E_PWM_ON);				//	����MOEʹ�ܲ����������������MD�����HD��������ʵ��MOE����
	}
	else
	{
		SetPWMCmdMA2MD(pMA2MD, E_PWM_OFF);				//	���ò����������������MD�����HD��������ʵ��MOE����
	}

	SetBrakePWMDutyMA2MD(pMA2MD, getPWMBrakeDuty(pObj));	//	�����ƶ���ȵ�MD�㣬��MD�����HD��������ʵ��PWM�������
	
	SetMoeBlankTimeMA2MD(pMA2MD,5);
	SetMotorCmdMA2MD(pMA2MD, E_MOTOR_RESET);				//	������Ч���ʱ�䣬MD��������
}

static PSTR_PubDatMAMD GetPointerMA2MD(STR_MACtrProc *pObj)
{
	return &(pObj->mMaToMd);
}

static pSTR_PubDatMAUA GetPointerMA2UA(STR_MACtrProc *pObj)
{
	return &(pObj->mMaToUa);
}


/*******************************************************************************
 �������ƣ�    static void InitObserverDone(STR_MACtrProc *pObj)
 ����������    ��ʼ���۲���
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool InitObserverDone(STR_MACtrProc *pObj)
{
	bool t_bResult = FALSE;
	if( (pObj->m_SMACtrlBit.m_bObParaReq == 1)						//	�۲���������ʼ������
		&& (pObj->m_SMACtrlBit.m_bObParaUpdate == 1)				//	�۲���������Ҫ����
		&& (getNonInitResultMD2MA(&pObj->mMdToMa))					//	û����������������ִ��
	   )
	{
		updateObParaMALayer(pObj->mg_pCtrlObj);						//	���¹۲�������
		pObj->m_SMACtrlBit.m_bObParaReq			= 0;				//	����۲���������ʼ�������־
		pObj->m_SMACtrlBit.m_bObParaUpdate		= 0;				//	�۲�����������Ҫ����(�������)
		pObj->m_SMACtrlBit.m_bObParaInitDone	= 0;				//	�۲���������ʼ��δ���
	}
	
	if(pObj->m_SMACtrlBit.m_bObParaInitDone == 0)					//	�۲���������ʼ��δ��� ?
	{
		if(pObj->m_SMACtrlBit.m_bObParaUpdate == 0)					//	�۲�����������Ҫ����(�������) ?
		{
			if(!getObsevInitResultMD2MA(&pObj->mMdToMa))			//	MD���ʼ���Ƿ����
			{
				setObsevInitFCmdMA2MD(&pObj->mMaToMd);				//	δ��ɣ����ͳ�ʼ������
			}
			else
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);					//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bObParaUpdate		= 1;		//	�´γ�ʼ��ʱ���۲���������Ҫ����		
			}
		}
		else
		{		
			if(getNonInitResultMD2MA(&pObj->mMdToMa))				//	�۲�����������Ҫ����(�������) �������
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);					//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bObParaInitDone	= 1;		//	�۲���������ʼ�����		
			}
		}
	}
	else
	{
		if(pObj->m_SMACtrlBit.m_bObParaReq == 0)					//	�۲���������ʼ����ɣ�û�г�ʼ������
		{
			t_bResult = TRUE;										//	�������
		}
	}
	return t_bResult;
}

/*******************************************************************************
 �������ƣ�    static void InitAcrDone(STR_MACtrProc *pObj)
 ����������    ��ʼ��MD��FOC���Ʊ���
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool InitAcrDone(STR_MACtrProc *pObj)
{
	bool t_bResult = FALSE;
	
	if((pObj->m_SMACtrlBit.m_bAcrParaReq == 1)							//	������������ʼ������
		&& (pObj->m_SMACtrlBit.m_bAcrParaUpdate == 1)					//	������������Ҫ����
		&& (getNonInitResultMD2MA(&pObj->mMdToMa))						//	û����������������ִ��
	  )
	{
		updateAcrParaMALayer(pObj->mg_pCtrlObj);						//	���µ���������
		pObj->m_SMACtrlBit.m_bAcrParaReq	= 0;						//	���������������ʼ�������־
		pObj->m_SMACtrlBit.m_bAcrParaUpdate	= 0;						//	��������������Ҫ����(�������)
		pObj->m_SMACtrlBit.m_bAcrInitDone	= 0;						//	������������ʼ��δ���
	}
	
	if(pObj->m_SMACtrlBit.m_bAcrInitDone == 0)							//	������������ʼ��δ��� ?
	{
		if(pObj->m_SMACtrlBit.m_bAcrParaUpdate == 0)					//	��������������Ҫ����(�������) ?
		{
			if(!getAcrInitResultMD2MA(&pObj->mMdToMa))					//	MD���ʼ���Ƿ����
			{
				setAcrInitFCmdMA2MD(&pObj->mMaToMd);					//	δ��ɣ����ͳ�ʼ������
			}
			else
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);						//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bAcrParaUpdate	= 1;				//	�´γ�ʼ��ʱ��������������Ҫ����
			}
		}
		else
		{
			if(getNonInitResultMD2MA(&pObj->mMdToMa))					//	��������������Ҫ����(�������) �������
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);						//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bAcrInitDone	= 1;				//	������������ʼ�����
			}
		}
	}
	else
	{		
		if(pObj->m_SMACtrlBit.m_bAcrParaReq == 0)						//	������������ʼ����ɣ�û�г�ʼ������
		{
			t_bResult = TRUE;											//	�������
		}
	}
	return t_bResult;
}

/*******************************************************************************
 �������ƣ�    static void InitSVPWMDone(STR_MACtrProc *pObj)
 ����������    ��ʼ��MD��SVPWM���Ʋ���
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool InitSVPWMDone(STR_MACtrProc *pObj)
{
	bool t_bResult = FALSE;
	if( (pObj->m_SMACtrlBit.m_bSVPWMParaReq == 1)						//	SVPWM������ʼ������
		&& (pObj->m_SMACtrlBit.m_bSVPWMParaUpdate == 1)					//	SVPWM������Ҫ����
		&& (getNonInitResultMD2MA(&pObj->mMdToMa))						//	û����������������ִ��
	  )
	{
		updateSVPWMParaMALayer(pObj->mg_pCtrlObj);						//	����SVPWM����
		pObj->m_SMACtrlBit.m_bSVPWMParaReq		= 0;					//	���SVPWM������ʼ�������־
		pObj->m_SMACtrlBit.m_bSVPWMParaUpdate	= 0;					//	SVPWM��������Ҫ����(�������
		pObj->m_SMACtrlBit.m_bSVPWMInitDone		= 0;					//	SVPWM������ʼ��δ���
	}
	if(pObj->m_SMACtrlBit.m_bSVPWMInitDone == 0)						//	SVPWM������ʼ��δ��� ?
	{
		if(pObj->m_SMACtrlBit.m_bSVPWMParaUpdate == 0)					//	SVPWM��������Ҫ����(�������) ?
		{
			if(!getSVPWMInitResultMD2MA(&pObj->mMdToMa))				//	MD���ʼ���Ƿ����
			{
				setSVPWMInitFCmdMA2MD(&pObj->mMaToMd);					//	δ��ɣ����ͳ�ʼ������
			}
			else
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);						//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bSVPWMParaUpdate	= 1;			//	�´γ�ʼ��ʱ��SVPWM������Ҫ����
			}
		}
		else
		{
			if(getNonInitResultMD2MA(&pObj->mMdToMa))					//	SVPWM��������Ҫ����(�������) �������
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);						//	����ɣ�����Ĭ������
				pObj->m_SMACtrlBit.m_bSVPWMInitDone		= 1;			//	SVPWM������ʼ�����
			}
		}
	}
	else
	{
		setDefaultCmdMA2MD(&pObj->mMaToMd);								//	����ɣ�����Ĭ������
		if(pObj->m_SMACtrlBit.m_bSVPWMParaReq == 0)						//	SVPWM������ʼ����ɣ�û�г�ʼ������
			{
			t_bResult =  TRUE;											//	�������
			}		
	}
	return t_bResult;
}

/*******************************************************************************
 �������ƣ�    static void motorControlParaInitDone(STR_MACtrProc *pObj)
 ����������    �������MD��ȫ����ʼ������
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool motorControlParaInitDone(STR_MACtrProc *pObj)
{
	bool t_bReturn;

	t_bReturn		= InitObserverDone(pObj);					//	��ʼ���۲������������ʱ����TRUE

	if(t_bReturn)		//	�۲���������ʼ����ɺ󣬽���SVPWM������ʼ��
	{
		t_bReturn	= InitSVPWMDone(pObj);						//	��ʼ��SVPWM����, ���ʱ����TRUE
	}

	if(t_bReturn)		//	SVPWM������ʼ����ɺ󣬽��е�����������ʼ��
	{
		t_bReturn	= InitAcrDone(pObj);						//	��ʼ������������, ���ʱ����TRUE
	}

	if(t_bReturn)		//	������������ʼ����ɺ�
		{
		if( (pObj->m_SMACtrlBit.m_bObParaInitDone == 1)			//����ʼ�����ʱ��������ʼ�����
			&& (pObj->m_SMACtrlBit.m_bAcrInitDone == 1)
			&& (pObj->m_SMACtrlBit.m_bSVPWMInitDone == 1)
			)
			{
			t_bReturn		= TRUE;
			}
		else
			{
			t_bReturn		= FALSE;
			}
		}	
	return t_bReturn;
}
/*******************************************************************************
 �������ƣ�    static void motorAcrDataInitDone(STR_MACtrProc *pObj)
 ����������    ������������ݳ�ʼ������
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool motorAcrDataInitDone(STR_MACtrProc *pObj)
{
	bool t_bReturn = FALSE;

	if(pObj->m_SMACtrlBit.m_bObParaInitDone == 1)		//	�۲���������ʼ������ܽ��е�����������ʼ��
	{
		t_bReturn	= InitAcrDone(pObj);
	}

	if(t_bReturn)
	{
		if( (pObj->m_SMACtrlBit.m_bObParaInitDone == 1)		//	�۲���������ʼ����ɱ�־
			&& (pObj->m_SMACtrlBit.m_bAcrInitDone == 1)		//	������������ʼ����ɱ�־
			)
		{
			t_bReturn		= TRUE;						//	������������ʼ�����
		}
		else
		{
			t_bReturn		= FALSE;
		}
	}
	return t_bReturn;
}

/*******************************************************************************
 �������ƣ�    static void setInitAcrParaReq(STR_MACtrProc *pObj)
 ����������    ����MD���ʼ������
 ���������    pObjָ��
 ���������    ��
 �� �� ֵ��    
 ����˵����    		   
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/2/19      V1.0           Tonghua Li          ����
 *******************************************************************************/

void setInitAcrParaReq(STR_MACtrProc *pObj)				//	���õ�����������ʼ������
{
	pObj->m_SMACtrlBit.m_bAcrParaReq	= 1;
}

void setInitObserverParaReq(STR_MACtrProc *pObj)		//	���ù۲���������ʼ������
{
	pObj->m_SMACtrlBit.m_bObParaReq	= 1;
}

void setInitSvpwmParaReq(STR_MACtrProc *pObj)			//	����SVPWM������ʼ������
{
	pObj->m_SMACtrlBit.m_bSVPWMParaReq	= 1;
}



/*****************************************************************************
 * ������   : bool InitDriverMain(STR_MACtrProc *pObj)
 * ˵��     : ������ʼ�������� TRUE ---- ��ǰ����ִ�����
 *          : ����ʵ�֣�������ʼ��
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
bool InitDriverMain(STR_MACtrProc *pObj)
{
	bool t_bReturn = FALSE;
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	t_bReturn = motorControlParaInitDone(pObj);    //�������MD��ȫ����ʼ������
	if(t_bReturn)
	{
		StateInit(pObj);
		/*****�����������ʼ��********/
		if(pObj->m_SMACtrlBit.m_bFaultInitReq == 1)		//	����FAULT��ʼ������
		{
    		FaultInit(pObj);							//	FAULT��ʼ��
			FaultRecoverInit(pObj);
			pObj->m_SMACtrlBit.m_bFaultInitReq	= 0;	//	AULT��ʼ������ 			��0
			pObj->m_SMACtrlBit.m_bFaultInitDone = 1;	//	FAULT��ʼ����ɱ�־ 	��1
		}
	}
	
	//
	handleIfMotorSpdZero(pObj);     //������Ƿ�ֹͣ
	
	//update cmd MA2MD
	SetDefaultMA2MD(pObj);								//	����Ĭ�ϲ�������

	//update MA SR infor to UA	

	return t_bReturn;
}

/*****************************************************************************
 * ������   : void PowerOnOffDriverMain(STR_MACtrProc *pObj,bool t_bOn)
 * ˵��     : ���ʰ����µ紦����
 *          : t_bOn     TRUE �ϵ�
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.12.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
bool PowerOnOffDriverMain(STR_MACtrProc *pObj, bool t_bOn)
{
	bool t_bReturn = FALSE;
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	StateInit(pObj);

	t_bReturn = motorAcrDataInitDone(pObj);	    //�����������ʼ������

	handleIfMotorSpdZero(pObj);					//	������Ƿ�ֹͣ
	
	SetDefaultMA2MD(pObj);						//	����Ĭ�ϲ�������

	if(t_bOn)
	{
		//����������м̵����պϵȴ���

	}
	else
	{
		//����������м̵����Ͽ��ȴ���
		setFaultStatMA2UA(pS_MA2UA, TRUE);		//	����FAULT��־��ͨ���������ݽ�����֪ͨUA��
	}

	//update MA SR infor to UA
	
	return t_bReturn;
}
/*****************************************************************************
 * ������   : void CalibDriverMain(STR_MACtrProc *pObj)
 * ˵��     : ���ʰ�У��������
 *          : ����ʵ�ָ�����Ҫ���
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
void CalibDriverMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	pObj->m_SMACtrlBit.m_bCalibFinish = 1;

	handleIfMotorSpdZero(pObj);
	
	// update MA infor to MD
	SetDefaultMA2MD(pObj);

}

/*****************************************************************************
 * ������   : bool StopMotorMain(STR_MACtrProc *pObj)
 * ˵��     : ���stop״̬������ TRUE ---- ��ǰ����ִ�����
 *          : ����ʵ��StateInit()
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
bool StopMotorMain(STR_MACtrProc *pObj)
{	
	bool t_bReturn = FALSE;
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	t_bReturn = motorControlParaInitDone(pObj);			//	STOP״̬ʱ�����Խ��й۲�����SVPWM����������������������
														//	���Ը�����Ҫ��Ӷ�̬�������벢���ó�ʼ������
	if(t_bReturn)										//	������ʼ�����
	{
		StateInit(pObj);								//	������ʼ�� (�����ڹ۲����ı���ת�������ȣ��������ȳ�ʼ���۲�����)
		
		/*****�����������ʼ��********/
		if(pObj->m_SMACtrlBit.m_bFaultInitReq == 1)		//	FAULT������ʼ�����󣬿ɶ�̬�޸Ĳ������г�ʼ��
		{
    		FaultInit(pObj);
			FaultRecoverInit(pObj);
			pObj->m_SMACtrlBit.m_bFaultInitReq = 0;
		}
		else											//	û��FAULT������ʼ������
		{
			FaultInitTask(pObj);						//  ��ʼ��FAULT����
		}
		pObj->m_SMACtrlBit.m_bFaultInitDone = 1;
		pObj->m_SMACtrlBit.m_bPostOK	= 1;			//	POST��־	��1
	}	
	
	handleIfMotorSpdZero(pObj);							//	������Ƿ�ֹͣ
	
	// update MA infor to MD
	SetDefaultMA2MD(pObj);								//	����Ĭ�ϲ�������

	//update MA SR infor to UA
	
	if (pObj->m_SMACtrlBit.m_bUAMOE)					//	MOE�Ƿ�ʹ��
	{
		setPWMStatMA2UA(pS_MA2UA,TRUE);					//	����MOEʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
	}
	
	return t_bReturn;
}

/*****************************************************************************
 * ������   : void ChargeDriverMain(STR_MACtrProc *pObj)
 * ˵��     : ���ʰ�Ԥ��纯��
 *          : ����ʵ��StateCharge()
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
void ChargeDriverMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	StateCharge(pObj);									//	Ԥ��紦����
	pObj->m_SMACtrlBit.m_bMotorStopped	= TRUE;			//	���õ��ֹͣ��־������Ƿ�����ֹͣȡ����Ӧ��
														//	������MOE�رգ�Ҫͨ��������˳�������ж�
														//	��ǰ״̬�£���־ΪTRUʱ���������ת����

	// update MA infor to MD
	SetBrakePWMDutyMA2MD(pS_MA2MD,pObj->m_pAppCommData->nChargePwmWidth);
	
	//SetMoeBlankTimeMA2MD(pS_MA2MD,5);
	SetMotorCmdMA2MD(pS_MA2MD, E_MOTOR_RESET);			//	����MD��״̬��ͨ���������ݽ�����֪ͨMD��

	//update MA SR infor to UA	
	
	setPWMStatMA2UA(pS_MA2UA,TRUE);						//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : void MotorTrakingMain(STR_MACtrProc *pObj)
 * ˵��     : ˳����⺯��
 *          : ����ʵ��StateDirCheck()
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
void MotorTrakingMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);
	
	StateDirCheck(pObj);								//	˳����⺯��
	

	pObj->m_SMACtrlBit.m_bMotorStopped = FALSE;			//	Ĭ�ϵ������ת��״̬

	
	#if (DRV0_BEMF_CHECK_EABLE==FUNCTION_ON)			//�������ʹ�ܷ����Ƶ�·˳���ʱ�������
		SetPWMCmdMA2MD(pS_MA2MD, E_PWM_OFF);
		SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_BEMF_TRACKING);
		#else																	//	�۲���˳���
		SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);				//	���ݿ������MDʹ��MOE			
		SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_TRACKING);	//	����MD��״̬Ϊ˳�����״̬
		#endif


	//update MA SR infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);						//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}


/*****************************************************************************
 * ������   : void MotorTracking2Run(STR_MACtrProc *pObj)
 * ˵��     : ���˳���������к���
 *          : 
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/03/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
void MotorTracking2Run(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	pObj->m_pAppCommData->bOpenRunFlag	= 1;		//	����״̬���Ϊ˳���

	handleSpdSetpoint(pObj);						//	����˳������ٶȳ�ʼ���ٶȻ��ٶ��趨ֵ
	pObj->m_SMACtrlBit.m_bMotorStopped	= FALSE;	//	���������

	// update MA infor to MD	
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);				//	���ݿ������MDʹ��MOE		
	//SetCurRefMA2MD(pS_MA2MD,0,0);
	SetSpeedRefMA2MD(pS_MA2MD,pObj->m_pMotorSpeed->wSpeedRef);	//	�����ٶ��趨ֵ��MD��

	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_SYN);			//	����MD��״̬Ϊ����״̬ E_MOTOR_SYN

	//update MA SR infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);	
}


/*****************************************************************************
 * ������   : void MotorTracking2Brake(STR_MACtrProc *pObj)
 * ˵��     : ���˳������ƶ���������ֹ�޸�FOC����
 *          : 
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
void MotorTracking2Brake(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	StateBrake(pObj);										//	˳���� �ƶ�����
	pObj->m_pAppCommData->bOpenRunFlag	= 0;

	// update MA infor to MD
	SetBrakePWMDutyMA2MD(pS_MA2MD,getPWMBrakeDuty(pObj));	//	�趨PWM���
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);						//	���ݿ������MDʹ��MOE	
	setDefaultCmdMA2MD(pS_MA2MD);							//	����Ĭ�ϲ�������
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_BRAKE);				//	����MD��״̬Ϊ����״̬ E_MOTOR_BRAKE

	//update MA SR infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);							//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : bool MotorInitIPDMain(void)
 * ˵��     : �����ʼλ�ü���ʼ������
 *          : 
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
bool MotorInitIPDMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	bool			t_return = FALSE;

	// update MA infor to MD	
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);	
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_RESET);

	if(pObj->m_SMACtrlBit.m_bIPDInitReq == 0)				//	IPD��ʼ�������־
	{
		pObj->m_SMACtrlBit.m_bIPDInitDone	= 0;
		if(getNonInitResultMD2MA(&pObj->mMdToMa))			//	û����������������ִ��
		{
			setIPDInitFCmdMA2MD(pS_MA2MD);					//	����IPD������ʼ�����������MD��
			pObj->m_SMACtrlBit.m_bIPDInitReq = 1;			//	IPD��ʼ�������־ ��1
		}
		else
		{
			setDefaultCmdMA2MD(pS_MA2MD);					//	����Ĭ�ϲ�������
		}
	}
	else											//	����IPD��ʼ������
	{
		if(pObj->m_SMACtrlBit.m_bIPDInitDone == 0)			//	IPD������ʼ�����? 0 ----δ���
		{
			if(!getIPDInitResultMD2MA(&pObj->mMdToMa))		//	MD���Ƿ����IPD������ʼ������ ?
			{	
				setIPDInitFCmdMA2MD(pS_MA2MD);				//	δ��ɣ�����IPD������ʼ�����������MD��
			}
			else
			{												//	�����
				setDefaultCmdMA2MD(pS_MA2MD);				//	����Ĭ�ϲ�������
				pObj->m_SMACtrlBit.m_bIPDInitDone	= 1;	//	IPD������ʼ����ɱ�־	��1
			}
		}
		else
		{													//	IPD������ʼ�������
			if(getNonInitResultMD2MA(&pObj->mMdToMa))		//  MD���޲�������״̬
		{
				pObj->m_SMACtrlBit.m_bIPDInitReq		= 0;		//	IPD��ʼ�������־ ���
				t_return								= TRUE;		//	IPD��ʼ�����
			}
			else
			{												//  MD�����������������״̬ʱ������Ϊ�޲���״̬
				setDefaultCmdMA2MD(pS_MA2MD);
			}
		}
	}

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);							//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��

	if(getIPDDoneResultMD2MA(&pObj->mMdToMa))				//	��ȡMD���ʼ����������� TRUE�������
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 1;				//	IPD��ʼ�����
	}
	else
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 0;
	}

	return t_return;										//	���ز������
}

bool MotorInitIPDMainWait(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	bool			t_return = FALSE;

	// update MA infor to MD	
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_OFF);	
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_RESET);

	if(pObj->m_SMACtrlBit.m_bIPDInitReq == 0)
	{
		pObj->m_SMACtrlBit.m_bIPDInitDone	= 0;
		if(getNonInitResultMD2MA(&pObj->mMdToMa))
		{
			setIPDInitFCmdMA2MD(pS_MA2MD);
			pObj->m_SMACtrlBit.m_bIPDInitReq = 1;
		}
		else
		{
			setDefaultCmdMA2MD(pS_MA2MD);
		}
	}
	else
	{
		if(pObj->m_SMACtrlBit.m_bIPDInitDone == 0)
		{
			if(!getIPDInitResultMD2MA(&pObj->mMdToMa))
			{	
				setIPDInitFCmdMA2MD(pS_MA2MD);
			}
			else
			{
				setDefaultCmdMA2MD(pS_MA2MD);
				pObj->m_SMACtrlBit.m_bIPDInitDone	= 1;
			}
		}
		else
		{
			if(getNonInitResultMD2MA(&pObj->mMdToMa))
			{
				pObj->m_SMACtrlBit.m_bIPDInitReq		= 0;
				t_return								= TRUE;
			}
			else
			{
				setDefaultCmdMA2MD(pS_MA2MD);
			}
		}
	}

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);

	if(getIPDDoneResultMD2MA(&pObj->mMdToMa))
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 1;
	}
	else
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 0;
	}

	return t_return;
}


/*****************************************************************************
 * ������   : bool MotorIPDMain(void)
 * ˵��     : �����ʼλ�ü�⺯��
 *          : 
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
void MotorIPDMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	// update MA infor to MD	

	if(pObj->m_SMACtrlBit.m_bMotorDirtion == CW)			//	�趨Ϊ CW����
	{
		setIPDCWStartFCmdMA2MD(pS_MA2MD);					//	CW�Ƕȴ���MD��ʵ��
	}
	else
	{
		setIPDCCWStartFCmdMA2MD(pS_MA2MD);					//	CCW�Ƕȴ���MD��ʵ��
	}
	
	SetIPDEnableMA2MD(pS_MA2MD,1);							//	IPD���ʹ�ܱ�־

	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);						//	���ݿ������MDʹ��MOE	
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_IPD);					//	����MD��״̬Ϊ����״̬ E_MOTOR_IPD

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);							//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��

	if(getIPDDoneResultMD2MA(&pObj->mMdToMa))				//	��ȡMD��IPD�������
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 1;				//	IPD��ɱ�־ ��1 ----- �����
		setDefaultCmdMA2MD(pS_MA2MD);
	}
	else
	{
		pObj->m_SMACtrlBit.m_bIPDFinish = 0;				//	IPD��ɱ�־ ��0 ----- δ���
	}
}

/*****************************************************************************
 * ������   : void MotorAlignMain(void)
 * ˵��     : ������ຯ��
 *          : ����ʵ��StateAlign()
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
void MotorAlignMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);
	
	StateAlign(pObj);										//	���ദ����

	pObj->m_SMACtrlBit.m_bMotorStopped = TRUE;				//	���ֹͣ״̬

	// update MA infor to MD
	SetCurRefMA2MD(pS_MA2MD,pObj->m_nDCur_Reference,pObj->m_nQCur_Reference);	//	����dq�����ָ��ֵ
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);						//	���ݿ������MDʹ��MOE	
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_ALIGN);				//	����MD��״̬Ϊ����״̬ E_MOTOR_ALIGN

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);							//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : void MotorCloseMain(void)
 * ˵��     : ���run״̬�������һ��Ϊ�ٶȻ������ʻ��Ĵ���
 *          : ������̼�StateRun()����ʵ��
 *			: 
 * ���˼· ��1.����ʵ�ʸ��ؼ����ٶ�/����/���������¡��ٶȻ�/���ʻ���PI
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/02/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
void MotorCloseMain(STR_MACtrProc *pObj)
{	
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);
	

	StateRun(pObj);                 //״̬������
	
	handleIfMotorSpdZero(pObj);     //������Ƿ�ֹͣ

	#if (DRV0_UNDVOLT_PROT_ENABLE==1)
														//	��ѹ�쳣�����Ƿ�����
	s16 temp16;
	if(pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_FBSlvcCfg0.bVoltageProtectionEnable == 1)
	{
		pObj->m_pMotorException->nBusVoltage	= pObj->mMdToMa.nVdc;
		if(motorStopOrExceptionHandle(pObj->m_SMACtrlBit.m_bMC_RunFlg, pObj->m_pMotorException,&temp16))
		{			
			handleCurSetpoint(pObj,temp16);							//	��ѹ�쳣���趨�ٶ�ָ��
			if(pObj->m_eStopMode != 0)								//	�ǹر�MOEֹͣʱ
			{
				pObj->m_SMACtrlBit.m_bExceptionEnable	= 1;		//	�쳣��־ ��1
			}
		}
		else
		{
			pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;			//	���쳣ʱ���쳣��־ ��0
		}
	}
	else
	{
		pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;				//	��ѹ�쳣����δʹ��ʱ���쳣��־ ��0
	}
	#endif

	// update MA infor to MD
	SetCurRefMA2MD(pS_MA2MD,pObj->m_nDCur_Reference,pObj->m_nQCur_Reference);	//	����dq�����ָ��ֵ
	SetSpeedRefMA2MD(pS_MA2MD,pObj->m_pMotorSpeed->wSpeedRef);		//	�����ٶ�ָ��ֵ��MD��
	SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);								//	���ݿ������MDʹ��MOE	
	SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_SYN);							//	����MD��״̬Ϊ����״̬ E_MOTOR_SYN

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);									//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : void MotorBreakMain(STR_MACtrProc *pObj)
 * ˵��     : ����ƶ����ƺ���
 *          : ����ʵ��StateBrake()
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/02/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
void MotorBreakMain(STR_MACtrProc *pObj)
{
	#if (DRV0_STOP_MODE != 0)
	s16 temp16;
	#endif
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);

	// update MA infor to MD
	#if (DRV0_STOP_MODE == 1)    //�����
	{
		StateRun(pObj);												//	������
		handleSpdSetpoint(pObj);									//	�ٶ�����ֵ����

		#if(DRV0_UNDVOLT_PROT_ENABLE == 1)							//	��ѹ�쳣����
		{
			pObj->m_pMotorException->nBusVoltage	= pObj->mMdToMa.nVdc;
			//	��ѹ�쳣����
			if(motorStopOrExceptionHandle(pObj->m_SMACtrlBit.m_bMC_RunFlg, pObj->m_pMotorException,&temp16))
			{
				handleCurSetpoint(pObj,temp16);						//	�ٶ�����ֵ����
				pObj->m_SMACtrlBit.m_bExceptionEnable	= 1;		//	�쳣��־ ��1
			}
			else
			{	
				handleCurSetpointClr(pObj);							//	�ٶ�����ֵ����
				pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;		//	���쳣ʱ���쳣��־ ��0
			}
		}
		#else
		{	
			handleCurSetpointClr(pObj);								//	�ٶ�����ֵ����
			pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;			//	���쳣��������ʱ���쳣��־ ��0
		}		
		#endif
		
		SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);							//	���ݿ������MDʹ��MOE	
		SetCurRefMA2MD(pS_MA2MD,pObj->m_nDCur_Reference,pObj->m_nQCur_Reference);	//	����dq�����ָ��ֵ
		SetSpeedRefMA2MD(pS_MA2MD,pObj->m_pMotorSpeed->wSpeedRef);	//	�����ٶ�ָ��ֵ��MD��
		SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_SYN);						//	����MD��״̬Ϊ����״̬ E_MOTOR_SYN
		
		handleIfMotorSpdZero(pObj);									//	������Ƿ�ֹͣ
	}
	#elif (DRV0_STOP_MODE == 2)     //�̽�
	{
		StateBrake(pObj);											//	�̽��ƶ�������
		handleCurSetpointClr(pObj);									//	�ٶ�����ֵ����

		#if(DRV0_UNDVOLT_PROT_ENABLE == 1)							//	��ѹ�����Ƿ�����
		pObj->m_pMotorException->nBusVoltage	= pObj->mMdToMa.nVdc;
		if(motorStopOrExceptionHandle(pObj->m_SMACtrlBit.m_bMC_RunFlg, pObj->m_pMotorException,&temp16))
		{
			pObj->m_SMACtrlBit.m_bExceptionEnable	= 1;			//	�쳣��־ ��1
			SetPWMBrakeDuty(pObj,2048);								//	����PWM���
		}
		else
		{			
			pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;			//	�쳣��־ ��0
			SetPWMBrakeDuty(pObj,0);
		}
		#else

		pObj->m_SMACtrlBit.m_bExceptionEnable	= 0;				//	��ѹ����������	�쳣��־ ��0
		SetPWMBrakeDuty(pObj,0);
		#endif
		
		SetPWMCmdMA2MD(pS_MA2MD, E_PWM_ON);							//	���ݿ������MDʹ��MOE	
		SetCurRefMA2MD(pS_MA2MD,0,0);
		SetSpeedRefMA2MD(pS_MA2MD,pObj->m_pMotorSpeed->wSpeedRef);
		SetBrakePWMDutyMA2MD(pS_MA2MD,getPWMBrakeDuty(pObj));
		SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_BRAKE);					//	����MD��״̬Ϊ����״̬ E_MOTOR_BRAKE
	}
	#else                    //�ر�MOE
	{
		SetTime_CountDown(pObj);									//	��ʱ����
		if (SetTime_IsElapsed(pObj->m_nSetTimeLeftCnt))				//	ͣ��ʱ���Ƿ񵽴�
		{
			pObj->m_SMACtrlBit.m_bMotorStopped			= TRUE;		//	���ֹͣ
		}
		else
		{
			pObj->m_SMACtrlBit.m_bMotorStopped			= FALSE;	//	���������
		}
		SetPWMCmdMA2MD(pS_MA2MD, E_PWM_OFF);						//	���ݿ������MD�ر�MOE	
		SetCurRefMA2MD(pS_MA2MD,0,0);
		SetSpeedRefMA2MD(pS_MA2MD,0);
		SetMotorCmdMA2MD(pS_MA2MD,E_MOTOR_RESET);					//	����MD��״̬Ϊ����״̬ E_MOTOR_RESET
	}
	#endif

	//update MA infor to UA	
	setPWMStatMA2UA(pS_MA2UA,TRUE);									//	����PWMʹ�ܱ�־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : void MotorFaultMain(STR_MACtrProc *pObj)
 * ˵��     : ������Ͽ��ƺ���
 *          : ����ʵ��StateFault(pObj)
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
void MotorFaultMain(STR_MACtrProc *pObj)
{
	STR_PubDatMAMD	*pS_MA2MD = GetPointerMA2MD(pObj);
	STR_PubDatMAUA	*pS_MA2UA = GetPointerMA2UA(pObj);
	
	StateFault(pObj);												//	FAULT ������

	// update MA infor to MD
	SetFaultMA2MD(pObj);											//	����FAULT ���������MD��

	//update MA infor to UA
	setFaultStatMA2UA(pS_MA2UA, TRUE);								//	����FAULT��־Ϊ1��ͨ���������ݽ�����֪ͨUA��
	setPWMStatMA2UA(pS_MA2UA,(pObj->mMdToMa.ePwmStatus == E_PWM_ON)); //	����MD��PWM״̬����MOE��־��ͨ���������ݽ�����֪ͨUA��
}

/*****************************************************************************
 * ������   : void MotorMainOffLineId(STR_MACtrProc *pObj)
 * ˵��     : ����ʶ����
 *          : ����ʵ��
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
void MotorMainOffLineId(STR_MACtrProc *pObj)
{
	//	���߲���ʶ����
}

/*****************************************************************************
 * ������   : void MotorMainOpen(STR_MACtrProc *pObj)
 * ˵��     : ���ǿ�ϳ���
 * ���˼· ������ʵ�ּ�StateOpen()����
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
void MotorMainOpen(STR_MACtrProc *pObj)
{
	//	�������д���������ǰͳһΪ�ջ���������
}



/*****************************************************************************
 * ������   : void MotorMainOnLineId(STR_MACtrProc *pObj)
 * ˵��     : ����ʶ����
 *          : ����ʵ��
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.08.17
 *****************************************************************************/
void MotorMainOnLineId(STR_MACtrProc *pObj)
{
	//	���߲���ʶ����
}


/*****************************************************************************
 * ������   : void StateInit()
 * ˵��     : ״̬��ʼ��
 * ���˼· ��1.������ʼ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/03/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
static void StateInit(STR_MACtrProc *pObj)
{
	s32 t_wTmp;	
	s16 t_nTmp;
	u16 t_nuTmp;
	STR_TransCoefElement *pAppToCore	= &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);

	PSTR_DrvCfgPara	pDrvCfgPara			= pObj->mg_pCtrlObj->m_pDrvCfgPara;


	pObj->m_SMACtrlBit.m_bAlignFinish	= 0;			//	���������ɱ�־ 
	
	//	����ָ����ر�����0
	pObj->m_nQCurrentSet				= 0;
	pObj->m_nQCur_Reference				= 0;
	pObj->m_nDCurrentSet				= 0;
	pObj->m_nDCur_Reference				= 0;
	pObj->m_wDCurFilter					= 0;
	pObj->m_wQCurFilter					= 0;

	pObj->m_pAppCommData->wSvcMinFreq	= pDrvCfgPara->mS_FBSlvcCfg0.m_wCloseFreq;  //�����϶�Ƶ�� 
    pObj->m_pAppCommData->bOpenRunFlag	= 0;
    pObj->m_pAppCommData->nLoopCntr		= 0;			//	��·��������0

    //	˳��������
    //˳���бջ��ٶȳ�ʼ��
    pObj->m_pAppCommData->wDirTrackFreq = pDrvCfgPara->mS_FBDirCheck.m_wSpdTrackOnFreq; 			

	//ɲ���ٶȳ�ʼ��
    pObj->m_pAppCommData->wDirEbreakFreq = pDrvCfgPara->mS_FBDirCheck.m_wBrakeFreqAntiDirec;       

					//	���ֹͣ��������ֵ��ʼ��
    pObj->m_pOnTheFlyDetect->nMotorStopCurrentThd	= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBDirCheck.m_wMotorStopCurrentThd);//MOTOR_STOP_CUR_THD_M0;    /*���ֹͣ��������ֵ */
					//	���ֹͣ���������仯ֵ��ʼ��
    pObj->m_pOnTheFlyDetect->nMotorStopCurDifThd	= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBDirCheck.m_wMotorStopCurDifThd);//MOTOR_STOP_CUR_DIF_THD_M0; /*���ֹͣ��������ֵ */
    pObj->m_pOnTheFlyDetect->wFreqAvg				= 0;		//	˳�����Ƶ��ֵ��0
    pObj->m_pOnTheFlyDetect->nMotorVolMagAvg		= 0;		//	˳������ѹֵ��0

    pObj->m_SMACtrlBit.m_bMotorDirtion				= CW;   /* ���ת�� */

    //Ԥ���
    pObj->m_pAppCommData->bChargeFlag				= 0;    //Ԥ��翪ʼ��־����
    pObj->m_pAppCommData->bChargeEndFlag			= 0; 	//Ԥ�����ɱ�־����

	//	DQ�����˲�ϵ������
	pObj->m_pAppCommData->nDCurFilFactor = (s16)(10240/(10 + (s16)pDrvCfgPara->mS_FBCurLoop.m_wDCurFilTimeSet));
	pObj->m_pAppCommData->nQCurFilFactor = (s16)(10240/(10 + (s16)pDrvCfgPara->mS_FBCurLoop.m_wQCurFilTimeSet));

    /*****��λ������ʼ��********/
	t_wTmp = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBAllign.m_wAlignCurAcc);		//	��λ�������ٶ� A/S
	pObj->m_pSTR_CurRampGen->wRampIncrease = ((s64)t_wTmp * 16777) >> 8;
	
	t_wTmp = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBAllign.m_wAlignCurDec);		//	��λ�������ٶ� A/S
	pObj->m_pSTR_CurRampGen->wRampDecrease = ((s64)t_wTmp * 16777) >> 8;

    /*****���в�����ʼ��********/
	pObj->m_pMotorSpeed->wSpeedRef			= 0;
	pObj->m_pMotorSpeed->wSpeedSet			= 0;
	pObj->m_pMotorSpeed->wSpeedRampACCStep	= App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wSpdRunAcc)*65536/1000;	//�任Ϊ1Hz/ms
	pObj->m_pMotorSpeed->wSpeedRampDECStep	= App2CoreFreqTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wSpdRunDec)*65536/1000;	//�任Ϊ1Hz/ms

	ramp32GenInit(&pObj->m_pMotorSpeed->STR_SpdRampGen);		//	�ٶ�ָ��滮���ݽṹ��ʼ��

	ramp32GenInit(pObj->m_pSTR_CurRampGen);						//	����ָ��滮���ݽṹ��ʼ��

	pObj->m_pMotorSpeed->nSpeedFiltTime	= (s16)(10240/(10 + pDrvCfgPara->mS_FBSlvcCfg0.m_wSpeedFilTime ));
																//	�ٶȷ����˲�ϵ������	Q10

	pObj->m_wPower001WFil				= 0;					//	�����˲�ֵ 0.01W
	pObj->m_nPower001WFactor			= 1024/(1+10);			//	�����˲�ϵ������
	/*****�ٶȻ�������ʼ��********/
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)
    {
		s32 t_wTmpKp;
		s32 t_wTmpKi;
        pObj->m_pMotorSpeed->mSpeedPI.KP = pDrvCfgPara->mS_FBSpdLoop.PASRKp;   //�ٶȻ� Kp	P_ASR_KP_M0
        pObj->m_pMotorSpeed->mSpeedPI.KI = pDrvCfgPara->mS_FBSpdLoop.PASRKi;   //�ٶȻ� Ki P_ASR_KI_M0
        pObj->m_pMotorSpeed->mSpeedPI.wIntegral = 0;
        pObj->m_pMotorSpeed->mSpeedPI.wUpperLimitOutput = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wIqMax);	//�ٶȻ�������ֵ IQMAX_M0
        pObj->m_pMotorSpeed->mSpeedPI.wLowerLimitOutput = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBSpdLoop.m_wIqMin);	//�ٶȻ������Сֵ IQMAX_M0
        pObj->m_pMotorSpeed->mSpeedPI.wInError	= 0;

		#if 0
		t_nTmp = pDrvCfgPara->mS_FBSlvcCfg0.m_wSpeedFilTime + 25;		
		//�Զ������ٶȻ�PI����
		SpeedPIAutoTunning(pObj->mg_nMACtrlProcID,
						5000,
						t_nTmp,
						&t_wTmpKp,
						&t_wTmpKi);

		pObj->m_pMotorSpeed->mSpeedPI.KP = sat(t_wTmpKp,0,32767);		//	PI�����޷�
		pObj->m_pMotorSpeed->mSpeedPI.KI = sat(t_wTmpKi,0,32767);
		#endif
		
		pObj->m_pMotorSpeed->wSpeedEst				= 0;				//	�ٶȱ�����ʼ��
        pObj->m_pMotorSpeed->wSpeedfbk				= 0;
		pObj->m_pMotorSpeed->wSpeedfbkFilter		= 0;
        pObj->m_pMotorSpeed->wPowerLimitSpeedChangeSet	= 0;			//	�ٶȻ��޹��ʱ�����0
		pObj->m_pMotorSpeed->wPowerLimitSpeedChangeSetInc	= 0;

		pObj->m_pMotorSpeed->nPowerLimitCntr		= 0;
		pObj->m_pMotorSpeed->nPowerLimitPeriod		= pDrvCfgPara->mS_FBSpdLoop.nPowerLmtCalcCycle;
																		//	�ٶȻ��޹���ִ�����ڳ�ʼ��

		pObj->m_pAppCommData->nLoopPeriod			= pDrvCfgPara->mS_FBSpdLoop.nSpeedLoopCycle;
																		//	�ٶȻ�ִ�����ڳ�ʼ��
    }   
    /*****���ʻ�������ʼ��********/
	#elif (DRV0_CLOSE_LOOP == POWER_LOOP)
    {
        PowerLoopInit(pObj);									//	���ʻ�������ʼ��
		pObj->m_pAppCommData->nPowerFistFlag		= 0;		//	�״ν��빦�ʻ���־	��0
		pObj->m_pAppCommData->nLoopPeriod			= pDrvCfgPara->mS_FBPwrLoop.nPLoopCalcCycle;	//	���ʻ���������
    }
	#endif

	t_nTmp	= (s16)App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wIdStartSet);		//	D������������ʼ��	
	t_wTmp	= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wIqStartSet);			//	Q������������ʼ��
	t_nuTmp	= pDrvCfgPara->mS_FBSlvcCfg0.TransTime2Close;										//	���ջ��л�ʱ��

	#if (DRV0_WEAKEN_FLUX_STATUS == TRUE)
	InitDQCurRefGenWithWeakenFlux(pObj->mg_nMACtrlProcID,t_nTmp,(s16)t_wTmp, (s16)t_nuTmp);	// DQ�����滮(�����Ź���)��ʼ������
	#else
	InitDQCurRefGen(pObj->mg_nMACtrlProcID,t_nTmp,(s16)t_wTmp, (s16)t_nuTmp);				// DQ�����滮��ʼ������
	#endif

    /*****���໷·����ʱ�������ʼ��********/
    pObj->m_pTime->nChargeTime			= 0;
	pObj->m_pTime->nChargePeriod		= pDrvCfgPara->mS_GlobalCfg.m_nuChargeTime;		 	//	Ԥ���ʱ���ʼ��
	
    pObj->m_pTime->nAlignTime			= 0;
	pObj->m_pTime->nAlignLowPeriod		= pDrvCfgPara->mS_FBAllign.nPlat1Time;			 	//	����ʱ���ʼ��
	pObj->m_pTime->nAlignAllPeriod		= pDrvCfgPara->mS_FBAllign.nPlat1Time + pDrvCfgPara->mS_FBAllign.nPlat2Time;
	
    pObj->m_pTime->nDirCheckTime		= 0;
	pObj->m_pTime->nDirCheckPeriod		= pDrvCfgPara->mS_FBDirCheck.nSpdTrackDelayTime;	//	˳�����ʱ���ʼ��
	
    pObj->m_pTime->nStopDelayTime		= 0;
	pObj->m_pTime->nStopDelayTimePeriod	= pDrvCfgPara->mS_FBDirCheck.nMotorStopDelayTime;	//	ͣ����ʱ�ȴ�ʱ��
	
    pObj->m_pTime->nStopDelayCntr		= 0;
	pObj->m_pTime->nStopDelayPeriod		= pDrvCfgPara->mS_FBDirCheck.nMotorStopTime;		//	ͣ��ʱ���ʼ��
	
	#if(DRV0_UNDVOLT_PROT_ENABLE == 1)
	
		//	��ѹ����	�ٶ�ֵ��дָ���ʼ��
	pObj->m_pMotorException->pwSpeedfbk	= &(pObj->m_pMotorSpeed->wSpeedfbk);
	pObj->m_pMotorException->pwSpeedRef = &(pObj->m_pMotorSpeed->wSpeedRef);

		//	��ѹ����	��ѹ�趨ֵ��ʼ��
	pObj->m_pMotorException->brakeCurrent			= App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wIqBrake);
	pObj->m_pMotorException->chargeStartVoltage		= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wBusVoltStart);
	pObj->m_pMotorException->ChargeRestoreVoltage	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wBusVoltRestore);
	pObj->m_pMotorException->chargeFinishVoltage	= App2CoreVoltTrans(pAppToCore,pDrvCfgPara->mS_FBSlvcCfg0.m_wBusVoltEnd);
	#endif

	MinMotorVoltSpd2Core(pObj->mg_nMACtrlProcID);
}

/*****************************************************************************
 * ������   : void StateCharge(void)
 * ˵��     : Ԥ��纯�������Ծٵ��ݽ���Ԥ��磬���ڲ�ͬ��Ӳ��Ҫע�����Ԥ���ʱ��
 * ���˼· �����δ�A�ࡢB�ࡢC�����ţ����Ծٵ��ݳ�硣
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
  -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *****************************************************************************/
static void StateCharge(STR_MACtrProc *pObj)
{	
	if(pObj->m_pTime->nChargePeriod == 0)
	{
		pObj->m_pAppCommData->bChargeEndFlag = 2;	//Ԥ������
	}
	
	if(pObj->m_pAppCommData->bChargeFlag == 0)
	{
		pObj->m_pAppCommData->bChargeFlag = 1;
		pObj->m_pAppCommData->nChargePwmWidth		= (s16)(4096 * 0.3);		 //Ԥ���ռ�ձ�
		pObj->m_pTime->nChargeTime  = 0;
		SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_OFF);
	}
	else
	{
		if(  pObj->m_pTime->nChargeTime == 0)  //��A������PWM
		{			
			setChargeAFCmdMA2MD(&pObj->mMaToMd);
			SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_ON);
		}
		else if(  pObj->m_pTime->nChargeTime == pObj->m_pTime->nChargePeriod)  //��B������PWM //CHARGE_TIME_M0
		{
			setChargeBFCmdMA2MD(&pObj->mMaToMd);
			SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_ON);
		}
		else if( pObj->m_pTime->nChargeTime == (pObj->m_pTime->nChargePeriod * 2) ) //��C������PWM
		{
			setChargeCFCmdMA2MD(&pObj->mMaToMd);
			SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_ON);
		}

		if( pObj->m_pTime->nChargeTime < (pObj->m_pTime->nChargePeriod * 3))	// CHARGE_TIMEΪÿ��Ԥ���ʱ�䣬�ɸ���Ӳ��ʵ������
		{
			pObj->m_pTime->nChargeTime ++ ;
		}
		else
		{
			if(pObj->m_pAppCommData->bChargeEndFlag == 0)
			{
				setDefaultCmdMA2MD(&pObj->mMaToMd);
				SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_OFF);
				pObj->m_pAppCommData->nChargePwmWidth		=  0;

				if(pObj->mMdToMa.nuFineStatus == E_FC_NON)
				{
					setChargeDoneInitFCmdMA2MD(&pObj->mMaToMd);
					pObj->m_pAppCommData->bChargeEndFlag = 1;
				}
			}
			else if(pObj->m_pAppCommData->bChargeEndFlag == 1)
			{
				if(pObj->mMdToMa.nuFineStatus == E_FC_NON)
				{
					pObj->m_pAppCommData->bChargeEndFlag = 2;
				}

			}
		}
	}

	if(pObj->m_pAppCommData->bChargeEndFlag == 2)//Ԥ������
	{
		if(pObj->mMdToMa.nuFineStatus == E_FC_NON)
		{
			StateInit(pObj);
			setInitAcrParaReq(pObj);			
			pObj->m_pAppCommData->bChargeEndFlag	= 3;			
		}
	}

	if(pObj->m_pAppCommData->bChargeEndFlag == 3)
	{
		if(motorAcrDataInitDone(pObj))
		{
			pObj->m_pAppCommData->bChargeFlag		= 0;
			pObj->m_pTime->nChargeTime				= 0;

			pObj->m_pAppCommData->nChargePwmWidth	= 0;
			SetPWMCmdMA2MD(&pObj->mMaToMd,E_PWM_ON);
			pObj->m_pAppCommData->bChargeEndFlag	= 4;
		}
	}
}

/*****************************************************************************
 * ������   : void StateDirCheck(void)
 * ˵��     : ˳�������򣬳��õļ�ⷽʽΪ�ջ������ͨ�������Ƽ���ٶ�ֵ
 * ���˼· ��˳�����������ջ�����;����״̬�����stop״̬���������ȫֹͣ��ֹ����;\
              ����������һֱ��⣨Ҳ���Ը���ʵ��Ӧ�ý���ɲ������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/03/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
static void StateDirCheck(STR_MACtrProc *pObj)
{
	STR_TransCoefElement *pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
	if(++ pObj->m_pTime->nDirCheckTime >= pObj->m_pTime->nDirCheckPeriod )
	{
		pObj->m_pTime->nDirCheckTime		= 0;

			//	��ȡMD���˳������� Ƶ��ֵ
		pObj->m_pOnTheFlyDetect->wFreqAvg	= Core2AppFreqTrans(&pObj->mg_pCtrlObj->mTransCoef.mCoreToApp,pObj->mMdToMa.wFreq);
			
		switch(pObj->m_SMACtrlBit.m_bMotorDirtion)
		{
		case CW://˳ʱ��
		{
			if(pObj->m_pOnTheFlyDetect->wFreqAvg < -pObj->m_pAppCommData->wDirEbreakFreq)	//��ʱ�볬���٣�һֱ��⣬��������Ч��; ���ڲ�ͬ��Ӧ�ã�Ҳ���Խ���ɲ��״̬��
			{
				pObj->m_eTrackingResult			= TRACKING;
			}
			else if(pObj->m_pOnTheFlyDetect->wFreqAvg < pObj->m_pAppCommData->wDirTrackFreq)//��ʱ����٣�ɲ��
			{					
				pObj->m_pTime->nStopDelayTime	= pObj->m_pTime->nStopDelayTimePeriod;
				pObj->m_eTrackingResult			= TRACKING_2_BRAKE;							//ɲ������
			}
			else//˳ʱ�� ���� ����ջ�����
			{				
				pObj->m_nQCur_Reference = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBCurLoop.m_wIqSet); // IQ_SET_M0
				if(pObj->m_SMACtrlBit.m_bMotorDirtion == CCW)
				{
					pObj->m_nQCur_Reference = -pObj->m_nQCur_Reference;
				}				
				handleSpdSetpoint(pObj);
				pObj->m_eTrackingResult				= TRACKING_2_RUN;
			}
			break;
		}
		case CCW://��ʱ��
		{
			if(pObj->m_pOnTheFlyDetect->wFreqAvg < -pObj->m_pAppCommData->wDirTrackFreq)//��ʱ����٣��ջ�����
			{
				pObj->m_nQCur_Reference = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBCurLoop.m_wIqSet); // IQ_SET_M0
				if(pObj->m_SMACtrlBit.m_bMotorDirtion == CCW)
				{
					pObj->m_nQCur_Reference = -pObj->m_nQCur_Reference;
				}		
				handleSpdSetpoint(pObj);
				pObj->m_eTrackingResult				= TRACKING_2_RUN;
			}
			else if(pObj->m_pOnTheFlyDetect->wFreqAvg < pObj->m_pAppCommData->wDirEbreakFreq)//����״̬��ɲ������
			{
				pObj->m_pTime->nStopDelayTime		= pObj->m_pTime->nStopDelayTimePeriod;
				pObj->m_eTrackingResult				= TRACKING_2_BRAKE;//ɲ��״̬
			}
			else  //˳ʱ�� �����٣�һֱ��⣬��������Ч��;���ڲ�ͬ��Ӧ�ã�Ҳ���Խ���ɲ��״̬��
			{
				pObj->m_eTrackingResult				= TRACKING;
			}
			break;
		}
		default:
			pObj->m_eTrackingResult				= TRACKING;
			break;
		}
	}
	else
	{
		pObj->m_eTrackingResult			= TRACKING;
	}
}

/*****************************************************************************
 * ������   : void StateAlign(void)
 * ˵��     : ���Ԥ��λ��ʹ�ܳ�ʼλ�ü�⹦��ʱ���˺���ֻ���ṩ����ǿ�ϵ�����\
              ���ζ�λʱ�������1ms��
 * ���˼· �����ζ�λ;���������Ƕȡ���λ��������λʱ�䣬ʹ����̶��ڸ����Ƕ��ϡ�\
              ��λʱ����趨���Ե����ȫ��ֹ��ʱ��Ϊ׼��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
static void StateAlign(STR_MACtrProc *pObj)
{
	STR_TransCoefElement *pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;

	u32 t_TmpTheta;

	
	pObj->m_pMotorSpeed->wSpeedRef	= 0;
	pObj->m_pMotorSpeed->STR_SpdRampGen.wRampOut = 0;
	if( pObj->m_pTime->nAlignTime < pObj->m_pTime->nAlignLowPeriod )
	{	//��һ�ζ�λ����
		pObj->m_pTime->nAlignTime ++;		
		pObj->m_pSTR_CurRampGen->wRampIn = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBAllign.m_wPlat1Cur);
	}
	else if( pObj->m_pTime->nAlignTime < pObj->m_pTime->nAlignAllPeriod)
	{	//�ڶ��ζ�λ����
		pObj->m_pTime->nAlignTime ++;
		pObj->m_pSTR_CurRampGen->wRampIn = App2CoreCurTrans(pAppToCore,pDrvCfgPara->mS_FBAllign.m_wPlat2Cur);
	}
	else
	{	//���뿪������״̬
		pObj->m_SMACtrlBit.m_bAlignFinish		= TRUE;					//	�������
		pObj->m_pTime->nAlignTime				= 0;
	}
	
	ramp32GenCalc(pObj->m_pSTR_CurRampGen);								//	����ָ��б�¹滮����
	
	pObj->m_nDCur_Reference = pObj->m_pSTR_CurRampGen->wRampOut;		//	��ֵ��D���������ֵ�Ͳο�ֵ
	pObj->m_nDCurrentSet = pObj->m_nDCur_Reference;
	pObj->m_wDCurFilter			= ((s32)pObj->m_nDCur_Reference) << 10;	//	ȥ������ָ���˲�������Ӱ��
	SetDCurRefInitValue(pObj->mg_nMACtrlProcID, pObj->m_nDCurrentSet);	//	����D�����ָ���ʼֵ

	if(!IsNeedIPD(pObj))				//�Ƿ����ó�ʼλ�ü�⹦��
	{  
		//	û��IPDʱ�������趨�Ķ���Ƕ���Ϊ��Ƕ�
		//	��IPDʱ������IPD�ĽǶ���Ϊ��ǰ��Ƕ�
		t_TmpTheta			= (u32)(pDrvCfgPara->mS_FBAllign.m_wuAlignAngle & 0xFFFF);
		setOBElectAngleEst(pObj->mg_nMACtrlProcID,t_TmpTheta << 16);	//	���õ�Ƕȵ��۲�������Ƕ���
	}
}
/*****************************************************************************
 * ������   : void StateRun(void)
 * ˵��     : ���run״̬�������һ��Ϊ�ٶȻ������ʻ��Ĵ���
 * ���˼· ��1.����ʵ�ʸ��ؼ����ٶ�/����/���������¡��ٶȻ�/���ʻ���PI       \
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 2023/03/22      V1.1           Tonghua Li          �޸�
 *****************************************************************************/
static void StateRun(STR_MACtrProc *pObj)
{
	STR_TransCoefElement *pAppToCore = &(pObj->mg_pCtrlObj->mTransCoef.mAppToCore);
	PSTR_MechQuantity		t_pMotorSpeed	= pObj->m_pMotorSpeed;
//	s32 t_wTemp;	
	
	/*******************�ٶȻ�����*******************************/
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)
	{
		if(++pObj->m_pAppCommData->nLoopCntr >= pObj->m_pAppCommData->nLoopPeriod)    //�ٶȻ���·�������ڡ�
		{
			pObj->m_pAppCommData->nLoopCntr = 0;

			t_pMotorSpeed->wSpeedSet = App2CoreFreqTrans(pAppToCore,pObj->mUaToMa.wFreqCmd); //�ٶȲο�������ת����core���ݸ�ʽ

			#if (DRV0_POWER_LIMIT_STATUS == TRUE)          //�޹���ʹ�ܣ��ٶȻ���������������
			{
				pObj->m_pPower->wPowerValue				= pObj->m_wPower001W;
				t_pMotorSpeed->wPowerLimitValue			= pObj->mUaToMa.wPowerCmd;
				PowerLimitCalc(t_pMotorSpeed,pObj->m_pPower->wPowerValue);		//	�������ƣ� ���Ƶ�ʸı�ֵwPowerLimitSpeedChangeSet
																				//	����Ƶ���趨ֵ
				t_pMotorSpeed->wSpeedSet				= t_pMotorSpeed->wSpeedSet - t_pMotorSpeed->wPowerLimitSpeedChangeSet;
			}			
			#endif

			HandleSpdSetpointCmd(pObj);							//	Ƶ���趨ֵ���������趨����ȴ�����յ���Ƶ���趨ֵ		
			HandleSpdSetpointRamp(pObj);						//	Ƶ���趨ֵб�¹滮��
			t_pMotorSpeed->wSpeedRef	= t_pMotorSpeed->STR_SpdRampGen.wRampOut;
			
			// ����ʱ�ٶȻ����������ʼ������Iq_START��������
			QCurRefGenValue(pObj->mg_nMACtrlProcID,&t_pMotorSpeed->mSpeedPI.wIntegral,22,t_pMotorSpeed->wSpeedSet);

			if(pObj->m_pAppCommData->bOpenRunFlag == 1)       //˳����⵽����run״̬
			{
				t_pMotorSpeed->mSpeedPI.wIntegral = 0;
				pObj->m_pAppCommData->bOpenRunFlag = 2;			//	�л�����������״̬
			}
			
			if(getRunStatus(pObj->mg_nMACtrlProcID))			//	���ջ�״̬?
			{
				pObj->m_pAppCommData->bOpenRunFlag = 3;			//	�ٶȱջ�ģʽ
			}
			pObj->m_nQCurrentSet = SpeedLoopReg(t_pMotorSpeed);// �ٶȻ�PI����

		}
	}

	/*******************����������*******************************/		
	#elif (DRV0_CLOSE_LOOP == CURRENT_LOOP)
	{
		PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;		
		s32 t_wTemp1,t_wTemp;

		t_wTemp	= ABS(pObj->mUaToMa.wTorqCmd);					//	��ȡUA���ת��ָ��
		
		if(pObj->m_SMACtrlBit.m_bMotorDirtion == CW)			//	CW�Ƕ�����ָ��Ϊ��
		{
			pObj->m_nQCurrentSet = App2CoreCurTrans(pAppToCore,t_wTemp);
		}
		else if(pObj->m_SMACtrlBit.m_bMotorDirtion == CCW)		//	CW�Ƕ�����ָ��Ϊ��
		{
			pObj->m_nQCurrentSet = -App2CoreCurTrans(pAppToCore,t_wTemp);
		}

		//	UA���趨��Ƶ��ָ��С���������Ƶ��ʱ������Ϊ�������Ƶ��
		t_wTemp 	= pDrvCfgPara->mS_FBSlvcCfg0.m_wMinRunFreq;
		t_wTemp1 	= pObj->mUaToMa.wFreqCmd;
		if(ABS(t_wTemp1) <= t_wTemp)
		{
			t_wTemp1	= t_wTemp;
		}		
		t_pMotorSpeed->wSpeedSet = App2CoreFreqTrans(pAppToCore,t_wTemp1); //	��ȡ�ٶȲο�����
		
		HandleSpdSetpointCmd(pObj);								//	�������� Ƶ��ָ���		
		if(ABS(t_pMotorSpeed->wSpeedfbk) >= ABS(t_pMotorSpeed->wSpeedSet))
		{
			t_pMotorSpeed->wSpeedSet	= t_pMotorSpeed->wSpeedfbk;
		}	

		HandleSpdSetpointRamp(pObj);							//	Ƶ�ʹ滮
		t_pMotorSpeed->wSpeedRef	= t_pMotorSpeed->STR_SpdRampGen.wRampOut;
	}

	/*******************���ʻ�����*******************************/
	#elif (DRV0_CLOSE_LOOP == POWER_LOOP)
	{
		PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
		s32 t_wTemp1, t_wTemp;
		
		if(++pObj->m_pAppCommData->nLoopCntr >= pObj->m_pAppCommData->nLoopPeriod) //���ʻ���·�������ڡ�
		{
			pObj->m_pAppCommData->nLoopCntr = 0;

			//��ȡ����ָ��ָ
			pObj->m_pPower->wPowerSet			= pObj->mUaToMa.wPowerCmd;				// 0.01W
			pObj->m_pPower->wPowerValue			= pObj->m_wPower001W;
			#if(DRV0_SPPED_LIMIT_STATUS == TRUE)     //���ٹ���ʹ�� �����ʻ��������ת�١�
			//if(pObj->m_pAppCommData->bLoopLmtEnable)
			{
				//��ȡ�ٶ�����ֵ
				pObj->m_pPower->wSpeedLimitValue	= App2CoreFreqTrans(pAppToCore,pObj->mUaToMa.wFreqCmd);
				SpeedLimitCalc(t_pMotorSpeed->wSpeedfbk, pObj->m_pPower);	//���ٶ�,���ƹ����������

				// �������ٶȺ�����������ø�������
				pObj->m_pPower->wPowerSet = pObj->m_pPower->wPowerSet - pObj->m_pPower->wSpeedLimitPowerChangeSet;
			}
			#endif

			t_wTemp	= ABS(pObj->mUaToMa.wTorqCmd);

			if(pObj->m_SMACtrlBit.m_bMotorDirtion == CW)		// CW����ʱ Ϊ����
			{
				pObj->m_pPower->wPowerSet = pObj->m_pPower->wPowerSet;
				pObj->m_pPower->wPowerValue = pObj->m_pPower->wPowerValue;
				pObj->m_nQCurrentSet = App2CoreCurTrans(pAppToCore,t_wTemp);
			}
			else												// CW����ʱ Ϊ������ֵȡ��
			{
				pObj->m_pPower->wPowerSet = -pObj->m_pPower->wPowerSet;
				pObj->m_pPower->wPowerValue = -pObj->m_pPower->wPowerValue;
				pObj->m_nQCurrentSet = -App2CoreCurTrans(pAppToCore,t_wTemp);
			}
			
			if(pObj->m_pAppCommData->nPowerFistFlag == 0) //�״ν����ٶȻ������ʻ��ĳ�ʼֵ����ΪIq
			{
				pObj->m_nQCur_Reference	= pObj->m_nQCurrentSet;	
				pObj->m_pAppCommData->nPowerFistFlag = 1;
				pObj->m_pPower->struPowerRamp.wRef = pObj->m_pPower->wPowerValue;
				pObj->m_pPower->struPowerPI.wIntegral = ((s32)pObj->m_nQCur_Reference << 16);
			}

			pObj->m_pPower->struPowerRamp.wSet = pObj->m_pPower->wPowerSet;
			pObj->m_pPower->wPowerRef = RampControl(&pObj->m_pPower->struPowerRamp);	//���ʻ����º���

			PowerLoopReg(pObj->m_pPower);				  //���ʻ�PI����
			pObj->m_nQCurrentSet					=pObj->m_pPower->nQCurrentSet;		//	�趨���ʻ��������ָ��ֵ
		}

		//	UA���趨��Ƶ��ָ��С���������Ƶ��ʱ������Ϊ�������Ƶ��
		t_wTemp 	= pDrvCfgPara->mS_FBSlvcCfg0.m_wMinRunFreq;
		t_wTemp1 	= pObj->mUaToMa.wFreqCmd;
		
		if(ABS(t_wTemp1) <= ABS(t_wTemp))
		{
			t_wTemp1	= t_wTemp;
		}

		t_pMotorSpeed->wSpeedSet = App2CoreFreqTrans(pAppToCore, t_wTemp1); //�ٶȲο�����

		if(pObj->m_SMACtrlBit.m_bMotorDirtion == CW)							//	���ʻ����ٶ�ָ���
		{
			if(t_pMotorSpeed->wSpeedSet < 0 )
			{
				t_pMotorSpeed->wSpeedSet = -t_pMotorSpeed->wSpeedSet;
			}					
		}
		else if(pObj->m_SMACtrlBit.m_bMotorDirtion == CCW)
		{
			if(t_pMotorSpeed->wSpeedSet > 0)
			{
				t_pMotorSpeed->wSpeedSet = - t_pMotorSpeed->wSpeedSet;
			}
		}
		
		HandleSpdSetpointRamp(pObj);											//	�ٶȹ滮�������趨ֵ
		t_pMotorSpeed->wSpeedRef	= t_pMotorSpeed->STR_SpdRampGen.wRampOut;
	}
	#endif

	#if (DRV0_WEAKEN_FLUX_STATUS == TRUE)
	pObj->m_nDCurrentSet = getDCurRefGenWithEnWeakenFlux(pObj->mg_nMACtrlProcID);		//	������ʱ d�����ָ���
	#else
	pObj->m_nDCurrentSet = getDCurRefGen(pObj->mg_nMACtrlProcID);						//	��������ʱ d�����ָ���
	#endif

	CurrentLoopRef_filter(pObj);    													//	dq�����������ֵ�˲�
}

/*****************************************************************************
 * ������   : void StateFault(void)
 * ˵��     : ����״̬����Ҫ�ǽ��й���״̬����������
 * ���˼· ��1.�ڹ���״̬�������⵽������ʧ��������stru_Faults��Ӧ����λ��\
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *****************************************************************************/
static void StateFault(STR_MACtrProc *pObj)
{
    pObj->m_SMACtrlBit.m_bMC_RunFlg	= 0;		//	���RUN���б�־
 //   FaultRecover(pObj);    //���ϻָ�������
}

/*****************************************************************************
 ������   : static void StateBrake(STR_MACtrProc *pObj)
 ˵��     : ���ֹͣ�������жϵ���Ƿ�ֹ״̬
 ���˼· ��1.ͨ��mOnTheFlyDetect.bMotorStopDecFlag��״̬���жϵ���Ƿ��ھ�ֹ״̬�����ж����
                ��ֹ��ȥ���г�ʼλ�ü�����Ԥ��λ����
 ����     ����
 ����ֵ   ����
 �޸�����	   �汾��		   �޸���			 �޸�����
 -----------------------------------------------------------------------------
  2022/08/17	  V1.0			 Tonghua Li 		 ����
 *****************************************************************************/
static void StateBrake(STR_MACtrProc *pObj)
{
	u32 t_CurrentSqrt;
	STR_Time_t *pstruTime = pObj->m_pTime;

	t_CurrentSqrt			= pObj->mMdToMa.nMotorCur;

	BrakeStateCheck(t_CurrentSqrt,pObj->m_pOnTheFlyDetect);				//	�ƶ���⺯��
	
    if(pObj->m_pOnTheFlyDetect->bMotorStopDecFlag)						//���ֹͣ��־λΪ1���ж����Ϊ��ֹ״̬
    {
        if( pstruTime->nStopDelayCntr < pstruTime->nStopDelayPeriod)	//���ֹͣʱ���˲�������STOP_TIME�ε�����ھ�ֹ״̬���ж����Ϊ��ֹ״̬
        {
            pstruTime->nStopDelayCntr ++;
            pObj->m_pOnTheFlyDetect->bMotorStopDecFlag = 0;
        }
        else
        {
            if( pstruTime->nStopDelayTime > 0)     						//�ж������ֹ�����ʱ���������ж�������ʵ�ʵ�����ص����ӳ�ʱ��
            {
                pstruTime->nStopDelayTime --;
            }
            else														//	�ж����ֹͣ
            {
                pObj->m_pMotorSpeed->wSpeedRef				= 0;		//	�ٶ�ֵ��0
                pObj->m_pMotorSpeed->wSpeedSet				= 0;
                pObj->m_pOnTheFlyDetect->bMotorStopDecFlag	= 0;
                pstruTime->nStopDelayCntr					= 0;
				pObj->m_SMACtrlBit.m_bMotorStopped			= TRUE;		//	�����ֹͣ
            }
        }
    }
    else
    {
        pstruTime->nStopDelayCntr							= 0;
		pObj->m_SMACtrlBit.m_bMotorStopped					= FALSE;	//	���δֹͣ
    }

}

/*******************************************************************************
 �������ƣ�    void updateMAInfo2UALocal(STR_MACtrProc *pObj)
 ����������    update MA��Ϣ��UA��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 2022/12/17      V1.0           Tonghua Li          �޸�
 *******************************************************************************/
void updateMAInfo2UALocal(STR_MACtrProc *pObj)
{
	STR_PubDatMAUA	*pMa2Ua = &pObj->mMaToUa;
	STR_PubDatMDMA	*pMd2Ma = &pObj->mMdToMa;

	STR_TransCoefElement 	*pCoreToApp	= &(pObj->mg_pCtrlObj->mTransCoef.mCoreToApp);

	pMa2Ua->wVdcDec			= Core2AppVolTrans(pCoreToApp,pMd2Ma->nVdc);		//	����ĸ�ߵ�ѹֵ����λ0.01V
	pMa2Ua->wVolDec			= Core2AppVolTrans(pCoreToApp,pMd2Ma->nMotorVol);	//	���µ����ѹֵ����λ0.01V
	
	//pMa2Ua->nInvTemp		= pObj->m_nInvTemp;
	
	pMa2Ua->wCurDec			= Core2AppCurTrans(pCoreToApp,pMd2Ma->nMotorCur);	//	���µ���������Чֵ����λ0.001A
	
	pMa2Ua->wSpeedDec		= Core2AppFreqTrans(pCoreToApp,pMd2Ma->wFreq);		//	���µ��Ƶ��ֵ����λ0.01Hz
	
	pMa2Ua->uFault			= pObj->mg_UFault.R;								//	���±�����Ϣ���������λ����
	pMa2Ua->eMotorStsAck	= pObj->mUaToMa.eAppCmd;							//	���¿����������UA��

	setRunningStatMA2UA(pMa2Ua, (pObj->m_SMACtrlBit.m_bMotorStopped == 0));		//	���µ���Ƿ����е�UA��
	setPostStatMA2UA(pMa2Ua, pObj->m_SMACtrlBit.m_bPostOK);						//	����MA��POST��־��UA��
	setCfgFinishMA2UA(pMa2Ua, TRUE);											//	���²���������ɱ�־��UA�㣬Ԥ��
}

/*******************************************************************************
 �������ƣ�    static void CurrentLoopRef_filter(STR_MACtrProc *pObj)

 ����������    �����ο��˲�������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/7/25      V1.0           Li Tonghua          ����
 *******************************************************************************/
static void CurrentLoopRef_filter(STR_MACtrProc *pObj)
    {
	s32 t_wTmp32;	
		//	D�����ָ�� һ���˲���
	t_wTmp32					= pObj->m_nDCurrentSet - (pObj->m_wDCurFilter >> 10);
	pObj->m_wDCurFilter			+= ((s32)t_wTmp32 * pObj->m_pAppCommData->nDCurFilFactor);
	pObj->m_nDCur_Reference		= (pObj->m_wDCurFilter >> 10);
		//	Q�����ָ�� һ���˲���
	t_wTmp32					= pObj->m_nQCurrentSet - (pObj->m_wQCurFilter >> 10);
	pObj->m_wQCurFilter			+= ((s32)t_wTmp32 * pObj->m_pAppCommData->nQCurFilFactor);
	pObj->m_nQCur_Reference		= (pObj->m_wQCurFilter >> 10);	
}

/*******************************************************************************
 �������ƣ�    static s16 SpeedLoopReg(MechanicalQuantity *this)
 ����������    �ٶȻ�PI����
 ���������    MechanicalQuantity *this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/08/17      V1.0           Tonghua Li          ����
 *******************************************************************************/
#if (DRV0_CLOSE_LOOP==SPEED_LOOP)
static s16 SpeedLoopReg(STR_MechQuantity *this)
{
    this->mSpeedPI.wInError =  this->wSpeedRef - this->wSpeedfbk;    

   return (SpeedPIRegulator(&this->mSpeedPI));		//	�ٶ�PI��������
}
#endif

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */






/*****END OF FILE****/
