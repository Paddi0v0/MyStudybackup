#include "Global_Variable.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    int main(void)
 ����������    ���������
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2015/11/5      V1.0           Howlet Li          ����
 *******************************************************************************/

int main(void)
{
    Hardware_init(); 										/* Ӳ����ʼ������ */
	
	struFOC_CtrProc.nSys_TimerPWM = 0;  

    while(struFOC_CtrProc.nSys_TimerPWM < 1500) {;}

    sys_init(); 											/* ϵͳ��ʼ������ */

    if(strucontrol_handle.hand_speed_adc > HBSPD_BASE) 		/* ���ɳ����� �ٶȲ������ڻ�׼ֵ��ת�ѹ��� */
    {
		stru_motoronekeyrun.break_hand_err_flg |= BIT0;
        stru_motoronekeyrun.power_up_hand_err_flg |= BIT0;
    }

    for (;;)
    {
        Task_Scheduler(); 									/* ��ѭ������ */
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
