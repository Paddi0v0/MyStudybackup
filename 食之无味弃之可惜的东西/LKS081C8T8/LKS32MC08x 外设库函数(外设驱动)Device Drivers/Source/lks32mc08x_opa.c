/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_opa.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� OPA�˷������������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��06��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */

#include "lks32mc08x_opa.h"


/**
 *@brief @b ��������:   void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct)
 *@brief @b ��������:   OPA��ʼ������
 *@see���������ݣ�       ��
 *@param���������       enumOPA opa , OPA_InitTypeDef *OPA_InitStruct 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			    OPA_InitTypeDef OPA_InitStruct;
				OPA_StructInit(&OPA_InitStruct);       //��ʼ���ṹ��
				OPA_InitStruct.OPA_CLEna = ENABLE;     //ʹ��OPA
				OPA_InitStruct.OPA_Gain = PGA_GAIN_20; //OPA�ڲ��������ѡ��200��10.2
				OPA_Init(OPA0, &OPA_InitStruct);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��02�� <td>1.0       <td>cfwu           <td>����
 * </table>
 */
void OPA_Init(enumOPA opa, OPA_InitTypeDef* OPA_InitStruct)
{
	uint32_t tmp1, tmp2;
	SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
	tmp1 = SYS_AFE_REG0;
	tmp2 = SYS_AFE_REG5;

	if (opa == OPA0)
	{
		tmp1 &= ~(BIT0 | BIT1);
		tmp1 |= OPA_InitStruct->OPA_Gain;

		tmp2 &= ~BIT2;
		tmp2 |= OPA_InitStruct->OPA_CLEna << 2;
	}
	else if (opa == OPA1)
	{
		tmp1 &= ~(BIT2 | BIT3);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 2);

		tmp2 &= ~BIT3;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 3);
	}
	else if (opa == OPA2)
	{
		tmp1 &= ~(BIT4 | BIT5);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 4);

		tmp2 &= ~BIT4;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 4);
	}
	else if (opa == OPA3)
	{
		tmp1 &= ~(BIT6 | BIT7);
		tmp1 |= (OPA_InitStruct->OPA_Gain << 6);

		tmp2 &= ~BIT5;
		tmp2 |= (OPA_InitStruct->OPA_CLEna << 5);
	}

	SYS_AFE_REG0 = tmp1;
	SYS_AFE_REG5 = tmp2;
	SYS_WR_PROTECT = 0x0;   /* ����ϵͳ�Ĵ���д���� */
}

/**
 *@brief @b ��������:   void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct)
 *@brief @b ��������:   OPA�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       OPA_InitTypeDef *OPA_InitStruct 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			    OPA_InitTypeDef OPA_InitStruct;
				OPA_StructInit(&OPA_InitStruct);       //��ʼ���ṹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��02�� <td>1.0       <td>cfwu           <td>����
 * </table>
 */
void OPA_StructInit(OPA_InitTypeDef* OPA_InitStruct)
{
   OPA_InitStruct->OPA_Gain = PGA_GAIN_4P25;
   OPA_InitStruct->OPA_CLEna = DISABLE;
}

/**
 *@brief @b ��������:   void OPA_OUT(enumOPA opa, FuncState state)
 *@brief @b ��������:   OPA�ⲿ�����������ⲿP2.7����
 *@see���������ݣ�       ��
 *@param���������       enumOPA opa, FuncState state
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			   OPA_OUT(OPA0,ENABLE); //��OPA0���ͨ���ⲿP2.7�����ͳ�
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��04��20�� <td>1.0       <td>HuangMG        <td>����
 * </table>
 */
void OPA_OUT(enumOPA opa, FuncState state)
{
	SYS_WR_PROTECT = 0x7a83;   /* ���ϵͳ�Ĵ���д���� */
  if(state != DISABLE)
	{
		if (opa == OPA0)
		{		
		  SYS_AFE_REG2 = 0x01;
		}
		else if (opa == OPA1)
		{
			SYS_AFE_REG2 = 0x02;
		}
		else if (opa == OPA2)
		{
			SYS_AFE_REG2 = 0x03;
		}
		else if (opa == OPA3)
		{
			SYS_AFE_REG2 = 0x04;
		}
  }else{
		  SYS_AFE_REG2 = 0x00;
	}
   SYS_WR_PROTECT = 0x0;   /* ����ϵͳ�Ĵ���д���� */
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
