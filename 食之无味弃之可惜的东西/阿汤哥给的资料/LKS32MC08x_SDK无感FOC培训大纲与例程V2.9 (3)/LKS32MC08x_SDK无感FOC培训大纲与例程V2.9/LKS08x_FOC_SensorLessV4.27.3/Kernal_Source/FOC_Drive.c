/*******************************************************************************
 * 版权所有 (C)2016, LINKO Semiconduct
 *
 * 文件名称： FOC.Drive.c
 * 文件标识：
 * 内容摘要： 电机矢量控制电流内环及关键控制算法
 * 其它说明： 无
 * 当前版本： V 2.0
 * 作    者： WhenCai Zhao Howlet Li
 * 完成日期： 2020年9月10日
 *
 * 修改记录2：
 *    修改日期：2020年9月10日
 *    版 本 号：V2.0
 *    修 改 人：Howlet Li
 *    修改内容：格式整理
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "FOC_Drive.h"
#include "global_variable.h"
#include "hardware_config.h"
#include "mc_math.h"
#include "MC_Parameter.h"



/*******************************************************************************
 函数名称：    void AdcSampleCal(stru_FOC_CtrProcDef *this)
 功能描述：    电流采样
 输入参数：    stru_FOC_CtrProcDef *this 结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：    1.电流采样，采集AB两相电流
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0            Wencai.Zhao Howlet Li          创建
 *******************************************************************************/
void AdcSampleCal(stru_FOC_CurrLoopDef *this)
{
    stru_CurrPhaseUVW t_StatCurrUV;

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
    t_StatCurrUV = SVPWM_1ShuntGetPhaseCurrent(this); /* 读取ADC采样到的相电流值 */
#else

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
    t_StatCurrUV = SVPWM_2ShuntGetPhaseCurrent(this); /* 读取ADC采样到的相电流值 */
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
    t_StatCurrUV = SVPWM_3ShuntGetPhaseCurrent(this); /* 读取ADC采样到的相电流值 */
#endif
#endif
#endif

    /* 电流标幺化处理 */
			this->mStatCurrUVW.nPhaseU = _IQ15mpy(t_StatCurrUV.nPhaseU, struFluxOB_Param.CurrentPerUnitGAIN);
			this->mStatCurrUVW.nPhaseV = _IQ15mpy(t_StatCurrUV.nPhaseV, struFluxOB_Param.CurrentPerUnitGAIN);
			this->mStatCurrUVW.nPhaseW = -(this->mStatCurrUVW.nPhaseU + this->mStatCurrUVW.nPhaseV);
    /* clark变换 */
	  if(struFOC_CtrProc.bMotorDirtionCtrlPhase == CW)
		{
       this->mStatCurrAlfaBeta.nBeta = _IQ15mpy(18919, (this->mStatCurrUVW.nPhaseV - this->mStatCurrUVW.nPhaseW));
    }else{
			 this->mStatCurrAlfaBeta.nBeta = _IQ15mpy(18919, (this->mStatCurrUVW.nPhaseW - this->mStatCurrUVW.nPhaseV));
		}
		this->mStatCurrAlfaBeta.nAlph = this->mStatCurrUVW.nPhaseU;
}

/*******************************************************************************
 函数名称：    void FOC_Model(stru_FOC_CtrProcDef *this)
 功能描述：    PMSM电机Q轴转矩控制，D轴电流控制处理，实现FOC控制的核心电流环
 输入参数：    stru_FOC_CtrProcDef *this 结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0       Wencai.Zhao Howlet Li    创建
 *******************************************************************************/
#define SVPWM_MODE 0 /* SVPWM单独跑*/
#if SVPWM_MODE
  s16 tElectAngle;
#else
  u16 tElectAngle;
#endif
void FOC_Model(stru_FOC_CtrProcDef *pCtrProc)
{
    stru_FOC_CurrLoopDef *this;

    this = &struFOC_CurrLoop;
    AdcSampleCal(this);          /* 电流采样 */

    switch(pCtrProc->eSysState)
    {
    case DIR_CHECK:
    {
        OnTheFlyDetectPro();     /* 顺逆风转速检测 */
        /*此处不加break，顺逆风检测需要用到电流环计算*/
    }
    case OPEN_RUN:
    case ALIGN:
    case RUN:
    case HALL_LEARN:
    case HALL_RUN:
    {
        OpenCloseAngleSwitch();  /* 开环角度，闭环角度切换 */

#if   SVPWM_MODE
      tElectAngle += 20;
			struFluxOB_Param.wElectAngle = tElectAngle<<16;
#else
        GET_ELECT_ANGLE();
#endif
			
        this->mTrigSinCos = Trig_Functions(tElectAngle); /* sincos查表 */

        /* Park变换，将qIalpha，qIbeta变换到旋转的dq坐标系下  */
        this->mStatCurrDQ.nAxisQ  = _IQ15mpy(this->mStatCurrAlfaBeta.nBeta, this->mTrigSinCos.hCos) -
                                    _IQ15mpy(this->mStatCurrAlfaBeta.nAlph, this->mTrigSinCos.hSin);
        this->mStatCurrDQ.nAxisD  = _IQ15mpy(this->mStatCurrAlfaBeta.nAlph, this->mTrigSinCos.hCos) +
                                    _IQ15mpy(this->mStatCurrAlfaBeta.nBeta, this->mTrigSinCos.hSin);
        
				struFOC_CurrLoop.npmsmSvcImFw = AutoFieldWeakReg();/*自动弱磁*/
			  				
        CurrentLoopReg(this);   /* D Q轴电流环PI调节*/
        /* 过调制程度控制，MAX_MODULE_VALUE = 1/sqrt(3)*32767 为标准SVPWM, 大于此值进入过调制,最大值为1.15倍标准值 */
        this->mStatVoltAlfaBeta.nAlph = _IQ12mpy(this->mStatVoltAlfaBeta.nAlph, MAX_MODULE_VALUE);
        this->mStatVoltAlfaBeta.nBeta = _IQ12mpy(this->mStatVoltAlfaBeta.nBeta, MAX_MODULE_VALUE);

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
        SVPWM_1ShuntGEN(this);     /*Valpha Vbeta 生成SVPWM空间矢量调制 单电阻方式 */
#else
#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
        SVPWM_2ShuntGEN(this);     /*Valpha Vbeta 生成SVPWM空间矢量调制 双电阻方式 */
#else
#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET))
        SVPWM_3ShuntGEN(this);     /*Valpha Vbeta 生成SVPWM空间矢量调制 三电阻方式*/
#endif
#endif
#endif
        this->MCPWMx_RegUpdate();  /* 加载MCPWM模块占空比值，加载MCPWM模块ADC触发点寄存器值 */

#if (ROTOR_SENSOR_TYPE == ROTOR_SENSORLESS)
        PmsmFluxObserve();         /* 观测器角度估算 */
#endif
        break;
    }
    case BRAKE:
    {   /* 直流制动 */

        BrakeStateCheck();              /* 刹车状态判断，判断是否可靠刹车 */

        this->mVoltUVW_PWM.nPhaseU = 0;
        this->mVoltUVW_PWM.nPhaseV = 0; /* 下桥全开 刹车 */
        this->mVoltUVW_PWM.nPhaseW = 0;

        this->mVoltUVW_NegPWM.nPhaseU = 0;
        this->mVoltUVW_NegPWM.nPhaseV = 0; /* 下桥全开 刹车 */
        this->mVoltUVW_NegPWM.nPhaseW = 0;

        this->MCPWMx_RegUpdate();       /* 加载MCPWM模块占空比值，加载MCPWM模块ADC触发点寄存器值 */

        break;
    }
    default:
        break;
    }
}

/*******************************************************************************
 函数名称：    CurrentLoopReg(stru_FOC_CurrLoopDef *this)
 功能描述：    D、Q轴电流计算，输出Vd、Vq
 输入参数：    stru_FOC_CurrLoopDef *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void CurrentLoopReg(stru_FOC_CurrLoopDef *this)
{
    this->mPI_Torque.wInError = this->nQCur_Reference - this->mStatCurrDQ.nAxisQ; /* Q轴输入误差 */
    this->mPI_Flux.wInError   = this->nDCur_Reference - this->mStatCurrDQ.nAxisD + this->npmsmSvcImFw; /* D轴输入误差 */

    /* 误差限幅，可根据实际负载调整，影响PI响应的快慢 */
    this->mPI_Torque.wInError = sat(this->mPI_Torque.wInError, -0x800, 0x800);
    this->mPI_Flux.wInError   = sat(this->mPI_Flux.wInError, -0x800, 0x800);

#if SVPWM_MODE
    this->mStatVoltDQ.nAxisQ = 0;        /* Q轴电流PI计算，输出Vq 4096 对应额定电压*/
    this->mStatVoltDQ.nAxisD = 600;     /* D轴电流PI计算， 输出Vd 4096 对应额定电压*/
#else
    this->mStatVoltDQ.nAxisQ = CurrentPIRegulator(&this->mPI_Torque);   /* Q轴电流PI计算，输出Vq */
    this->mStatVoltDQ.nAxisD = CurrentPIRegulator(&this->mPI_Flux);     /* D轴电流PI计算，输出Vd */
#endif
	
    ModuCircle_Limitation();  /* 电压极限圆限制 */
}


/*******************************************************************************
 函数名称：    void SpeedLoopReg(MechanicalQuantity *this)
 功能描述：    速度环PI计算
 输入参数：    MechanicalQuantity *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/10/8      V1.0           Howlet Li          创建
 *******************************************************************************/
void SpeedLoopReg(MechanicalQuantity *this)
{
    /***************************************************************************
     1、霍尔反馈闭环时候，反馈来源于struHallProcess.wSpeedfbkHall
     this->mSpeedPI.wInError =  this->wSpeedRef - struHallProcess.wSpeedfbkHall;
     2、无感速度闭环时候，反馈来源于struMotorSpeed.wSpeedfbk
     this->mSpeedPI.wInError =  this->wSpeedRef - this->wSpeedfbk;
    *************************************************************************/
#if (ROTOR_SENSOR_TYPE == ROTOR_HALL_SENSOR)
    this->mSpeedPI.wInError =  this->wSpeedRef - struHallProcess.wSpeedfbkHall;
#else
    this->mSpeedPI.wInError =  this->wSpeedRef - this->wSpeedfbk;
#endif

    this->mSpeedPI.wInError = sat(this->mSpeedPI.wInError, -32767, 32767);

    struFOC_CurrLoop.nQCurrentSet = SpeedPIRegulator(&this->mSpeedPI);
}

/*******************************************************************************
 函数名称：    void FOC_InitstruParam(void)
 功能描述：    FOC 相关控制变量及结构体初始化
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void FOC_InitstruParam(void)
{


    struFOC_CurrLoop.pStruFOC_ConstPar = &struFOC_ConstParam;

    struFOC_CurrLoop.mPI_Torque.KP = struFluxOB_Param.nQ_CurrLoopKP;
    struFOC_CurrLoop.mPI_Torque.KI = struFluxOB_Param.nQ_CurrLoopKI;
    /* Lower Limit for Output limitation */
    struFOC_CurrLoop.mPI_Torque.wLowerLimitOutput = (s16)VQMIN * 65536;
    /* Upper Limit for Output limitation */
    struFOC_CurrLoop.mPI_Torque.wUpperLimitOutput = (s16)VQMAX * 65536;
    struFOC_CurrLoop.mPI_Torque.wIntegral = 0;

    struFOC_CurrLoop.mPI_Flux.KP = struFluxOB_Param.nD_CurrLoopKP;
    struFOC_CurrLoop.mPI_Flux.KI = struFluxOB_Param.nD_CurrLoopKI;
    /* Lower Limit for Output limitation */
    struFOC_CurrLoop.mPI_Flux.wLowerLimitOutput = VDMIN * 65536;
    /* Upper Limit for Output limitation */
    struFOC_CurrLoop.mPI_Flux.wUpperLimitOutput = VDMAX * 65536;
    struFOC_CurrLoop.mPI_Flux.wIntegral = 0;

    struFOC_CurrLoop.MCPWMx_RegUpdate = &MCPWM0_RegUpdate;   /* 初始化MCPWM寄存器更新函数指针 */

    struFOC_CurrLoop.bSVPWM_Type = 1;                       /* 7段式SVPWM调制方式 */

    struFOC_CurrLoop.nVoltageCircleLim = 4000;              /* 电压极限圆限制值 Q12格式，最大值4096*/

    struFOC_CurrLoop.nWeakFieldLim = AUTO_FW_LIM;           /* 自动弱磁D轴电流限制 */

}

/*******************************************************************************
 函数名称：    void StopMotorImmdly(stru_FOC_CtrProcDef *this)
 功能描述：    电机急停函数
 输入参数：    stru_FOC_CtrProcDef *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void StopMotorImmdly(stru_FOC_CtrProcDef *this)
{


}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

