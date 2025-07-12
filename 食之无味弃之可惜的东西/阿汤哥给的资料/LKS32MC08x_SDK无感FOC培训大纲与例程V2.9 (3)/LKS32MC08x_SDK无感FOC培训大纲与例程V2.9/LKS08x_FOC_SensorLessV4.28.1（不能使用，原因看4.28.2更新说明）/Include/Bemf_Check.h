/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * �ļ����ƣ� Bemf_Check.h
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ������ļ�
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2020��8��20��
 *
 * �޸ļ�¼1��
 * �޸����ڣ� 2022��8��18��
 * �� �� �ţ� V 2.0
 * �� �� �ˣ� Lizyt
 * �޸����ݣ� ��������ٶȽ��
 *******************************************************************************/

#ifndef __BEMF_CHECK_H_
#define __BEMF_CHECK_H_

#include "basic.h"
#include "MC_Type.h"
#include "MC_parameter.h"
#include "hardware_config.h"

typedef struct
{
    s16 nPhaseU;
    s16 nPhaseV;
    s16 nPhaseW;
} stru_BemfPhaseUVW_Def;

typedef struct
{
    s16 nAlpha;
    s16 nBeta;
} stru_BemfPhaseAlpahBeta_Def;

typedef struct
{
    s16 nBemf_Checkttime;                          //���綯�Ƽ���ʱ��
    stru_BemfPhaseUVW_Def         nBemf_ADC;       //adc����ֵ
    stru_BemfPhaseUVW_Def         nBemf_ADCoffset;       //adc����ֵ
    stru_BemfPhaseUVW_Def         nBemf_Value;     //�����󷴵綯��Core����
    stru_BemfPhaseAlpahBeta_Def   nBemf_AlphaBeta; //alpha beta�ᷴ�綯�Ƶ�ѹ
    s32 wBemfUVWSum;                                //���෴�綯��֮��
    s16 nBemfUVWAver;                               //���෴�綯��֮�͵�ƽ��
    s16 nBemfUVWAmp;                                //Alpha beta�ľ����������綯�Ʒ�ֵ
    u16 nBemfUVWAmpValue;                           //Alpha beta�ľ����������綯�Ʒ�ֵ����ֵ��λ 0.01V
    s16 nVbus;                                      //Ԥ��ĸ�ߵ�ѹ���ںϳ�FOC�������ѹ
    u16 unBemf_Angle;                               //����ǰ���綯�Ƽ���ת�ӽǶ�
    u16 unBemf_AngleSec;                            //���綯�Ƽ���ת�ӽǶ��������
    s16 unBemf_AngleLast;                           //��һʱ�̷��綯�Ƽ���ת�ӽǶ�
    s16 nBemf_DetaAngle;                            //���綯�Ƽ���ת�ӽǶȲ���ڼ����ٶ�
    s32 wBemf_DetaAngleSum;                         //���綯�Ƽ���ת�ӽǶȲ���ڼ����ٶ�
    s16 nBemf_DetaAngleAver;                        //���綯�Ƽ���ת�ӽǶȲ���ڼ����ٶ�ƽ��
    s16 nBemf_DetaAngleCnt;                         //���綯�Ƽ���ת�ӽǶȲ���ڼ����ٶ�ƽ��
    s16 nBemf_Freq;                                 //���綯�Ƽ���Ƶ��
    s16 nBemf_FreqAve;                              //���綯�Ƽ����˲�Ƶ�ʣ������ж� APP������
    s16 nBemf_KeFreq;                               //Ke���㷴�綯�Ƽ���Ƶ��
    s16 nBemf_KeFreqAve;                            //Ke���㷴�綯�Ƽ���Ƶ��     APP������
    stru_RC_Def  struRCBemf_Speed;                  //���綯�Ƽ����ٶȵ�ͨ�˲�
    s16 nVbemf_Base;                                /**/
    s16 nVbemf_BaseValue;                           /*��ѹ������Χ 0.01V��λ*/
		s16 nBemf_AdvanceAngle;                         /*���綯�Ƴ�ǰ��*/   
} stru_BEMF_Def; /* ���綯�Ƶ�·�ṹ�� */

//���巴�綯�Ʊ���
extern stru_BEMF_Def    stru_BEMF_Check;
extern void  Bemf_CheckInit(stru_BEMF_Def *pstru_BEMF , s16 angle_lead);
extern void Bemf_CheckProcess(stru_BEMF_Def *pstru_BEMF, s16 phaseA, s16 phaseB, s16 phaseC);
/****************���綯�Ƽ���л����޸еĽǶȸ�ֵ*****************/
extern void BemfSwitch(stru_BEMF_Def *pstru_BEMF);  

#endif
