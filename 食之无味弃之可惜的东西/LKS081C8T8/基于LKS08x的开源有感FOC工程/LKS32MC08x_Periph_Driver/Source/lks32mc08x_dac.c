/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� LKS32MC081_dac.c
 * �ļ���ʶ��
 * ����ժҪ�� DAC������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Zhu Jie
 * ������ڣ� 2015��11��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "lks32mc08x_dac.h"
#include "lks32mc08x.h"
#include "lks32mc08x_sys.h"
#include "string.h"

/*******************************************************************************
 �������ƣ�    void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
 ����������    DAC�ṹ���ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              ����
 *******************************************************************************/
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
    memset(DAC_InitStruct, 0, sizeof(DAC_InitTypeDef));
}
/*******************************************************************************
 �������ƣ�    void DAC_init(DAC_InitTypeDef* DAC_InitStruct)
 ����������    DAC��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/18      V1.0          Zhu Jie           ����
 *******************************************************************************/
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct)
{
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_DAC, ENABLE);  /* DAC ʱ��ʹ�� */
	
	  SYS_WR_PROTECT = 0x7a83;  /* �����Ĵ���д���� */
	
	  SYS_AFE_REG1 &= ~((u32)0x03 << 6);              /* DAC����Ĵ������� */
    SYS_AFE_REG1 |= DAC_InitStruct->DAC_GAIN << 6;  /* ����DAC������Ϊ1.2V */
	  
	  if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_1V2)
		{ /* ����DAC 1.2V����У��ֵ */
			SYS_AFE_DAC_DC = Read_Trim(0x00000344);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000334);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_3V0)
		{ /* ����DAC 3.0V����У��ֵ */
			SYS_AFE_DAC_DC = Read_Trim(0x00000340);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000330);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_4V85)
		{ /* ����DAC 4.85V����У��ֵ */
			SYS_AFE_DAC_DC = Read_Trim(0x00000348);
			SYS_AFE_DAC_AMC = Read_Trim(0x00000338);
		}
		
	  SYS_AFE_REG3 |= DAC_InitStruct->DACOUT_EN << 11;/* DAC������� */
	  
	  SYS_WR_PROTECT = 0xffff;  /* �����Ĵ���д���� */
}

/*******************************************************************************
 �������ƣ�    void DAC_OutputValue(uint32_t DACValue)
 ����������    DAC�����������ֵ����
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              ����
 *******************************************************************************/
void DAC_OutputValue(uint32_t DACValue)
{
    SYS_WR_PROTECT = 0x7a83;  /* �����Ĵ���д���� */
	
	  if(DACValue >=4095)
		{
		    DACValue = 4095;
		} 
	  SYS_AFE_DAC = DACValue ;
	
	  SYS_WR_PROTECT = 0xffff;  /* �����Ĵ���д���� */
}

/*******************************************************************************
 �������ƣ�    void DAC_OutputVoltage(uint32_t DACVoltage)
 ����������    DAC���ģ������ֵ����
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����    �������DACVoltageΪQ12��ʽ�������1V��ӦDACVoltage = 1<<12 = 4096
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/04/18     V1.0           Zhu Jie              ����
 *******************************************************************************/
void DAC_OutputVoltage(uint16_t DACVoltage)
{
	  u32 temp = 0;
	  u32 range = 0;
	
	  if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_1V2)
		{ /* ����DAC 1.2V����У��ֵ */
			range = (uint16_t)((1.0/1.2)*BIT12);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_3V0)
		{ /* ����DAC 3.0V����У��ֵ */
			range = (uint16_t)((1.0/3.0)*BIT12);
		}
		else if (((SYS_AFE_REG1 >> 6) & 0x03) == DAC_RANGE_4V85)
		{ /* ����DAC 4.85V����У��ֵ */
			range = (uint16_t)((1.0/4.85)*BIT12);
		} 
	
	  temp = (DACVoltage * range + BIT11) >> 12; 
	
		if(temp >=4095)
		{
		    temp = 4095;
		}
	  SYS_AFE_DAC = temp ;
	
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
