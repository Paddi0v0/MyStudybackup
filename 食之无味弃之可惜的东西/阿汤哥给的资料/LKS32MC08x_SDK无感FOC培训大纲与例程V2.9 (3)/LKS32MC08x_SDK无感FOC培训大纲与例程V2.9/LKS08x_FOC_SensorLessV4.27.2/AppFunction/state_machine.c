/*******************************************************************************
 * 版权所有 (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： state_machine.c
 * 文件标识：
 * 内容摘要： state machine
 * 其它说明： 无
 * 当前版本： V1.0
 * 作    者： andrew kong
 * 完成日期： 2019年11月19日
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
 函数名称：    void Sys_State_Machine(stru_FOC_CtrProcDef *this)
 功能描述：    系统状态机 电机状态各状态切换调度
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void Sys_State_Machine(void)
{
    switch (struFOC_CtrProc.eSysState)
    {
    case IDLE:
    {
        /* 空闲状态 */

        if (struFOC_CtrProc.bMC_RunFlg)
        {
            StateInit();

#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS)
            struFOC_CtrProc.eSysState = CHARGE; /* 进入充电状态 */
#else
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)
            if (GET_HALL_LEARN_STATE())
            {
                /* 进入HALL自学习状态 */

                PWMOutputs(ENABLE);
                struFOC_CtrProc.eSysState = HALL_LEARN;
            }
            else
            {
                StateInit();
                PWMOutputs(ENABLE);
                struFOC_CtrProc.eSysState = HALL_RUN;   //有Hall运行
            }
#endif
#endif
        }
        break;
    }
    case CHARGE:
    {
        StateCharge();                         //预充电处理函数
        break;
    }
    case DIR_CHECK:                           //顺逆风检测状态
    {
#if(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
        struFOC_CtrProc.eSysState = POS_SEEK;
#else
        StateDirCheck();                      //顺逆风处理函数
#endif
        break;
    }
    case INIT:
    {
        /* 初始化状态 */

        StateInit();

        PWMOutputs(ENABLE);
        struFOC_CtrProc.eSysState = ALIGN;

        break;
    }
    case POS_SEEK:                            //初始位置检测状态
    {
        StatePosSeek();                       //初始位置检测处理函数

        struFOC_CtrProc.eSysState = INIT;

        break ;
    }
    case ALIGN:                                //预定位状态
    {
        StateAlign();                          //预定位处理函数

        break ;
    }
    case OPEN_RUN:                                //开环强拖状态
    {
        StateOpen();                              //开环处理函数
        break ;
    }
    case RUN:
    {
        /* 运行状态  */

        StateRun();                               //闭环处理函数
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
        /* 电机刹车状态 */
        StateStop();                          //停止判定处理函数
        SetTimeOut_Counter(struFOC_CtrProc.nSetTimeLeftCnt, STATE_MACHINE_200MS);
        break;
    }
    case WAIT:
    {
        /* 等待状态 */
        if (SetTime_IsElapsed(struFOC_CtrProc.nSetTimeLeftCnt))
        {
            struFOC_CtrProc.eSysState = IDLE;
        }

        break;
    }
    case FAULT:
    {
        /* 故障状态 */
        StateFault();                         //故障处理函数

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

    if ((struFOC_CtrProc.bMC_RunFlg == 0) && (stru_Faults.R == 0))  //无故障并且启动标志位清0，关闭输出，进入空闲状态
    {
        PWMOutputs(DISABLE);
        SetTimeOut_Counter(struFOC_CtrProc.nSetTimeLeftCnt, STATE_MACHINE_200MS);
        struFOC_CtrProc.eSysState = WAIT;
    }
}


/*****************************************************************************
 * 函数名   : void StateInit()
 * 说明     : 状态初始化
 * 设计思路 ：1.变量初始化
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateInit(void)
{
    s16 t_nTimer;

    PWMOutputs(DISABLE);

    //顺逆风检测
    struAppCommData.nDirTrackFreq = User2AppFreqTrans(SPEED_TRACK_ON_FREQ_THH); //顺风切闭环速度初始化
    struAppCommData.nDirEbreakFreq = User2AppFreqTrans(EBRK_ON_FREQ_THH);       //刹车速度初始化

    mOnTheFlyDetect.bMotorStopDecFlag = 0;
    mOnTheFlyDetect.nMotorStopCurrentThd = MOTOR_STOP_CUR_THD;    /*电机停止检测电流阈值 */
    mOnTheFlyDetect.nMotorStopCurDifThd = MOTOR_STOP_CUR_DIF_THD; /*电机停止检测电流阈值 */
    mOnTheFlyDetect.nFreqAvg = 0;
    mOnTheFlyDetect.nFreq = 0;

    mOnTheFlyDetect.nElectAngle = 0;
    mOnTheFlyDetect.wBemfFreq = 0;
    mOnTheFlyDetect.wAngleDpp = 0;
    mOnTheFlyDetect.wBEMF_DppValueAcc = 0;
    mOnTheFlyDetect.nMotorVolMag = 0;
    mOnTheFlyDetect.wVolMagAvg = 0;

    OnTheFlyDetectInit();  //顺逆风启动初始化

    struFOC_CtrProc.bMotorDirtionCtrl = CW;   /* 电机转向 */

    struFOC_CtrProc.bMotorDirtionCtrlPhase = CW;   /* 软件改变相线的方式，改变电机转向 */
    //预充电
    struAppCommData.bChargeFlag = 0;    //预充电开始标志清零
    struAppCommData.bChargeEndFlag = 0; //预充电完成标志清零

//初始位置检测
    struAppCommData.nStartAngleComp = User2AppAngleTrans(U_START_ANGLE_COMP);  //初始位置检测补偿角度初始化

    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC)); //电流加速调整值
    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC)); // 电流减速调整值


    /*****电流参数初始化********/
    struFOC_CurrLoop.mStatCurrUVW.nPhaseU = PWM_PERIOD / 2;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseV = PWM_PERIOD / 2;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseW = PWM_PERIOD / 2;
    struFOC_CurrLoop.MCPWMx_RegUpdate();                    /* 加载MCPWM模块占空比值，加载MCPWM模块ADC触发点寄存器值 */

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

    /*****DQ轴电流环参数初始化********/

    struFOC_CurrLoop.mPI_Flux.wIntegral = 0;
    struFOC_CurrLoop.mPI_Flux.wLastError = 0;

    struFOC_CurrLoop.mPI_Torque.wIntegral = 0;
    struFOC_CurrLoop.mPI_Torque.wLastError = 0;

    /*****电压参数初始化********/

    struFOC_CurrLoop.mStatVoltAlfaBeta.nAlph = 0;
    struFOC_CurrLoop.mStatVoltAlfaBeta.nBeta = 0;

    struFOC_CurrLoop.mStatVoltDQ.nAxisD = 0;
    struFOC_CurrLoop.mStatVoltDQ.nAxisQ = 0;

    struFOC_CurrLoop.mVdtComp.nPhaseU = 0;
    struFOC_CurrLoop.mVdtComp.nPhaseV = 0;
    struFOC_CurrLoop.mVdtComp.nPhaseW = 0;


    /*****定位参数初始化********/
    struAppCommData.wStartCurSet1 = User2AppCurTrans(U_START_CUR_SET_F); //第一段定位电流
    struAppCommData.wStartCurSet2 = User2AppCurTrans(U_START_CUR_SET_S); //第二段定位电流

    /*****开环运行参数初始化********/

    mOpenForceRun.nFreqAccStep = 0;
    mOpenForceRun.nFreqDecStep = 0;
    mOpenForceRun.wRampFreqRef = 0;
    mOpenForceRun.nStartCurSet = 0;
    mOpenForceRun.wStartCurRamp = 0;
    mOpenForceRun.nStartCurRef = 0;
    struAppCommData.wSvcMinFreq = User2AppFreqTrans(OPEN_ANGLE_TAG_FREQ);  //开环拖动频率
    struAppCommData.bOpenRunFlag = 0;

    struAppCommData.nLoopCntr = 0;
    /*****速度环参数初始化********/
#if (CLOSE_LOOP == SPEED_LOOP)
    {
        struMotorSpeed.mSpeedPI.KP = P_ASR_KP;                                                 //速度环 Kp
        struMotorSpeed.mSpeedPI.KI = P_ASR_KI;                                                 //速度环 Ki
        struMotorSpeed.mSpeedPI.wIntegral = 0;
        struMotorSpeed.mSpeedPI.wUpperLimitOutput = App2CoreCurTrans(User2AppCurTrans(IQMAX)); //速度环输出最大值
        struMotorSpeed.mSpeedPI.wLowerLimitOutput = App2CoreCurTrans(User2AppCurTrans(IQMIN));//速度环输出最小值
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
    /*****功率环参数初始化********/
#elif (CLOSE_LOOP == POWER_LOOP)
    {
        PowerLoopInit();
    }
#endif

#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS)
    /*****估算环参数初始化********/
//    FluxObserveInit();
    PmsmFluxObIni();             //估算环变量初始环函数
    struFluxOB_Param.nObserveMinSpeed = App2CoreFreqTrans(User2AppFreqTrans(OPEN_ANGLE_TAG_FREQ));
#endif

    /* 弱磁参数初始化 */
    AFWR.KI = 10;
    AFWR.KP = 50;
    AFWR.INTEG = 0;
    AFWR.LIM_H = 0;
    AFWR.LIM_L = -struFOC_CurrLoop.nWeakFieldLim;
    struFOC_CurrLoop.npmsmSvcImFw = 0;


    /*****环路计算时间参数初始化********/
    struTime.nChargeTime = 0;
    struTime.nAlignTime = 0;
    struTime.nDirCheckTime = 0;
    struTime.nLoopDelyTime = 0;
    struTime.nStopDelayTime = 0;
    struTime.nStopDelayCntr = 0;
    struTime.nOpenRunCntr = 0;

    /************************电流环细调*****************************/
//   	struFOC_CurrLoop.mPI_Flux.KP = 8000;   //ID环路(励磁) KP
//	  struFOC_CurrLoop.mPI_Flux.KI = 4000;   //ID环路(励磁) KI
//	
//	  struFOC_CurrLoop.mPI_Torque.KP = 8000; //IQ环路(转矩) KP
//	  struFOC_CurrLoop.mPI_Torque.KI = 4000; //IQ环路(转矩) KI

    /*****错误检测参数初始化********/
    FaultInit();

    t_nTimer = struFOC_CtrProc.nSys_TimerPWM;

    ADC0_init();
    while (ABS(struFOC_CtrProc.nSys_TimerPWM - t_nTimer) < 2)
    {
    }

}


/*****************************************************************************
 * 函数名   : void StateCharge(void)
 * 说明     : 预充电函数，对自举电容进行预充电，对于不同的硬件要注意调整预充电时间
 * 设计思路 ：依次打开A相、B相、C相下桥，对自举电容充电。
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateCharge(void)
{
    if (struAppCommData.bChargeFlag == 0)
    {
        struAppCommData.bChargeFlag = 1;

        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU = (u16)(PWM_PERIOD * 0.3); //预充电占空比
        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV = (u16)(PWM_PERIOD * 0.3); //预充电占空比
        struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW = (u16)(PWM_PERIOD * 0.3); //预充电占空比

        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
        struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

        struFOC_CurrLoop.MCPWMx_RegUpdate();          /* 加载MCPWM模块占空比值，加载MCPWM模块ADC触发点寄存器值 */

        struTime.nChargeTime  = 0;
    }
    else
    {
        if (struTime.nChargeTime == 0)  //打开A相下桥PWM
        {
            MCPWM_PRT = 0x0000DEAD;

            MCPWM_IO01 = 0x0C08 | mIPD_CtrProc.hDriverPolarity;
            MCPWM_IO23 = 0x000C | mIPD_CtrProc.hDriverPolarity;

            MCPWM_PRT = 0x0000;

            PWMOutputs(ENABLE);    //使能输出
            __nop();
        }
        else if (struTime.nChargeTime == CHARGE_TIME)  //打开B相下桥PWM
        {
            MCPWM_PRT = 0x0000DEAD;
            MCPWM_IO01 = 0x080C | mIPD_CtrProc.hDriverPolarity;
            MCPWM_PRT = 0x0000;
            __nop();
        }
        else if (struTime.nChargeTime == CHARGE_TIME * 2) //打开C相下桥PWM
        {
            MCPWM_PRT = 0x0000DEAD;
            MCPWM_IO23 = 0x08 | mIPD_CtrProc.hDriverPolarity;
            MCPWM_PRT = 0x0000;
            __nop();
        }

        if (struTime.nChargeTime < (CHARGE_TIME * 3))  //CHARGE_TIME为每段预充电时间，可根据硬件实际设置
        {
            struTime.nChargeTime ++ ;
        }
        else
        {
            PWMOutputs(DISABLE);
            __disable_irq();

            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU = 0;
            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV = 0;    /* 清零MCPWM比较值寄存器 */
            struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW = 0;

            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseU = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseU;
            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseV = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseV;
            struFOC_CurrLoop.mVoltUVW_NegPWM.nPhaseW = -struFOC_CurrLoop.mVoltUVW_PWM.nPhaseW;

            struFOC_CurrLoop.MCPWMx_RegUpdate();          /* 加载MCPWM模块占空比值，加载MCPWM模块ADC触发点寄存器值 */
            MCPWM_UPDATE = 0xFF;

            MCPWM_init();
            ADC_STATE_RESET();
            __enable_irq();
            struAppCommData.bChargeEndFlag = 1;
        }
    }

    if (struAppCommData.bChargeEndFlag == 1) //预充电完成
    {
        struAppCommData.bChargeFlag = 0;
        struTime.nChargeTime = 0;
        struAppCommData.bChargeEndFlag = 0;

        StateInit();

#if (DIR_CHECK_STATUS == FALSE)      //如果是顺逆风检测状态使能，则进入顺逆风检测状态
#if (SEEK_POSITION_STATUS == TRUE)  //如果初始位置检测状态使能，则进入初始位置检测状态
        {
            struFOC_CtrProc.eSysState = POS_SEEK;
            PWMOutputs(DISABLE);
        }
#elif (SEEK_POSITION_STATUS == FALSE) //如果初始位置检测状态不使能，则进入预定位状态，给定预定位角度
        {
            struFOC_CtrProc.eSysState = INIT;
            struFluxOB_Param.wElectAngleOpen = ((u32)(User2AppAngleTrans(ALIGN_ANGLE) * (u32)struApp2Core.angle >> (struApp2Core.angleShftNum))) << 16;
        }
#endif
#else
        PWMOutputs(ENABLE);
        struFOC_CtrProc.eSysState = DIR_CHECK;
#endif
    }
}

/*****************************************************************************
 * 函数名   : void StateDirCheck(void)
 * 说明     : 顺逆风检测程序，常用的检测方式为闭环估算和通过反电势检测速度值
 * 设计思路 ：顺风高速则切入闭环运行;低速状态则进入stop状态，当电机完全停止后静止启动;\
              当逆风高速则一直检测（也可以根据实际应用进行刹车）。
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
extern s16 s16UtRohRef;
s16 t_FreqAvg = 0;
void StateDirCheck(void)
{

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
        case CW://顺时针
        {
            if (t_FreqAvg < -struAppCommData.nDirEbreakFreq) //逆时针超高速，一直检测，附带减速效果; 对于不同的应用，也可以进入刹车状态。
            {
                struFOC_CtrProc.eSysState = BRAKE;//刹车处理
                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else if (t_FreqAvg < struAppCommData.nDirTrackFreq) //逆时针与顺风低速，刹车
            {
                struFOC_CtrProc.eSysState = BRAKE;//刹车处理
                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else//顺时针 高速 切入闭环运行
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
        case CCW://逆时针
        {
            if (t_FreqAvg < -struAppCommData.nDirTrackFreq) //逆时针高速，闭环运行
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
            else if (t_FreqAvg < struAppCommData.nDirEbreakFreq) //低速状态，刹车处理
            {
                struFOC_CtrProc.eSysState = BRAKE;//刹车状态

                struTime.nStopDelayTime = STOP_DELAY_TIME;
            }
            else  //顺时针 超高速，一直检测，附带减速效果;对于不同的应用，也可以进入刹车状态。
            {
                struFOC_CtrProc.eSysState = BRAKE;//刹车处理
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
 * 函数名   : void StatePosSeek(void)
 * 说明     : 初始位置检测程序，电机静止状态检测电机电角度
 * 设计思路 ：采用脉冲注入方式，检测电机静止状态的电角度
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StatePosSeek(void)
{
    mIPD_CtrProc.bIPD_State = 1;                             /* 调置为IPD检测状态 */
    mIPD_CtrProc.bIPD_StepFlg = 0;
    mIPD_CtrProc.bCurrentSampleType = CURRENT_SAMPLE_TYPE;

    mIPD_CtrProc.wIPD_PlusWidthSet = IPD_PLUS_TIME_WIDTH;    /* 脉冲注入宽度 */
    mIPD_CtrProc.wIPD_IdleWaitSet = IPD_PLUS_WAIT_TIME;      /* 脉冲注入后，电流衰减到零等待时间设置 */
    mIPD_CtrProc.nPWM_PERIOD = PWM_PERIOD;

    ADC_SOFTWARE_TRIG_ONLY();
    ADC_STATE_RESET();

    __disable_irq();                  /* 关闭中断 中断总开关 */
    IPD_RotorPosEst();                /* 6脉冲注入，初始位置检测 */
    __enable_irq();                   /* 开启总中断 */

    ADC0_init();                      /* ADC初始化 */
    MCPWM_init();                     /* PWM初始化 */

    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
    {
        struFluxOB_Param.wElectAngleOpen  = (struAppCommData.nStartAngleComp + mIPD_CtrProc.IPD_Angle) << 16; //初始角度给定
    }
    else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
    {
        struFluxOB_Param.wElectAngleOpen  = (mIPD_CtrProc.IPD_Angle - struAppCommData.nStartAngleComp) << 16; //初始角度给定
    }
}

/*****************************************************************************
 * 函数名   : void StateAlign(void)
 * 说明     : 电机预定位。使能初始位置检测功能时，此函数只是提供开环强拖电流，\
              两段定位时间均给到1ms。
 * 设计思路 ：两段定位;给定电机电角度、定位电流、定位时间，使电机固定在给定角度上。\
              定位时间的设定，以电机完全静止的时间为准。
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateAlign(void)
{
    struMotorSpeed.wSpeedRef = 0;
    if (struTime.nAlignTime < DC_HOLDTIME_TIME_LENTH)
    {
        //第一段定位处理
        struTime.nAlignTime ++;
        mOpenForceRun.nStartCurSet = struAppCommData.wStartCurSet1;
    }
    else if (struTime.nAlignTime < DC_ALIGN_TOTAL_LENTH)
    {
        //第二段定位处理
        struTime.nAlignTime ++;
        mOpenForceRun.nStartCurSet = struAppCommData.wStartCurSet2;
    }
    else
    {
        //进入开环爬坡状态
        struFOC_CtrProc.eSysState = OPEN_RUN;//ALIGN  OPEN_RUN
        struTime.nAlignTime = 0;
    }

    mOpenForceRun.nCurrentAccStep = User2AppCurTrans(ALIGN_CURRENT_ACC);  //电流加速调整值
    mOpenForceRun.nCurrentDecStep = User2AppCurTrans(ALIGN_CURRENT_DEC);  //电流减速调整值
    OpenLoopCurRamp(&mOpenForceRun);                                      //定位电流爬坡程序
    struFOC_CurrLoop.nDCur_Reference = (s32)struApp2Core.cur * mOpenForceRun.nStartCurRef >>
                                       (struApp2Core.curShftNum); //设定定位电流，给到Id
    struFOC_CurrLoop.nDCurrentSet = struFOC_CurrLoop.nDCur_Reference;
}



/*****************************************************************************
 * 函数名   : void StateOpen(void)
 * 说明     : 电机强拖程序
 * 设计思路 ：1.给定电机强拖频率、强拖电流、强拖加速度，使电机从静止开始拖动到设定的强拖频率。           \
              2.当电机达到设定频率，且估算角度和强拖角度误差在（10.98°~ 98.87°）之内则判断为估算         \
                已经跟随到强拖角度，连续5次则从开环切换到闭环。                                          \
              3.由于强拖给定的是Id，所以在程序留了从Id-->Iq电流的切换时间，切换时间的长短依照实际电机负载\
                来调整。（总的原则，负载轻则切换时间短;负载重则切换时间长，对于灯扇类应用则可以适当延长，\
                等扇叶完全展开后再切入速度闭环。                                                         \
              4.开环状态用来测试电机极对数，计算公式：极对数N = 60f/Speed ，其中f为电机电频率，Speed为电机\
                机械转速。
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateOpen(void)
{
    if (struAppCommData.bOpenRunFlag == 0)
    {
        if (struFOC_CtrProc.bMotorDirtionCtrl == CW)  //根据运行方向，调整强拖频率的方向
        {
            mOpenForceRun.wOpen2CloseFreq = struAppCommData.wSvcMinFreq;  //顺时针则频率给正
        }
        else
        {
            mOpenForceRun.wOpen2CloseFreq = -struAppCommData.wSvcMinFreq; //逆时针则频率给负
        }

        mOpenForceRun.nFreqAccStep = User2AppFreqTrans(FREQ_ACC);         //强拖频率加速调整值
        mOpenForceRun.nFreqDecStep = User2AppFreqTrans(FREQ_DEC);         //强拖频率减速调整值

        SpeedReferenceGen(&mOpenForceRun);  //强拖频率爬坡程序
        struMotorSpeed.wSpeedSet = mOpenForceRun.wRampFreqRef >> 7;
        struMotorSpeed.wSpeedRef = App2CoreFreqTrans(struMotorSpeed.wSpeedSet);

        if (struMotorSpeed.wSpeedSet == mOpenForceRun.wOpen2CloseFreq)    //电机达到设定的拖动频率
        {
            struAppCommData.wThetaErr = struFluxOB_Param.wElectAngleEst - struFluxOB_Param.wElectAngleOpen; //计算估算角度和强拖给定角度的误差
            struAppCommData.wThetaErr = ABS(struAppCommData.wThetaErr) >> 16;

            if ((struAppCommData.wThetaErr > 2000) && (struAppCommData.wThetaErr < 18000)) //估算角度和强拖给定角度误差在（10.98°~ 98.87°）之间则判断为估算正确。
            {
                struAppCommData.nMatchCnt ++;
            }
            else
            {
                struAppCommData.nMatchCnt = 0;
            }

            if (struAppCommData.nMatchCnt > MATCH_TIME)   //角度误差连续5次在设定范围内则进入估算闭环
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

                    struFOC_CurrLoop.nDCurrentSet = App2CoreCurTrans(User2AppCurTrans(U_START_CUR_SET_F));//d轴电流给定，由于开环拖动给定的是Id，闭环估算时Id逐渐将为0，Iq逐渐从0加到给定值
                    //所以切换到闭环后预留了电流切换时间。如果电流切换的速度过快，则会造成转矩波动。

                    struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));//电流加速调整值
                    struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));//电流减速调整值

                    CurrentLoopAxisD_Set();
                    CurrentLoopAxisQ_Set();

                }
#endif
            }
        }
    }
}

/*****************************************************************************
 * 函数名   : void StateOpen(void)
 * 说明     : 电机run状态处理程序，一般为速度环、功率环的处理
 * 设计思路 ：1.根据实际负载计算速度/功率/电流的爬坡、速度环/功率环的PI       \
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
float Iq_Set = 0.8;
void StateRun(void)
{
    if (struAppCommData.bOpenRunFlag == 1) //DQ轴电流切换程序
    {
        if (struTime.nOpenRunCntr < OPEN2CLOSE_RUN_COV_TIME)
        {
            // 这段时间内，使Iq逐渐上升到开环拖动电流，Id逐渐降低为第一段定位电流。时间长短跟负载匹配。
            struTime.nOpenRunCntr++;
            /* D/Q轴电流变化斜率设置 */
            struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));
            struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(OPEN2CLOSE_RUN_CURRENT_RAMP));
        }
        else
        {
            //Id设置为0,根据实际设置，计算速度环或者功率环
            struTime.nOpenRunCntr = 0;
            struAppCommData.bOpenRunFlag = 2;

            struAppCommData.nCurrentACC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_ACC));
            struAppCommData.nCurrentDEC = App2CoreCurTrans(User2AppCurTrans(TORQUE_MODE_CURRENT_CHANGE_DEC));

            if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
            {
                struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));      //Q轴电流给定
            }
            else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
            {
                struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));    //Q轴电流给定
            }
        }
    }
    else if (struAppCommData.bOpenRunFlag == 2)
    {

        /*******************速度环处理*******************************/
#if (CLOSE_LOOP == SPEED_LOOP)
        {
            if (struTime.nLoopDelyTime < STATE04_WAITE_TIME)//速度环参数初始化时间，大部分应用设置为100ms
            {
                struTime.nLoopDelyTime ++;

                struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCur_Reference << 22;
                struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;
            }
            else
            {
                if (++struAppCommData.nLoopCntr >= SPEED_LOOP_CNTR)   //速度环环路计算周期。
                {
                    struAppCommData.nLoopCntr = 0;

                    if (struTime.nLoopDelyTime == STATE04_WAITE_TIME)  //首次进入速度环，速度环的初始值设置为Iq。
                    {
                        struTime.nLoopDelyTime ++;
                        struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCurrentSet << 22;
                        struMotorSpeed.wSpeedRef = struMotorSpeed.wSpeedfbk;
                    }

#if (POWER_LIMIT_STATUS == TRUE)          //限功率使能，速度环限制最大输出功率
                    {
                        PowerLimitCalc(&struMotorSpeed, struPower.wPowerValue);
                        struMotorSpeed.wSpeedSet = struMotorSpeed.wPowerLimitSpeedRef - struMotorSpeed.wPowerLimitSpeedSet;
                    }
#elif (POWER_LIMIT_STATUS == FALSE)
                    {
                        //调速指令，速度参考给定SPEED_SET 单位Hz
                        //struMotorSpeed.wSpeedSet = App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET)); //速度参考给定
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

                    SpeedLoop_Set(&struMotorSpeed); //速度爬坡

                    SpeedLoopReg(&struMotorSpeed);// 速度环PI计算

                }
            }
        }

        /*******************电流环处理*******************************/
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

        /*******************功率环处理*******************************/
#elif (CLOSE_LOOP == POWER_LOOP)
        {
            if (++struAppCommData.nLoopCntr >= POWER_LOOP_CNTR) //功率环环路计算周期。
            {
                struAppCommData.nLoopCntr = 0;

#if(SPPED_LIMIT_STATUS == TRUE)     //限速功能使能 ，功率环限制最大转速。
                {
                    SpeedLimitCalc(struMotorSpeed.wSpeedfbk, &struPower);  //限速度计算
                    
                    struPower.wPowerSet = struPower.wSpeedLimitPowerRef - struPower.wSpeedLimitPowerSet;      // 根据限速度函数的输出设置给定功率
                }
#elif (SPPED_LIMIT_STATUS == FALSE)
                {
                    struPower.wPowerSet = POWER_CALC(POWER_SET);//功率参考给定
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

                if (struAppCommData.nPowerFistFlag == 0) //首次进入速度环，功率环的初始值设置为Iq
                {
                    struAppCommData.nPowerFistFlag = 1;
                    struPower.struPowerRamp.wRef = struPower.wPowerValue;
                    struPower.struPowerPI.wIntegral = ((s32)struFOC_CurrLoop.nQCur_Reference << 16);
                }

                struPower.struPowerRamp.wSet = struPower.wPowerSet;
                struPower.wPowerRef = RampControl(&struPower.struPowerRamp);    //功率环爬坡函数

                PowerLoopReg(&struPower);                 //功率环PI计算
            }
        }
#endif
    }

    struFOC_CurrLoop.nDCurrentSet = 0;       //Id设置为0
    CurrentLoopAxisD_Set();
    CurrentLoopAxisQ_Set();

}

/*****************************************************************************
 * 函数名   : void StateFault(void)
 * 说明     : 故障状态，主要是进行故障状态的重启处理
 * 设计思路 ：1.在故障状态，如果检测到故障消失，则清零stru_Faults对应故障位。\
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateFault(void)
{
    struFOC_CtrProc.bMC_RunFlg = 0;

    PWMOutputs(DISABLE);

//    FaultRecover();    //故障恢复处理函数
}

/*****************************************************************************
 * 函数名   : void StateStop(void)
 * 说明     : 电机停止函数，判断电机是否静止状态
 * 设计思路 ：1.通过mOnTheFlyDetect.bMotorStopDecFlag的状态来判断电机是否在静止状态，在判定电机
                静止后去进行初始位置检测或者预定位动作
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateStop(void)
{
    if (mOnTheFlyDetect.bMotorStopDecFlag)  //电机停止标志位为1则判定电机为静止状态
    {
        if (struTime.nStopDelayCntr < STOP_TIME)   //电机停止时间滤波，连续STOP_TIME次电机处于静止状态则判定电机为静止状态
        {
            struTime.nStopDelayCntr ++;
            mOnTheFlyDetect.bMotorStopDecFlag = 0;
        }
        else
        {
            if (struTime.nStopDelayTime > 0)     //判定电机静止后的延时处理，消除判定误差，根据实际电机负载调整延迟时间
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

#if (SEEK_POSITION_STATUS == TRUE)  //如果初始位置检测状态使能，则进入初始位置检测状态
                {
                    struFOC_CtrProc.eSysState = POS_SEEK;
                    PWMOutputs(DISABLE);
                }
#elif (SEEK_POSITION_STATUS == FALSE) //如果初始位置检测状态不使能，则进入预定位状态，给定预定位角度
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
 函数名称：    void CurrentLoopAxisD_Set(void)
 功能描述：    电流环D轴电流参考斜坡给定
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
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
 函数名称：    void CurrentLoopAxisQ_Set(void)
 功能描述：    电流环Q轴电流参考斜坡给定
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
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
 函数名称：    void SpeedLoop_Set(MechanicalQuantity *this)
 功能描述：    速度参考斜坡函数
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/10/8      V1.0           Andrew Kong          创建
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
 * 函数名   : void StateRun(void)
 * 说明     : 电机Hallrun状态处理程序，一般为带hall传感器速度环的处理
 * 设计思路 ：hall速度闭环;电流环的电流输入给定;hall传感器切到无感速度运行
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void StateHallRun(void)
{
    /*****************************************hallTosensorless*********************************************/
#if (CLOSE_LOOP == SPEED_LOOP)
    {
        if (struTime.nLoopDelyTime < STATE04_WAITE_TIME)//速度环参数初始化时间，大部分应用设置为100ms
        {
            struTime.nLoopDelyTime ++;
            if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
            {
                struMotorSpeed.mSpeedPI.wIntegral = (s64)struFOC_CurrLoop.nQCur_Reference << 22;
            }
            else
            {
                struMotorSpeed.mSpeedPI.wIntegral = -(s64)struFOC_CurrLoop.nQCur_Reference << 22;
            }

            struMotorSpeed.wSpeedRef = struHallProcess.wSpeedfbkHall;
        }
        else
        {
            if (++struAppCommData.nLoopCntr >= SPEED_LOOP_CNTR)   //速度环环路计算周期。
            {
                struAppCommData.nLoopCntr = 0;

#if (POWER_LIMIT_STATUS == TRUE)          //限功率使能，速度环限制最大输出功率
                {
                    PowerLimitCalc(&struMotorSpeed, struPower.wPowerValue);
                    struMotorSpeed.wSpeedSet = struMotorSpeed.wSpeedSet - struMotorSpeed.wPowerLimitSpeedSet;
                }
#elif (POWER_LIMIT_STATUS == FALSE)
                {
                    if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
                    {
                        //调速指令，速度参考给定SPEED_SET 单位Hz
                        //struMotorSpeed.wSpeedSet = App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET));
                        struMotorSpeed.wSpeedSet = struAppCommData.wSpeedValue;
                    }
                    else
                    {
                        //struMotorSpeed.wSpeedSet = -App2CoreFreqTrans(User2AppFreqTrans(SPEED_SET));
                        struMotorSpeed.wSpeedSet = -struAppCommData.wSpeedValue;
                    }
                }
#endif      //输入速度限幅  
                //struMotorSpeed.wSpeedSet = sat(struMotorSpeed.wSpeedSet,-App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MAX_FAULT)),App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MAX_FAULT)));

            }
        }

        //爬坡加减速的设置
        struMotorSpeed.wSpeedRampACCStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_ACC));
        struMotorSpeed.wSpeedRampDECStep = App2CoreFreqTrans(User2AppFreqTrans(SPEED_RUN_DEC));
        //速度爬坡 Ramp
        SpeedLoop_Set(&struMotorSpeed);

        //输出的电流值 currentSet 经过电流加减速的作用实际传到电流环
        SpeedLoopReg(&struMotorSpeed);// 速度环PI计算

    }
    /*******************电流环处理*******************************/
#elif (CLOSE_LOOP == CURRENT_LOOP)
    {
        //正反方向给定iq
        if (struFOC_CtrProc.bMotorDirtionCtrl == CW)
        {
            struFOC_CurrLoop.nQCurrentSet = App2CoreCurTrans(User2AppCurTrans(IQ_SET));
        }
        else if (struFOC_CtrProc.bMotorDirtionCtrl == CCW)
        {
            struFOC_CurrLoop.nQCurrentSet = -App2CoreCurTrans(User2AppCurTrans(IQ_SET));
        }

        //Id设置为0
        struFOC_CurrLoop.nDCurrentSet = 0;
    }
#endif

    //DQ轴电流给定值变化斜率
    CurrentLoopAxisD_Set();
    CurrentLoopAxisQ_Set();
}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */



