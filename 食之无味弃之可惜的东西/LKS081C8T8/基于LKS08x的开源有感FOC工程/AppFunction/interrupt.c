#include "Global_Variable.h"
#include "hardware_config.h"
#include "MC_hall.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    void TIMER0_IRQHandler(void)
 ����������    TIMER0�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void UTIMER0_IRQHandler(void)
{	/* ʱ��500us */
    UTIMER_IF = 0x07;

    if (struFOC_CtrProc.nSetTimeLeftCnt)
    {
        struFOC_CtrProc.nSetTimeLeftCnt--;
    }

    struTaskScheduler.nTimeBaseFlg = 1;
}

/*******************************************************************************
 �������ƣ�    void HALL_IRQHandler(void)
 ����������    HALL�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void HALL_IRQHandler(void)
{
	HALL_IRQProcess(&struHallProcess);
}

/*******************************************************************************
 �������ƣ�    void ADC0_IRQHandler(void)
 ����������    ADC0�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void ADC0_IRQHandler(void)
{
    ADC0_IF |= BIT1 | BIT0;

    struFOC_CtrProc.nSys_TimerPWM++;

    struHallProcess.wHallPWMTimerCnt++;

    struTaskScheduler.bPWM_UpdateFlg = 1; 									/* PWM���±�־ */

    struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentadc = ((s16)(GET_BUS_ADC_RESULT) - struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentOffset);  	/* ĸ�ߵ��� */
    struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentadcfir = lowPass_filter(&struFOC_CtrProc.struBusCurr_RC, struFOC_CtrProc.struFOC_CurrLoop.nBusCurrentadc);

    struFOC_CtrProc.struFOC_CurrLoop.nBusVoltage = GET_BUS_VOL_ADC_RESULT; 	/* ĸ�ߵ�ѹ */

    strucontrol_handle.hand_speed_adc = GET_SPEED_ADC_RESULT; 				/* ת�ѵ�ѹ */
	
	Hall_ElecAnglePWM_Process(&struHallProcess); 							/* HALL�Ƕȴ��� */

    if (struFOC_CtrProc.eSysState == RUN)
    {
		FOC_Model(&struFOC_CtrProc);
    }
    else
    {  
        ADC0_CHN2 = ADC0_3th_MSK | ADC1_3th_MSK;
        ADC0_CHN3 = ADC0_4th_MSK | ADC1_4th_MSK;
		
		if (struHallProcess.bCloseLoopAngleFlg)
		{
			closeLoopAnglePLL(&struHallProcess);
		}
		else
		{
			reduceToqueAnglePll(&struHallProcess);
		}
    }
}

/*******************************************************************************
 �������ƣ�    void CMP_IRQHandler(void)
 ����������    �Ƚ����жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void CMP_IRQHandler(void)
{
    if(CMP_IF & (BIT0 | BIT1))
    {
        volatile u8 t_i, t_cnt;

        CMP_IF = BIT0 | BIT1;
        t_cnt = 0;
        for(t_i = 0; t_i < 5; t_i++)
        {
#if (CMP_CHOICE == 0)
			if(SYS_AFE_CMP & BIT14)
            {
                t_cnt ++;
            }
#else
#if (CMP_CHOICE == 1) 
			if(SYS_AFE_CMP & BIT15)
            {
                t_cnt ++;
            }
#endif   
#endif
        }

        if(t_cnt > 3)
        {
            StopMotorImmdly();
            stru_Faults.B.SoftCurretError = 1;
        }
		
#if (CMP_CHOICE == 0)
		MCPWM_EIF = BIT4;
#else
#if (CMP_CHOICE == 1) 
		MCPWM_EIF = BIT5;
#endif  
#endif
    }
}

/*******************************************************************************
 �������ƣ�    u8 SetTime_IsElapsed(void)
 ����������    �жϵȴ�ʱ���Ƿ�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

u8 SetTime_IsElapsed(void)
{
    if (struFOC_CtrProc.nSetTimeLeftCnt == 0)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*******************************************************************************
 �������ƣ�    void HardFault_Handler(void)
 ����������    HardFault_Handler�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void HardFault_Handler(void)
{
    MCPWM_PRT       = 0x0000DEAD;
    MCPWM_FAIL &= MCPWM_MOE_DISABLE_MASK;

    NVIC_SystemReset();
}

/*******************************************************************************
 �������ƣ�    void SetTimeOut_Counter(u16 hTimeout)
 ����������    ���õȴ������ȴ�ʱ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void SetTime_TimeOut_Counter(u16 hTimeout)
{
    struFOC_CtrProc.nSetTimeLeftCnt = hTimeout;
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/

