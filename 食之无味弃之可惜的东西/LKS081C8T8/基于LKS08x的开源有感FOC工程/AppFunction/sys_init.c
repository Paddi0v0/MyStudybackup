#include "Global_Variable.h"
#include "hardware_config.h"
#include "MC_hall.h"
#include "function_config.h" 

/*******************************************************************************
 �������ƣ�    void sys_init(void)
 ����������    ϵͳ������ʼ�� 
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
 
void sys_init(void)
{
    set_OPA_offset(&struFOC_CtrProc.struFOC_CurrLoop);	/* ������ͻ�ȡ�Լ�ƫ�û�ȡ */
	
	FOC_InitstruParama();								/* FOC ��ؿ��Ʊ������ṹ���ʼ�� */
	
	functional_variables_init();  						/* ���ܱ�����ʼ�� */

    HALL_InitHallMeasure(&struHallProcess);  			/* HALL ����ṹ���ʼ�� */ 
}

/*******************************************************************************
 �������ƣ�    void set_OPA_offset(stru_FOC_CurrLoopDef *this)
 ����������    ���ݵ�����ͻ�ȡƫ��
 ���������    stru_FOC_CurrLoopDef *this �ṹ��ָ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void set_OPA_offset(stru_FOC_CurrLoopDef *this)
{   
    this->Stru_ADC_sampParama.ADC0_A_B_Cur_First = (ADC0_CUR_A_1ST_MSK | ADC1_CUR_B_1ST_MSK );
	this->Stru_ADC_sampParama.ADC0_A_C_Cur_First = (ADC0_CUR_A_1ST_MSK | ADC1_CUR_C_1ST_MSK );
	this->Stru_ADC_sampParama.ADC0_B_C_Cur_First = (ADC0_CUR_B_1ST_MSK | ADC1_CUR_C_1ST_MSK );

	this->nPhaseAOffset = struFlashParama.phaseAoffset;
	this->nPhaseBOffset = struFlashParama.phaseBoffset;
	this->nPhaseCOffset = struFlashParama.phaseCoffset;

    this->nBusCurrentOffset = struFlashParama.phaseBusOffset;
    this->nPhaseBOffset1 = struFlashParama.phaseB1offset;
}

/*******************************************************************************
 �������ƣ�    void Flash_Write_Pro(void)  
 ����������    �洢�ͻ���������
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void Flash_Write_Pro(void)    	
{
    if(struFOC_CtrProc.eSysState == IDLE)
    {
        if((stru_motorcomprehensive.setup_parameters_flg) && (struFOC_CtrProc.struFOC_CurrLoop.nBusVoltage > stru_VoltageProtect.hLowVoltageThd))
        {
            u16 t_cnt = 0;
            u32 t_FlashAddr = 0;
            FlashParamaStruct *flashStru;
            volatile u32 t_FlashBuf[20] = {0};

            t_FlashAddr = HALL_LEARN_ADDR;

            for (t_cnt = 0; t_cnt < 20; t_cnt++)
            {
                t_FlashBuf[t_cnt] = REG32(t_FlashAddr);
                t_FlashAddr += 4;
            }

            __disable_irq();

            stru_motorcomprehensive.setup_parameters_flg = 0;
            flashStru = (FlashParamaStruct*)&t_FlashBuf[0];

            flashStru->hallType = struFlashParama.hallType;

            flashStru->hall_Sequence[0] = struHallProcess.bHallCommTab[0];
            flashStru->hall_Sequence[1] = struHallProcess.bHallCommTab[1];
            flashStru->hall_Sequence[2] = struHallProcess.bHallCommTab[2];
            flashStru->hall_Sequence[3] = struHallProcess.bHallCommTab[3];
            flashStru->hall_Sequence[4] = struHallProcess.bHallCommTab[4];
            flashStru->hall_Sequence[5] = struHallProcess.bHallCommTab[5];

            flashStru->busCurrentLim = struFOC_CtrProc.originalmax_bus_current;

            flashStru->version[0] = VERSION0;
            flashStru->version[1] = VERSION1;
            flashStru->version[2] = VERSION2;
            flashStru->version[3] = VERSION3;

            flashStru->BEMF_coefficient = BEMFCOE;
            flashStru->hall_offsetAngle = 1000;
            flashStru->hallBroken_flg = 0;
            flashStru->null1 = 0;
            flashStru->null2 = struFOC_CtrProc.originalmax_bus_current;
			
			erase_flag = 0x9A0D361F;   //д��������
			EraseSector(0x7800,0);
			progm_flag = 0x9AFDA40C;   //д�������
			ProgramPage(0x7800, 512, (u8*)&t_FlashBuf[0], 0);
            __enable_irq();
			
			struHallProcess.bCloseLoopAngleFlg = 0;
			struHallProcess.bHallRunFlg |= HALL_COM_ERR;
        }
    }
}

/*******************************************************************************
 �������ƣ�    void  Power_Up_Init(void)
 ����������    �ϵ��ʼ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/

void  Power_Up_Init(void)															/* ��ʼ�ϵ�4����� */
{
    if(strupower_up.power_up_time < 400)
    {
        strupower_up.power_up_time++;

        struFOC_CtrProc.struFOC_CurrLoop.nBusVoltagefir = struFOC_CtrProc.struFOC_CurrLoop.nBusVoltage;

        if((strupower_up.power_up_time > 10) && (strupower_up.power_up_time < 110)) /* ������д��� */
        {
			strucontrol_handle.actual_out_powre = 0;
			struHallProcess.bHallState = ReadHallState(); 							/* ������״̬ */
            struHallProcess.bHallState_org = struHallProcess.bHallState;
        }
    }
}










