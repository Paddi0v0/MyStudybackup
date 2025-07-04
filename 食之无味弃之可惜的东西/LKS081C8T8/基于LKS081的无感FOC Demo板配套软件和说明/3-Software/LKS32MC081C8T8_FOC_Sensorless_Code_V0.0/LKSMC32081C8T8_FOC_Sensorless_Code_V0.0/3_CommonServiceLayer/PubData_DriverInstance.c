#include "project_config.h"
#include "PubData_DriverInstance.h"
#include "MC_Parameter_M0.h"
#include "string.h"


/*******************************************************************************/
//ȫ�ֱ���������

/*******************************************************************************/
//�ⲿ����������

/*******************************************************************************/
//�ֲ���̬����������


/*******************************************************************************/
#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
u8 gs_bDownReqCmd		__attribute__((at(ADDR_DOWNLOAD_WRITE_LOCATION))) = 0;
u8 gs_bDownAckStatus	__attribute__((at(ADDR_DOWNLOAD_READ_LOCATION))) = 0;
u8 gs_bDownloadStatus = 0;
u16 gs_timeCnt;
#endif

//�ֲ���̬����������

//	���ò���ͨ��������αָ�������ָ����FLASH�ռ�

const STR_DrvCfgPara		tS_DrvCfgPara[MAX_CFG_PARA_ID] __attribute__((at(ADDR_PARA_START_LOCATION))) = {
															DRV_CFG_PARA_TAB_M(0)	
															};

//	ȫ�����ò���ͨ��������αָ�������ָ����FLASH�ռ�
const STR_DrvCfgGlobal		tS_DrvCfgGlobal __attribute__((at(ADDR_PARA_LOCATION))) = DRV_CFG_GLOBAL_PARA_TAB;

															 

// FOC ���ƶ��󴫵ݲ������ݽṹ
static bool 				tB_MotorFocParaIsValid 			= FALSE;
static STR_MotorFocParDef 	tS_MotorFocParM[MAX_DRV_MODULE_USED];			// FOC �������ݽṹ��


// ������ƶ���
// ����������ƶ�����Ϊ�ֲ���̬����������ͨ������getCtrlObjectPtr()����
// M0�̶���ӦPWM0, M1�̶���ӦPWM1,
const u8	gS_PWMObjDefined[MAX_DRV_MODULE_USED] = {
													DRV0_PWM_ID
													};



STR_ControlObject gS_PhyObjInstanceM[MAX_DRV_MODULE_USED];

// tS_DrvCfgPara[]��gS_PhyObjInstanceM[] �±�Ĭ�϶�Ӧ��Ӧ���пɴ�������ӳ��

/*
����ӳ���Ӧ��ϵ
	gS_PhyObjInstanceM[n]
	
							tS_DrvCfgPara[m]
												   ----------	sS_UACtrlProc[id]	
												|
												|
							gS_PhyObjInstanceM[id]  ----------	commom layer
									|
									|
										----------				sS_MACtrProc[id]
									|
									|
										----------			     gS_FocObjM[id] 
									|						          |
									|						          |
									|
										----------				gS_MotorHalDrvHandleM[id] ����
														
*
*/



/*******************************************************************************/
/*****************************end***********************************************/

u8	getPWMIDUsed(u8 objID)
{
	#ifdef PWM_MODULE_DYNAMIC_CONFIG
	return tS_DrvCfgPara.mS_GlobalCfg.m_bPWMUsedID;
	#else
	return gS_PWMObjDefined[objID];
	#endif
}
/*****************************************************************************
 * ������   : bool initGlobalPhyObj(void)
 * ˵��     : ���ÿ��ƶ������ó�ʼ��
 *			: m_bPWM_IdΪ��ǰ��PWM���� 0 ---- PWM0 
 * ���˼· ����������
 *			����ʼ��gS_PhyObjInstanceM[]ȫ�ֱ�����tS_MotorFocParM������MA��ʼ��
 * ����     ����
 * ����ֵ   ��
 * �޸�ʱ�� ��2022.10.27
 *****************************************************************************/
void initGlobalPhyObj(void)
{
	u8 t_nuObjID;
	for(t_nuObjID = 0;t_nuObjID < MAX_DRV_MODULE_USED; t_nuObjID++)
	{
		memset(&gS_PhyObjInstanceM[t_nuObjID],0,sizeof(STR_ControlObject));		
		gS_PhyObjInstanceM[t_nuObjID].m_pMotorFocPar	= &tS_MotorFocParM[t_nuObjID];	//	��ʼ��FOC�����ṹ��ָ��
		gS_PhyObjInstanceM[t_nuObjID].m_pDrvCfgPara 	= &tS_DrvCfgPara[t_nuObjID];	//	��ʼ�����ò����ṹ��ָ��
		gS_PhyObjInstanceM[t_nuObjID].m_bPWM_Id			= getPWMIDUsed(t_nuObjID);		//	��ʼ����ǰ�����ӦPWM��ID
	}
}

/*****************************************************************************
 * ������   : bool checkParaCfgValid(void)
 * ˵��     : У�����ò����Ƿ���Ч
 *			:  
 * ���˼· ����������
 *			��
 * ����     ����
 * ����ֵ   ��
 * �޸�ʱ�� ��2023.01.29
 *****************************************************************************/
bool checkParaCfgValid(void)
{
	#if (PARA_CHECK_ENBALE == FUNCTION_ON)
	if((tS_DrvCfgGlobal.header == PARA_CFG_HEADER)
		&& (tS_DrvCfgGlobal.mainVersion == PARA_CFG_MAIN_VERSION)
		&& (tS_DrvCfgGlobal.version == PARA_CFG_VERSION)
		&& (tS_DrvCfgGlobal.subversion == PARA_CFG_SUB_VERSION)
		&& (tS_DrvCfgGlobal.length == PARA_CFG_LENGTH)
		&& (tS_DrvCfgGlobal.objNumber == PARA_CFG_NUMBER)
		)
	{
				//	У�����ò�����ͷ���汾�š����ݳ��ȵ���Ϣ
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	#else
	
		return TRUE;	
	#endif
}

/*****************************************************************************
 * ������   : bool ifMotorFOCParaIsValid (void)
 * ˵��     : MD���ӿڱ����Ƿ���Ч
 * ���˼· ����������
 * ����     ����
 * ����ֵ   ��TRUE OR FALSE
 * �޸�ʱ�� ��2022.09.16
 *****************************************************************************/

bool ifMotorFOCParaIsValid (void)
{
	return tB_MotorFocParaIsValid;
}
/*****************************************************************************
 * ������   : void updateMotorFocPara(void)
 * ˵��     : MD���ӿڱ�������
 * ���˼· ����������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.09.16
 *****************************************************************************/
void updateMotorFocPara(void)
{
	// �ź������� 
	tB_MotorFocParaIsValid	= FALSE;

	// add ��ʼ��MD���ӿڱ�������

	// �����������Ϊ��Ч
	tB_MotorFocParaIsValid	= TRUE;
	// �ź�������
}

/*****************************************************************************
 * ������   : void BondingMotorDrvObject(void)
 * ˵��     : �� MD���ӿڱ������� �� ��������
 * ���˼· �����ð�
 *			�����ò�����������ƶ���Ĭ��	���ò���0 --- ���ƶ���0
 *									���ò���1 --- ���ƶ���1
 *										..............
 *									���ò���n --- ���ƶ���n
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.09.16 V0.01 By Tonghua Li
 * �޸�ʱ�� ��2022.11.01 V0.02 By Tonghua Li
 *****************************************************************************/
void BondingMotorDrvObject(void)
{
	initGlobalPhyObj();	
}

/*****************************************************************************
 * ������   : bool reBondingMotorDriveParaDat(u8 objID, u8 objCfgID)
 * ˵��     : �ذ� ��������
 * ���˼· �����ݲ�������ID�Զ���ID�������ð�
 *			: �ذ󶨺���Ҫ���в�����ʼ�����ɵ�����ʵ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2022.09.16 V0.01 By Tonghua Li
 * �޸�ʱ�� ��2022.11.01 V0.02 By Tonghua Li
 *****************************************************************************/

// ������ڶ��һ�İ�ʱ���絥��֧�ֶ�������ֻ�����е��������ʼ��ʱ�ض��� 

// ����gS_PhyObjInstanceM[objID]��PWM_IDͨ������귽ʽʵ�֣�MD/HD�㲻֧�ֶ�̬��
// Ŀǰ��֧���޸�gS_PhyObjInstanceM[objID]��PWM_ID��Ӧ�Ĳ���������m_pDrvCfgPara
bool reBondingMotorDriveParaDat(u8 objID, u8 objCfgID)
{
	bool bResult = FALSE;
	if(objID >= MAX_DRV_MODULE_USED)
	{
		return bResult;
	}
	if(objCfgID < MAX_CFG_PARA_ID)
	{		
		gS_PhyObjInstanceM[objID].m_pDrvCfgPara		= &tS_DrvCfgPara[objCfgID];
	}

	return bResult;	
}

/*****************************************************************************
 * ������   : bool reBondingMotorDrvObject(u8 objID, u8 objCfgID)
 * ˵��     : �ذ� MD���ӿڱ������� �� ��������
 * ���˼· ��ͨ��objID���ҿ��ƶ���ָ��
 * ����     ��objID
 * ����ֵ   �����ƶ���ָ��
 * �޸�ʱ�� ��2022.09.16 V0.01 By Tonghua Li
 * �޸�ʱ�� ��2022.10.27 V0.02 By Tonghua Li
 *****************************************************************************/

PSTR_ControlObject getCtrlObjectPtr(u8 objID)
{
	if(objID >= MAX_DRV_MODULE_USED)
	{
		return NULL;
	}
	return &gS_PhyObjInstanceM[objID];
}


PSTR_DrvCfgPara getCfgParaPtr(u8 objID)
{
	if(objID >= MAX_DRV_MODULE_USED)
	{
		return NULL;
	}
	return gS_PhyObjInstanceM[objID].m_pDrvCfgPara;
}

u8 getCtrlObjectID(u8 PWM_ID)
{
	u8 t_nuObjID;
	for(t_nuObjID = 0;t_nuObjID < MAX_DRV_MODULE_USED; t_nuObjID++)
	{		
		if(gS_PhyObjInstanceM[t_nuObjID].m_bPWM_Id == PWM_ID)
		{
			return t_nuObjID;
		}
	}

	return t_nuObjID;	
}


s16 getParaVFCompCofig(u8 t_bObjId)
{
	PSTR_DrvCfgPara			t_pDrvCfgPara;
	t_pDrvCfgPara	= gS_PhyObjInstanceM[t_bObjId].m_pDrvCfgPara;
	return t_pDrvCfgPara->mS_FBSlvcCfg0.m_VFDeadZoneComp;
}

/*****************************************************************************
 * ������   : void downloadParaHandle(bool t_bMotorRunning)
 * ˵��     : �����Ƿ��ܹ�����
 * ���˼· ���Ƿ��ܹ����أ������ֹʱ�������ء�ͨ��Լ�����ֽ�������ж�
 * ����     ��t_bMotorRunning
 * ����ֵ   ��void
 * �޸�ʱ�� ��2023.02.09 V0.01 By Tonghua Li
 * �޸�ʱ�� ��
 *****************************************************************************/
#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
void downloadParaHandle(bool t_bMotorRunning)
{
	switch (gs_bDownloadStatus)
	{
		case  0:			// Step 1
		{
			if(gs_bDownReqCmd == DOWNLOAD_WRITE_REQ1)
			{
				gs_bDownAckStatus = DOWNLOAD_READ_ACK1;
				gs_timeCnt++;
				if(gs_timeCnt >= DOWNLOAD_WRITE_REQ_TIME_OUT)
				{
					gs_timeCnt = 0;
				}
			}
			else
			{ 
				if((gs_bDownReqCmd == DOWNLOAD_WRITE_REQ2) && (gs_timeCnt != 0))
				{					
					gs_bDownloadStatus  = 1;
				}
				else
				{
					gs_bDownAckStatus = DOWNLOAD_READ_ACK_INVLAD;
				}
				gs_timeCnt			= 0;
			}
			break;
		}

		case  1:			// Step 2
		{
			if(gs_bDownReqCmd == DOWNLOAD_WRITE_REQ2)
			{
				gs_bDownAckStatus = DOWNLOAD_READ_ACK2;
				gs_timeCnt++;
				if(gs_timeCnt >= DOWNLOAD_WRITE_REQ_TIME_OUT)
				{
					gs_timeCnt = 0;
				}
			}
			else
			{				
				if((gs_bDownReqCmd == DOWNLOAD_WRITE_REQ3) && (gs_timeCnt != 0))
				{	
					if(!t_bMotorRunning)
					{
						gs_timeCnt			= DOWNLOAD_STOP_WAIT_MS;
					}
					gs_bDownloadStatus  = 2;
				}
				else
				{
					gs_bDownloadStatus	= 0;
					gs_timeCnt			= 0;
					gs_bDownAckStatus	= DOWNLOAD_READ_ACK_INVLAD;
				}				
			}
			break;
		}

		case  2:							// Step 3
		{
			if(gs_bDownReqCmd == DOWNLOAD_WRITE_REQ3)
			{
				if(!t_bMotorRunning)		// Can be downloaded
				{
					gs_timeCnt++;
					if(gs_timeCnt >= DOWNLOAD_STOP_WAIT_MS)		//�ȴ�ʱ�䵽��
					{
						gs_bDownAckStatus = DOWNLOAD_READ_ACKOK;
					}
				}
				else						// Wait to stop
				{
					gs_timeCnt = 0;
					gs_bDownAckStatus = DOWNLOAD_READ_ACK_DOING;
				}
			}
			else
			{
				gs_bDownloadStatus	= 0;
				gs_bDownAckStatus	= DOWNLOAD_READ_ACK_INVLAD;
			}
			break;
		}

		default:
		{
			gs_bDownloadStatus  = 0;
			gs_timeCnt			= 0;
			gs_bDownAckStatus	= DOWNLOAD_READ_ACK_INVLAD;
			break;
		}
	}
}

bool checkIfDownload(void)
{
	return (gs_bDownloadStatus == 2);
}
#endif

///////////////////////
