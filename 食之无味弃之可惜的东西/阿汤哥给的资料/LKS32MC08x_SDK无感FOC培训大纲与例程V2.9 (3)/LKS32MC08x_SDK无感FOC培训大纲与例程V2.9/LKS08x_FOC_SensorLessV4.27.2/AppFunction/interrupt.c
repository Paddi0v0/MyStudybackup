/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� interrupt.c
 * �ļ���ʶ��
 * ����ժҪ�� �жϷ�������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��8��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2020��8��5��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet
 * �޸����ݣ�����
 *
 *******************************************************************************/

#include "hardware_config.h"
#include "global_variable.h"
#include "fault_detection.h"
#include "PowerCalculation.h"
#include "SEGGER_RTT.h"

#if (RTT_FUNCTION == FUNCTION_ON)
struct {
    s16 data0;
    s16 data1;
    s16 data2;
//    s16 data3;
} Rttstru;
#endif

#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT) || (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)\
     ||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
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

    struFOC_CtrProc.nSys_TimerPWM++;          /* PWMʱ�������� */

    struTaskScheduler.bPWM_UpdateFlg = 1;     /* PWM���±�־ */ 

    struFOC_CurrLoop.nBusVoltage = GET_BUS_VOL_ADC_RESULT; /* ĸ�ߵ�ѹ���� */

	  struAppCommData.nVspADC = (s16)ADC0_DAT5; /*��λ���źŲ������*/
	
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)   
    Hall_ElecAnglePWM_Process(&struHallProcess); /* HALL�Ƕȴ��� */
#endif
  
    FOC_Model(&struFOC_CtrProc);              /* FOC�����ڻ� */

    if(struFOC_CtrProc.eSysState == RUN)
    {
        FaultCurrentCheck(&struFOC_CurrLoop.mStatCurrUVW);  //�������
        PowerCalc(&struFOC_CurrLoop.mStatCurrDQ, &struFOC_CurrLoop.mStatVoltDQ, &struPower);
        struCurrentAmplitude.bTheta = (struFluxOB_Param.wElectAngle >> 16);
        CurrentAmplitudeCalc(&struFOC_CurrLoop.mStatCurrUVW, &struCurrentAmplitude, &struImaxCurrent);
    }   
    
	  if((MCPWM_EIF & BIT4) || (MCPWM_EIF & BIT5))//MCPWM_FALL�¼� Ӳ�������ж�
		{
       MCPWM_EIF = BIT4|BIT5;
       stru_Faults.B.HardCurretError = 1;// Ӳ��������־λ��1
		}
		
    #if(RTT_FUNCTION == FUNCTION_ON)
    {
        Rttstru.data0 = (struFluxOB_Param.wElectAngleOpen >> 16);   //�����Ƕ�   
        Rttstru.data1 = (struFluxOB_Param.wElectAngleEst >> 16);    //�۲����Ƕ�
//			  Rttstru.data0 = (struFOC_CurrLoop.mStatCurrUVW.nPhaseU);    //U������Ƕ�
//			  Rttstru.data1 = (struFOC_CurrLoop.mStatCurrUVW.nPhaseV);    //U������Ƕ�
        Rttstru.data2 = (struFOC_CurrLoop.mStatCurrUVW.nPhaseW);    //U������Ƕ�
        
        SEGGER_RTT_Write(1, &Rttstru, 6);
    }
    #endif

}
#endif

#if(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
/*******************************************************************************
 �������ƣ�    void MCPWM_IRQHandler(void)
 ����������    MCPWM�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void MCPWM_IRQHandler(void)
{

    MCPWM_IF = BIT1 | BIT0;
  
    struFOC_CtrProc.nSys_TimerPWM++;          /* PWMʱ�������� */

    struTaskScheduler.bPWM_UpdateFlg = 1;     /* PWM���±�־ */ 

    struFOC_CurrLoop.nBusVoltage = GET_BUS_VOL_ADC_RESULT; /* ĸ�ߵ�ѹ���� */

	  struAppCommData.nVspADC = (s16)ADC0_DAT5; /*��λ���źŲ������*/
	
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)   
    Hall_ElecAnglePWM_Process(&struHallProcess); /* HALL�Ƕȴ��� */
#endif
	
    FOC_Model(&struFOC_CtrProc);              /* FOC�����ڻ� */


    if(struFOC_CtrProc.eSysState == RUN)
    {
        FaultCurrentCheck(&struFOC_CurrLoop.mStatCurrUVW);  //�������
        PowerCalc(&struFOC_CurrLoop.mStatCurrDQ, &struFOC_CurrLoop.mStatVoltDQ, &struPower);
        struCurrentAmplitude.bTheta = (struFluxOB_Param.wElectAngle >> 16);
        CurrentAmplitudeCalc(&struFOC_CurrLoop.mStatCurrUVW, &struCurrentAmplitude, &struImaxCurrent);
    }
	  if((MCPWM_EIF & BIT4) || (MCPWM_EIF & BIT5))//MCPWM_FALL�¼� Ӳ�������ж�
		{		   
       MCPWM_EIF = BIT4|BIT5;
       stru_Faults.B.HardCurretError = 1;// Ӳ��������־λ��1
		}
    
    #if(RTT_FUNCTION == FUNCTION_ON)
    {
        Rttstru.data0 = (struFluxOB_Param.wElectAngleOpen >> 16);   //�����Ƕ�   
        Rttstru.data1 = (struFluxOB_Param.wElectAngleEst >> 16);    //�۲����Ƕ�
        Rttstru.data2 = (struFOC_CurrLoop.mStatCurrUVW.nPhaseU);    //U������Ƕ�
        
        SEGGER_RTT_Write(1, &Rttstru, 6);
    }
    #endif
}
#endif

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
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)    
    HALL_IRQProcess(HALL, &struHallProcess);
#endif  

}


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
{
    /* ʱ��500us */
    UTIMER_IF |= TIMER_IF_ZERO;

    struTaskScheduler.bTimeCnt1ms++;
    struTaskScheduler.nTimeCnt10ms ++;
    struTaskScheduler.nTimeCnt500ms++;
}

/*******************************************************************************
 �������ƣ�    void TIMER1_IRQHandler(void)
 ����������    TIMER1�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UTIMER1_IRQHandler(void)
{

}

/*******************************************************************************
 �������ƣ�    void TIMER2_IRQHandler(void)
 ����������    TIMER2�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UTIMER2_IRQHandler(void)
{

}

/*******************************************************************************
 �������ƣ�    void TIMER3_IRQHandler(void)
 ����������    TIMER3�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UTIMER3_IRQHandler(void)
{

}
/*******************************************************************************
 �������ƣ�    void ENCODER0_IRQHandler(void)
 ����������    ENCODER1�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void ENCODER0_IRQHandler(void)
{

}

/*******************************************************************************
 �������ƣ�    void ENCODER0_IRQHandler(void)
 ����������    ENCODER1�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void ENCODER1_IRQHandler(void)
{

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
    CMP_IF = BIT0 | BIT1;

    if(CMP_IF & (BIT1))
    {
        volatile u8 t_bi, t_bcnt;

        t_bcnt = 0;
        for(t_bi = 0; t_bi < 5; t_bi++)
        {
            if(SYS_AFE_CMP & BIT15) /* BIT14 CMP0 OUT Flag| BIT15 CMP1 OUT Flag */
            {   
                t_bcnt ++;
            }
        }

        if(t_bcnt > 3)
        {
            PWMOutputs(DISABLE);
            MCPWM_EIF = BIT4|BIT5;
            stru_Faults.B.HardCurretError = 1;// Ӳ��������־λ��1
        }
    }
}



/*******************************************************************************
 �������ƣ�    void UART0_IRQHandler(void)
 ����������    UART0�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UART0_IRQHandler(void)
{
}

/*******************************************************************************
 �������ƣ�    void UART1_IRQHandler(void)
 ����������    UART1�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void UART1_IRQHandler(void)
{
  

}

/*******************************************************************************
 �������ƣ�    void SysTick_Handler(void)
 ����������    ϵͳ�δ�ʱ�ж�
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SysTick_Handler(void)
{

}

/*******************************************************************************
 �������ƣ�    void SleepTimer_IRQHandler(void)
 ����������    ���߻����жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SleepTimer_IRQHandler(void)
{
    while(1);
}

/*******************************************************************************
 �������ƣ�    void GPIO_IRQHandler(void)
 ����������    GPIO�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void GPIO_IRQHandler(void)
{

}

/*******************************************************************************
 �������ƣ�    void I2C0_IRQHandler(void)
 ����������    I2C0�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void I2C0_IRQHandler(void)
{

}

/*******************************************************************************
 �������ƣ�    void SPI0_IRQHandler(void)
 ����������    SPI0�жϴ�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SPI0_IRQHandler(void)
{

}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */


