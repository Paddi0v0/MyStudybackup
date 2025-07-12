/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC08x_dac.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DAC������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
#include "lks32mc08x_dac.h"
#include "string.h"

/**
 *@brief @b ��������:   void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
 *@brief @b ��������:   DAC�ṹ���ʼ��
 *@see�����ú�����       ��
 *@param���������      ADC_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��   
 *@par ʾ�����룺
 *@code    
           DAC_StructInit DAC_InitStructure;
		   DAC_StructInit(&DAC_InitStructure); //��ʼ���ṹ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
    memset(DAC_InitStruct, 0, sizeof(DAC_InitTypeDef));
}
/**
 *@brief @b ��������:   void DAC_init(DAC_InitTypeDef* DAC_InitStruct)
 *@brief @b ��������:   DAC��ʼ������
 *@see�����ú�����      SYS_AnalogModuleClockCmd()
 *@param���������      DAC_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			  DAC_InitTypeDef DAC_InitStructure;
			  DAC_StructInit(&DAC_InitStructure);
			  DAC_InitStructure.DAC_GAIN = DAC_RANGE_1V2;//DAC����Ϊ1.2V
			  DAC_InitStructure.DACOUT_EN = DISABLE ;    //DACͨ��IO P0[0]���
			  DAC_Init(&DAC_InitStructure);              // DAC��ʼ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
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


/**
 *@brief @b ��������:   void DAC_OutputValue(uint32_t DACValue)
 *@brief @b ��������:   DAC�����������ֵ����
 *@see�����ú�����       ��
 *@param���������       DACValue:DAC�����ѹ��Ӧ������
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DAC_OutputValue(2048);//DAC���2048*1.2/4096 = 0.6V
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void DAC_OutputValue(uint32_t DACValue)
{	
	  if(DACValue >=4095)
		{
		    DACValue = 4095;
		} 
	  SYS_AFE_DAC = DACValue ;	
}

/**
 *@brief @b ��������:   void DAC_OutputVoltage(uint32_t DACVoltage)
 *@brief @b ��������:   DAC���ģ������ֵ����
 *@see�����ú�����       ��
 *@param���������       DACVoltage:DAC�����ѹ��Ӧ������
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        �������DACVoltageΪQ12��ʽ�������1V��ӦDACVoltage = 1<<12 = 4096
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DAC_OutputVoltage(BIT12 * 0.6);//DAC���0.6V
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
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
