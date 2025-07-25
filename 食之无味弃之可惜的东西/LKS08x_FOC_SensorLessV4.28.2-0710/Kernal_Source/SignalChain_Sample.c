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
 * 修改记录1：
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


#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_1SHUNT)
/*******************************************************************************
 函数名称：    stru_CurrPhaseUVW SVPWM_1ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
 功能描述：    单电阻采样算法根据SVPWM调制扇区，得到U, V相电流
 输入参数：    stru_FOC_CurrLoopDef *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
stru_CurrPhaseUVW SVPWM_1ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
{

    stru_Array_Def mADC_Result;
    stru_CurrPhaseUVW Local_Stator_Currents;
    s32 wAux;
    s16 hCurrA, hCurrB, hCurrC;

    u8 bCurrASamp = 0, bCurrBSamp = 0;

    GET_ADC0_DATA(mADC_Result);
    switch (this->bSampCur1)
    {
    case SAMP_IA:

        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;
    case SAMP_IB:
        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;
    case SAMP_IC:
        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        break;

    case SAMP_NIA:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_NIB:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_NIC:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);
        break;
    case SAMP_OLDA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;
        break;

    case SAMP_OLDB:
        hCurrB = this->mStatCurrUVW.nPhaseV;
        bCurrBSamp = 1;
        break;

    case SAMP_OLDC:
        hCurrC = this->mStatCurrUVW.nPhaseW;

        break;


    default:

        break;
    }

    GET_ADC1_DATA(mADC_Result);
    // Second sampling point
    switch (this->bSampCur2)
    {
    case SAMP_IA:
        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrA = sat(wAux, S16_MIN, S16_MAX);
        bCurrASamp = 1;
        break;
    case SAMP_IB:
        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrB = sat(wAux, S16_MIN, S16_MAX);
        bCurrBSamp = 1;
        break;
    case SAMP_IC:
        wAux = mADC_Result.nData0 - (s32)(this->nBusShuntOffset);
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        break;
    case SAMP_NIA:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_NIB:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_NIC:
        wAux = (s32)(this->nBusShuntOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        break;
    case SAMP_OLDA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;
        break;

    case SAMP_OLDB:
        hCurrB = this->mStatCurrUVW.nPhaseU;
        bCurrBSamp = 1;
        break;

    case SAMP_OLDC:
        hCurrC = this->mStatCurrUVW.nPhaseU;
        break;

    default:

        break;
    }

    if (bCurrASamp == 0)
    {   /* 饱和处理 */
        wAux = -hCurrB - hCurrC;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);
    }
    if (bCurrBSamp == 0)
    {
        wAux = -hCurrA - hCurrC;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);
    }


    Local_Stator_Currents.nPhaseU = hCurrA;
    Local_Stator_Currents.nPhaseV = hCurrB;

    return (Local_Stator_Currents);
}

/*******************************************************************************
 函数名称：    void ADC_NormalModeCFG(void)
 功能描述：    ADC正常工作模式通道配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC_NormalModeCFG(void)
{
 
//  ADC0_CHN0 = ADC_1SHUNT_CURR_CH | (ADC_1SHUNT_CURR_CH<<8);
//  ADC0_CHN1 = ADC_1SHUNT_CURR_CH | (ADC_1SHUNT_CURR_CH<<8);
//  ADC0_CHN2 = ADC_BUS_VOL_CHANNEL| (ADC0_VSP_CHANNEL<<8);  
//  ADC0_CHN3 = ADC0_TEMP_CHANNEL  | (BEMF_CH_A<<8); 
//  ADC0_CHN4 = BEMF_CH_B          | (BEMF_CH_C<<8); 
  
	ADC_ChannelConfig(ADC0,CHN0,ADC_1SHUNT_CURR_CH); /*ADC第0次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN1,ADC_1SHUNT_CURR_CH); /*ADC第1次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN2,ADC_1SHUNT_CURR_CH); /*ADC第2次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN3,ADC_1SHUNT_CURR_CH); /*ADC第3次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN4,ADC_BUS_VOL_CHANNEL);/*ADC第4次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN5,ADC0_VSP_CHANNEL);   /*ADC第5次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN6,ADC0_TEMP_CHANNEL);  /*ADC第6次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN7,BEMF_CH_A);          /*ADC第7次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN8,BEMF_CH_B);          /*ADC第8次采样通道配置*/
	ADC_ChannelConfig(ADC0,CHN9,BEMF_CH_C);          /*ADC第9次采样通道配置*/
	
}



#endif

#if (CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_2SHUNT)
/*******************************************************************************
 函数名称：    stru_CurrPhaseUVW SVPWM_2ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
 功能描述：    根据SVPWM调制扇区，得到U, V相电流
 输入参数：    stru_FOC_CurrLoopDef *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
stru_CurrPhaseUVW SVPWM_2ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
{
    stru_Array_Def mADC_Result;
    stru_CurrPhaseUVW Local_Stator_Currents;
    s32 wAux;
    s16 hCurrA = 0, hCurrB = 0, hCurrC = 0;

    u8 bCurrASamp = 0, bCurrBSamp = 0;
    GET_ADC0_DATA(mADC_Result);
    /* First sampling point */
    switch (this->bSampCur1)
    {
    case SAMP_IA:

        wAux = (s32)(this->nPhaseAOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_IB:

        wAux = (s32)(this->nPhaseBOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_IC:

        wAux = (s32)(this->nPhaseCOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;
    case SAMP_NIA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;

        break;
    case SAMP_NIB:
        hCurrB = this->mStatCurrUVW.nPhaseV;
        bCurrBSamp = 1;

        break;
    case SAMP_NIC:
        hCurrC = 0 - this->mStatCurrUVW.nPhaseU - this->mStatCurrUVW.nPhaseV;

        break;
    default:
        wAux = 0;
        break;
    }

    GET_ADC1_DATA(mADC_Result);
    /* Second sampling point */
    switch (this->bSampCur2)
    {
    case SAMP_IA:
        wAux = (s32)(this->nPhaseAOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_IB:

        wAux = (s32)(this->nPhaseBOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_IC:

        wAux = (s32)(this->nPhaseCOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        break;
    case SAMP_NIA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;

        break;
    case SAMP_NIB:
        hCurrB = this->mStatCurrUVW.nPhaseV;
        bCurrBSamp = 1;

        break;
    case SAMP_NIC:
        hCurrC = 0 - this->mStatCurrUVW.nPhaseU - this->mStatCurrUVW.nPhaseV;

        break;
    default:
        wAux = 0;
        break;
    }
    if (bCurrASamp == 0)
    {   /* 饱和处理 hd */
        wAux = -hCurrB - hCurrC;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);
    }
    if (bCurrBSamp == 0)
    {
        wAux = -hCurrA - hCurrC;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);
    }

    Local_Stator_Currents.nPhaseU = hCurrA;
    Local_Stator_Currents.nPhaseV = hCurrB;

    return (Local_Stator_Currents);
}

/*******************************************************************************
 函数名称：    void ADC_NormalModeCFG(void)
 功能描述：    ADC正常工作模式通道配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC_NormalModeCFG(void)
{
//	  ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
//    ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
//    ADC0_CHN2 = ADC_BUS_VOL_CHANNEL    | (ADC0_VSP_CHANNEL << 8); 
	
	  ADC_ChannelConfig(ADC0,CHN0,ADC0_CURRETN_A_CHANNEL);/*ADC第0次采样通道配置*/  //ADC_DAT0
	  ADC_ChannelConfig(ADC0,CHN1,ADC0_CURRETN_B_CHANNEL);/*ADC第1次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN2,ADC0_CURRETN_A_CHANNEL);/*ADC第2次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN3,ADC0_CURRETN_B_CHANNEL);/*ADC第3次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN4,ADC_BUS_VOL_CHANNEL);   /*ADC第4次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN5,ADC0_VSP_CHANNEL);      /*ADC第5次采样通道配置*/   //ADC_DAT5
	  ADC_ChannelConfig(ADC0,CHN6,ADC_BUS_VOL_CHANNEL);   /*ADC第6次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN7,BEMF_CH_A);             /*ADC第7次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN8,BEMF_CH_B);             /*ADC第8次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN9,BEMF_CH_C);             /*ADC第9次采样通道配置*/  //DAT9
    MCPWM_TMR0 = (10-PWM_PERIOD); // 100-PWM_PERIOD
}

#endif


#if ((CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_3SHUNT)||(CURRENT_SAMPLE_TYPE == CURRENT_SAMPLE_MOSFET)) 
/*******************************************************************************
 函数名称：    stru_CurrPhaseUVW SVPWM_3ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
 功能描述：    根据SVPWM调制扇区，得到U, V相电流
 输入参数：    stru_FOC_CurrLoopDef *this  结构体指针
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
stru_CurrPhaseUVW SVPWM_3ShuntGetPhaseCurrent(stru_FOC_CurrLoopDef *this)
{
    stru_Array_Def mADC_Result;
    stru_CurrPhaseUVW Local_Stator_Currents;
    s32 wAux;
    s16 hCurrA = 0, hCurrB = 0, hCurrC = 0;

    u8 bCurrASamp = 0, bCurrBSamp = 0;
    GET_ADC0_DATA(mADC_Result);
    /* First sampling point */
    switch (this->bSampCur1)
    {
    case SAMP_IA:

        wAux = (s32)(this->nPhaseAOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_IB:

        wAux = (s32)(this->nPhaseBOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_IC:

        wAux = (s32)(this->nPhaseCOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;
    case SAMP_NIA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;

        break;
    case SAMP_NIB:
        hCurrB = this->mStatCurrUVW.nPhaseV;
        bCurrBSamp = 1;

        break;
    case SAMP_NIC:
        hCurrC = 0 - this->mStatCurrUVW.nPhaseU - this->mStatCurrUVW.nPhaseV;

        break;
    default:
        wAux = 0;
        break;
    }

    GET_ADC1_DATA(mADC_Result);
    /* Second sampling point */
    switch (this->bSampCur2)
    {
    case SAMP_IA:
        wAux = (s32)(this->nPhaseAOffset) - mADC_Result.nData0;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);

        bCurrASamp = 1;
        break;

    case SAMP_IB:

        wAux = (s32)(this->nPhaseBOffset) - mADC_Result.nData0;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);

        bCurrBSamp = 1;
        break;

    case SAMP_IC:

        wAux = (s32)(this->nPhaseCOffset) - mADC_Result.nData0;
        hCurrC = sat(wAux, S16_MIN, S16_MAX);

        break;
    case SAMP_NIA:
        hCurrA = this->mStatCurrUVW.nPhaseU;
        bCurrASamp = 1;

        break;
    case SAMP_NIB:
        hCurrB = this->mStatCurrUVW.nPhaseV;
        bCurrBSamp = 1;

        break;
    case SAMP_NIC:
        hCurrC = 0 - this->mStatCurrUVW.nPhaseU - this->mStatCurrUVW.nPhaseV;

        break;
    default:
        wAux = 0;
        break;
    }
    if (bCurrASamp == 0)
    {   /* 饱和处理 hd */
        wAux = -hCurrB - hCurrC;
        hCurrA = sat(wAux, S16_MIN, S16_MAX);
    }
    if (bCurrBSamp == 0)
    {
        wAux = -hCurrA - hCurrC;
        hCurrB = sat(wAux, S16_MIN, S16_MAX);
    }

    Local_Stator_Currents.nPhaseU = hCurrA;
    Local_Stator_Currents.nPhaseV = hCurrB;

    return (Local_Stator_Currents);
}

/*******************************************************************************
 函数名称：    void ADC_NormalModeCFG(void)
 功能描述：    ADC正常工作模式通道配置
 输入参数：    无
 输出参数：    无
 返 回 值：    无
 其它说明：
 修改日期      版本号          修改人            修改内容
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          创建
 *******************************************************************************/
void ADC_NormalModeCFG(void)
{
//    ADC0_CHN0 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
//    ADC0_CHN1 = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
//    ADC0_CHN2 = ADC_BUS_VOL_CHANNEL    | (ADC0_VSP_CHANNEL << 8);  
  
	  ADC_ChannelConfig(ADC0,CHN0,ADC0_CURRETN_A_CHANNEL);/*ADC第0次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN1,ADC0_CURRETN_B_CHANNEL);/*ADC第1次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN2,ADC0_CURRETN_A_CHANNEL);/*ADC第2次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN3,ADC0_CURRETN_B_CHANNEL);/*ADC第3次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN4,ADC_BUS_VOL_CHANNEL);   /*ADC第4次采样通道配置*/
	  ADC_ChannelConfig(ADC0,CHN5,ADC0_VSP_CHANNEL);      /*ADC第5次采样通道配置*/
	
	
    struFOC_CurrLoop.ADC_A_B_CurrentSamp = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_B_CHANNEL << 8);
    struFOC_CurrLoop.ADC_A_C_CurrentSamp = ADC0_CURRETN_A_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8 );
    struFOC_CurrLoop.ADC_B_C_CurrentSamp = ADC0_CURRETN_B_CHANNEL | (ADC0_CURRETN_C_CHANNEL << 8 );
}

#endif

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */

