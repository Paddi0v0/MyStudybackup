/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_tmp.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� оƬ�����¶ȴ����������������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��3��9��  <td>1.0     <td>Zhu Jie     <td>�޸�
 * </table>
 */

#include "lks32mc07x_tmp.h"
#include "lks32mc07x_sys.h"
Stru_TempertureCof_Def m_TempertureCof;     /**< �¶ȴ�����ϵ�� */

/**
 *@brief @b ��������:   void TempSensor_Init(void)
 *@brief @b ��������:   �¶ȴ�������ʼ��
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		   TempSensor_Init(); //�¶ȴ�������ʼ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��5�� <td>1.0     <td>Howlet      <td>����
 * <tr><td>2023��3��9��  <td>2.0     <td>Zhu Jie     <td>�޸�
 * </table>
 */
void TempSensor_Init(void)
{
    SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_TMP,ENABLE);

	  m_TempertureCof.nCofA    = Read_Trim(0x000014D0);
    m_TempertureCof.nOffsetB = Read_Trim(0x000014D4);
	  SYS_WR_PROTECT = 0x7a93;   /* ����ϵͳ�Ĵ���д���� */
}

/**
 *@brief @b ��������:   s16 GetCurrentTemperature(s16 ADC_value)
 *@brief @b ��������:   ��ȡ��ǰ�¶�ֵ
 *@see���������ݣ�       ��
 *@param���������       ADC_value: ADCͨ��14Ϊ�¶ȴ�������ADC�������ֵ
 *@param���������       ��
 *@return�� �� ֵ��      t_Temperture����ǰ�¶�ֵ����λ��1��Lsb����0.1��
 *@note����˵����        ADC_value�����ADCֵ���������Ҷ��룬�������Ѿ�����ADC������Ҷ����ж�
 *@warning              ��
 *@par ʾ�����룺
 *@code
           s16 temp_value = 0;
		   temp_value = GetCurrentTemperature(ADC0_DATA0); //��ȡ��ǰ�¶�ֵ,��λ0.1��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��5�� <td>1.0     <td>Howlet      <td>����
 * <tr><td>2023��3��9��  <td>2.0     <td>Zhu Jie     <td>�޸�
 * </table>
 */
s16 GetCurrentTemperature(s16 ADC_value)
{
    s16 t_Temperture;  
    if(ADC1->CFG & BIT10)
		{
        t_Temperture = (m_TempertureCof.nOffsetB - ((s32)m_TempertureCof.nCofA * ADC_value) / 1000);
		}
		else
		{
		    t_Temperture = (m_TempertureCof.nOffsetB - ((s32)m_TempertureCof.nCofA * (ADC_value >> 4)) / 1000);
		}

    return t_Temperture;
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
