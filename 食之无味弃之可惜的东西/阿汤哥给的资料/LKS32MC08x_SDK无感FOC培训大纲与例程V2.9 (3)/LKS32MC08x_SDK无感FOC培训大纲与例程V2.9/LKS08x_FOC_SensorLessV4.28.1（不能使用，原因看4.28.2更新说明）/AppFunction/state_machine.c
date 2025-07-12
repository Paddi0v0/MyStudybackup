/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� state_machine.c
 * �ļ���ʶ��
 * ����ժҪ�� state machine
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2019��11��19��
 *
 *******************************************************************************/
#include "state_machine.h"
#include "MC_parameter.h"
#include "FOC_Drive.h"
#include "Global_Variable.h"
#include "hardware_init.h"
#include "fault_detection.h"
#include "KeyScan.h"
#include "PmsmFluxObserve.h"
#include "Time_Process.h"
#include "PowerCalculation.h"

stru_Time_t         struTime;

void CurrentLoopAxisD_Set(void);
void CurrentLoopAxisQ_Set(void);
void StateHallRun(void);

void SpeedLoop_Set(MechanicalQuantity *this);

s16 OpenLoopCurRamp(stru_OpenForceRunDef *this);
void SpeedReferenceGen(stru_OpenForceRunDef *this);

/*******************************************************************************
 �������ƣ�    void Sys_State_Machine(stru_FOC_CtrProcDef *this)
 ����������    ϵͳ״̬�� ���״̬��״̬�л�����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void Sys_State_Machine(void)
{
    switch (struFOC_CtrProc.eSysState)
    {
    case IDLE:
    {
        /* ����״̬ */

        if (struFOC_CtrProc.bMC_RunFlg)
        {
            StateInit();

#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS)
            struFOC_CtrProc.eSysState = CHARGE; /* ������״̬ */
#else
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)
            if (GET_HALL_LEARN_STATE())
            {
                /* ����HALL��ѧϰ״̬ */

                PWMOutputs(ENABLE);
                struFOC_CtrProc.eSysState = HALL_LEARN;
            }
            else
            {
                StateInit();
                PWMOutputs(ENABLE);
                struFOC_CtrProc.eSysState = HALL_RUN;   //��Hall����
            }
#endif
#endif
        }
        break;
    }
    case CHARGE:
    {
        StateCharge();                         //Ԥ��紦����
        break;
    }
 case BEMF_CHECK:                          /*���綯�Ƽ��*/
    {
        StateBemfDirCheck();                  //�����Ƽ�⺯��
        break;
    }		
    case DIR_CHECK:                           //˳�����״̬
    {
#if(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
        struFOC_CtrProc.eSysState = POS_SEEK;
#else
        StateDirCheck();                      //˳��紦����
#endif
        break;
    }
    case INIT:
    {
        /* ��ʼ��״̬ */

        StateInit();

        PWMOutputs(ENABLE);
        struFOC_CtrProc.eSysState = ALIGN;

        break;
    }
    case POS_SEEK:                            //��ʼλ�ü��״̬
    {
        StatePosSeek();                       //��ʼλ�ü�⴦����

        struFOC_CtrProc.eSysState = INIT;

        break ;
    }
    case ALIGN:                                //Ԥ��λ״̬
    {
        StateAlign();                          //Ԥ��λ������

        break ;
    }
    case OPEN_RUN:                                //����ǿ��״̬
    {
        StateOpen();                              //����������
        break ;
    }
    case RUN:
    {
        /* ����״̬  */

        StateRun();                               //�ջ�������
        break;
    }
    case HALL_RUN:
    {
        StateHallRun();
        break;
    }

    case HALL_LEARN:
    {
        if (GET_HALL_LEARN_STATE() == HALL_LEARN_FINISH)
        {
            PWMOutputs(DISABLE);

            SetTimeOut_Counter(struFOC_CtrProc.nSetTimeLeftCnt, STATE_MACHINE_200MS);
            struFOC_CtrProc.eSysState = WAIT;
        }
        break;
    }
    case BRAKE:
    {
        /* ���ɲ��״̬ */
        StateStop();                          //ֹͣ�ж�������
        SetTimeOut_Counter(struFOC_CtrProc.nSetTimeLeftCnt, STATE_MACHINE_200MS);
        break;
    }
		case BEMF_BRAKE:  /* ������ɲ������*/
    {
        StateBemfStop();
        break;
    }
    case WAIT:
    {
        /* �ȴ�״̬ */
        if (SetTime_IsElapsed(struFOC_CtrProc.nSetTimeLeftCnt))
        {
            struFOC_CtrProc.eSysState = IDLE;
        }

        break;
    }
    case FAULT:
    {
        /* ����״̬ */
        StateFault();                         //���ϴ�����

        if (stru_Faults.R == 0)
        {
            struFOC_CtrProc.eSysState = IDLE;
        }

        break;
    }
    default:
    {
        break;
    }
    }

    if ((struFOC_CtrProc.bMC_RunFlg == 0) && (stru_Faults.R == 0))  //�޹��ϲ���������־λ��0���ر�������������״̬
    {
        PWMOutputs(DISABLE);
        SetTimeOut_Counter(struFOC_CtrProc.nSetTimeLeftCnt, STATE_MACHINE_200MS);
        struFOC_CtrProc.eSysState = WAIT;
    }
}


/*****************************************************************************
 * ������   : void StateInit()
 * ˵��     : ״̬��ʼ��
 * ���˼· ��1.������ʼ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateInit(void)
{
    s16 t_nTimer;

    PWMOutputs(DISABLE);

    //˳�����
    struAppCommData.nDirTrackFreq = User2AppFreqTrans(SPEED_TRACK_ON_FREQ_THH); //˳���бջ��ٶȳ�ʼ��
    struAppCommData.nDirEbreakFreq = User2AppFreqTrans(EBRK_ON_FREQ_THH);       //ɲ���ٶȳ�ʼ��

    mOnTheFlyDetect.bMotorStopDecFlag = 0;
    mOnTheFlyDetect.nMotorStopCurrentThd = MOTOR_STOP_CUR_THD;    /*���ֹͣ��������ֵ */
    mOnTheFlyDetect.nMotorStopCurDifThd = MOTOR_STOP_CUR_DIF_THD; /*���ֹͣ��������ֵ */
    mOnTheFlyDetect.nFreqAvg = 0;
    mOnTheFlyDetect.nFreq = 0;

    mOnTheFlyDetect.nElectAngle = 0;
    mOnTheFlyDetect.wBemfFreq = 0;
    mOnTheFlyDetect.wAngleDpp = 0;
    mOnTheFlyDetect.wBEMF_DppValueAcc = 0;
    mOnTheFlyDetect.nMotorVolMag = 0;
    mOnTheFlyDetect.wVolMagAvg = 0;

    OnTheFlyDetectInit();  //˳���������ʼ��

    /* �����Ƽ�����*/
    struAppCommData.nBemfTrackVolt = BEMF_TRACK_VOLTAGE * struUser2App.vol;   //0.01V
    struAppCommData.wBemfTrackFreq = User2AppFreqTrans(BEMF_TRACK_FREQ);
    struAppCommData.wDirTrackFreq  = User2AppFreqTrans(BEMF_TRACK_ON_FREQ_THH);  //������ڴ�Ƶ�� ����ջ�Ƶ��

    struFOC_CtrProc.bMotorDirtionCtrl = CW;   /* ���ת�� */
#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS) 
    struFOC_CtrProc.bMotorDirtionCtrlPhase = CCW;   /* ����ı����ߵķ�ʽ���ı���ת�� */
#endif
    //Ԥ���
    struAppCommData.bChargeFlag = 0;    //Ԥ��翪ʼ��־����
    struAppCommData.bChargeEndFlag = 0; //Ԥ�����ɱ�־����

//��ʼλ�ü��
    struAppCommData.nStartAngleComp = User2AppAngleTrans(U_START_ANGLE_COMP);  //��ʼλ�ü�ⲹ���Ƕȳ�ʼ��

    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC)); //�������ٵ���ֵ
    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC)); // �������ٵ���ֵ


    /*****����������ʼ��********/
    struFOC_CurrLoop.mStatCurrUVW.nPhaseU = PWM_PERIOD / 2;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseV = PWM_PERIOD / 2;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseW = PWM_PERIOD / 2;
    struFOC_CurrLoop.MCPWMx_RegUpdate();                    /* ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ */

    struFOC_CurrLoop.nDCur_Reference = 0;
    struFOC_CurrLoop.nQCur_Reference = 0;

    struFOC_CurrLoop.mStatCurrDQ.nAxisD = 0;
    struFOC_CurrLoop.mStatCurrDQ.nAxisQ = 0;

    struFOC_CurrLoop.mStatCurrAlfaBeta.nAlph = 0;
    struFOC_CurrLoop.mStatCurrAlfaBeta.nBeta = 0;

    struFOC_CurrLoop.nDCurrentSet = 0;
    struFOC_CurrLoop.nQCurrentSet = 0;

    struFOC_CurrLoop.mStatCurrUVW.nPhaseU = 0;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseV = 0;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseW = 0;

    struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU = 0;
    struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV = 0;
    struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW = 0;

    /*****DQ�������������ʼ��********/

    struFOC_CurrLoop.mPI_Flux.wIntegral = 0;
    struFOC_CurrLoop.mPI_Flux.wLastError = 0;

    struFOC_CurrLoop.mPI_Torque.wIntegral = 0;
    struFOC_CurrLoop.mPI_Torque.wLastError = 0;

    /*****��ѹ������ʼ��********/

    struFOC_CurrLoop.mStatVoltAlfaBeta.nAlph = 0;
    struFOC_CurrLoop.mStatVoltAlfaBeta.nBeta = 0;

    struFOC_CurrLoop.mStatVoltDQ.nAxisD = 0;
    struFOC_CurrLoop.mStatVoltDQ.nAxisQ = 0;

    struFOC_CurrLoop.mVdtComp.nPhaseU = 0;
    struFOC_CurrLoop.mVdtComp.nPhaseV = 0;
    struFOC_CurrLoop.mVdtComp.nPhaseW = 0;


    /*****��λ������ʼ��********/
    struAppCommData.wStartCurSet1 = User2AppCurTrans(U_START_CUR_SET_F); //��һ�ζ�λ����
    struAppCommData.wStartCurSet2 = User2AppCurTrans(U_START_CUR_SET_S); //�ڶ��ζ�λ����

    /*****�������в�����ʼ��********/

    mOpenForceRun.nFreqAccStep = 0;
    mOpenForceRun.nFreqDecStep = 0;
    mOpenForceRun.wRampFreqRef = 0;
    mOpenForceRun.nStartCurSet = 0;
    mOpenForceRun.wStartCurRamp = 0;
    mOpenForceRun.nStartCurRef = 0;
    struAppCommData.wSvcMinFreq = User2AppFreqTrans(OPEN_ANGLE_TAG_FREQ);  //�����϶�Ƶ��
    struAppCommData.bOpenRunFlag = 0;

    struAppCommData.nLoopCntr = 0;
    /*****�ٶȻ�������ʼ��********/
#if (CLOSE_LOOP == SPEED_LOOP)
    {
        struMotorSpeed.mSpeedPI.KP = P_ASR_KP;                                                 //�ٶȻ� Kp
        struMotorSpeed.mSpeedPI.KI = P_ASR_KI;                                                 //�ٶȻ� Ki
        struMotorSpeed.mSpeedPI.wIntegral = 0;
        struMotorSpeed.mSpeedPI.wUpperLimitOutput = App2CoreCurTrans(User2AppCurTrans(IQMAX)); //�ٶȻ�������ֵ
        struMotorSpeed.mSpeedPI.wLowerLimitOutput = App2CoreCurTrans(User2AppCurTrans(IQMIN));//�ٶȻ������Сֵ
        struMotorSpeed. mSpeedPI.wInError = 0;

        struMotorSpeed.wSpeedRef = 0;
        struMotorSpeed.wSpeedSet = 0;

        struMotorSpeed.wSpeedRampACCStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_ACC));
        struMotorSpeed.wSpeedRampDECStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_DEC));

        struMotorSpeed.wSpeedfbk = 0;

        struMotorSpeed.wPowerLimitSpeedSet = 0;
        struMotorSpeed.wPowerLimitSpeedRef = App2CoreFreqTrans(User2AppFreqTrans(POWER_LIMIT_SPEED));
        struMotorSpeed.wPowerLimitValue =  POWER_CALC(POWER_LIMIT_VALUE);
    }
    /*****���ʻ�������ʼ��********/
#elif (CLOSE_LOOP == POWER_LOOP)
    {
        PowerLoopInit();
    }
#endif

#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS)
    /*****���㻷������ʼ��********/
//    FluxObserveInit();
    PmsmFluxObIni();             //���㻷������ʼ������
    struFluxOB_Param.nObserveMinSpeed = App2CoreFreqTrans(User2AppFreqTrans(OPEN_ANGLE_TAG_FREQ));
#endif

    /* ���Ų�����ʼ�� */
    AFWR.KI = 10;
    AFWR.KP = 50;
    AFWR.INTEG = 0;
    AFWR.LIM_H = 0;
    AFWR.LIM_L = -struFOC_CurrLoop.nWeakFieldLim;
    struFOC_CurrLoop.npmsmSvcImFw = 0;


    /*****��·����ʱ�������ʼ��********/
    struTime.nChargeTime = 0;
    struTime.nAlignTime = 0;
    struTime.nDirCheckTime = 0;
    struTime.nLoopDelyTime = 0;
    struTime.nStopDelayTime = 0;
    struTime.nStopDelayCntr = 0;
    struTime.nOpenRunCntr = 0;

    /************************������ϸ��*****************************/
//   	struFOC_CurrLoop.mPI_Flux.KP = 8000;   //ID��·(����) KP
//	  struFOC_CurrLoop.mPI_Flux.KI = 4000;   //ID��·(����) KI
//	
//	  struFOC_CurrLoop.mPI_Torque.KP = 8000; //IQ��·(ת��) KP
//	  struFOC_CurrLoop.mPI_Torque.KI = 4000; //IQ��·(ת��) KI

    /*****�����������ʼ��********/
    FaultInit();

    t_nTimer = struFOC_CtrProc.nSys_TimerPWM;

    ADC0_init();
    while (ABS(struFOC_CtrProc.nSys_TimerPWM - t_nTimer) < 2)
    {
    }

}


/*****************************************************************************
 * ������   : void StateCharge(void)
 * ˵��     : Ԥ��纯�������Ծٵ��ݽ���Ԥ��磬���ڲ�ͬ��Ӳ��Ҫע�����Ԥ���ʱ��
 * ���˼· �����δ�A�ࡢB�ࡢC�����ţ����Ծٵ��ݳ�硣
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateCharge(void)
{
    if (struAppCommData.bChargeFlag == 0)
    {
        struAppCommData.bChargeFlag = 1;

        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU = (u16)(PWM_PERIOD * 0.3); //Ԥ���ռ�ձ�
        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV = (u16)(PWM_PERIOD * 0.3); //Ԥ���ռ�ձ�
        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW = (u16)(PWM_PERIOD * 0.3); //Ԥ���ռ�ձ�

        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

        struFOC_CurrLoop.MCPWMx_RegUpdate();          /* ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ */

        struTime.nChargeTime  = 0;
    }
    else
    {
        if (struTime.nChargeTime == 0)  //��A������PWM
        {
            MCPWM_PRT = 0x0000DEAD;

            MCPWM_IO01 = 0x0C08 | mIPD_CtrProc.hDriverPolarity;
            MCPWM_IO23 = 0x000C | mIPD_CtrProc.hDriverPolarity;

            MCPWM_PRT = 0x0000;

            PWMOutputs(ENABLE);    //ʹ�����
            __nop();
        }
        else if (struTime.nChargeTime == CHARGE_TIME)  //��B������PWM
        {
            MCPWM_PRT = 0x0000DEAD;
            MCPWM_IO01 = 0x080C | mIPD_CtrProc.hDriverPolarity;
            MCPWM_PRT = 0x0000;
            __nop();
        }
        else if (struTime.nChargeTime == CHARGE_TIME * 2) //��C������PWM
        {
            MCPWM_PRT = 0x0000DEAD;
            MCPWM_IO23 = 0x08 | mIPD_CtrProc.hDriverPolarity;
            MCPWM_PRT = 0x0000;
            __nop();
        }

        if (struTime.nChargeTime < (CHARGE_TIME * 3))  //CHARGE_TIMEΪÿ��Ԥ���ʱ�䣬�ɸ���Ӳ��ʵ������
        {
            struTime.nChargeTime ++ ;
        }
        else
        {
            PWMOutputs(DISABLE);
            __disable_irq();

            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU = 0;
            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV = 0;    /* ����MCPWM�Ƚ�ֵ�Ĵ��� */
            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW = 0;

            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

            struFOC_CurrLoop.MCPWMx_RegUpdate();          /* ����MCPWMģ��ռ�ձ�ֵ������MCPWMģ��ADC������Ĵ���ֵ */
            MCPWM_UPDATE = 0xFF;

            MCPWM_init();
            ADC_STATE_RESET();
            __enable_irq();
            struAppCommData.bChargeEndFlag = 1;
        }
    }

    if (struAppCommData.bChargeEndFlag == 1) //Ԥ������
    {
        struAppCommData.bChargeFlag = 0;
        struTime.nChargeTime = 0;
        struAppCommData.bChargeEndFlag = 0;

        StateInit();

#if (DIR_BEMF_CHECK_STATUS == TRUE)
#if DIR_BEMF_CHECK_MODE
        //�������˳�����Ҫ����PWM
        PWMOutputs(ENABLE);
        struFOC_CtrProc.eSysState = DIR_CHECK;
#else
        //���綯�Ƽ���·��Ҫ�ܹر�PWM
        PWMOutputs(DISABLE);
        struFOC_CtrProc.eSysState = BEMF_CHECK;
#endif
#else
#if (SEEK_POSITION_STATUS == TRUE)  //�����ʼλ�ü��״̬ʹ�ܣ�������ʼλ�ü��״̬
        {
            struFOC_CtrProc.eSysState = POS_SEEK;
            PWMOutputs(DISABLE);
        }
#elif (SEEK_POSITION_STATUS == FALSE) //�����ʼλ�ü��״̬��ʹ�ܣ������Ԥ��λ״̬������Ԥ��λ�Ƕ�
        {
            struFOC_CtrProc.eSysState = INIT;
            struFluxOB_Param.wElectAngleOpen = ((u32)(User2AppAngleTrans(ALIGN_ANGLE) * (u32)struApp2Core.angle >> (struApp2Core.angleShftNum))) << 16;
        }
#endif
#endif

    }
}
/*****************************************************************************
 * ������   : void StateBEMFDirCheck(void)
 * ˵��     : ˳��������ͨ�������Ƽ���ٶ�ֵ
 * ���˼· ��˳�����������ջ�����;����״̬�����stop״̬���������ȫֹͣ��ֹ����;\
              ����������һֱ��⣨Ҳ���Ը���ʵ��Ӧ�ý���ɲ������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2021.05.16
 *****************************************************************************/
void StateBemfDirCheck(void)
{
#if 1
    if(++struTime.nBemfheckTime >= BEMF_CHECK_TIME)
    {
        struTime.nBemfheckTime = 0;
        switch(struFOC_CtrProc.bMotorDirtionCtrl)
        {
        case CW://˳ʱ��
        {
            if(stru_BEMF_Check.nBemfUVWAmpValue > struAppCommData.nBemfTrackVolt)     //��λ��0.01V
            {
                if(stru_BEMF_Check.nBemf_FreqAve < (-struAppCommData.wBemfTrackFreq))     /*������ת��̫�ߣ���ȴ����ٺ��ٴ����������ɹ�������ѹ*/
                {
                    struFOC_CtrProc.eSysState = BEMF_BRAKE;
                }
                else if(stru_BEMF_Check.nBemf_FreqAve < struAppCommData.wDirTrackFreq)    //����
                {
                    struFOC_CtrProc.eSysState = BEMF_BRAKE;

                }
                else//˳ʱ�� ���� ����ջ�����
                {
                    struFluxOB_Param.nSTO_KP = PLL_KP_GAIN;              /* ���й۲���PLL KP ���� */
                    struFluxOB_Param.nSTO_KI = PLL_KI_GAIN;              /* ���й۲���PLL KI ���� */

                    struTime.nLoopDelyTime = STATE04_WAITE_TIME + 1;

                    struTime.nLoopDelyTime = 0;

                    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC));
                    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC));

                    struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));      //Q���������
                    struFOC_CurrLoop.nQCur_Reference = struFOC_CurrLoop.nQCurrentSet;

                    struMotorSpeed.wSpeedfbk = App2CoreFreqTrans(stru_BEMF_Check.nBemf_FreqAve);
                    struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;

                    struAppCommData.bOpenRunFlag = 2;
                    PWMOutputs(ENABLE);
                    struFOC_CtrProc.eSysState = RUN;

                }
            }
            else
            {
                struFOC_CtrProc.eSysState = BEMF_BRAKE;
            }

            break;
        }
        case CCW://��ʱ��
        {
            if(stru_BEMF_Check.nBemfUVWAmpValue > struAppCommData.nBemfTrackVolt)//struAppCommData.nBemfTrackVolt��λ0.01V
            {
                if(stru_BEMF_Check.nBemf_FreqAve > (struAppCommData.wBemfTrackFreq))  /* ������ת��̫�ߣ���ȴ����ٺ��ٴ����������ɹ�������ѹ*/
                {
                    struFOC_CtrProc.eSysState = BEMF_BRAKE;
                }
                else if(stru_BEMF_Check.nBemf_FreqAve > (-struAppCommData.wDirTrackFreq))//����
                {
                    struFOC_CtrProc.eSysState = BEMF_BRAKE;

                }
                else//��ʱ�� ���� ����ջ�����
                {
                    struFluxOB_Param.nSTO_KP = PLL_KP_GAIN;              /* ���й۲���PLL KP ���� */
                    struFluxOB_Param.nSTO_KI = PLL_KI_GAIN;              /* ���й۲���PLL KI ���� */

                    struTime.nLoopDelyTime = STATE04_WAITE_TIME + 1;

                    struTime.nLoopDelyTime = 0;

                    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC));
                    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC));

                    struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));    //Q���������
                    struFOC_CurrLoop.nQCur_Reference = struFOC_CurrLoop.nQCurrentSet;

                    struMotorSpeed.wSpeedfbk = App2CoreFreqTrans(stru_BEMF_Check.nBemf_FreqAve);
                    struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;

                    struAppCommData.bOpenRunFlag = 2;
                    PWMOutputs(ENABLE);
                    struFOC_CtrProc.eSysState = RUN;
                }
            }
            else
            {
                struFOC_CtrProc.eSysState = BEMF_BRAKE;
            }
            break;
        }
        default:
            break;
        }
    }
#endif

}
/*****************************************************************************
 * ������   : void StateDirCheck(void)
 * ˵��     : ˳�������򣬳��õļ�ⷽʽΪ�ջ������ͨ�������Ƽ���ٶ�ֵ
 * ���˼· ��˳�����������ջ�����;����״̬�����stop״̬���������ȫֹͣ��ֹ����;\
              ����������һֱ��⣨Ҳ���Ը���ʵ��Ӧ�ý���ɲ������
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
extern s16 s16UtRohRef;
void StateDirCheck(void)
{
    s32 t_FreqAvg = 0;
    struFOC_CurrLoop.nDCur_Reference = 0;
    struFOC_CurrLoop.nQCur_Reference = 0;

    if (s16UtRohRef < 0)
    {
        t_FreqAvg = -mOnTheFlyDetect.nFreqAvg;
    }
    else
    {
        t_FreqAvg = mOnTheFlyDetect.nFreqAvg;
    }
#if 1
    if (++ struTime.nDirCheckTime >= SPEED_TRACK_DELAYTIME)
    {
        struTime.nDirCheckTime = 0;
        switch (struFOC_CtrProc.bMotorDirtionCtrl)
        {
        case CW://˳ʱ��
        {
            if (t_FreqAvg < -struAppCommData.nDirEbreakFreq) //��ʱ�볬���٣�һֱ��⣬��������Ч��; ���ڲ�ͬ��Ӧ�ã�Ҳ���Խ���ɲ��״̬��
            {
                struFOC_CtrProc.eSysState = BRAKE;//ɲ������
                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else if (t_FreqAvg < struAppCommData.nDirTrackFreq) //��ʱ����˳����٣�ɲ��
            {
                struFOC_CtrProc.eSysState = BRAKE;//ɲ������
                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else//˳ʱ�� ���� ����ջ�����
            {
                if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                {
                    struFOC_CurrLoop.nQCur_Reference = App2CoreCurTrans(User2AppCurTrans(IQ_SET));
                }
                else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
                {
                    struFOC_CurrLoop.nQCur_Reference = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));
                }

                struFOC_CtrProc.eSysState = RUN;

                struAppCommData.bOpenRunFlag = 2;

            }
            break;
        }
        case CCW://��ʱ��
        {
            if (t_FreqAvg < -struAppCommData.nDirTrackFreq) //��ʱ����٣��ջ�����
            {
                if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                {
                    struFOC_CurrLoop.nQCur_Reference = App2CoreCurTrans(User2AppCurTrans(IQ_SET));
                }
                else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
                {
                    struFOC_CurrLoop.nQCur_Reference = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));
                }

                struFOC_CtrProc.eSysState = RUN;
                struAppCommData.bOpenRunFlag = 2;
            }
            else if (t_FreqAvg < struAppCommData.nDirEbreakFreq) //����״̬��ɲ������
            {
                struFOC_CtrProc.eSysState = BRAKE;//ɲ��״̬

                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else  //˳ʱ�� �����٣�һֱ��⣬��������Ч��;���ڲ�ͬ��Ӧ�ã�Ҳ���Խ���ɲ��״̬��
            {
                struFOC_CtrProc.eSysState = BRAKE;//ɲ������
                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            break;
        }
        default:
            break;
        }
    }
#endif
}

/*****************************************************************************
 * ������   : void StatePosSeek(void)
 * ˵��     : ��ʼλ�ü����򣬵����ֹ״̬�������Ƕ�
 * ���˼· ����������ע�뷽ʽ���������ֹ״̬�ĵ�Ƕ�
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StatePosSeek(void)
{
    mIPD_CtrProc.bIPD_State = 1;                             /* ����ΪIPD���״̬ */
    mIPD_CtrProc.bIPD_StepFlg = 0;
    mIPD_CtrProc.bCurrentSampleType = CURRENT_SAMPLE_TYPE;

    mIPD_CtrProc.wIPD_PlusWidthSet = IPD_PLUS_TIME_WIDTH;    /* ����ע���� */
    mIPD_CtrProc.wIPD_IdleWaitSet = IPD_PLUS_WAIT_TIME;      /* ����ע��󣬵���˥������ȴ�ʱ������ */
    mIPD_CtrProc.nPWM_PERIOD = PWM_PERIOD;

    ADC_SOFTWARE_TRIG_ONLY();
    ADC_STATE_RESET();

    __disable_irq();                  /* �ر��ж� �ж��ܿ��� */
    IPD_RotorPosEst();                /* 6����ע�룬��ʼλ�ü�� */
    __enable_irq();                   /* �������ж� */

    ADC0_init();                      /* ADC��ʼ�� */
    MCPWM_init();                     /* PWM��ʼ�� */

    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
    {
        struFluxOB_Param.wElectAngleOpen  = (struAppCommData.nStartAngleComp + mIPD_CtrProc.IPD_Angle) << 16; //��ʼ�Ƕȸ���
    }
    else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
    {
        struFluxOB_Param.wElectAngleOpen  = (mIPD_CtrProc.IPD_Angle - struAppCommData.nStartAngleComp) << 16; //��ʼ�Ƕȸ���
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
 *****************************************************************************/
void StateAlign(void)
{
    struMotorSpeed.wSpeedRef = 0;
    if (struTime.nAlignTime < DC_HOLDTIME_TIME_LENTH)
    {
        //��һ�ζ�λ����
        struTime.nAlignTime ++;
        mOpenForceRun.nStartCurSet = struAppCommData.wStartCurSet1;
    }
    else if (struTime.nAlignTime < DC_ALIGN_TOTAL_LENTH)
    {
        //�ڶ��ζ�λ����
        struTime.nAlignTime ++;
        mOpenForceRun.nStartCurSet = struAppCommData.wStartCurSet2;
    }
    else
    {
        //���뿪������״̬
        struFOC_CtrProc.eSysState = OPEN_RUN;//ALIGN  OPEN_RUN
        struTime.nAlignTime = 0;
    }

    mOpenForceRun.nCurrentAccStep = User2AppCurTrans(ALIGN_CURRENT_ACC);  //�������ٵ���ֵ
    mOpenForceRun.nCurrentDecStep = User2AppCurTrans(ALIGN_CURRENT_DEC);  //�������ٵ���ֵ
    OpenLoopCurRamp(&mOpenForceRun);                                      //��λ�������³���
    struFOC_CurrLoop.nDCur_Reference = (s32)struApp2Core.cur * mOpenForceRun.nStartCurRef >>
                                       (struApp2Core.curShftNum); //�趨��λ����������Id
    struFOC_CurrLoop.nDCurrentSet = struFOC_CurrLoop.nDCur_Reference;
}



/*****************************************************************************
 * ������   : void StateOpen(void)
 * ˵��     : ���ǿ�ϳ���
 * ���˼· ��1.�������ǿ��Ƶ�ʡ�ǿ�ϵ�����ǿ�ϼ��ٶȣ�ʹ����Ӿ�ֹ��ʼ�϶����趨��ǿ��Ƶ�ʡ�           \
              2.������ﵽ�趨Ƶ�ʣ��ҹ���ǶȺ�ǿ�ϽǶ�����ڣ�10.98��~ 98.87�㣩֮�����ж�Ϊ����         \
                �Ѿ����浽ǿ�ϽǶȣ�����5����ӿ����л����ջ���                                          \
              3.����ǿ�ϸ�������Id�������ڳ������˴�Id-->Iq�������л�ʱ�䣬�л�ʱ��ĳ�������ʵ�ʵ������\
                �����������ܵ�ԭ�򣬸��������л�ʱ���;���������л�ʱ�䳤�����ڵ�����Ӧ��������ʵ��ӳ���\
                ����Ҷ��ȫչ�����������ٶȱջ���                                                         \
              4.����״̬�������Ե�������������㹫ʽ��������N = 60f/Speed ������fΪ�����Ƶ�ʣ�SpeedΪ���\
                ��еת�١�
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateOpen(void)
{
    if (struAppCommData.bOpenRunFlag == 0)
    {
        if (struFOC_CtrProc.bMotorDirtionCtrl == CW)  //�������з��򣬵���ǿ��Ƶ�ʵķ���
        {
            mOpenForceRun.wOpen2CloseFreq = struAppCommData.wSvcMinFreq;  //˳ʱ����Ƶ�ʸ���
        }
        else
        {
            mOpenForceRun.wOpen2CloseFreq = -struAppCommData.wSvcMinFreq; //��ʱ����Ƶ�ʸ���
        }

        mOpenForceRun.nFreqAccStep = User2AppFreqTrans(FREQ_ACC);         //ǿ��Ƶ�ʼ��ٵ���ֵ
        mOpenForceRun.nFreqDecStep = User2AppFreqTrans(FREQ_DEC);         //ǿ��Ƶ�ʼ��ٵ���ֵ

        SpeedReferenceGen(&mOpenForceRun);  //ǿ��Ƶ�����³���
        struMotorSpeed.wSpeedSet = mOpenForceRun.wRampFreqRef >> 7;
        struMotorSpeed.wSpeedRef = App2CoreFreqTrans(struMotorSpeed.wSpeedSet);

        if (struMotorSpeed.wSpeedSet == mOpenForceRun.wOpen2CloseFreq)    //����ﵽ�趨���϶�Ƶ��
        {
            struAppCommData.wThetaErr = struFluxOB_Param.wElectAngleEst - struFluxOB_Param.wElectAngleOpen; //�������ǶȺ�ǿ�ϸ����Ƕȵ����
            struAppCommData.wThetaErr = ABS(struAppCommData.wThetaErr) >> 16;

            if ((struAppCommData.wThetaErr > 2000) && (struAppCommData.wThetaErr < 18000)) //����ǶȺ�ǿ�ϸ����Ƕ�����ڣ�10.98��~ 98.87�㣩֮�����ж�Ϊ������ȷ��
            {
                struAppCommData.nMatchCnt ++;
            }
            else
            {
                struAppCommData.nMatchCnt = 0;
            }

            if (struAppCommData.nMatchCnt > MATCH_TIME)   //�Ƕ��������5�����趨��Χ����������ջ�
            {
#if (OPEN_RUN_STATUS == TRUE)
                {
                }
#elif (OPEN_RUN_STATUS == FALSE)
                {
                    struFOC_CtrProc.eSysState = RUN;

                    struAppCommData.bOpenRunFlag = 1;

                    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                    {
                        struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(U_START_CUR_SET_S));
                    }
                    else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
                    {
                        struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(U_START_CUR_SET_S));
                    }

                    struFOC_CurrLoop.nDCurrentSet = App2CoreCurTrans(User2AppCurTrans(U_START_CUR_SET_F));//d��������������ڿ����϶���������Id���ջ�����ʱId�𽥽�Ϊ0��Iq�𽥴�0�ӵ�����ֵ
                    //�����л����ջ���Ԥ���˵����л�ʱ�䡣��������л����ٶȹ��죬������ת�ز�����

                    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));//�������ٵ���ֵ
                    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));//�������ٵ���ֵ

                    CurrentLoopAxisD_Set();
                    CurrentLoopAxisQ_Set();

                }
#endif
            }
        }
    }
}

/*****************************************************************************
 * ������   : void StateOpen(void)
 * ˵��     : ���run״̬�������һ��Ϊ�ٶȻ������ʻ��Ĵ���
 * ���˼· ��1.����ʵ�ʸ��ؼ����ٶ�/����/���������¡��ٶȻ�/���ʻ���PI       \
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
float Iq_Set = 0.8;
void StateRun(void)
{
    if (struAppCommData.bOpenRunFlag == 1) //DQ������л�����
    {
        if (struTime.nOpenRunCntr < OPEN2CLOSE_RUN_COV_TIME)
        {
            // ���ʱ���ڣ�ʹIq�������������϶�������Id�𽥽���Ϊ��һ�ζ�λ������ʱ�䳤�̸�����ƥ�䡣
            struTime.nOpenRunCntr++;
            /* D/Q������仯б������ */
            struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));
            struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));
        }
        else
        {
            //Id����Ϊ0,����ʵ�����ã������ٶȻ����߹��ʻ�
            struTime.nOpenRunCntr = 0;
            struAppCommData.bOpenRunFlag = 2;

            struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC));
            struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC));

            if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
            {
                struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));      //Q���������
            }
            else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
            {
                struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));    //Q���������
            }
        }
    }
    else if (struAppCommData.bOpenRunFlag == 2)
    {

        /*******************�ٶȻ�����*******************************/
#if (CLOSE_LOOP == SPEED_LOOP)
        {
            if (struTime.nLoopDelyTime < STATE04_WAITE_TIME)//�ٶȻ�������ʼ��ʱ�䣬�󲿷�Ӧ������Ϊ100ms
            {
                struTime.nLoopDelyTime ++;

                struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCur_Reference << 22;
                struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;
            }
            else
            {
                if (++struAppCommData.nLoopCntr >= SPEED_LOOP_CNTR)   //�ٶȻ���·�������ڡ�
                {
                    struAppCommData.nLoopCntr = 0;

                    if (struTime.nLoopDelyTime == STATE04_WAITE_TIME)  //�״ν����ٶȻ����ٶȻ��ĳ�ʼֵ����ΪIq��
                    {
                        struTime.nLoopDelyTime ++;
                        struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCurrentSet << 22;
                        struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;
                    }

#if (POWER_LIMIT_STATUS == TRUE)          //�޹���ʹ�ܣ��ٶȻ���������������
                    {
                        PowerLimitCalc(&struMotorSpeed, struPower.wPowerValue);
                        struMotorSpeed.wSpeedSet = struMotorSpeed.wPowerLimitSpeedRef - struMotorSpeed.wPowerLimitSpeedSet;
                    }
#elif (POWER_LIMIT_STATUS == FALSE)
                    {
                        //����ָ��ٶȲο�����SPEED_SET ��λHz
                        //struMotorSpeed.wSpeedSet = App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET)); //�ٶȲο�����
                        struMotorSpeed.wSpeedSet = struAppCommData.wSpeedValue;

                    }
#endif

                    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                    {
                        if (struMotorSpeed.wSpeedSet < 0)
                        {
                            struMotorSpeed.wSpeedSet = -struMotorSpeed.wSpeedSet;
                        }
                    }
                    else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
                    {
                        if (struMotorSpeed.wSpeedSet > 0)
                        {
                            struMotorSpeed.wSpeedSet = - struMotorSpeed.wSpeedSet;
                        }
                    }

                    SpeedLoop_Set(&struMotorSpeed); //�ٶ�����

                    SpeedLoopReg(&struMotorSpeed);// �ٶȻ�PI����

                }
            }
        }

        /*******************����������*******************************/
#elif (CLOSE_LOOP == CURRENT_LOOP)
        {

            if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
            {
                struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(Iq_Set));
            }
            else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
            {
                struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(Iq_Set));
            }
				    struMotorSpeed.wSpeedRef =  struMotorSpeed.wSpeedfbk;
        }

        /*******************���ʻ�����*******************************/
#elif (CLOSE_LOOP == POWER_LOOP)
        {
            if (++struAppCommData.nLoopCntr >= POWER_LOOP_CNTR) //���ʻ���·�������ڡ�
            {
                struAppCommData.nLoopCntr = 0;

#if(SPPED_LIMIT_STATUS == TRUE)     //���ٹ���ʹ�� �����ʻ��������ת�١�
                {
                    SpeedLimitCalc(struMotorSpeed.wSpeedfbk, &struPower);  //���ٶȼ���
                    
                    struPower.wPowerSet = struPower.wSpeedLimitPowerRef - struPower.wSpeedLimitPowerSet;      // �������ٶȺ�����������ø�������
                }
#elif (SPPED_LIMIT_STATUS == FALSE)
                {
                    struPower.wPowerSet = POWER_CALC(POWER_SET);//���ʲο�����
                }
#endif

                if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                {
                    struPower.wPowerSet = struPower.wPowerSet;
                    struPower.wPowerValue = struPower.wPowerValue;
                }
                else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
                {
                    struPower.wPowerSet = -struPower.wPowerSet;
                    struPower.wPowerValue = -struPower.wPowerValue;
                }

                if (struAppCommData.nPowerFistFlag == 0) //�״ν����ٶȻ������ʻ��ĳ�ʼֵ����ΪIq
                {
                    struAppCommData.nPowerFistFlag = 1;
                    struPower.struPowerRamp.wRef = struPower.wPowerValue;
                    struPower.struPowerPI.wIntegral = ((s32)struFOC_CurrLoop.nQCur_Reference << 16);
                }

                struPower.struPowerRamp.wSet = struPower.wPowerSet;
                struPower.wPowerRef = RampControl(&struPower.struPowerRamp);    //���ʻ����º���

                PowerLoopReg(&struPower);                 //���ʻ�PI����
            }
        }
#endif
    }

    struFOC_CurrLoop.nDCurrentSet = 0;       //Id����Ϊ0
    CurrentLoopAxisD_Set();
    CurrentLoopAxisQ_Set();

}

/*****************************************************************************
 * ������   : void StateFault(void)
 * ˵��     : ����״̬����Ҫ�ǽ��й���״̬����������
 * ���˼· ��1.�ڹ���״̬�������⵽������ʧ��������stru_Faults��Ӧ����λ��\
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateFault(void)
{
    struFOC_CtrProc.bMC_RunFlg = 0;

    PWMOutputs(DISABLE);

//    FaultRecover();    //���ϻָ�������
}

/*****************************************************************************
 * ������   : void StateBemfStop(void)
 * ˵��     : ���ɲ���ȴ�
 * ���˼· ��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2021.05.22
 *****************************************************************************/
void StateBemfStop(void)
{
    if( struTime.nBemfBrakeCntr < BEMF_TIME)
    {
        PWMOutputs(ENABLE);
        struTime.nBemfBrakeCntr ++;
    }
    else
    {
        struTime.nBemfBrakeCntr = 0;
        PWMOutputs(DISABLE);
#if (SEEK_POSITION_STATUS == TRUE)  //�����ʼλ�ü��״̬ʹ�ܣ�������ʼλ�ü��״̬
        {
            struFOC_CtrProc.eSysState = POS_SEEK;

        }
#elif (SEEK_POSITION_STATUS == FALSE) //�����ʼλ�ü��״̬��ʹ�ܣ������Ԥ��λ״̬������Ԥ��λ�Ƕ�
        {
            struFOC_CtrProc.eSysState = INIT;
            struFluxOB_Param.wElectAngleOpen = ((u32)(User2AppAngleTrans(ALIGN_ANGLE) * (u32)struApp2Core.angle >> (struApp2Core.angleShftNum))) << 16;
        }
#endif
    }
}

/*****************************************************************************
 * ������   : void StateStop(void)
 * ˵��     : ���ֹͣ�������жϵ���Ƿ�ֹ״̬
 * ���˼· ��1.ͨ��mOnTheFlyDetect.bMotorStopDecFlag��״̬���жϵ���Ƿ��ھ�ֹ״̬�����ж����
                ��ֹ��ȥ���г�ʼλ�ü�����Ԥ��λ����
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateStop(void)
{
    if (mOnTheFlyDetect.bMotorStopDecFlag)  //���ֹͣ��־λΪ1���ж����Ϊ��ֹ״̬
    {
        if (struTime.nStopDelayCntr < STOP_TIME)   //���ֹͣʱ���˲�������STOP_TIME�ε�����ھ�ֹ״̬���ж����Ϊ��ֹ״̬
        {
            struTime.nStopDelayCntr ++;
            mOnTheFlyDetect.bMotorStopDecFlag = 0;
        }
        else
        {
            if (struTime.nStopDelayTime > 0)     //�ж������ֹ�����ʱ���������ж�������ʵ�ʵ�����ص����ӳ�ʱ��
            {
                struTime.nStopDelayTime --;
            }
            else
            {
                struFluxOB_Param.wElectAngleOpen = 0;
                struMotorSpeed.wSpeedRef = 0;
                struMotorSpeed.wSpeedSet = 0;
                mOnTheFlyDetect.bMotorStopDecFlag = 0;
                struTime.nStopDelayCntr = 0;

#if (SEEK_POSITION_STATUS == TRUE)  //�����ʼλ�ü��״̬ʹ�ܣ�������ʼλ�ü��״̬
                {
                    struFOC_CtrProc.eSysState = POS_SEEK;
                    PWMOutputs(DISABLE);
                }
#elif (SEEK_POSITION_STATUS == FALSE) //�����ʼλ�ü��״̬��ʹ�ܣ������Ԥ��λ״̬������Ԥ��λ�Ƕ�
                {
                    struFOC_CtrProc.eSysState = INIT;
                    struFluxOB_Param.wElectAngleOpen = ((u32)(User2AppAngleTrans(ALIGN_ANGLE) * (u32)struApp2Core.angle >> (struApp2Core.angleShftNum))) << 16;
                }
#endif
            }
        }
    }
    else
    {
        struTime.nStopDelayCntr = 0;
    }

}

/*******************************************************************************
 �������ƣ�    void CurrentLoopAxisD_Set(void)
 ����������    ������D������ο�б�¸���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void CurrentLoopAxisD_Set(void)
{
    if (struFOC_CurrLoop.nDCur_Reference > struFOC_CurrLoop.nDCurrentSet)
    {
        if ((struFOC_CurrLoop.nDCur_Reference - struFOC_CurrLoop.nDCurrentSet) > struAppCommData.nCurrentDEC)
        {
            struFOC_CurrLoop.nDCur_Reference -= struAppCommData.nCurrentDEC;
        }
        else
        {
            struFOC_CurrLoop.nDCur_Reference --;
        }
    }

    if (struFOC_CurrLoop.nDCur_Reference <= struFOC_CurrLoop.nDCurrentSet)
    {
        if ((struFOC_CurrLoop.nDCurrentSet - struFOC_CurrLoop.nDCur_Reference) > struAppCommData.nCurrentACC)
        {
            struFOC_CurrLoop.nDCur_Reference += struAppCommData.nCurrentACC;
        }
        else
        {
            struFOC_CurrLoop.nDCur_Reference ++;
        }

    }

}

/*******************************************************************************
 �������ƣ�    void CurrentLoopAxisQ_Set(void)
 ����������    ������Q������ο�б�¸���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void CurrentLoopAxisQ_Set(void)
{
    if (struFOC_CurrLoop.nQCur_Reference > struFOC_CurrLoop.nQCurrentSet)
    {
        if ((struFOC_CurrLoop.nQCur_Reference - struFOC_CurrLoop.nQCurrentSet) > struAppCommData.nCurrentDEC)
        {
            struFOC_CurrLoop.nQCur_Reference -= struAppCommData.nCurrentDEC;
        }
        else
        {
            struFOC_CurrLoop.nQCur_Reference --;
        }
    }

    if (struFOC_CurrLoop.nQCur_Reference <= struFOC_CurrLoop.nQCurrentSet)
    {
        if ((struFOC_CurrLoop.nQCurrentSet - struFOC_CurrLoop.nQCur_Reference) > struAppCommData.nCurrentACC)
        {
            struFOC_CurrLoop.nQCur_Reference += struAppCommData.nCurrentACC;
        }
        else
        {
            struFOC_CurrLoop.nQCur_Reference ++;
        }

    }
}

/*******************************************************************************
 �������ƣ�    void SpeedLoop_Set(MechanicalQuantity *this)
 ����������    �ٶȲο�б�º���
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/10/8      V1.0           Andrew Kong          ����
 *******************************************************************************/
void SpeedLoop_Set(MechanicalQuantity *this)
{
    if (this->wSpeedRef < this->wSpeedSet)
    {
        this->wSpeedRef += this->wSpeedRampACCStep;

        if (this->wSpeedRef >= this->wSpeedSet)
        {
            this->wSpeedRef = this->wSpeedSet;
        }
    }

    if (this->wSpeedRef > this->wSpeedSet)
    {
        this->wSpeedRef -= this->wSpeedRampDECStep;

        if (this->wSpeedRef <= this->wSpeedSet)
        {
            this->wSpeedRef = this->wSpeedSet;
        }
    }
}



/*****************************************************************************
 * ������   : void StateRun(void)
 * ˵��     : ���Hallrun״̬�������һ��Ϊ��hall�������ٶȻ��Ĵ���
 * ���˼· ��hall�ٶȱջ�;�������ĵ����������;hall�������е��޸��ٶ�����
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
void StateHallRun(void)
{
    /*****************************************hallTosensorless*********************************************/
#if (CLOSE_LOOP == SPEED_LOOP)
	if(struTime.nOpenRunCntr < CURR_TO_SPEED_TIME)
	{
	  struTime.nOpenRunCntr ++;
	  if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
			{
					struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));
			}
			else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
			{
					struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));
			}      
		
	}else
   {
        if (struTime.nLoopDelyTime < STATE04_WAITE_TIME)//�ٶȻ�������ʼ��ʱ�䣬�󲿷�Ӧ������Ϊ100ms
        {
            struTime.nLoopDelyTime ++;

            struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCur_Reference << 22;

            struMotorSpeed.wSpeedRef = struHallProcess.wSpeedfbkHall;
        }
        else
        {
            if (++struAppCommData.nLoopCntr >= SPEED_LOOP_CNTR)   //�ٶȻ���·�������ڡ�
            {
                struAppCommData.nLoopCntr = 0;
							
#if (POWER_LIMIT_STATUS == TRUE)          //�޹���ʹ�ܣ��ٶȻ���������������
                {
                    PowerLimitCalc(&struMotorSpeed, struPower.wPowerValue);
                    struMotorSpeed.wSpeedSet = struMotorSpeed.wSpeedSet - struMotorSpeed.wPowerLimitSpeedSet;
                }
#elif (POWER_LIMIT_STATUS == FALSE)
                {
                    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                    {
                        //����ָ��ٶȲο�����SPEED_SET ��λHz
                        //struMotorSpeed.wSpeedSet = App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET));
                        struMotorSpeed.wSpeedSet = struAppCommData.wSpeedValue;
                    }
                    else
                    {
                        //struMotorSpeed.wSpeedSet = -App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET));
                        struMotorSpeed.wSpeedSet = -struAppCommData.wSpeedValue;
                    }
                }
#endif      //�����ٶ��޷�  
                //struMotorSpeed.wSpeedSet = sat(struMotorSpeed.wSpeedSet,-App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MAX_FAULT)),App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MAX_FAULT)));

            }
        }

        //���¼Ӽ��ٵ�����
        struMotorSpeed.wSpeedRampACCStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_ACC));
        struMotorSpeed.wSpeedRampDECStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_DEC));
        //�ٶ����� Ramp
        SpeedLoop_Set(&struMotorSpeed);

        //����ĵ���ֵ currentSet ���������Ӽ��ٵ�����ʵ�ʴ���������
        SpeedLoopReg(&struMotorSpeed);// �ٶȻ�PI����

    }
    /*******************����������*******************************/
#elif (CLOSE_LOOP == CURRENT_LOOP)
    {
        //�����������iq
        if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
        {
            struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));
        }
        else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
        {
            struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));
        }      
    }
#endif
    //Id����Ϊ0
    struFOC_CurrLoop.nDCurrentSet = 0;
    //DQ���������ֵ�仯б��
    CurrentLoopAxisD_Set();
    CurrentLoopAxisQ_Set();
}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



