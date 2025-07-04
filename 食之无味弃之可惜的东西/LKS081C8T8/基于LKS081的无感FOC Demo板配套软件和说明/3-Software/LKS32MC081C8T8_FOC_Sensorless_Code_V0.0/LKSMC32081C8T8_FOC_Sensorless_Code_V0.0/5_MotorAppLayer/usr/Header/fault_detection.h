/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� fault_detection.h
 * �ļ���ʶ��
 * ����ժҪ�� fault detectiong: volt ,current, temperature, stall, empty, phaseloss,\
 *
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2019��11��28��
 * �޸���ʷ:
 *              2022-11-22 ��ӱ����������ù��ܣ��޸Ĳ��ֱ������룬Li Tonghua
 *******************************************************************************/
#ifndef __FAULT_DETECTION_H
#define __FAULT_DETECTION_H

#include "basic.h"
#include "mc_type.h"
#include "state_machine.h"

#define FAULT_DETECT_CYCLE		(5)
typedef struct
{
    s16 nCheckCnt1;
    s16 nCheckCnt2;
    s16 nRecoverCntr;
    s16 nStartDelay;
    s16 nRecoverTime;
} STR_FaultTime;

typedef struct
{
    s16 nOverCurrent;            //�������ֵ
    s16 nOverVoltage;            //��ѹֵ
    s16 nUnderVoltage;           //Ƿѹֵ
    s16 nOverVoltageRecover;     //��ѹ�ָ�ֵ
    s16 nUnderVoltageRecover;    //Ƿѹ�ָ�ֵ
    s16 nEmptyCurrent;           //��ˮ��ת����ֵ
    s32 wEmptySpeed;             //��ˮ��תת��ֵ
	s16 nStallCurrent;           //��ת������
	s32 wStallCurMax;			 //��ת���ߵ���
	s32 wStallCurMin;			 //��ת���͵���
	s32 wStallSpeedfactor;       //��ת����ٶ�ϵ��
	s16 nStallCurFlag;           //��ת���ϱ�־
	s16 nStallFlag;              //��ת���ϱ�־
    s16 nStartFlag;              //�������ϱ�־
    s16 nPhaseLossCurrent;       //ȱ������趨ֵ
    s16 nPhaseLossFlag;          //ȱ���־λ�����ֲ�ͬ��ȱ�෽ʽ
} STR_FaultVariable;

typedef union
{
    u16 R;
    struct
    {
        u16 SoftCurretError : 1;//���������־  0x0001  1
        u16 HardCurretError : 1;//Ӳ��������־  0x0002  2
        u16 VoltOverError   : 1;//��ѹ���ϱ�־  0x0004  4
        u16 VoltUnderError  : 1;//Ƿѹ���ϱ�־  0x0008  8
        u16 StallError      : 1;//��ת���ϱ�־  0x0010  16
        u16 EmptyError      : 1;//��ת���ϱ�־  0x0020  32
        u16 PhaseLossError  : 1;//ȱ����ϱ�־  0x0040  64
        u16 TempOverError   : 1;//�¶ȹ��ϱ�־  0x0080  128
        u16 OffsetError     : 1;//ƫ�ù��ϱ�־  0x0100  256
        u16 StartError      : 1;//�������ϱ�־  0x0200  512
        u16                 : 6;//Ԥ��
    } B;
} STR_FaultStatus;

typedef enum
{
    E_FAULT_PRIO_HIGH = 0,				// ��������
	E_FAULT_PRIO_BELOW_UA = 1,			// UA�趨����
	E_FAULT_PRIO_LOW = 2				// ��ͼ���
	
} ENU_FaultPrio;


typedef struct
{
    u8  bFlag;					//ȱ�����־λ
    u16 nTheta;					//������ֵ������
    u32 wPhATemp;               //A�������ֵ�м�ֵ
    u32 wPhBTemp;				//B�������ֵ�м�ֵ
    u32 wPhCTemp;				//C�������ֵ�м�ֵ
    u16 nPhA;					//A�������ֵ
    u16 nPhB;   				//B�������ֵ
    u16 nPhC;					//C�������ֵ
}STR_CurrentAmplitude;
                
extern volatile STR_FaultStatus stru_Faults;
extern STR_FaultVariable        stru_FaultValue;
extern STR_CurrentAmplitude     struCurrentAmplitude;
extern STR_VectorUVW			struImaxCurrent;

extern STR_FaultTime struFaultEmptyTime;
extern STR_FaultTime struFaultStallTime;
extern STR_FaultTime struFaultCurrentTime;
extern STR_FaultTime struFaultTempTime;
extern STR_FaultTime struFaultPhaseTime;

#endif

