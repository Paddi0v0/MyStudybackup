#include "absacc.h"
#include "MC_hall.h"
#include "MC_type.h"
#include "time_process.h"
#include "hardware_config.h"
#include "function_config.h" 

stru_TaskSchedulerDef struTaskScheduler;    			/* ������Ƚṹ�� */

volatile stru_FaultStatus_t stru_Faults;    			/* ϵͳ������Ƚṹ�� */

stru_FOC_CtrProcDef struFOC_CtrProc;        			/* FOC����������̽ṹ�� */

stru_HallProcessDef  struHallProcess;    				/* Hall�źŴ���ṹ�� */

VoltageProtect_Struct stru_VoltageProtect;    			/* ������ѹ�ṹ�� */

stru_speed_control_handle strucontrol_handle;			/* ת�ѵ��ٽṹ�� */

stru_motor_adjust_speed stru_motoradjust_speed;			/* ������й��̽ṹ�� */

stru_break_eabs strubreak_eabs;							/* ɲ�����ܽṹ�� */

stru_motor_onekeyrun stru_motoronekeyrun;				/* һ���޸����ܽṹ�� */

stru_motor_comprehensive stru_motorcomprehensive;		/* �洢����ת���ܽṹ�� */

stru_power_up strupower_up;								/* �ϵ��ʼ���ṹ�� */

Stru_TempertureCof_Def m_TempertureCof;     			/* �¶ȴ�����ϵ�� */


volatile const FlashParamaStruct struFlashParama __at(HALL_LEARN_ADDR) =
{
    HALL0, HALL1, HALL2, HALL3, HALL4, HALL5, 0, 0,
    HALLTYPE, /* 120��Hall */
    MOTORTYPE0, /* �������0 60�ȳ�ǰ�� */
    0, 0, /* �ֳ����� */
    (CURRENT_LIM_VALUE), /* ĸ�ߵ������� */
    VERSION0, VERSION1, VERSION2, VERSION3, /* ����汾��0.3.0.1 */
    BEMFCOE, /* ���綯��ϵ�� 28  */
    HALL_OFFSET, /* hall��ǰ�� */
    NUL1,	/* ���ٱ�־ */
    NUL11,	/* ���ٱ�־ */
    (CURRENT_LIM_VALUE), /* ĸ�ߵ������� */
};









/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
