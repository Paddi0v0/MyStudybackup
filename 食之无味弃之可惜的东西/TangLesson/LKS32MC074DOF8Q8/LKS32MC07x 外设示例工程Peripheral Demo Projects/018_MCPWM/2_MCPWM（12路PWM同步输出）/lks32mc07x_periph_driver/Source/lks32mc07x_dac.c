/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_dac.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DAC������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��3��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */

#include "lks32mc07x_dac.h"
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
 * <tr><td>2023��2��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
	memset(DAC_InitStruct,0,sizeof(DAC_InitTypeDef));
}

/**
 *@brief @b ��������:   void DAC_init(uint8_t DAC_Channel,DAC_InitTypeDef* DAC_InitStruct)
 *@brief @b ��������:   DAC��ʼ������
*@param���������       DAC_Channel:DAC_Channel_0/DAC_Channel_1,DAC_InitTypeDef
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
				DAC_InitStre.TIG_CH_EN = DISABLE; 		  	//ʧ��UTIMER����
				DAC_InitStre.DAC_STEP = 0;						  	//����ֵΪ0
			  DAC_Init(DAC_Channel_0,&DAC_InitStructure);              // DAC��ʼ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */

void DAC_Init(uint8_t DAC_Channel,DAC_InitTypeDef* DAC_InitStruct)
{
		SYS_WR_PROTECT = 0x7a83;  /* �����Ĵ���д���� */
		SYS_AFE_REG1 &= ~((u32)0x01 << (6 + DAC_Channel));              /* DAC����Ĵ������� */
		SYS_AFE_REG1 |= DAC_InitStruct->DAC_GAIN << (6 + DAC_Channel);  /* ����DAC������Ϊ1.2V ��4.8V*/
	
		if(DAC_Channel_0 == DAC_Channel)
		{
			if (((SYS_AFE_REG1 >> 6) & 0x01) == DAC_RANGE_1V2)
			{ /* ����DAC 1.2V����У��ֵ */
				SYS_AFE_DAC0_DC = Read_Trim(0x0000145C);
				SYS_AFE_DAC0_AMC = Read_Trim(0x00001458);
			}
			else if (((SYS_AFE_REG1 >> 6) & 0x01) == DAC_RANGE_4V85)
			{ /* ����DAC 4.85V����У��ֵ */
				SYS_AFE_DAC0_DC = Read_Trim(0x00001454);
				SYS_AFE_DAC0_AMC = Read_Trim(0x00001450);

			}
			SYS_AFE_DAC_CTRL |= (DAC_InitStruct->DAC_STEP << 4) | (DAC_InitStruct->TIG_CH_EN);
		}
		else if(DAC_Channel_1 == DAC_Channel)
		{
			if (((SYS_AFE_REG1 >> 7) & 0x01) == DAC_RANGE_1V2)
			{ /* ����DAC 1.2V����У��ֵ */
				SYS_AFE_DAC1_DC = Read_Trim(0x0000146C);
				SYS_AFE_DAC1_AMC = Read_Trim(0x00001468);
			}
			else if (((SYS_AFE_REG1 >> 7) & 0x01) == DAC_RANGE_4V85)
			{ /* ����DAC 4.85V����У��ֵ */
				SYS_AFE_DAC1_DC = Read_Trim(0x00001464);
				SYS_AFE_DAC1_AMC = Read_Trim(0x00001460);
			}
	    SYS_AFE_DAC_CTRL |= (DAC_InitStruct->DAC_STEP << 12) | (DAC_InitStruct->TIG_CH_EN << 8);
		}
		SYS_WR_PROTECT = 0xffff;  /* �����Ĵ���д���� */
}
/**
 *@brief @b ��������:   void DAC_OutputValue(uint8_t DAC_Channel,uint32_t DACValue)
 *@brief @b ��������:   DAC�����������ֵ����
 *@see�����ú�����       ��
 *@param���������       DAC_Channel��DAC_Channel_0/DAC_Channel_1��DACValue:DAC�����ѹ��Ӧ������
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DAC_OutputValue(DAC_Channel_0,2048);//DAC���2048*1.2/4096 = 0.6V
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */
void DAC_OutputValue(uint8_t DAC_Channel,uint32_t DACValue)
{
	DACValue &= 0xFFF;//�ж�DACValue������4095

	if(DAC_Channel_0 == DAC_Channel)
	{
		SYS_AFE_DAC0 = DACValue;
	}
	else if(DAC_Channel_1 == DAC_Channel)
  {
		SYS_AFE_DAC1 = DACValue;
  }
}
/**
 *@brief @b ��������:   void DAC_OutputVoltage(uint8_t DAC_Channel,uint32_t DACVoltage)
 *@brief @b ��������:   DAC���ģ������ֵ����
 *@see�����ú�����       ��
 *@param���������       DAC_Channel��DAC_Channel_0/DAC_Channel_1��DACVoltage:DAC�����ѹ��Ӧ������
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        �������DACVoltageΪQ12��ʽ�������1V��ӦDACVoltage = 1<<12 = 4096
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DAC_OutputVoltage(DAC_Channel_0,BIT12 * 0.6);//DAC���0.6V
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */

void DAC_OutputVoltage(uint8_t DAC_Channel,uint16_t DACVoltage)
{
	u32 temp = 0;
	u32 range = 0;
	
	if (((SYS_AFE_REG1 >> (6+DAC_Channel)) & 0x01) == DAC_RANGE_1V2)
	{ /* ����DAC 1.2V����У��ֵ */
		range = (uint16_t)((1.0/1.2)*BIT12);
	}
	else if (((SYS_AFE_REG1 >> (6+DAC_Channel)) & 0x01) == DAC_RANGE_4V85)
	{ /* ����DAC 4.85V����У��ֵ */
		range = (uint16_t)((1.0/4.85)*BIT12);
	} 
	
	temp = (DACVoltage * range + BIT11) >> 12; 
	temp &= 0xFFF;//���ֵ4095����

	if(DAC_Channel_0 == DAC_Channel)
	{	
		SYS_AFE_DAC0 = temp ;
	}
	else if(DAC_Channel_1 == DAC_Channel)
	{
		SYS_AFE_DAC1 = temp ;
	}	
}
/**
 *@brief @b ��������:   void DAC_Cmd(uint8_t DAC_Channel,FuncState state)
 *@brief @b ��������:   DAC����ʹ��
 *@see�����ú�����       ��
 *@param���������       DAC_Channel��ͨ��DAC_Channel_0/DAC_Channel_1��state��ENABLE/DISABLE
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
          DAC_Cmd(DAC_Channel_0,ENABLE);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */

void DAC_Cmd(uint8_t DAC_Channel,FuncState state)
{
	if(DAC_Channel_0 == DAC_Channel)
	{
		SYS_AnalogModuleClockCmd(SYS_AnalogModule_DAC0, state);  /* DAC0 ʱ��ʹ�� */
	}
	else if(DAC_Channel_1 == DAC_Channel)
	{
		 SYS_AnalogModuleClockCmd(SYS_AnalogModule_DAC1,state);  /* DAC1 ʱ��ʹ�� */
	}
}

void DAC_Output(uint8_t DAC_Channel,DAC_InitTypeDef* DAC_InitStruct)
{
	  SYS_WR_PROTECT = 0x7a83;  /* �����Ĵ���д���� */
	if(DAC_Channel_0 == DAC_Channel)
	{
	  SYS_AFE_REG3 |= DAC_InitStruct->DACOUT_EN << 11;/* DAC������� */
	}
	else if(DAC_Channel_1 == DAC_Channel)
	{
	  SYS_AFE_REG3 |= DAC_InitStruct->DACOUT_EN << 15;/* DAC������� */
	}
	  SYS_WR_PROTECT = 0xffff;  /* �����Ĵ���д���� */
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
	
