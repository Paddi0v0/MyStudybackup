/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� Time_Process.c
 * �ļ���ʶ��
 * ����ժҪ�� ��ʱ��غ���
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet Li
 * ������ڣ� 2020��8��16��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��16��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet Li
 * �޸����ݣ�����
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "basic.h"
#include "project_mcu.h"
#include "hardware_config.h"

#include "MC_Parameter_M0.h"

#include "math.h"
#include "hardware_init.h"
#include "PubData_DriverInstance.h"
#include "segger_rtt.h"

void ADC_1Shunt_NormalModeCFG(void);
void CurrentOffsetCalibration(void);
#if (DEBUG_PWM_OUTPUT == TEST_ON)
void DebugPWM_OutputFunction(void); /* ���Ե�ʱ�����50%��PWM���� */
#endif

extern PSTR_ControlObject getCtrlObjectPtr(u8 objID);
extern void ADC0_init(void);
extern void ADC1_init(void);
extern u8 getLibMaxObjNumber(void);
extern void DebugPWM_OutputFunction(void);

/*******************************************************************************
 �������ƣ�    bool ConfigData_check(void)
 ����������    �������ݼ�֧�ֿ���
 ���������    ��
 ���������    ��
 �� �� ֵ��    TRUE ------- ƥ��   FALSE ----- ��ƥ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2023/1/15      V1.0           Li Tonghua          ����
 *******************************************************************************/
bool ConfigData_check(void)
{
	if(checkParaCfgValid())				//	������ò����汾�ŵ��Ƿ���Ч����Ҫ����LMSMotor���ز���У��
	{
		BondingMotorDrvObject();		//	У��ͨ�����󶨿��ƶ���
	}
	else
	{
		return FALSE;					//	������Ч�����ú�����Ҫ���г�����
	}

	//	У����Ƿ�ƥ��?
	if(getLibMaxObjNumber() < PLANTFORM_DRV_MODULE_NUM)
	{
		return FALSE;					//	�������С�ڵ�ǰ����Ķ���������ⲻ֧��
	}	
	return TRUE;
}


/*******************************************************************************
 �������ƣ�    int sys_init(void)
 ����������    ϵͳ������ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void sys_init(void)
{
	InitMotorDriveLayer();
	InitMotorAppLayer();
	InitUsrAppLayer();

	CurrentOffsetCalibration();                 /* ��ȡ��������ͨ��ƫ�� */
	
	//gS_TaskScheduler.sVersion = &sVersion[0];/* ��ʼ���汾�� */
    
    #if(MCPWM_DEBUG == FUNCTION_ON)
    DebugPWM_OutputFunction();
    	// debug �̶�PWM ������Σ����ڲ���Ӳ��
		// Ҳ���Բ���VF����
		// ����ʱȷ��Ӳ��������������	
    #endif
    
	#if (RTT_FUNCTION == FUNCTION_ON)
    /* JScope RTTģʽ��ʼ�� */
    SEGGER_RTT_ConfigUpBuffer(1, "JScope_i2i2i2i2", bRttBuf, 2048, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
	#endif
}

/*******************************************************************************
 �������ƣ�    static void CurrentOffsetReadPWM0(STR_PubDatHDMD* tS_pHdToMd, u16 t_nuCalPeriod)
 ����������    ��PWM0ģ���Ӧ�ĵ���Offsetֵ
 �����ı�    ��
 ���������    �������ָ��
 ���������    �������ָ��ش�
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����    �漰Ӳ�������������Ӳ������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/11/01 	 V1.0			Tonghua Li		  ����ʷ�汾����
 *******************************************************************************/
#if (DRV0_PWM_ID == PLANTFORM_EPWM0)
static void CurrentOffsetReadPWM0(STR_PubDatHDMD* tS_pHdToMd, u16 t_nuCalPeriod)
{
    volatile s32 t_offset1, t_offset2, t_offset3;
    volatile s32 t_offset4;

    volatile u32 t_dlay;

    __disable_irq();


    MCPWM0_PRT = 0x0000DEAD; /* enter password to unlock write protection */
    MCPWM0_TH00 = 0x00;
    MCPWM0_TH01 = 0x00;
    MCPWM0_TH10 = 0x00;
    MCPWM0_TH11 = 0x00;
    MCPWM0_TH20 = 0x00;
    MCPWM0_TH21 = 0x00;
    MCPWM0_UPDATE = 0xff;    /* write whatever value to trigger update */
    MCPWM0_PRT = 0x0000CAFE; //write any value other than 0xDEAD to enable write protection

    PWMOutputs(DISABLE);	//���ڷ�MOS�����������ʹ��Ҳ�ɲ�ʹ��

    ADC0_TRIG = 0;

	ADC_SOFTWARE_TRIG_ONLY();//���������
    ADC0_CHN0 = 4; //����4��ͨ��

	ADC0_STATE_RESET();

    //��ʱ�����offset����
    for(t_dlay = 0; t_dlay < 0x7ffff; t_dlay++);
	

    //������Offset
	#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN3, M0_ADC_BUS_CURR_CH, ADC_GAIN3V6);
	#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
	ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 4) | (ADC_BUS_VOL_CHANNEL<<8) | (M0_ADC_BUS_CURR_CH << 12);
	#elif  (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)
	ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 4) | (ADC0_CURRETN_C_CHANNEL<<8) | (M0_ADC_BUS_CURR_CH << 12);
	#endif

    t_offset1 = 0;
    t_offset2 = 0;
    t_offset3 = 0;
    t_offset4 = 0;

	for(t_dlay = 0; t_dlay < (t_nuCalPeriod); t_dlay++)
    {
        /* Clear the ADC0 JEOC pending flag */
        //ADC0_SWT = 0x00005AA5;
		ADC_SoftTrgEN(ADC0, ENABLE);   //������� �ڲ���Ҫ����

        while(!(ADC0_IF & BIT0));

        ADC0_IF |= BIT1 | BIT0;
		ADC0_STATE_RESET();

        t_offset1 +=  (s16)((ADC0_DAT0));
        t_offset2 +=  (s16)((ADC0_DAT1));
        t_offset3 += (s16)((ADC0_DAT2));
        t_offset4 += (s16)(ADC0_DAT3);
    }

	ADC0_init();

	#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	tS_pHdToMd->nBusShuntOffset = (s16)(t_offset1/t_nuCalPeriod);
	#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
	tS_pHdToMd->nPhaseUOffset=(s16)(t_offset1/t_nuCalPeriod);
	tS_pHdToMd->nPhaseVOffset=(s16)(t_offset2/t_nuCalPeriod);
	#elif((EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
	tS_pHdToMd->nPhaseUOffset=(s16)(t_offset1/t_nuCalPeriod);
	tS_pHdToMd->nPhaseVOffset=(s16)(t_offset2/t_nuCalPeriod);
	tS_pHdToMd->nPhaseWOffset=(s16)(t_offset3/t_nuCalPeriod);
	#endif
}

#elif (DRV0_PWM_ID == PLANTFORM_EPWM1)
/*******************************************************************************
 �������ƣ�    static void CurrentOffsetReadPWM1(void)
 ����������    ��PWM1ģ���Ӧ�ĵ���Offsetֵ
 �����ı�    ��
 ���������    �������ָ��
 ���������    �������ָ��ش�
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����	   �漰Ӳ�������������Ӳ������
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/11/01 	 V1.0			Tonghua Li		  ����ʷ�汾����
 *******************************************************************************/
static void CurrentOffsetReadPWM1(STR_PubDatHDMD* tS_pHdToMd,u16 t_nuCalPeriod)
{

    volatile s32 t_offset1, t_offset2, t_offset3;
    volatile s32 t_offset4;

    volatile u32 t_dlay;

    __disable_irq();

	MCPWM0_PRT = 0x0000DEAD; /* enter password to unlock write protection */
	MCPWM0_TH30 = 0x00;
	MCPWM0_TH31 = 0x00;
	MCPWM0_TH40 = 0x00;
	MCPWM0_TH41 = 0x00;
	MCPWM0_TH50 = 0x00;
	MCPWM0_TH51 = 0x00;

	MCPWM0_UPDATE = 0xff;	 /* write whatever value to trigger update */
	MCPWM0_PRT = 0x0000CAFE; //write any value other than 0xDEAD to enable write protection

    PWMOutputs(DISABLE);	//���ڷ�MOS�����������ʹ��Ҳ�ɲ�ʹ��

	
    ADC0_TRIG = 0;

	ADC_SOFTWARE_TRIG_ONLY();//���������
    ADC0_CHN0 = 4; //����4��ͨ��

	ADC0_STATE_RESET();

    //��ʱ�����offset����
    for(t_dlay = 0; t_dlay < 0x7ffff; t_dlay++);
	

    //������Offset
#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	ADC_CHN_GAIN_CFG(ADC0, CHN0, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN1, ADC_1SHUNT_CURR_CH, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN2, ADC_BUS_VOL_CHANNEL, ADC_GAIN3V6);
    ADC_CHN_GAIN_CFG(ADC0, CHN3, M0_ADC_BUS_CURR_CH, ADC_GAIN3V6);
#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
	ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 4) | (ADC_BUS_VOL_CHANNEL<<8) | (M0_ADC_BUS_CURR_CH << 12);
#elif  (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)
	ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 4) | (ADC0_CURRETN_C_CHANNEL<<8) | (M0_ADC_BUS_CURR_CH << 12);
#endif

    t_offset1 = 0;
    t_offset2 = 0;
    t_offset3 = 0;
    t_offset4 = 0;

	for(t_dlay = 0; t_dlay < (t_nuCalPeriod); t_dlay++)
    {
        /* Clear the ADC0 JEOC pending flag */
        //ADC0_SWT = 0x00005AA5;
		ADC_SoftTrgEN(ADC0, ENABLE);   //������� �ڲ���Ҫ����

        while(!(ADC0_IF & BIT0));

        ADC0_IF |= BIT1 | BIT0;
		ADC0_STATE_RESET();

        t_offset1 += (s16)((ADC0_DAT0));
        t_offset2 += (s16)((ADC0_DAT1));
        t_offset3 += (s16)((ADC0_DAT2));
        t_offset4 += (s16)(ADC0_DAT3);
    }

	ADC0_init();

#if (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
	tS_pHdToMd->nBusShuntOffset = (s16)(t_offset1/t_nuCalPeriod);//

#elif (EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
	tS_pHdToMd->nPhaseUOffset=(s16)(t_offset1/t_nuCalPeriod);
	tS_pHdToMd->nPhaseVOffset=(s16)(t_offset2/t_nuCalPeriod);
#elif((EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(EPWM0_CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
	tS_pHdToMd->nPhaseUOffset=(s16)(t_offset1/t_nuCalPeriod);
	tS_pHdToMd->nPhaseVOffset=(s16)(t_offset2/t_nuCalPeriod);
	tS_pHdToMd->nPhaseWOffset=(s16)(t_offset3/t_nuCalPeriod);
#endif		
}

#endif

/*******************************************************************************
 �������ƣ�    static void CurrentOffsetCalibrationPWM_ID(u8 t_bPwmId,STR_PubDatHDMD* tS_pHdToMd)
 ����������    ����PWM��ID�Ŷ�����Offsetֵ
 ���������    t_bPwmId  PWM��ID��
 			   STR_PubDatHDMD* tS_pHdToMd  �ش�ָ��
 ���������    �������ָ��ش�
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/11/1      V1.0           Tonghua Li          ����ʷ�汾����
 *******************************************************************************/
#if (DRV0_PWM_ID == PLANTFORM_EPWM0)
static void CurrentOffsetCalibrationPWM_ID(u8 t_bPwmId,STR_PubDatHDMD* tS_pHdToMd,u16 t_nuCalPeriod)
{
	CurrentOffsetReadPWM0(tS_pHdToMd,t_nuCalPeriod);	
}
#elif (DRV0_PWM_ID == PLANTFORM_EPWM1)
static void CurrentOffsetCalibrationPWM_ID(u8 t_bPwmId,STR_PubDatHDMD* tS_pHdToMd,u16 t_nuCalPeriod)
{
	CurrentOffsetReadPWM1(tS_pHdToMd,t_nuCalPeriod);	
}
#endif



static void CurrentOffsetCalibrationPWM(void)
{
	PSTR_ControlObject	t_pObj;
	u16 t_nuCalPeriod;
	t_pObj		= getCtrlObjectPtr(0);
	t_nuCalPeriod	= t_pObj->m_pDrvCfgPara->mS_GlobalCfg.m_nuADCCaliTimes;
	CurrentOffsetCalibrationPWM_ID(t_pObj->m_bPWM_Id,&t_pObj->mGlobalDatPackage.mHdToMd,t_nuCalPeriod);
}

/*******************************************************************************
 �������ƣ�    void CurrentOffsetCalibration(void)
 ����������    ���е���AD����У��
 ���������    ��
 ���������    �������ָ��ش�����Ӧ�����ݽṹ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/11/1      V1.0           Tonghua Li          ����ʷ�汾����
 *******************************************************************************/
void CurrentOffsetCalibration(void)
{
    volatile u32 t_dlay;

    //��ʱʱ��
    for(t_dlay = 0; t_dlay < 0x6ffff; t_dlay++);

	CurrentOffsetCalibrationPWM();
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
