/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� USER_usrApp.c
 * �ļ���ʶ��
 * ����ժҪ�� UA �û������ļ�
 * ����˵���� �û��ĳ�������ڸ��ļ����
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2023��03��29��
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PubData_MA_UA.h"
#include "MC_Math.h"
#include "MC_type.h"
#include "Time_Process.h"
#include "UA_Data.h"
#include "PubData_DriverInstance.h" 
#include "string.h"



/*******************************************************************************/
//ȫ�ֱ���������


/*******************************************************************************/
//�ⲿ����������


/*******************************************************************************/
//�ֲ���̬����������

/*******************************************************************************/
//�ֲ���̬����������
extern STR_UACtrProc	sS_UACtrlProc[UA_OBJ_MAX_DEFINE];


#ifdef UA_DEBUG
volatile bool runM0UaCmd = FALSE;
bool runM0UaCmdAuto = FALSE;
volatile s32 runM0SpdSetpoint = 20*100;
s16 runM0Period = 0;

bool runM1UaCmd = FALSE;
bool runM1UaCmdAuto = FALSE;
s32 runM1SpdSetpoint = 100*100;
s16 runM1Period = 0;

/*******************************************************************************
 �������ƣ�    void updateM0DebugUA(pSTR_UACtrProc pObj)
 ����������    UA��������û������ڴ˺����±�д�����߼���
               ͨ��UAָ����޸�����·���MD��ʵ�ֵ������
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
void updateM0DebugUA(pSTR_UACtrProc pObj)
{
/**************��ʱ�Զ���ͣ����***************/		
	if(runM0UaCmdAuto)
	{
		runM0Period++;
		if(runM0Period >= 10000)
		{
			if(runM0UaCmd == FALSE)
			{
				runM0UaCmd = TRUE;
			}
			else
			{
				runM0UaCmd = FALSE;
			}
			runM0Period = 0;
		}
	}
	else
	{
		runM0Period	= 0;
	}
/**************��ʱ�Զ���ͣ����***************/

	
	pObj->m_wSetpoint					= runM0SpdSetpoint;  //ת���趨ֵ
	
	if(runM0UaCmd)                                 //UA���������Ʊ�־��1
	{
		pObj->m_StruCmd.m_bMoeEnable	= 1;
		pObj->m_StruCmd.m_bRunFlag		= 1;
	}
	else                                          //UA���������Ʊ�־��0
	{
		pObj->m_StruCmd.m_bMoeEnable	= 0;
		pObj->m_StruCmd.m_bRunFlag		= 0;
	}

	if(pObj->m_StruCmd.m_bDownLoadStart == 1)     //���������ڼ����б�־��0
	{
/* ��ӹ��ܴ��룬һ���û����붼�͵��״̬��صģ���ͬ״ִ̬�в�ͬ
���ܴ��빦�ܣ����������״̬��������״̬ = ����ʱ�����Եƿ�������״̬���Եƹ� */
		pObj->m_StruCmd.m_bMoeEnable	= 0;
		pObj->m_StruCmd.m_bRunFlag		= 0;
	}
}

/*******************************************************************************
 �������ƣ�    void updateM1DebugUA(pSTR_UACtrProc pObj)
 ����������    UA��������û������ڴ˺����±�д�����߼���
               ͨ��UAָ����޸�����·���MD��ʵ�ֵ������
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
void updateM1DebugUA(pSTR_UACtrProc pObj)
{
/**************��ʱ�Զ���ͣ����***************/
	if(runM1UaCmdAuto)
	{
		runM1Period++;
		if(runM1Period >= 10000)
		{
			if(runM1UaCmd == FALSE)
			{
				runM1UaCmd = TRUE;
			}
			else
			{
				runM1UaCmd = FALSE;
			}
			runM1Period = 0;
		}
	}
	else
	{
		runM1Period	= 0;
	}
/**************��ʱ�Զ���ͣ����***************/
	
	
	pObj->m_wSetpoint					= runM1SpdSetpoint;    //ת���趨ֵ
	
	if(runM1UaCmd)                                   //UA���������Ʊ�־��1
	{
		pObj->m_StruCmd.m_bMoeEnable	= 1;
		pObj->m_StruCmd.m_bRunFlag		= 1;
	}
	else                                            //UA���������Ʊ�־��0
	{
		pObj->m_StruCmd.m_bMoeEnable	= 0;
		pObj->m_StruCmd.m_bRunFlag		= 0;
	}
}

#endif

/*******************************************************************************
 �������ƣ�    void updateUASetpoint(pSTR_UACtrProc pObj)
 ����������    �����UAָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����		�û��ɸ�����Ҫ�����Ӧ�Ĵ��������ߴ���ʽ
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
void updateUASetpoint(pSTR_UACtrProc pObj)
{	
	PSTR_DrvCfgPara	pDrvCfgPara = pObj->mg_pCtrlObj->m_pDrvCfgPara;
	
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)			//�ٶȻ�

	#if (DRV0_POWER_LIMIT_STATUS == TRUE)		//�ٶȻ��޹���
		pObj->m_wSetpoint		= pDrvCfgPara->mS_FBSpdLoop.m_wPowerLmtSpdValue;
		pObj->m_wPowerSetpoint	= pDrvCfgPara->mS_FBSpdLoop.m_wPowerLmtValue;
		#endif
		
	pObj->m_wTqSetpoint = pDrvCfgPara->mS_FBCurLoop.m_wIqSet;

	#elif (DRV0_CLOSE_LOOP == CURRENT_LOOP)		//������
	pObj->m_wTqSetpoint = pDrvCfgPara->mS_FBCurLoop.m_wIqSet;
	
	#elif (DRV0_CLOSE_LOOP == POWER_LOOP)		//���ʻ�
	
		#if(DRV0_SPPED_LIMIT_STATUS == TRUE)   //���ʻ����ٶ�
		pObj->m_wPowerSetpoint	= pDrvCfgPara->mS_FBPwrLoop.m_wLmdSpdPowerSetting;
		pObj->m_wSetpoint		= pDrvCfgPara->mS_FBPwrLoop.m_wSpdLmtValue;
		#else
		pObj->m_wPowerSetpoint	= pDrvCfgPara->mS_FBPwrLoop.m_wPowerSetting;
		#endif
		
	pObj->m_wTqSetpoint = pDrvCfgPara->mS_FBCurLoop.m_wIqSet;
	#endif
	
	pObj->mUaToMa.wFreqCmd		= pObj->m_wSetpoint;            //ת��ָ���趨
	pObj->mUaToMa.wPowerCmd 	= pObj->m_wPowerSetpoint;       //����ָ���趨
	pObj->mUaToMa.wTorqCmd		= pObj->m_wTqSetpoint;          //����ָ���趨
}

/*******************************************************************************
 �������ƣ�    void getGearRatio4UA(pSTR_UACtrProc pObj)
 ����������    UAָ����������ݴ�����ϵ����Ԥ����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
s16 getGearRatio4UA(pSTR_UACtrProc pObj)
{
	return 200;	
}

/*******************************************************************************
 �������ƣ�    ENUM_MotorBrakeMode getBrakeMode4UA(pSTR_UACtrProc pObj)
 ����������    �����UAɲ��ָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
ENUM_MotorBrakeMode getBrakeMode4UA(pSTR_UACtrProc pObj)
{
	#if (DRV0_STOP_MODE == 1)
		return E_STOP_ZERO_CUR;    //������ض�
	#elif (DRV0_STOP_MODE == 2)
		return E_STOP_SHORT;       //���ű۶̽�
	#else
		return E_STOP_MOE_OFF;     //MOE�ض�
	#endif	
}


/*******************************************************************************
 �������ƣ�    ENUM_MotorBrakeMode getFaultBrakeMode4UA(pSTR_UACtrProc pObj)
 ����������    UA����ָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/3/29      V1.1           Tonghua Li          ����
 *******************************************************************************/
ENUM_MotorBrakeMode getFaultBrakeMode4UA(pSTR_UACtrProc pObj)
{
	return getBrakeMode4UA(pObj);
}

/*******************************************************************************/
/*****************************end***********************************************/

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



