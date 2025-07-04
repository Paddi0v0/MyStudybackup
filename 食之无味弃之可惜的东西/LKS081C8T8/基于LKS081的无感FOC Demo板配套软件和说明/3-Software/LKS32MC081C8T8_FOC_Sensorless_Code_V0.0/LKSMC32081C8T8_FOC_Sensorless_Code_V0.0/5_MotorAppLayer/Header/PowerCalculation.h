/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� PowerCalculation.h
 * �ļ���ʶ��
 * ����ժҪ�� ���ʴ���
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2020��10��9��
 *
 *******************************************************************************/
#ifndef __POWER_H
#define __POWER_H

#include "basic.h"
#include "mc_type.h"
#include "PID_regulators.h"


#define POWER_LOWPASS_SCALE   (5)     //���ʼ����˲�ϵ��
#define POWER_SHIFT           (7)     //���ʼ�����λϵ��������ʵ�ʼ�����ֵ���޸Ĵ˲����������ֵС���ʵ���С�˲����������ʵ��Ӵ�

//***********************������ϲ���*******************//
#define POWER_SLOPE                     (100)  	//����б��  //(694.1)  //����б��
#define POWER_DIFF                      (0) 	//���߲�ֵ  //(1140.6) //���߲�ֵ
#define POWER_CALC(val)                 (POWER_SLOPE * val - POWER_DIFF)


typedef struct                        //��ͨ
{
    s32 wInput;                       //������
    s32 wOutput;                      //�����
    s16 nK1;                          //ϵ��
    s64 lTemp;                        //�м�ֵ
} stru_LowPass_t;

typedef struct
{
    s32 wRef;                        //����ֵ
    s32 wSet;                        //�趨ֵ
    s16 nACCStep;                    //����ֵ
    s16 nDECStep;                    //����ֵ
} stru_Ramp_t;


typedef struct                      
{
    s32 wPowerValue;                  //����ʵ��ֵ
    
    stru_Ramp_t  struPowerRamp;       //����б�½ṹ��
    
    stru_PIRegulator struPowerPI;     //���ʻ�PI�ṹ��
    
    s16 nSpeedLimitCntr;              //��ת�����ڼ���
    s16 nSpeedLimitPeriod;            //��ת������
    s32 wSpeedLimitValue;             //��������ʱת������ֵ    
    s32 wSpeedLimitPowerRef;          //��ת�ٹ��ʸ���ֵ		//0.01W
    
	s32 wSpeedLimitPowerChangeSet;		//��ת�ٹ��ʵ����仯ֵ	//0.01W
    
    s32 wPowerRef;                    //����ramp����ֵ			//0.01W
    s32 wPowerSet;                    //����ָ���趨ֵ			//0.01W
    s16 nQCurrentSet;                 //���ʻ��������iq
} STR_PowerDef,*pSTR_PowerDef;


extern s32 RampControl(stru_Ramp_t *pstruRamp);

#endif
