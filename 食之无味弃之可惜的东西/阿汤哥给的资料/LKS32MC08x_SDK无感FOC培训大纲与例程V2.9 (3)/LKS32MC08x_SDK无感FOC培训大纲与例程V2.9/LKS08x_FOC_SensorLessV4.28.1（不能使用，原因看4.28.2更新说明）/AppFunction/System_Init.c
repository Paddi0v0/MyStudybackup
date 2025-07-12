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
#include "Global_Variable.h"
#include "hardware_config.h"
#include "function_config.h"
#include "fault_detection.h"
#include "MC_parameter.h"
#include "math.h"


void CurrentOffsetCalibration(void);
void FluxObserveInit(void);

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
    FaultInit();                              /* ���ϼ���ʼ�� */
    FaultRecoverInit();                       /* ���ϻָ���ʼ�� */

    CurrentOffsetCalibration();               /* ��ȡ��������ͨ��ƫ�� */

    FluxObserveInit();                        /* �۲���������ʼ�� */

		Bemf_CheckInit(&stru_BEMF_Check,BEMF_CHECK_ANGLE_COMP);/* ���綯�Ƽ���ʼ�� */

		
#if ((ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR) || (HALL2SENSORLESS_ANGLE_VERIFY == TRUE))
	  struFOC_CtrProc.bMotorDirtionCtrlPhase = CW;   /* �и�����ı����ߵķ�ʽ���ı���ת���޸е��Ƶ��˴���
	                                                     �и�ʱvoid StateInit(void)�ڵ�ע�͵��� */
    HALL_InitHallMeasure(&struHallProcess);   /* HALL��������ʼ�� */
#endif

    struTaskScheduler.sVersion = &sVersion[0];/* ��ʼ���汾�� */

#if (DEBUG_PWM_OUTPUT == TEST_ON)
    DebugPWM_OutputFunction(); /* ���Ե�ʱ�����25%��PWM���� */
#endif
}


/*******************************************************************************
 �������ƣ�    void CurrentOffsetCalibration(void)
 ����������    ������Offsetֵ
 ���������    stru_FOC_CtrProcDef *this  �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void CurrentOffsetCalibration(void)
{
    u16 CalibCnt = 0;
    stru_OffsetDef  struOffset;
    u32 t_delay;

    __disable_irq();

    ADC0_TRIG = 0;//�ر�Ӳ������

    ADC_SOFTWARE_TRIG_ONLY();//���������
    ADC0_CHNT0 = 4; //����4��ͨ��

    ADC_STATE_RESET()//ADC0 return to idle

    //��ʱ�����offset����
    for(t_delay = 0; t_delay < 0x7ffff; t_delay++);

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    ADC0_CHN0 = ADC_1SHUNT_CURR_CH | (ADC_1SHUNT_CURR_CH << 8);
    ADC0_CHN1 = ADC_BUS_VOL_CHANNEL | (M0_ADC_BUS_CURR_CH << 8);
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN1 = ADC_BUS_VOL_CHANNEL | (M0_ADC_BUS_CURR_CH << 8);
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)
    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN1 = ADC0_CURRETN_C_CHANNEL | (M0_ADC_BUS_CURR_CH << 8);
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET)
    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    ADC0_CHN1 = ADC0_CURRETN_C_CHANNEL | (M0_ADC_BUS_CURR_CH << 8);

    struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU = 0;
    struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV = 0;
    struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW = 0;
    MCPWM0_RegUpdate();
    PWMOutputs(ENABLE);


#endif
#endif
#endif
#endif
    struOffset.IPhAFilt    = 0;
    struOffset.IPhBFilt    = 0;
    struOffset.UBusFilt    = 0;
    struOffset.IBusFilt    = 0;

    for(CalibCnt = 0; CalibCnt < (1 << ADC_GET_OFFSET_SAMPLES); CalibCnt++)
    {
        /* Clear the ADC0 JEOC pending flag */
        ADC0_SWT = 0x00005AA5;

        while(!(ADC0_IF & BIT0));

        ADC0_IF |= BIT1 | BIT0;

        struOffset.IPhAFilt +=  (s16)((ADC0_DAT0));
        struOffset.IPhBFilt +=  (s16)((ADC0_DAT1));
        struOffset.IPhCFilt += (s16)((ADC0_DAT2));
        struOffset.IBusFilt += (s16)(ADC0_DAT3);
    }

    ADC0_init();

    ADC_NormalModeCFG();

    __enable_irq();

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    struFOC_CurrLoop.nBusShuntOffset =  (s16)(struOffset.IPhAFilt >> ADC_GET_OFFSET_SAMPLES);
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    struFOC_CurrLoop.nPhaseAOffset = (s16)(struOffset.IPhAFilt >> ADC_GET_OFFSET_SAMPLES);
    struFOC_CurrLoop.nPhaseBOffset = (s16)(struOffset.IPhBFilt >> ADC_GET_OFFSET_SAMPLES);
    if((ABS(struFOC_CurrLoop.nPhaseAOffset) > ADC_OFFSET_ERROR) ||
            (ABS(struFOC_CurrLoop.nPhaseBOffset) > ADC_OFFSET_ERROR))
    {
        stru_Faults.B.OffsetError = 1;
    }

#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
    struFOC_CurrLoop.nPhaseAOffset = (s16)(struOffset.IPhAFilt >> ADC_GET_OFFSET_SAMPLES);
    struFOC_CurrLoop.nPhaseBOffset = (s16)(struOffset.IPhBFilt >> ADC_GET_OFFSET_SAMPLES);
    struFOC_CurrLoop.nPhaseCOffset = (s16)(struOffset.IPhCFilt >> ADC_GET_OFFSET_SAMPLES);

    if((ABS(struFOC_CurrLoop.nPhaseAOffset) > ADC_OFFSET_ERROR) ||
            (ABS(struFOC_CurrLoop.nPhaseBOffset) > ADC_OFFSET_ERROR) ||
            (ABS(struFOC_CurrLoop.nPhaseCOffset) > ADC_OFFSET_ERROR))
    {
        stru_Faults.B.OffsetError = 1;
    }
#endif
#endif
#endif



}

/*******************************************************************************
 �������ƣ�    void FluxObserveInit(void)
 ����������    �۲���������ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void FluxObserveInit(void)
{
    struFluxOB_Param.nSTO_KP = PLL_KP_GAIN;              /* �۲���PLL KP ���� */
    struFluxOB_Param.nSTO_KI = PLL_KI_GAIN;              /* �۲���PLL KI ���� */

    struFluxOB_Param.nCurrLoopIniKP = P_CURRENT_KP;      /* ��������ʼKP */
    struFluxOB_Param.nCurrLoopIniKI = P_CURRENT_KI;      /* ��������ʼKI */

    struFluxOB_Param.pStruMotorParame = &struMotorParam; /* ��������ṹ�� */
    struFluxOB_Param.pStruBoardParame = &struBoardParam; /* ���ʰ�Ӳ����·���Բ��� */

    FluxObserveParaCalc();                               /* �۲����������� */

    FOC_InitstruParam();                                 /* FOC ��ؿ��Ʊ������ṹ���ʼ�� */
}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
