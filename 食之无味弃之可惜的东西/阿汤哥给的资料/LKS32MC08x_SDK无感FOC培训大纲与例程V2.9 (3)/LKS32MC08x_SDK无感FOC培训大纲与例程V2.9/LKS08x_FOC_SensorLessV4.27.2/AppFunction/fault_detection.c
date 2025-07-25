/*******************************************************************************
 * 版权所有 (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * 文件名称： fault_detection.c
 * 文件标识：
 * 内容摘要： fault detectiong: volt ,current, temperature, stall, empty, phaseloss,\
 *
 * 其它说明： 无
 * 当前版本： V1.0
 * 作    者： andrew kong
 * 完成日期： 2019年11月28日
 *
 *******************************************************************************/
#include "fault_detection.h"
#include "MC_type.h"
#include "MC_parameter.h"
#include "Global_Variable.h"
#include "state_machine.h"
#include "FOC_Drive.h"

stru_CurrentAmplitude_t struCurrentAmplitude;    //缺相检测电流幅值结构体
stru_CurrPhaseUVW   struImaxCurrent;             //缺相检测相电流最大值结构体

stru_FaultVariable_t stru_FaultValue;
volatile stru_FaultStatus_t stru_Faults;

stru_FaultTime_t struFaultVoltTime;
stru_FaultTime_t struFaultEmptyTime;
stru_FaultTime_t struFaultStallTime;
stru_FaultTime_t struFaultCurrentTime;
stru_FaultTime_t struFaultTempTime;
stru_FaultTime_t struFaultPhaseTime;
stru_FaultTime_t struFaultCurrentTime;
stru_FaultTime_t struFaultStartTime;
/*****************************************************************************
 * 函数名   : void FaultCurrentCheck(s16 Ia, s16 Ib, s16 Ic)
 * 说明     : 软件过流检测
 * 设计思路 ：1. 如果三相电流绝对值超过设定的软件过流值，则判定为软件过流。
 * 参数     ：Ia、Ib、Ic
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void FaultCurrentCheck(stru_CurrPhaseUVW *pstruCurrent)
{
    if((ABS(pstruCurrent->nPhaseU) > stru_FaultValue.nOverCurrent) || (ABS(pstruCurrent->nPhaseV) > stru_FaultValue.nOverCurrent) || (ABS(pstruCurrent->nPhaseW) > stru_FaultValue.nOverCurrent))
    {
        PWMOutputs(DISABLE);
        stru_Faults.B.SoftCurretError = 1;
    }

}

/*****************************************************************************
 * 函数名   : void FaultVoltageCheck(s32 Udcb, stru_stru_FaultTime_t *FaultTime)
 * 说明     : 过欠压检测 
 * 设计思路 ：1.当母线电压大于过压设定值则判定为过压故障;当母线电压小于欠压设定值 \
 *              则判定为欠压故障。滤波时间为1s，这个可根据实际需求修改。
 * 参数     ：Udcb，stru_stru_FaultTime_t
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void FaultVoltageCheck(s32 Udcb, stru_FaultTime_t *pFaultTime)
{
    if(Udcb >= stru_FaultValue.nOverVoltage)    //母线电压大于过压设定值则时间累加
    {
        pFaultTime->nCheckCnt1 ++;
    }
    else if(Udcb < stru_FaultValue.nUnderVoltage) // 母线电压小于欠压设定值则时间累加
    {
        pFaultTime->nCheckCnt2 ++;
    }
    else
    {
        pFaultTime->nCheckCnt1 = 0;
        pFaultTime->nCheckCnt2 = 0;
    }

    if(pFaultTime->nCheckCnt1 >= 200)//时间超过1s则判定为过压
    {
        PWMOutputs(DISABLE);
        pFaultTime->nCheckCnt1 = 0;
        stru_Faults.B.VoltOverError = 1;//过压错误标志位置1
    }

    if(pFaultTime->nCheckCnt2 >= 200)//时间超过1s则判定为欠压
    {
        PWMOutputs(DISABLE);
        pFaultTime->nCheckCnt2 = 0;
        stru_Faults.B.VoltUnderError = 1;//欠压错误标志位置1
    }
}


/*****************************************************************************
 * 函数名   : FaultEmptyCheck(s16 Iq, s32 wSpeedFbk, stru_stru_FaultTime_t *FaultTime)
 * 说明     : 离水空转检测 
 * 设计思路 ：1.如果Iq小于空转电流设定值，并且转速大于空转转速设定值，则判定为离水空转。\
 *              滤波时间为1s，这个可根据实际需求修改。 
 * 参数     ：s16 nIq, s32 wSpeedFbk, stru_FaultTime_t *pstruFaultTime
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void FaultEmptyCheck(s16 nIq, s32 wSpeedFbk, stru_FaultTime_t *pstruFaultTime)
{
    if((ABS(nIq) < stru_FaultValue.nEmptyCurrent) && (ABS(wSpeedFbk) > stru_FaultValue.wEmptySpeed)) //如果Iq小于设定值，转速大于设定值则时间累加，否则时间清0
    {
        pstruFaultTime->nCheckCnt1 ++;
    }
    else
    {
        pstruFaultTime->nCheckCnt1 = 0;
    }

    if(pstruFaultTime->nCheckCnt1 >= 200)  //累加时间超过200 * 5ms = 1s则判定为离水空转
    {
        PWMOutputs(DISABLE);
        pstruFaultTime->nCheckCnt1 = 0;
        stru_Faults.B.EmptyError = 1;       //空转错误标志位置1
    }
}

/*****************************************************************************
 * 函数名   : FaultStallCheck(s32 SpeedFbk, s16 Iq, stru_CurrPhaseUVW *pIabcMax, stru_stru_FaultTime_t *FaultTime)
 * 说明     : 堵转检测 
 * 设计思路 ：堵转检测目前分为三种办法
 *            1.电机转速大于设定最大值或者小于设定最小值，则判定为堵转 ;                 \
 *            2.Iq大于设定值，转速小于设定值则判断堵转。主要思路为电流和转速正相关;      \
 *            3.三相电流幅值大于堵转电流设定值则判断为堵转。
 * 参数     ：s32 nSpeedFbk, s16 bIq, stru_CurrPhaseUVW *pstruIabcMax, stru_FaultTime_t *pstruFaultTime
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void FaultStallCheck(s32 nSpeedFbk, s16 bIq, stru_CurrPhaseUVW *pstruIabcMax, stru_FaultTime_t *pstruFaultTime)
{
    if(pstruFaultTime->nStartDelay < 40) //延时5ms *40 = 200ms，避开开环拖动的影响
    {
        pstruFaultTime->nStartDelay ++;
    }
    else
    {
        if((ABS(nSpeedFbk) > stru_FaultValue.wStallSpeedMax) || (ABS(nSpeedFbk) < stru_FaultValue.wStallSpeedMin))  //电机转速在实际运行范围之外则判定为堵转
        {
            if(pstruFaultTime->nCheckCnt1 < 100)      //滤波时间500ms 100*5ms
            {
                pstruFaultTime->nCheckCnt1 ++;
            }
            else                                 //判定为电机堵转
            {                                   
                PWMOutputs(DISABLE);
                pstruFaultTime->nCheckCnt1 = 0;
                stru_Faults.B.StallError = 1;         //堵转错误标志位置1
                stru_FaultValue.nStallFlag = 1;        //堵转标志位为1，用于区分触发了哪种堵转
            }
        }
        else
        {
            pstruFaultTime->nCheckCnt1 = 0;
        }

        if((ABS(nSpeedFbk) < stru_FaultValue.wStallSpeed) && (ABS(bIq) > stru_FaultValue.nStallCurrentIq)) //Iq和转速不匹配，Iq大、转速低，则判定为堵转。
        {
            if(pstruFaultTime->nCheckCnt2 < 400)      //滤波时间2s，400*5ms = 2000ms
            {
                pstruFaultTime->nCheckCnt2 ++;
            }
            else                                 //判定为电机堵转 
            {
                PWMOutputs(DISABLE);
                pstruFaultTime->nCheckCnt2 = 0;
                stru_Faults.B.StallError = 1;         //堵转错误标志位置1
                stru_FaultValue.nStallFlag = 2;       //堵转标志位为2，用于区分触发了哪种堵转
            }
        }
        else
        {
            pstruFaultTime->nCheckCnt2 = 0;
        }

        if((pstruIabcMax->nPhaseU > stru_FaultValue.nStallCurrent) || (pstruIabcMax->nPhaseV > stru_FaultValue.nStallCurrent) || (pstruIabcMax->nPhaseW > stru_FaultValue.nStallCurrent))//三相电流幅值超过堵转电流设定值则判定为堵转
        {
            PWMOutputs(DISABLE);
            stru_Faults.B.StallError = 1;            //堵转错误标志位置1
            stru_FaultValue.nStallFlag = 3;          //堵转标志位为3，用于区分触发了哪种堵转
        }

    }
}

/*****************************************************************************
 * 函数名   : FaultSencondStartCheck(s32 wSpeedFbk, s16 nIq, stru_CurrPhaseUVW *pstruIabcMax, stru_FaultTime_t *pstruFaultTime)
 * 说明     : 二次启动
 * 设计思路 ：二次启动，在进入启动5s内判断，时间根据实际修改
 *          ：1.进入open之后2.5s内还没进入闭环，时间要根据实际情况修改                 \
 *          ：2.转速过高或者过低，设定阈值和堵转检测相同                             \
 *          : 3.三相电流幅值大于电流设定值，设定值和堵转检测相同
 * 参数     ：s32 wSpeedFbk, s16 nIq, stru_CurrPhaseUVW *pstruIabcMax, stru_FaultTime_t *pstruFaultTime
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 *****************************************************************************/
void FaultStartCheck(s32 wSpeedFbk, s16 nIq, stru_CurrPhaseUVW *pstruIabcMax, stru_FaultTime_t *pstruFaultTime)
{
    if(pstruFaultTime->nStartDelay < 2000)                       //开环10s内检测，10s后不再检测  5ms*2000
    {
        pstruFaultTime->nStartDelay ++;

        if(struFOC_CtrProc.eSysState == RUN)
        {
            if((ABS(wSpeedFbk) > stru_FaultValue.wStallSpeedMax) || (ABS(wSpeedFbk) < stru_FaultValue.wStallSpeedMin))  //电机转速在实际运行范围之外则判定为堵转
            {
                if(pstruFaultTime->nCheckCnt1 < 40)             //滤波时间200ms 40*5ms
                {
                    pstruFaultTime->nCheckCnt1 ++;
                }
                else                                          //判定为启动错误
                {
                    PWMOutputs(DISABLE);
                    pstruFaultTime->nCheckCnt1 = 0;
                    stru_Faults.B.StartError = 1;               //启动错误标志位置1
                    stru_FaultValue.nStartFlag = 1;             //启动错误标志位为1
                }
            }
            else
            {
                pstruFaultTime->nCheckCnt1 = 0;
            }

        
            if((ABS(wSpeedFbk) < stru_FaultValue.wStallSpeed) && (ABS(nIq) > stru_FaultValue.nStallCurrentIq)) //Iq和转速不匹配，Iq大、转速低，则判定为堵转。
            {
                if(pstruFaultTime->nCheckCnt2 < 200)      //滤波时间1s，200*5ms = 1s
                {
                    pstruFaultTime->nCheckCnt2 ++;
                }
                else                                 //判定为启动错误
                {
                    PWMOutputs(DISABLE);
                    pstruFaultTime->nCheckCnt2 = 0;
                    stru_Faults.B.StartError = 1;         //启动错误标志位置1
                    stru_FaultValue.nStartFlag = 2;       //启动错误标志位为2，
                }
            }
            else
            {
                pstruFaultTime->nCheckCnt2 = 0;
            }
        }

        if((pstruIabcMax->nPhaseU > stru_FaultValue.nStallCurrent) || (pstruIabcMax->nPhaseV > stru_FaultValue.nStallCurrent) || (pstruIabcMax->nPhaseW > stru_FaultValue.nStallCurrent))//三相电流幅值超过堵转电流设定值则判定为堵转
        {
            PWMOutputs(DISABLE);
            stru_Faults.B.StartError = 1;               //启动错误标志位置1
            stru_FaultValue.nStartFlag = 3;             //启动错误标志位为3
        }
        
        if((struFOC_CtrProc.eSysState == OPEN_RUN)&&(pstruFaultTime->nStartDelay > 600))     //开环600*5ms = 3s后还没切入闭环，则判断为启动失败，具体时间按照实际项目调整
        {
            PWMOutputs(DISABLE);
            stru_Faults.B.StartError = 1;              //启动错误标志位置1
            stru_FaultValue.nStartFlag = 4;            //启动错误标志位为4
        }
    }

}

/***********************************************************************************
 * 函数名   : FaultPhaseCheck(stru_CurrPhaseUVW  *pstruImaxCurrent,stru_CurrentAmplitude_t *pstruCurrentAmplitude, stru_FaultTime_t *pstruFaultTime)
 * 说明     : 缺相检测 
 * 设计思路 ：1.三相电流幅值相差3倍，则判定为电机缺相，检测时间是2S，时间可以根据项目实际频率调整
 *            2.三相电流的积分值过小，持续1s，判定为缺相
 * 参数     ：stru_CurrPhaseUVW  *pstruImaxCurrent,stru_CurrentAmplitude_t *pstruCurrentAmplitude, stru_FaultTime_t *pstruFaultTime
 * 返回值   ：无
 * 修改时间 ：2020.08.17

 * 修改时间 : 2020.10.09 
 * 修改内容 ：调整函数传参
 ***********************************************************************************/
void FaultPhaseCheck(stru_CurrPhaseUVW  *pstruImaxCurrent,stru_CurrentAmplitude_t *pstruCurrentAmplitude, stru_FaultTime_t *pstruFaultTime)
{
    if(pstruFaultTime->nCheckCnt1 < 400)//400*5ms 
    {
        pstruFaultTime->nCheckCnt1 ++;
    }
    else
    {
        pstruFaultTime->nCheckCnt1 = 0;
        
        if(((pstruImaxCurrent->nPhaseU >> 2) > pstruImaxCurrent->nPhaseV) || (((pstruImaxCurrent->nPhaseV >> 2) > pstruImaxCurrent->nPhaseW)) ||
          (((pstruImaxCurrent->nPhaseW >> 2) > pstruImaxCurrent->nPhaseU)))            //如果三相电流幅值相差3倍则判定为缺相，倍数需要根据实际负载调整
        {
            PWMOutputs(DISABLE);
            stru_Faults.B.PhaseLossError = 1;//缺相错误标志位置1
            stru_FaultValue.nPhaseLossFlag = 1;//第一种缺相保护
        }
        
        pstruImaxCurrent->nPhaseU = 0;
        pstruImaxCurrent->nPhaseV = 0;
        pstruImaxCurrent->nPhaseW = 0;
    }

    if((pstruCurrentAmplitude->nPhA < I_PHASE_LOSS_FAULT)||(pstruCurrentAmplitude->nPhB < I_PHASE_LOSS_FAULT)||(pstruCurrentAmplitude->nPhC < I_PHASE_LOSS_FAULT)) //缺三相
    {
        if(pstruFaultTime->nCheckCnt2 < 50)     //滤波时间为200*0.5s = 1s
        {
            pstruFaultTime->nCheckCnt2 ++;
        }
        else
        {
            pstruFaultTime->nCheckCnt2 = 0;
            PWMOutputs(DISABLE);
            stru_Faults.B.PhaseLossError = 1;//缺相错误标志位置1
            stru_FaultValue.nPhaseLossFlag = 2;//第二种缺相保护
        }
    }
    else
    {
        pstruFaultTime->nCheckCnt2 = 0;
    }
}

/***********************************************************************************
 * 函数名   : FaultTempCheck(s16 nTemperature, stru_FaultTime_t *pFaultTime)
 * 说明     : 温度保护检测，针对NTC设计，对于PTC则需要修改 
 * 设计思路 ：1.根据NTC的阻值去判断是否发生过温故障
 * 参数     ：s16 nTemperature, stru_FaultTime_t *pFaultTime
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 ***********************************************************************************/
void FaultTempCheck(s16 nTemperature, stru_FaultTime_t *pstruFaultTime)
{
    if((nTemperature < TEMP_FAULT) || (nTemperature > TEMP_BREAK))    //NTC阻值小于设定值，则判定为过温故障;NTC阻值大于NTC开路值，则判定为NTC开路
    {
        pstruFaultTime->nCheckCnt1 ++;
    }
    else
    {
        pstruFaultTime->nCheckCnt1 = 0;
    }

    if(pstruFaultTime->nCheckCnt1 > 400)//滤波时间2s，400*5ms = 2000ms
    {
        PWMOutputs(DISABLE);
        stru_Faults.B.TempOverError = 1;//温度故障标志位置1
        pstruFaultTime->nCheckCnt1 = 0;
    }
}


/***********************************************************************************
 * 函数名   : FaultInit(void)
 * 说明     : 错误检测变量初始化
 * 设计思路 ：1.变量初始化
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 ***********************************************************************************/
void FaultInit(void)
{
    /********过流故障检测变量初始化*****/
    stru_FaultValue.nOverCurrent = App2CoreCurTrans(User2AppCurTrans(I_PH_OVERCURRENT_FAULT));
    struFOC_CurrLoop.mStatCurrUVW.nPhaseU = 0;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseV = 0;
    struFOC_CurrLoop.mStatCurrUVW.nPhaseW = 0;

    /******过欠压故障检测变量初始化*********/
    struFaultVoltTime.nCheckCnt1 = 0;
    struFaultVoltTime.nCheckCnt2 = 0;
    stru_FaultValue.nOverVoltage = User2AppVolTrans(U_DCB_OVERVOLTAGE_FAULT);
    stru_FaultValue.nUnderVoltage = User2AppVolTrans(U_DCB_UNDERVOLTAGE_FAULT);

    /******离水空转故障检测变量初始化********/
    struFaultEmptyTime.nCheckCnt1 = 0;
    stru_FaultValue.nEmptyCurrent = App2CoreCurTrans(User2AppCurTrans(I_PH_EMPTY_FAULT));
    stru_FaultValue.wEmptySpeed = App2CoreFreqTrans(User2AppFreqTrans(SPEED_EMPTY_FAULT));

    /******堵转故障检测变量初始化*********/
    struFaultStallTime.nCheckCnt1 = 0;
    struFaultStallTime.nCheckCnt2 = 0;
    struFaultStallTime.nStartDelay = 0;
    stru_FaultValue.nStallCurrent = App2CoreCurTrans(User2AppCurTrans(I_PH_STALL_FAULT));
    stru_FaultValue.wStallSpeedMax = App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MAX_FAULT));
    stru_FaultValue.wStallSpeedMin = App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_MIN_FAULT));
    stru_FaultValue.nStallCurrentIq = App2CoreCurTrans(User2AppCurTrans(IQ_STALL_FAULT));
    stru_FaultValue.wStallSpeed =  App2CoreFreqTrans(User2AppFreqTrans(SPEED_STALL_FAULT));
    stru_FaultValue.nStallFlag = 0;

    /******缺相故障检测变量初始化*********/
    struFaultPhaseTime.nCheckCnt1 = 0;
    
    struCurrentAmplitude.bFlag = 0;
    struCurrentAmplitude.nPhA = 0;
    struCurrentAmplitude.nPhB = 0;
    struCurrentAmplitude.nPhC = 0;
    struCurrentAmplitude.bTheta = 0;
    struCurrentAmplitude.wPhATemp = 0;
    struCurrentAmplitude.wPhBTemp = 0;
    struCurrentAmplitude.wPhCTemp = 0;

    struImaxCurrent.nPhaseU = 0;
    struImaxCurrent.nPhaseV = 0;
    struImaxCurrent.nPhaseW = 0;
    
    /*****温度故障检测变量初始化****/
    struFaultTempTime.nCheckCnt1 = 0;

    /*启动故障检测变量初始化*/ 
    struFaultStartTime.nCheckCnt1 = 0;
    struFaultStartTime.nCheckCnt2 = 0;
    struFaultStartTime.nStartDelay = 0;
}


/***********************************************************************************
 * 函数名   : FaultCheck(void)
 * 说明     : 故障检测处理
 * 设计思路 ：1.每个故障检测的处理周期为5ms
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 ***********************************************************************************/
void FaultCheck(void)
{
    static s16 t_nFlagCheck = 0;

    if(t_nFlagCheck < 5)
    {
        t_nFlagCheck ++;
    }
    else
    {
        t_nFlagCheck = 1;
    }


    switch(t_nFlagCheck)
    {
        case 1:
        { 
            FaultVoltageCheck(struFOC_CurrLoop.nBusVoltage, &struFaultVoltTime);                                        //过欠压检测
            break;
        }
        case 2:
        {
            if(struFOC_CtrProc.eSysState == RUN)          
            {
//                FaultStallCheck(struMotorSpeed.wSpeedfbk, struFOC_CurrLoop.mStatCurrDQ.nAxisQ, &struImaxCurrent, &struFaultStallTime);  //堵转检测
            }
            break;
        }
        case 3:
        {
//            if(struFOC_CtrProc.eSysState == RUN)
//            {
//                FaultEmptyCheck(struFOC_CurrLoop.mStatCurrDQ.nAxisQ, struMotorSpeed.wSpeedfbk, &struFaultEmptyTime);  //离水空转检测
//            }
//            
//            FaultTempCheck(struAppCommData.nTempADC, &struFaultTempTime);                                             //过温检测
            break;
        }
        case 4:
        {
            if(struFOC_CtrProc.eSysState == RUN)
            {
//                FaultPhaseCheck(&struImaxCurrent,&struCurrentAmplitude, &struFaultPhaseTime);                                    //缺相检测
            }
            break;
        }
        case 5:
        {
 
            if((struFOC_CtrProc.eSysState == OPEN_RUN)||(struFOC_CtrProc.eSysState == RUN))
            {
//                FaultStartCheck(struMotorSpeed.wSpeedfbk, struFOC_CurrLoop.mStatCurrDQ.nAxisQ, &struImaxCurrent, &struFaultStartTime);//二次启动检测
            }
            break;
        }
        default:
            break;
    }
}

/***********************************************************************************
 * 函数名   : FaultRecoverInit(void)
 * 说明     : 故障恢复变量初始化
 * 设计思路 ：变量初始化
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 ***********************************************************************************/
void FaultRecoverInit(void)
{
    stru_Faults.R = 0;
    struFaultEmptyTime.nRecoverCntr   = 0;
    struFaultStallTime.nRecoverCntr   = 0;
    struFaultCurrentTime.nRecoverCntr = 0;
    struFaultCurrentTime.nRecoverTime = 0;
    struFaultVoltTime.nRecoverCntr    = 0;
    struFaultTempTime.nRecoverCntr    = 0;
    stru_FaultValue.nOverVoltageRecover = User2AppVolTrans(U_DCB_OVERVOLTAGE_RECOVER);
    stru_FaultValue.nUnderVoltageRecover = User2AppVolTrans(U_DCB_UNDERVOLTAGE_RECOVER);
}

/***********************************************************************************
 * 函数名   : FaultRecover(void)
 * 说明     : 故障恢复程序处理
 * 设计思路 ：1.过欠压恢复：       母线电压在正常工作范围内，持续2s则清除过欠压故障标志位; \
 *            2.软件&硬件过流恢复：2s后清除过流故障标志位;                                 \
 *            3.堵转恢复：         2s后清除堵转故障标志位;                                 \
 *            4.缺相恢复：         2s后清除过流故障标志位;                                 \
 *            5.过温恢复：         NTC在正常工作范围内，持续2s则清除过温故障标志位; \
 *            6.启动恢复：         1s后重启
 * 参数     ：无
 * 返回值   ：无
 * 修改时间 ：2020.08.17
 ***********************************************************************************/
void FaultRecover(void)
{       
    /****************过欠压恢复****************/
    if((stru_Faults.B.VoltOverError == 1) || (stru_Faults.B.VoltUnderError == 1))
    {   //母线电压在欠压恢复值和过压恢复值之间则判定为正常状态
        if((struFOC_CurrLoop.nBusVoltage >= stru_FaultValue.nUnderVoltageRecover) 
          && (struFOC_CurrLoop.nBusVoltage <= stru_FaultValue.nOverVoltageRecover))
        {
            if(struFaultVoltTime.nRecoverCntr < VOLT_FAULT_RECOVER_TIME)     //滤波时间2s，然后清零过欠压故障标志位
            {
                struFaultVoltTime.nRecoverCntr ++;
            }
            else
            {
                stru_Faults.B.VoltOverError = 0;
                stru_Faults.B.VoltUnderError = 0;
                struFaultVoltTime.nRecoverCntr = 0;
            }
        }
        else
        {
            struFaultVoltTime.nRecoverCntr = 0;
        }
    }

    /****************过流恢复****************/
    if((stru_Faults.B.HardCurretError == 1) || (stru_Faults.B.SoftCurretError == 1))
    {
        if(struFaultCurrentTime.nRecoverCntr < CURRENT_FAULT_RECOVER_TIME)//2s后清零过流故障标志位
        {
            struFaultCurrentTime.nRecoverCntr ++;
        }
        else
        {
            if(stru_Faults.B.HardCurretError == 1)
            {
                stru_Faults.B.HardCurretError = 0;
            }

            if(stru_Faults.B.SoftCurretError == 1)
            {
                stru_Faults.B.SoftCurretError = 0;
            }

            struFaultCurrentTime.nRecoverTime ++;
            struFaultCurrentTime.nRecoverCntr = 0;
        }
    }

    /****************堵转恢复****************/
    if(stru_Faults.B.StallError == 1)
    {
        if(struFaultStallTime.nRecoverCntr < STALL_FAULT_RECOVER_TIME)   //2s后清零堵转故障标志位
        {
            struFaultStallTime.nRecoverCntr ++;
        }
        else
        {
            stru_Faults.B.StallError = 0;
            struFaultStallTime.nRecoverCntr = 0;
            stru_FaultValue.nStallFlag = 0;
        }
    }

    /****************离水空转恢复****************/
    if(stru_Faults.B.EmptyError == 1)
    {
        if(struFaultEmptyTime.nRecoverCntr < EMPTY_FAULT_RECOVER_TIME)//2s后清零离水空转故障标志位
        {
            struFaultEmptyTime.nRecoverCntr ++;
        }
        else
        {
            stru_Faults.B.EmptyError = 0;
            struFaultEmptyTime.nRecoverCntr = 0;
        }
    }

    /****************缺相恢复****************/
    if(stru_Faults.B.PhaseLossError == 1)
    {
        if(struFaultPhaseTime.nRecoverCntr < PHASELOSS_FAULT_RECOVER_TIME) //2s后清零缺相故障标志位
        {
            struFaultPhaseTime.nRecoverCntr ++;
        }
        else
        {
            stru_Faults.B.PhaseLossError = 0;
            struFaultPhaseTime.nRecoverCntr = 0;
        }
    }

    /****************过温恢复****************/
    if(stru_Faults.B.TempOverError == 1)
    {
        if((struAppCommData.nTempADC > TEMP_RECOVER) && (struAppCommData.nTempADC < TEMP_BREAK))//NTC阻值在设定阻值之间则判定为正常状态 
        {
            if(struFaultTempTime.nRecoverCntr < TEMP_FAULT_RECOVER_TIME)//滤波时间2s，然后清零过温故障标志位
            {
                struFaultTempTime.nRecoverCntr ++;
            }
            else
            {
                stru_Faults.B.TempOverError = 0;
                struFaultTempTime.nRecoverCntr = 0;
            }
        }
        else
        {
            struFaultTempTime.nRecoverCntr = 0;
        }
    }

    /****************启动失败恢复****************/
    if(stru_Faults.B.StartError == 1)
    {
        if(struFaultStartTime.nRecoverCntr < START_FAULT_RECOVER_TIME) //1s后清零缺相故障标志位
        {
            struFaultStartTime.nRecoverCntr ++;
        }
        else
        {
            stru_Faults.B.StartError = 0;
            struFaultStartTime.nRecoverCntr = 0;
            stru_FaultValue.nStartFlag = 0;
        }
    }

}

/***********************************************************************************
 * 函数名   : CurrentAmplitudeCalc(stru_CurrPhaseUVW *pstruCurrPhaseUVW,stru_SWLIBS_3wSyst *pCurrentAmplitude )
 * 说明     : 电流有效值计算， 
 * 设计思路 ：半个电流周期内对电流积分
 * 参数     ：stru_CurrPhaseUVW *pstruCurrPhaseUVW, stru_SWLIBS_3wSyst *pCurrentAmplitude
 * 返回值   ：无
 * 修改时间 ：2020.10.9
 ***********************************************************************************/
void CurrentAmplitudeCalc(stru_CurrPhaseUVW *pstruCurrPhaseUVW, stru_CurrentAmplitude_t *pstruCurrentAmplitude, stru_CurrPhaseUVW *pstruCurrentMAX)
{
    if((pstruCurrentAmplitude->bTheta > 0) && (pstruCurrentAmplitude->bTheta < 32768))    //0° < Theta < 180°
    {
         if(pstruCurrentAmplitude->bFlag == 0)
         {
             pstruCurrentAmplitude->bFlag = 1;
             pstruCurrentAmplitude->wPhATemp = 0;
             pstruCurrentAmplitude->wPhBTemp = 0;
             pstruCurrentAmplitude->wPhCTemp = 0;
         }
         
         pstruCurrentAmplitude->wPhATemp = pstruCurrentAmplitude->wPhATemp + ABS(pstruCurrPhaseUVW->nPhaseU);
         pstruCurrentAmplitude->wPhBTemp = pstruCurrentAmplitude->wPhBTemp + ABS(pstruCurrPhaseUVW->nPhaseV);
         pstruCurrentAmplitude->wPhCTemp = pstruCurrentAmplitude->wPhCTemp + ABS(pstruCurrPhaseUVW->nPhaseW);
         
         pstruCurrentAmplitude->wPhATemp = sat(pstruCurrentAmplitude->wPhATemp,0x0000000F,0xFFFFFFF0); //数字量超出范围，则不再累加
         pstruCurrentAmplitude->wPhBTemp = sat(pstruCurrentAmplitude->wPhBTemp,0x0000000F,0xFFFFFFF0); //数字量超出范围，则不再累加
         pstruCurrentAmplitude->wPhCTemp = sat(pstruCurrentAmplitude->wPhCTemp,0x0000000F,0xFFFFFFF0); //数字量超出范围，则不再累加
    }
    else
    {
        pstruCurrentAmplitude->bFlag = 0;
        pstruCurrentAmplitude->nPhA = pstruCurrentAmplitude->wPhATemp * struCore2App.cur >> struCore2App.curShftNum;
        pstruCurrentAmplitude->nPhB = pstruCurrentAmplitude->wPhBTemp * struCore2App.cur >> struCore2App.curShftNum;
        pstruCurrentAmplitude->nPhC = pstruCurrentAmplitude->wPhCTemp * struCore2App.cur >> struCore2App.curShftNum;
    }
    
    
    /***********取三相电流最大值，缺相检测里面会用到************/
    if(ABS(pstruCurrPhaseUVW->nPhaseU) > pstruCurrentMAX->nPhaseU)
    {
        pstruCurrentMAX->nPhaseU = ABS(pstruCurrPhaseUVW->nPhaseU);
    }

    if(ABS(pstruCurrPhaseUVW->nPhaseV) > pstruCurrentMAX->nPhaseV)
    {
        pstruCurrentMAX->nPhaseV = ABS(pstruCurrPhaseUVW->nPhaseV);
    }

    if(ABS(pstruCurrPhaseUVW->nPhaseW) > pstruCurrentMAX->nPhaseW)
    {
        pstruCurrentMAX->nPhaseW = ABS(pstruCurrPhaseUVW->nPhaseW);
    }
}
 
 

