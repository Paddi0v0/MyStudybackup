#ifndef _PUBDATA_MA_UA_H_
#define _PUBDATA_MA_UA_H_

#include "basic.h"

#define UA_FREQ_CMD_FORMAT						100			// 1Hz



//
/********************************************************************************************/
/* 
   bit07  |  bit06 | bit05 | bit04 | bit03 | bit02 | bit01 | bit00|
          |        |       |       |       |       |       |      |
--------------------------------------------------------------------------------------
 |   MOE  |   ���� |  ���� |  Ԥ�� |    ����ģʽ   |  ���� |ON/OFF|
 |  ��Ч  |   ���� |  ��� |                       |       |      |
    
                                                           |      |
                                                           |   1  |
-------------------------------------------------------------------------------------------------------------   
�ٶ�ģʽ
     1        0        0   |    0   |   0       0       0       1        0x81 ------- RUN �ٶ�ģʽ
     1        0        0   |    0       0       0       1       1        0x83 ------- RUN,�ٶ�ģʽ������ֵȡ��
-------------------------------------------------------------------------------------------------------------   

ת��ģʽ
     1        0        0   |    0   |   0       1       0       1        0x85 ------- RUN ת��ģʽ
     1        0        0   |    0       0       1       1       1        0x87------- RUN ת��ģʽ������ֵȡ��
-------------------------------------------------------------------------------------------------------------   

����ģʽ
     1        0        0   |    0   |   1       0       0       1        0x89 ------- RUN ����ģʽ  �ٶ�����ֵ
     1        0        0   |    0       1       0       1       1        0x8B ------- RUN ����ģʽ  �趨�ٶ�����ֵȡ��
-------------------------------------------------------------------------------------------------------------   

λ��ģʽ
	 1        0        0   |    0   |   1       1       0       1        0x8D ------- RUN λ��ģʽ  �ٶ�����ֵ
	 1        0        0   |    0       1       1       1       1        0x8F ------- RUN λ��ģʽ  �趨�ٶ�����ֵȡ��
-------------------------------------------------------------------------------------------------------------   

ֹͣ��ʽ
                                                            |  0  |
-------------------------------------------------------------------------------------------------------------   
     0        x        x   |    x       x       x       x       x        0x00 ------- �ر�MOE ֹͣ
     
-------------------------------------------------------------------------------------------------------------   
     1        1        0   |    x       x       x       x       x        0xC0 ------- FAULT ֹͣ

-------------------------------------------------------------------------------------------------------------   
     1        0        0   |    x       x       x       x       0        0x80 -------  ֹͣ


���������ʽ															 
--------------------------------------------------------------------------------------
     0        0        1   |    x       x       x       x       0        0xA0 -------  ���
     1        0        1   |    x       x       x       x       0        0xA0 -------  ���


********************************************************************************************/

typedef enum {	
	E_CMD_MOTOR_STOP_MOE_OFF	= 0x00,		// �ر�MOE, ����������Բ�����
	E_CMD_MOTOR_STOP			= 0x80,		// ֹͣ���
	
	E_CMD_MOTOR_SPEED_RUN		= 0x81,		// ���е��, �ٶ�ģʽ��ת������Ϊ�趨����
	E_CMD_MOTOR_SPEED_RUN_REV	= 0x83,		// ���е��, �ٶ�ģʽ��ת���������趨�����෴

	E_CMD_MOTOR_TORQUE_RUN		= 0x85,		// ���е��, ת��ģʽ��ת������Ϊ�趨����
	E_CMD_MOTOR_TORQUE_RUN_REV	= 0x87,		// ���е��, ת��ģʽ��ת���������趨�����෴

	E_CMD_MOTOR_POWER_RUN		= 0x89,		// ���е��, ����ģʽ��ת������Ϊ�趨����
	E_CMD_MOTOR_POWER_RUN_REV	= 0x8B,		// ���е��, ����ģʽ��ת���������趨�����෴

	E_CMD_MOTOR_POS_RUN			= 0x8D,		// ���е��, ����ģʽ��ת������Ϊ�趨���� 		Ԥ�� ---- �ݲ�֧��
	E_CMD_MOTOR_POS_RUN_REV		= 0x8F,		// ���е��, ����ģʽ��ת���������趨�����෴	Ԥ�� ---- �ݲ�֧��
	
	E_CMD_MOTOR_FAULT_DIMOE		= 0x40,		// FAULTֹͣ���,MOE�ر�
	E_CMD_MOTOR_FAULT_ENMOE		= 0xC0,		// FAULTֹͣ���,MOEʹ��
	E_CMD_MOTOR_FAULT_CLR_DIMOE	= 0x20,		// ������� MOE�ر�
	E_CMD_MOTOR_FAULT_CLR_ENMOE	= 0xA0,		// ������� MOEʹ��
}ENUM_MotorCmdWord;


typedef enum {
	E_STOP_MOE_OFF = 0,			//�ر�MOEֹͣ
	E_STOP_ZERO_CUR,			//�����ֹͣ
	E_STOP_SHORT,				//�̽�ֹͣ
	E_STOP_DONT_SUPPORT	
}ENUM_MotorBrakeMode;			//	���ֹͣ��ʽ


typedef struct {
	int16_t gWrFlag;
	ENUM_MotorCmdWord	eAppCmd;
	ENUM_MotorBrakeMode	eBrakeMode;
	int32_t		wFreqCmd;			// ��ֵΪ����  0.01Hz
	int32_t		wPowerCmd;			// 	0.001W
	int32_t		wTorqCmd;           //	0.001A
	int16_t		nGearRatio;
	int16_t		nMotorId;
	int16_t		nInvId;				//RSVD

}STR_PubDatUAMA;


typedef union
{
    u16 Reg;
    struct
    	{        
		u16 locked			: 1;	// Ӧ�ñ�־					0x0001
		u16 Fault			: 1;	// ���ڹ���					0x0002
		u16 Running			: 1;	// ���������		
		u16 PWMStatus		: 1;	// MA PWMʹ��״̬		
		
		u16 waitCfg			: 1;	// �ȴ�����
		u16 CfgDone			: 1;	// �������
		u16 POST			: 1;	// �ϵ����

		u16 reserved	 	: 9;	// Ԥ��		
		}Bit;
} UNION_Mot_Status_SR;


typedef struct {
	UNION_Mot_Status_SR		SR;
	ENUM_MotorCmdWord eMotorStsAck;

	int32_t			wVdcDec;			// 0.01V 
	int32_t			wCurDec;			// 0.001A
	int32_t			wVolDec;			// 0.01V 
	int32_t			wSpeedDec;			// 0.01Hz
	int16_t			nMotorTemp;			// 
	int16_t			nInvTemp;
	uint32_t		uFault;				//
}STR_PubDatMAUA, *pSTR_PubDatMAUA;		//PUB_DATA_UA2MA_STR;



// handle function from UA 2 MA : UA write function

static __inline void	SetAppCmdUA2MA(STR_PubDatUAMA*pUA2MA, ENUM_MotorCmdWord cmd)	//	UA����PWM������MA
{
	pUA2MA->eAppCmd		= cmd;
}

static __inline void	SetAppBrakeModeUA2MA(STR_PubDatUAMA*pUA2MA, ENUM_MotorBrakeMode mode) //	UA����ֹͣģʽ��MA
{
	pUA2MA->eBrakeMode	= mode;
}


static __inline void	SetAppFreqCmdUA2MA(STR_PubDatUAMA *pUA2MA, int32_t wFreqCmd)	//	UA����Ƶ��ָ��ֵ��MA
{
	pUA2MA->wFreqCmd	= wFreqCmd;
}

static __inline void	SetAppPowerCmdUA2MA(STR_PubDatUAMA *pUA2MA, int32_t wPowerCmd)	//	UA���ù���ָ��ֵ��MA
{
	pUA2MA->wPowerCmd	= wPowerCmd;
}

static __inline void	SetAppTorqCmdUA2MA(STR_PubDatUAMA *pUA2MA, int32_t wTorqCmd)	//	UA����ת��ָ��ֵ��MA
{
	pUA2MA->wTorqCmd	= wTorqCmd;
}

static __inline void	SetAppGearRatioUA2MA(STR_PubDatUAMA *pUA2MA, int16_t nGearRatio)	//	UA���ô����ȵ�MA��Ԥ��
{
	pUA2MA->nGearRatio	= nGearRatio;
}

static __inline void	SetAppMotorIDUA2MA(STR_PubDatUAMA *pUA2MA, int16_t nMotorId)	//	UA���õ��IDֵ��MA,Ԥ��
{
	pUA2MA->nMotorId	= nMotorId;
}

static __inline void	SetAppInvIDUA2MA(STR_PubDatUAMA *pUA2MA, int16_t nInvId)		//	UA���ù�������IDֵ��MA,Ԥ��
{
	pUA2MA->nInvId	= nInvId;
}


// handle function from MA 2 UA :  UA read function
static __inline ENUM_MotorCmdWord getMotorStatMD2MA(STR_PubDatMAUA*	pMA2UA)	//	UA��ȡMA���״̬
{
	return pMA2UA->eMotorStsAck;
}

static __inline int32_t getVdcDecMAUA(STR_PubDatMAUA*	pMA2UA)				//	UA��ȡMAĸ�ߵ�ѹֵ 0.01V
{
	return pMA2UA->wVdcDec;
}

static __inline int32_t getCurDecMAUA(STR_PubDatMAUA*	pMA2UA)				//	UA��ȡMA�������Чֵ 0.001A
{
	return pMA2UA->wCurDec;
}

static __inline int32_t getSpeedDecMAUA(STR_PubDatMAUA*	pMA2UA)				//	UA��ȡMA�ٶ�ֵ 0.01Hz
{
	return pMA2UA->wSpeedDec;
}

static __inline int16_t getMotorTempDecMAUA(STR_PubDatMAUA*	pMA2UA)			//	UA��ȡMA����¶ȣ�Ԥ��
{
	return pMA2UA->nMotorTemp;
}

static __inline int16_t getInvTempDecMAUA(STR_PubDatMAUA*	pMA2UA)			//	UA��ȡMA���������¶� ������Ҫʵ��
{
	return pMA2UA->nInvTemp;
}

static __inline bool NoFaultCodeFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA�����ڱ��� TRUE --- ������
{
	return (pMA2UA->uFault == 0);
}

static __inline bool HasFaultBitFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA���ڱ�����־λ TRUE --- ����
{
	return (pMA2UA->SR.Bit.Fault);
}


static __inline bool HasFaultFromMA(STR_PubDatMAUA*	pMA2UA)					//	MA���ڱ��� TRUE --- ����
{
	return (pMA2UA->uFault) ||(pMA2UA->SR.Bit.Fault);
}

static __inline bool IfPostFinishFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA POST���� TRUE --- ���
{
	return (pMA2UA->SR.Bit.POST);
}

static __inline bool IfWaitCfgFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA �ȴ��������� TRUE --- �ȴ�
{
	return (pMA2UA->SR.Bit.waitCfg);
}

static __inline bool IfCfgFinishFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA �������ý��� TRUE --- ���
{
	return (pMA2UA->SR.Bit.CfgDone);
}

static __inline bool IsRunningFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA ����Ƿ����� TRUE --- ����
{
	return (pMA2UA->SR.Bit.Running);
}

static __inline bool IsStoppedFromMA(STR_PubDatMAUA*	pMA2UA)				//	MA ����Ƿ�ֹͣ TRUE --- ֹͣ
{
	return (pMA2UA->SR.Bit.Running == 0);
}



// handle function from MA 2 UA :  MA write function
static __inline void setFaultStatMA2UA(STR_PubDatMAUA* pMA2UA, bool isFault )	//	MA���ñ�����UA
{
	pMA2UA->SR.Bit.Fault = isFault;
}

static __inline void setRunningStatMA2UA(STR_PubDatMAUA* pMA2UA, bool isRunning )	//	MA�������б�־��UA
{
	pMA2UA->SR.Bit.Running = isRunning;
}

static __inline void setPWMStatMA2UA(STR_PubDatMAUA* pMA2UA, bool isEnable )	//	MA����PWM״̬��UA
{
	pMA2UA->SR.Bit.PWMStatus = isEnable;
}



static __inline void setWaitCfgStatMA2UA(STR_PubDatMAUA* pMA2UA, bool isWaitPara)	//	MA���õȴ��������õ�UA
{
	pMA2UA->SR.Bit.waitCfg = isWaitPara;
}

static __inline void setCfgFinishMA2UA(STR_PubDatMAUA* pMA2UA, bool isDone)			//	MA���ò���������ɵ�UA
{
	pMA2UA->SR.Bit.CfgDone = isDone;
}

static __inline void setPostStatMA2UA(STR_PubDatMAUA* pMA2UA, bool isDone)			//	MA����POST��ɵ�UA
{
	pMA2UA->SR.Bit.POST = isDone;
}




#endif // _PUBDATA_MA_UA_H_
/////////////////////
