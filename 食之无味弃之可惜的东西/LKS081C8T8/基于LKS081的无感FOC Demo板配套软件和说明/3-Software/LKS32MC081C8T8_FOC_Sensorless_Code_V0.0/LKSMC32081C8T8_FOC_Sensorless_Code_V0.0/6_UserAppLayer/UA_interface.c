/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� USER_app.c
 * �ļ���ʶ��
 * ����ժҪ�� UA �ӿڼ�������
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��08��19��
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
extern PSTR_ControlObject getCtrlObjectPtr(u8 objID);
extern void UA_StateMachine(pSTR_UACtrProc pObj);
extern void downloadParaHandle(bool t_bMotorRunning);

#ifdef UA_DEBUG
extern void updateM0DebugUA(pSTR_UACtrProc pObj);
extern void updateM1DebugUA(pSTR_UACtrProc pObj);
#endif


/*******************************************************************************/
//�ֲ���̬����������
static bool init_UA_Crtl_Proc(pSTR_UACtrProc pObj);
static void handleUACtrlObj(u8 nuObjID);
static bool init_UA_Crtl_Proc(pSTR_UACtrProc pObj);
static bool initUACtrlObj(void);

/*******************************************************************************/
//�ֲ���̬����������
STR_UACtrProc	sS_UACtrlProc[UA_OBJ_MAX_DEFINE];

#ifdef _POLLING_MODE
static u8 t_nuObjID = 0;
#endif


/*******************************************************************************/
/*****************************end***********************************************/


/*******************************************************************************
 �������ƣ�    u8 get_Max_UAObj_Nbr(void)
 ����������    ��ѯ���ƶ������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ���ض������
 ����˵����    �ú������ⲿ������ã�
 			   �ú���Ϊ����ֱ�ӵ��ú���

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
u8 get_Max_UAObj_Nbr(void)
{
	return UA_OBJ_MAX_DEFINE;
}

/*******************************************************************************
 �������ƣ�    static bool init_UA_Crtl_Proc(pSTR_UACtrProc pObj)
 ����������    �����ƶ������UA��ʼ������
 ���������    ���ƶ���ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ע�����ݽṹָ�롢���ƶ����
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static bool init_UA_Crtl_Proc(pSTR_UACtrProc pObj)
{
	bool returnResult = TRUE;	

	// ע��ʵ�ʵĿ��ƶ��󣬸���ID����
	pObj->mg_pCtrlObj			= getCtrlObjectPtr(pObj->mg_nUACtrlProcID);	
	if(pObj->mg_pCtrlObj == NULL)
	{
		returnResult = FALSE;
	}

	pObj->m_eUASysState			= E_UA_INIT;       // ��ʼ��UA����״̬��
	pObj->m_bPWMStopMode		= pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_GlobalCfg.m_buStopPWMEnable;	// ��ʼ��PWMֹͣģʽ

	pObj->m_bFaultAutoClr		= pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_GlobalCfg.m_buFaultAutoClr;	// ��ʼ�������Զ����
	pObj->m_nuFaultClrPeriod	= pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_GlobalCfg.m_nuFaultClrPrd;	// ��ʼ�������������
	
	pObj->m_bLoopMode			= pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_GlobalCfg.m_buCtrLoopmode;	// ��ʼ������ģʽ
	pObj->m_bStopMode			= pObj->mg_pCtrlObj->m_pDrvCfgPara->mS_GlobalCfg.m_buStopMode;		// ��ʼ��ֹͣģʽ

	#if 0
	pObj->m_nuFaultClrCnt			= 0;		// �����������
	pObj->m_StruCmd.m_bFault		= 0;		// UA ���ϱ�־
	pObj->m_StruCmd.m_bFaultClr		= 0;		// UA �����������
	pObj->m_StruCmd.m_bRunFlag		= 0			// UA ���������־
	pObj->m_StruCmd.m_bMoeEnable	= 0;		// MOE ʹ�ܱ�־
	#endif
	pObj->m_StruCmd.m_bDirRevse		= CW;		// ����ȡ�������־
	
	return returnResult;
}

/*******************************************************************************
 �������ƣ�    pSTR_UACtrProc	get_UA_CtrPtr(u8 ObjID)
 ����������    �����ƶ���ID���в�ѯ���ƶ���ָ��
 ���������    �����ƶ���ID
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ú������ⲿ������ã�
 			   �ú���Ϊ����ֱ�ӵ��ú���

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
pSTR_UACtrProc	get_UA_CtrPtr(u8 ObjID)
{
	return &sS_UACtrlProc[0];	
}

/*******************************************************************************
 �������ƣ�    void handleUACtrlObj(u8 nuObjID)
 ����������    �����ƶ���ID����UA������
 ���������    �����ƶ���ID
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ú������ⲿ������ã���MA���ݽ����ڸú��������
 			   ͨ��������������ʵ��
 			   �ú���Ϊ����ֱ�ӵ��ú���

 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void handleUACtrlObj(u8 nuObjID)
{
	pSTR_UACtrProc pObj = &sS_UACtrlProc[0];
	
	#ifdef UA_DEBUG
	updateM0DebugUA(pObj);		/* UA�����ָ��� */
	#endif

	UA_StateMachine(pObj);		/* UA״̬������ */
}
/*******************************************************************************
 �������ƣ�    initUACtrlObj()
 ����������    �����ƶ������UA������
 ���������    ���ƶ���ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    ע�����ݽṹָ�롢���ƶ����
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/10/29      V1.1          Tonghua Li          ɾ��motor_ID
 *******************************************************************************/
static bool initUACtrlObj(void)
{
	pSTR_UACtrProc pObj = &sS_UACtrlProc[0];

	memset(pObj, 0, sizeof(STR_UACtrProc));	//ȫ�����ó�ʼ��
	pObj->mg_nUACtrlProcID	= 0;
	init_UA_Crtl_Proc(pObj);					//�����ƶ������UA��ʼ������

	return TRUE;
}

/*******************************************************************************
 �������ƣ�    bool checkifUARunning()
 ����������    ����Ƿ�����״̬
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    TRUE ----- ������
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/10/29      V1.1          Tonghua Li          ɾ��motor_ID
 *******************************************************************************/
#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
bool checkifUARunning(void)
{
	pSTR_UACtrProc pObj;	
	pObj	= get_UA_CtrPtr(0);                 //�����ƶ���ID���в�ѯ���ƶ���ָ��
	return (pObj->m_eUASysState == E_UA_RUN);
}
#endif	


/*******************************************************************************
 �������ƣ�    UACtrlObjTask(void)
 ����������    �����ƶ������UA������
 ���������    ���ƶ���ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �ú�����������������ã���MA���ݽ����ڸú��������
 			   ͨ��������������ʵ��
 			   �ú���Ϊ����ֱ�ӵ��ú���
 			   POLLING_MODE: 	ѭ�����ã�ÿ�ε���һ��
 			   ��POLLING_MODE:	һ�δ��������ж���
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void UACtrlObjTask(void)
{
	handleUACtrlObj(0);        //�����ƶ���ID����UA������

	#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
	downloadParaHandle(checkifUARunning());    //�����Ƿ��ܹ�����
	#endif
}

/*******************************************************************************
 �������ƣ�    void InitUsrAppLayer(void)
 ����������    ����UA������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    MA���ʼ������
 			   
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 2022/10/29      V1.1          Tonghua Li          ɾ��motor_ID
 *******************************************************************************/
void InitUsrAppLayer(void)
{
	initUACtrlObj();
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



