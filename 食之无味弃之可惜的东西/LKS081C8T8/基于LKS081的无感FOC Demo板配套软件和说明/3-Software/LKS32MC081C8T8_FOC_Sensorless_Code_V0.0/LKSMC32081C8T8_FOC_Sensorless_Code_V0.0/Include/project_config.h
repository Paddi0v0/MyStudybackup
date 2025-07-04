/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� project_config.h
 * �ļ���ʶ��
 * ����ժҪ�� ��������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Tonghua Li
 * ������ڣ� 2022��12��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2022��12��20��
 * �� �� �ţ� V 1.0
 * �� �� �ˣ� Tonghua Li
 * �޸����ݣ� ����
 *
 *******************************************************************************/

#ifndef __PROJECT_CONFIG_H_
#define __PROJECT_CONFIG_H_

#include "project_mcu.h"
#include "MC_Type.h"
#include "MC_configure_FB_data.h"
#include "MC_Parameter_M0.h"


/*********************************************************************************
*
* ƽ̨Ӳ����Ϣ���壬����������hardware_config.h����ϸ����
*
*		MCUʱ��
*		PWM������ID����
*
* ���������ļ�Ӧ����Щ��ͬ����Ϣ
**********************************************************************************/
#define MCU_MCLK_192M                   (192LL) 
#define MCU_MCLK_96M                    (96LL) 
#define MCU_MCLK_48M                    (48LL) 

/*-------------------------------------------------------------------------------*/
#define PLANTFORM_EPWM0					(0)			// �̶�ΪEPWM0
#define PLANTFORM_EPWM1					(1)			// �̶�ΪEPWM1

/*********************************************************************************/

/*********************************************************************************
* �û�������Ϣ��
*	����ѡ���MCU�źż�Ӳ����·����ͼ��������
**********************************************************************************/
// ֧�ֵ�PWMͨ�����ݣ�����֧��������Ԫ��Ŀ
// ĿǰMCU���֧��˫PWM�������Ϊ2
// ���Ը���MCU chipNumber �Զ����к궨��
// 45x/07xx 	--- 2��PWM��
// 08x/03x/05x	--- 1��PWM

#define PLANTFORM_MAX_PWM_MODULE_NUM	(1)			// �����ƽ̨���̶�Ϊ1������Ҫ�޸�

/*********************************************************************************
* ƽ̨֧�ֵĵ���������ʽ
* ��ǰ45xEVM�岻֧�����������: 	
*				EPWM0��Ӧ��AD֧�� ������/˫����
*				EPWM1��Ӧ��AD֧�� ������
* 08xEVM��:
*				EPWM0��Ӧ��AD֧�� ������/˫����/������
*
* 03xEVM��:
*				EPWM0��Ӧ��AD֧�� ������/˫����
*
*
* ����ʱ��ҪУ��Ӳ�������Ƿ�֧�֣�������ӦӲ������
* Ŀǰ	 EPWM0��Ӧ��AD֧�� ������/˫����
*		 EPWM1��Ӧ��AD֧�� ������
**********************************************************************************/
#define EPWM0_CURRENT_SAMPLE_TYPE		(CURRENT_SAMPLE_2SHUNT)		// �����ƽ̨����������ʽ����
	
/*********************************************************************************/
	
#define MCU_MCLK_USED                   (MCU_MCLK_96M)				// ��ǰMCU��Ƶ������MCU�޸�



/*********************************************************************************
*
* Drive Unit������Ԫģ��(DU) ��������
*
*		������Ԫģ���������    ------  ֧��������ԪDU������ʵ�ʵĵ����Ŀ�ɴ���������Ԫ
*		���������				------  ������������������������
*										�ɴ���������Ԫģ�����
*										ʵ�ֶ�������
*										�������� PubData_DriverInstance.c �ж���ʵ��
*
*				Ĭ��������Ԫ0   ----- ������0  �Ķ�Ӧ��ϵ
*				��������� ����ڵ���������Ԫ����
*				���������������������Ԫ����ʱ�����û���Ӵ�����в������°󶨵�������Ԫ
*
**********************************************************************************/
#define PLANTFORM_DRV_MODULE_NUM		(1)					// ������Ԫ��Ŀ��������̶�Ϊ1������Ҫ�޸�
#define PLANTFORM_MAX_CFG_PARA_ID		(1)					// ��������Ŀ�����޸�Ϊ������������е��ú���ά��
															// ��ʼ������

//���õĵ��ģ����Ŀ��ҪС������PWMģ�����
#if (PLANTFORM_DRV_MODULE_NUM > PLANTFORM_MAX_PWM_MODULE_NUM)
#error "Don't support Driver Module MUM"
#endif

//���õĵ��ģ����Ŀ��ҪС������PWMģ�����
#if (PLANTFORM_DRV_MODULE_NUM > PLANTFORM_MAX_CFG_PARA_ID)
#error "Check parameter num and Driver Module MUM"
#endif


/*********************************************************************************
* ��ǰӲ��ѡ���EPWMͨ������
*
* ������Ԫ  EPWMͨ���������鼰������ʽ����
*
* ������Ԫģ�����Ϊ1ʱ: ��������Ԫ0����
*		Ĭ��ΪEPWM0�Ͳ���M0����
*			�Ե�EPWM��MCU,��EPWM0����,MA��UA��ȵı����Ĭ��ΪM0�����á�
*			�Զ�EPWM��MCU,DU������DRV0_PWM_ID(EPWM0����EPWM1),��ѡ�����ò���DRV0_PARA_ID��
*				����Ҫ����DRV0_PARA_ID��Ӧ��MC_GlobalDef_Mx.h�е���Ӧ����,
*				CURRENT_SAMPLE_TYPE_Mx �� PWM_USED_ID_Mx Ϊ DRV0_CUR_SAMP_TYPE/DRV0_PWM_ID��
*
*
**********************************************************************************/
#if (PLANTFORM_DRV_MODULE_NUM == 1)		//������Ԫģ�����Ϊ1

// ������Ԫ0����
#define DRV0_PWM_ID						(PLANTFORM_EPWM0)			//  ѡ��EPWM0
#define DRV0_PARA_ID					(0)							//  ѡ�ò�����0����M0��������
#define DRV0_CUR_SAMP_TYPE				(EPWM0_CURRENT_SAMPLE_TYPE)

#define DRV0_BEMF_CHECK_EABLE			(FUNCTION_OFF)

/*-------------------------------------------------------------------------------*/
#elif (PLANTFORM_DRV_MODULE_NUM == 2)	//������Ԫģ�����Ϊ2
#error "Only Support single motor! "
#endif

/*********************************************************************************
* ��������Ԫʱ������ռ�����Ż�����֧��������Ԫ0
* �����趨�Ĳ����飬���ñ����ѡ��
* ��������Ԫʱ���������ò���ѡ��������к�ѡ�����
**********************************************************************************/
#if (DRV0_PARA_ID == 0)
	#define DRV0_CLOSE_LOOP				(CLOSE_LOOP_M0)
	#define DRV0_POWER_LIMIT_STATUS		(POWER_LIMIT_STATUS_M0)
	#define DRV0_SPPED_LIMIT_STATUS		(SPPED_LIMIT_STATUS_M0)
	#define DRV0_WEAKEN_FLUX_STATUS		(WEAKEN_FLUX_STATUS_M0)
	#define DRV0_PWM_ENABLE_STOP		(PWM_ENABLE_STOP_M0)
	#define DRV0_STOP_MODE				(STOP_MODE_M0)
	#define DRV0_UNDVOLT_PROT_ENABLE	(U_UNDVOLT_PROT_ENABLE_M0)
	#define DRV0_DIR_CHECK_ENABLE		(DIR_CHECK_STATUS_M0)
	#define DRV0_POS_SEEK_ENABLE		(SEEK_POSITION_STATUS_M0)
	#define DRV0_ALLIGN_CUR_ENABLE		(ALLIGN_STATUS_M0)
	#define DRV0_CHARGE_ENABLE			(CHARGE_STATUS_M0)
#else
	#error "Check parameter num in 1 DRV Unit mode"
#endif


/*********************************************************************************
* �����������ü�������Ϣ
*
**********************************************************************************/

#define QUICK_CURRENTKLOOP_FUNCTION		(FUNCTION_ON)		// ���ٵ�����ģʽ�����̵�����ִ��ʱ��

#define OBSERVER_PLL_CHANGE_ENABLE		(FUNCTION_ON)		// �۲���PLL����ʹ��

#define VF_START_FUNCTION_ENABLE		(FUNCTION_ON)		// VF����ʹ��

#if (VF_START_FUNCTION_ENABLE == FUNCTION_ON)
#define VECTOR_VF_COMP_ENABLE			(FUNCTION_ON)		// VF����ʹ��ʱ��VECTOR VF�����Զ�ʹ��
#else
#define VECTOR_VF_COMP_ENABLE			(FUNCTION_OFF)		// VECTOR VF����ʹ��
#endif

#define POWER_CALC_CALIB_FUNCTION_ENABLE	(FUNCTION_ON)	// ����У��ʹ�� y=a2 * x^2 +a1 * x + a0

#define RTT_FUNCTION                   FUNCTION_ON			/* RTT ���Թ��� */

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************
*�������� ��������LKSMotor ����
*
*��������Ĵ洢λ�ã��ڹ���rom�������Ѿ�����FLASH��С���ж��壬�˴���Ҫ��������
*����ADDR_PARA_LOCATION��ַ
*
*
**********************************************************************************/
#define DOWNLOAD_STOP_WAIT_MS			(8000)		// �������״̬����ʱǿ�Ƶȴ�ʱ�䣬������Ŀ���ú���ʱ��̶�
													// ���������״̬ʱ��ֱ������

#define DOWNLOAD_WRITE_REQ_TIME_OUT		(100)		// ms


// ATTENTION:
/********************************************************************************************/
// The fowwing is used in Keil V5 compiler
// In Keil AC6 compiler, as _at_ is changed, the address is directly used in 
//		__attribute__((section(".ARM.__at_0xFE00")))
// sizeof(STR_DrvCfgGlobal) = 8
// IN different MCU, the address is different

#define ADDR_DOWNLOAD_WRITE_LOCATION	(0x20001000)
#define ADDR_DOWNLOAD_READ_LOCATION		(0x20001001)

#define ADDR_PARA_LOCATION				(0xFE00)
#define ADDR_PARA_START_LOCATION		(ADDR_PARA_LOCATION + sizeof(STR_DrvCfgGlobal))

// ATTENTION END
/*********************************************************************************************/

#define PARA_CFG_HEADER					(0x80)
#define PARA_CFG_MAIN_VERSION			(0x0A)
#define PARA_CFG_VERSION				(0x04)
#define PARA_CFG_SUB_VERSION			(0x01)

#define PARA_CFG_LENGTH					(0x01)
#define PARA_CFG_NUMBER					(PLANTFORM_MAX_CFG_PARA_ID)


#define PARA_DOWNLOAD_ENBALE			(FUNCTION_ON)		//ʹ�ܲ���������������У��

#define PARA_CHECK_ENBALE				(FUNCTION_ON)		//ʹ�ܲ�������У�飬У��汾�ŵ�

#define DEBUG_PWM_OUTPUT          (FUNCTION_OFF) // �������3��PWM �ӵ��ǰʹ��
#endif  /* __PROJECT_CONFIG_H_ */

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

