/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� AppFunction.c
 * �ļ���ʶ��
 * ����ժҪ�� UA״̬������״̬����������
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��08��19��
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "project_config.h"
#include "PubData_MA_UA.h"
#include "MC_Math.h"
#include "MC_type.h"
#include "Time_Process.h"
#include "UA_Data.h"
#include "string.h"


/*******************************************************************************/
//ȫ�ֱ���������



/*******************************************************************************/
//�ⲿ����������
extern pSTR_UACtrProc getUACtrlProcPtr(u8 uaID);
#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
extern bool checkIfDownload(void);
#endif
extern void updateUASetpoint(pSTR_UACtrProc pObj);
extern ENUM_MotorBrakeMode getFaultBrakeMode4UA(pSTR_UACtrProc pObj);
extern ENUM_MotorBrakeMode getBrakeMode4UA(pSTR_UACtrProc pObj);
extern s16 getGearRatio4UA(pSTR_UACtrProc pObj);

/*******************************************************************************/
//�ֲ���̬����������
#if  (UA_CFG_DONT_USED == FALSE)
static void UAConfig(pSTR_UACtrProc pObj);
#endif
static void UARunCmdHandle(pSTR_UACtrProc pObj);
static void UAStopCmdHandle(pSTR_UACtrProc pObj);
static void UAStoppingCmdHandle(pSTR_UACtrProc pObj);
static void UAClrFaultCmdHandle(pSTR_UACtrProc pObj);
static void UAFaultStopHandle(pSTR_UACtrProc pObj);
static bool IfUARunCmd(pSTR_UACtrProc pObj);
static bool IfUAStopCmd(pSTR_UACtrProc pObj);


/*******************************************************************************/
//�ֲ���̬����������


/*******************************************************************************/
/*****************************end***********************************************/


/*******************************************************************************
 �������ƣ�    UA_StateMachine_Handle(u8 uaID)
 ����������    UA״̬��
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
void UA_StateMachine(pSTR_UACtrProc pObj)
{
	STR_PubDatMAUA			*pMA2UA = &pObj->mMaToUa;
	
  pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToUa.SR.Bit.locked = 1;	   //Ӧ�ñ�־����
	memcpy(pMA2UA,&pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToUa,sizeof(STR_PubDatMAUA));
	pObj->mg_pCtrlObj->mGlobalDatPackage.mMaToUa.SR.Bit.locked = 0;    //Ӧ�ñ�־���

    if( HasFaultFromMA(pMA2UA) || (pObj->m_StruCmd.m_bFault == 1) )
    {
        pObj->m_eUASysState	= E_UA_FAULT;			//	����ΪFAULT״̬
    }
	else
	{
		if( !IfPostFinishFromMA(pMA2UA) )
	 	{
			pObj->m_eUASysState	= E_UA_INIT;		//	MA��POSTδ���ʱΪINIT״̬
	 	}
	}

	switch ( pObj->m_eUASysState)					//	UA״̬���������
	{
	    case E_UA_INIT:
	    {  /* UA״̬����ʼ״̬ */ 
			
			#if  (UA_CFG_DONT_USED == TRUE)
			if(IfPostFinishFromMA(pMA2UA))
            {
                pObj->m_eUASysState			= E_UA_STOP;    /* UA״̬��ֹͣ״̬ */
            }
			#else			
            if(IfWaitCfgFromMA(pMA2UA))			//	��Ҫ��������ʱ������Ӧ����Ӳ��������߼�����
            {
                UAConfig(pObj);
            }

            if(IfCfgFinishFromMA(pMA2UA) && IfPostFinishFromMA(pMA2UA))
            {
                pObj->m_eUASysState			= E_UA_STOP;
            }
			#endif
	        break;
	    } 
		
	    case E_UA_STOP:
	    {   /* UA״̬��ֹͣ״̬ */
            if(IfUARunCmd(pObj))	
			{
				UARunCmdHandle(pObj);					//	����ָ���
				if(IsRunningFromMA(pMA2UA))				//	MA��ش�running״̬
				{
					pObj->m_eUASysState	= E_UA_RUN;		/* UA״̬������״̬ */
				}
			}
            else
			{
				UAStopCmdHandle(pObj);          		//	UA��ֹͣ�����
			}
	        break;
		}

	    case E_UA_RUN:
	    {  /* UA״̬������״̬ */
			#if (PARA_DOWNLOAD_ENBALE == FUNCTION_ON)
			if(checkIfDownload())			// ��������ʱ��ͣ�� 
			{
				pObj->m_StruCmd.m_bDownLoadStart = 1;
				pObj->m_StruCmd.m_bRunFlag = 0;
			}
			#endif

			if(IfUAStopCmd(pObj))						//	ָֹͣ��
			{
				UAStoppingCmdHandle(pObj);

				if( IsStoppedFromMA(pMA2UA) )
				{
					pObj->m_eUASysState	= E_UA_STOP;	/* UA״̬��ֹͣ״̬ */
				}
			}
			else					 //����ָ��
			{
				UARunCmdHandle(pObj);					//����ָ���
			}
	        break;
	    }

	    case E_UA_FAULT:
	    {  /* UA״̬������״̬ */
			if(pObj->m_StruCmd.m_bFault == 1)			//	���ڱ�������
	    {
				UAFaultStopHandle(pObj);				//	����ֹͣ����
			}
			else
			{			 
				if( NoFaultCodeFromMA(pMA2UA) ) 			//	MA����Ͻ������ʼ�������������׼��������ϱ�־λ
				{
					if(HasFaultBitFromMA(pMA2UA))
		            {
		                if( IsStoppedFromMA(pMA2UA) )
		                {
		                	if((pObj->m_bFaultAutoClr == 1) || (pObj->m_StruCmd.m_bFaultClr == 1))
		                	{
			                    pObj->m_nuFaultClrCnt++;
			                    if(pObj->m_nuFaultClrCnt >= pObj->m_nuFaultClrPeriod)
			                    {
			                        pObj->m_nuFaultClrCnt	= pObj->m_nuFaultClrPeriod;
						            UAClrFaultCmdHandle(pObj); //�����������
			                    }
		                	}
		                }
		                else
		                {
							pObj->m_nuFaultClrCnt		= 0;   /*�����������*/
		                }
					}
					else
		            {
		            	pObj->m_nuFaultClrCnt			= 0;
						pObj->m_eUASysState				= E_UA_STOP;	                
		            }
				}
				else 			 //MA����ϴ��ڣ����й��ϴ���
				{
					UAFaultStopHandle(pObj);
				}
			}
	        break;
	    }

	    default:
		{
			pObj->m_eUASysState				= E_UA_INIT;    //״̬��Ĭ�ϳ�ʼ״̬
			break;
		}
	}

	pObj->mg_pCtrlObj->mGlobalDatPackage.mUaToMa.gWrFlag = 1;
	memcpy(&pObj->mg_pCtrlObj->mGlobalDatPackage.mUaToMa,&pObj->mUaToMa,sizeof(STR_PubDatUAMA));
	pObj->mg_pCtrlObj->mGlobalDatPackage.mUaToMa.gWrFlag = 0;
}


/*******************************************************************************
 �������ƣ�    static void UAConfig(pSTR_UACtrProc pObj)
 ����������    ��̬����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
#if  (UA_CFG_DONT_USED == FALSE)
static void UAConfig(pSTR_UACtrProc pObj)
{
	pObj->mUaToMa.nGearRatio	= getGearRatio4UA(pObj);	 //UAָ�����
}
#endif



/***********************************�����ָ���*****************************************/
/*******************************************************************************
 �������ƣ�    static void UARunCmdHandle(pSTR_UACtrProc pObj)		 
 ����������    ����ָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void UARunCmdHandle(pSTR_UACtrProc pObj)
{
	#if (DRV0_CLOSE_LOOP == SPEED_LOOP)      //ת�ٻ�����
	{
		if(pObj->m_StruCmd.m_bDirRevse == 1)
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_SPEED_RUN_REV;  // ���е��, �ٶ�ģʽ��ת���������趨�����෴
		}
		else
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_SPEED_RUN;     // ���е��, �ٶ�ģʽ��ת���������趨������ͬ
		}
	}
	#elif (DRV0_CLOSE_LOOP == CURRENT_LOOP)  //����������
	{
		if(pObj->m_StruCmd.m_bDirRevse == 1)
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_TORQUE_RUN_REV; // ���е��, ת��ģʽ��ת���������趨�����෴
		}
		else
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_TORQUE_RUN;     // ���е��, ת��ģʽ��ת���������趨������ͬ
		}
	}
	#elif (DRV0_CLOSE_LOOP == POWER_LOOP)   //���ʻ�����
	{
		if(pObj->m_StruCmd.m_bDirRevse == 1)
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_POWER_RUN_REV;  // ���е��, ����ģʽ��ת���������趨�����෴
		}
		else
		{
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_POWER_RUN;      // ���е��, ����ģʽ��ת���������趨������ͬ
		}
	}
	#else
		#if (DRV0_PWM_ENABLE_STOP == TRUE)
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP;          // ֹͣ���
		#else
			pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP_MOE_OFF;  // �ر�MOE
		#endif	
	#endif
	
	updateUASetpoint(pObj);                                //UAָ�����
	pObj->mUaToMa.nGearRatio	= getGearRatio4UA(pObj);
}

/*******************************************************************************
 �������ƣ�    static void UAStopCmdHandle(pSTR_UACtrProc pObj)		 
 ����������    ����ָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void UAStopCmdHandle(pSTR_UACtrProc pObj)
{
	#if (DRV0_PWM_ENABLE_STOP == TRUE)
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP;			//	ֹͣ���
	#else
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP_MOE_OFF;	//	�ر�MOE
	#endif
	
	updateUASetpoint(pObj);									//	UAָ�����
	pObj->mUaToMa.eBrakeMode	= getBrakeMode4UA(pObj);	//	UAɲ��ָ�����
	
}

/*******************************************************************************
 �������ƣ�    static void UAClrFaultCmdHandle(pSTR_UACtrProc pObj)		 
 ����������    �������������
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void UAClrFaultCmdHandle(pSTR_UACtrProc pObj)
{
	#if (DRV0_PWM_ENABLE_STOP == TRUE)
	{
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_FAULT_CLR_ENMOE;		// ������� MOEʹ��
	}
	#else
	{
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_FAULT_CLR_DIMOE;		// ������� MOE�ر�
	}
	#endif
	updateUASetpoint(pObj);											//UAָ�����
	pObj->mUaToMa.eBrakeMode	= getFaultBrakeMode4UA(pObj);		//UA����ɲ��ָ�����
}

/*******************************************************************************
 �������ƣ�    static void UAStoppingCmdHandle(pSTR_UACtrProc pObj)		 
 ����������    ָֹͣ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void UAStoppingCmdHandle(pSTR_UACtrProc pObj)
{
	pObj->mUaToMa.eBrakeMode	= getFaultBrakeMode4UA(pObj);	//	UA����ɲ��ָ�����

	#if (DRV0_STOP_MODE == 0)
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP_MOE_OFF;		//	�ر�MOE
	#else
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP;				//	ֹͣ���
	#endif
	
	updateUASetpoint(pObj);										//	UAָ�����
}

/*******************************************************************************
 �������ƣ�    static void UAFaultStopHandle(pSTR_UACtrProc pObj)		 
 ����������    ����ָ�����
 ���������    pSTR_UACtrProc pObj
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/8/19      V1.0           Tonghua Li          ����
 *******************************************************************************/
static void UAFaultStopHandle(pSTR_UACtrProc pObj)
{
	pObj->mUaToMa.eBrakeMode	= getFaultBrakeMode4UA(pObj);	//UA����ɲ��ָ�����
	
	#if (DRV0_STOP_MODE == 0)
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP_MOE_OFF;		// �ر�MOE
	#else
		pObj->mUaToMa.eAppCmd	= E_CMD_MOTOR_STOP;				// ֹͣ���
	#endif
	
	updateUASetpoint(pObj);										//UAָ�����
}

/*******************************************************************************
 �������ƣ�    IfUARunCmd(pSTR_UACtrProc pObj) 
 ����������    UA������ָ���
 ���������    pSTR_UACtrProc 
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/16      V1.0       Wencai.Zhao				����
 *******************************************************************************/
static bool IfUARunCmd(pSTR_UACtrProc pObj)
{
	if( pObj->m_StruCmd.m_bMoeEnable)
	{
		return pObj->m_StruCmd.m_bRunFlag;
	}
	else
	{
		return FALSE;
	}
}

/*******************************************************************************
 �������ƣ�    UAFaultStopHandle(pSTR_UACtrProc pObj) 
 ����������    UA��ָֹͣ���
 ���������    pSTR_UACtrProc 
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/10/16      V1.0       Wencai.Zhao				����
 *******************************************************************************/
static bool IfUAStopCmd(pSTR_UACtrProc pObj)
{
	if( pObj->m_StruCmd.m_bMoeEnable)
	{
		return (pObj->m_StruCmd.m_bRunFlag == 0);
	}
	else
	{
		return TRUE;
	}
}

///////////////////////////////////////////////////
/************************ (C) Bright Power Semiconductor Co.ltd *****END OF FILE****/
