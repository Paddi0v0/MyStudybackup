/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_adc.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ADC������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��5�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:����NVR_Trim����ADC����ֵ
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2020��12��15�� <td>1.1     <td>Howlet     <td>�޸�
 * </table>
 */

#include "lks32mc08x_adc.h"

/**
 *@brief @b ��������:   void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
 *@brief @b ��������:   ADC��ʼ������
 *@see���������ݣ�      SYS_AnalogModuleClockCmd()
 *@param���������      ADCx��ADC0 , ADC_InitTypeDef *ADC_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			ADC_InitTypeDef ADC_InitStructure;
			ADC_StructInit(&ADC_InitStructure);                            // ��ʼ���ṹ��
			ADC_InitStructure.ADC_CLK_DIV  = ADC_Clock_48M;                // ADCʱ��ѡ��48MHz
			ADC_InitStructure.Align = ADC_LEFT_ALIGN;                      // ADC������������
			ADC_InitStructure.Trigger_Mode = ADC_2SEG_TRG;                 // ����ADCת��ģʽΪ˫��ʽ����
			ADC_InitStructure.FirSeg_Ch = ADC_2_TIMES_SAMPLE;              // ��һ�ι�����2��ͨ��
			ADC_InitStructure.SecSeg_Ch = ADC_2_TIMES_SAMPLE;              // �ڶ��ι�����2��ͨ��
			ADC_InitStructure.ThrSeg_Ch  = 0;                              // �����ι�����0��ͨ��
			ADC_InitStructure.FouSeg_Ch = 0;                               // ���Ķι�����0��ͨ��
			ADC_InitStructure.RefVol = RefVol_2V4;                         // ADC��׼��ѹ2.4V
			ADC_InitStructure.Gain0 = 0x0;                                 // ADC_DAT0-ADC_DAT9 (BIT0~BIT9)�������ֵ�·��������2/3
			ADC_InitStructure.Gain1 = 0x0;                                 // ADC_DAT10-ADC_DAT19(BIT10~BIT19)�������ֵ�·��������2/3
			ADC_InitStructure.Con_Sample = DISABLE;                        // �ر���������
			// �� UTIMER T0 T1 Ӳ����������
			ADC_InitStructure.MCPWM_Trigger_En  = ADC_MCPWM_T0_TRG|ADC_MCPWM_T1_TRG;// ����UTIMERӲ����һ�κ͵ڶ��δ�������
			ADC_InitStructure.UTIMER_Trigger_En = DISABLE;                 // �ر�UTIMERӲ����������
			ADC_InitStructure.Trigger_Cnt = 0;                             // ����(Trigger_Cnt+1)�δ����¼�ִ��һ��ת������
			ADC_InitStructure.IE = ADC_EOS0_IRQ_EN | ADC_EOS1_IRQ_EN;      // ADC��һ�����β��������ж�ʹ��
			ADC_Init(ADC0, &ADC_InitStructure);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2015��11��26�� <td>1.0     <td>Howlet Li       <td>����
 * </table>
 */
void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
	uint16_t t_reg;

	SYS_AnalogModuleClockCmd(SYS_AnalogModule_ADC, ENABLE); // ADCģ��ʹ��

	SYS_WR_PROTECT = 0x7A83;						 //���ϵͳ�Ĵ���д����
	SYS_AFE_REG1 |= ADC_InitStruct->RefVol << 3;		 // ADC��׼��ѹѡ��
	SYS_AFE_REG7 = ADC_InitStruct->ADC_CLK_DIV << 4; // ADCʱ��ѡ��
	SYS_WR_PROTECT = 0x0;							 /*�ر�ϵͳ�Ĵ���д����*/

	ADCx->IE = ADC_InitStruct->IE;

	t_reg = (ADC_InitStruct->Align) | (ADC_InitStruct->DAT0_TH_Type << 1);
	ADCx->CFG = t_reg;

	t_reg = (ADC_InitStruct->MCPWM_Trigger_En) | (ADC_InitStruct->UTIMER_Trigger_En << 4) |
			(ADC_InitStruct->Trigger_Cnt << 8) | (ADC_InitStruct->Trigger_Mode << 12) |
			(ADC_InitStruct->Con_Sample << 14);

	ADCx->TRG = t_reg; // TRIG

	ADCx->CHNT0 = ADC_InitStruct->FirSeg_Ch | (ADC_InitStruct->SecSeg_Ch << 8);
	ADCx->CHNT1 = ADC_InitStruct->ThrSeg_Ch | (ADC_InitStruct->FouSeg_Ch << 8);

	ADCx->DAT0_TH = ADC_InitStruct->DAT0_TH;

	ADCx->GAIN_0 = ADC_InitStruct->Gain0;

	ADCx->GAIN_1 = ADC_InitStruct->Gain1;

	if (ADC_InitStruct->Align == 0)
	{
		ADCx->DC_A0 = Read_Trim(0x00000310) << 4;
		ADCx->DC_A1 = Read_Trim(0x00000314) << 4;
		ADCx->AMC_A0 = Read_Trim(0x00000318);
		ADCx->AMC_A1 = Read_Trim(0x0000031C);

		ADCx->DC_B0 = Read_Trim(0x00000320) << 4;
		ADCx->DC_B1 = Read_Trim(0x00000324) << 4;
		ADCx->AMC_B0 = Read_Trim(0x00000328);
		ADCx->AMC_B1 = Read_Trim(0x0000032C);
	}
	else
	{
		ADCx->DC_A0 = Read_Trim(0x00000310);
		ADCx->DC_A1 = Read_Trim(0x00000314);
		ADCx->AMC_A0 = Read_Trim(0x00000318);
		ADCx->AMC_A1 = Read_Trim(0x0000031C);

		ADCx->DC_B0 = Read_Trim(0x00000320);
		ADCx->DC_B1 = Read_Trim(0x00000324);
		ADCx->AMC_B0 = Read_Trim(0x00000328);
		ADCx->AMC_B1 = Read_Trim(0x0000032C);
	}
}

/**
 *@brief @b ��������:   void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
 *@brief @b ��������:   ADC�ṹ���ʼ��
 *@see���������ݣ�      ��
 *@param���������      ADC_InitTypeDef *ADC_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
		   ADC_InitTypeDef ADC_InitStructure;
		   ADC_StructInit(&ADC_InitStructure); //��ʼ���ṹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��26�� <td>1.0     <td>Howlet     <td>�޸�
 * </table>
 */
void ADC_StructInit(ADC_InitTypeDef *ADC_InitStruct)
{
	ADC_InitStruct->IE = 0;
	ADC_InitStruct->Align = 0;
	ADC_InitStruct->Con_Sample = 0;
	ADC_InitStruct->Trigger_Cnt = 0;
	ADC_InitStruct->FirSeg_Ch = 0;
	ADC_InitStruct->SecSeg_Ch = 0;
	ADC_InitStruct->ThrSeg_Ch = 0;
	ADC_InitStruct->FouSeg_Ch = 0;
	ADC_InitStruct->Trigger_Mode = 0;
	ADC_InitStruct->MCPWM_Trigger_En = 0;
	ADC_InitStruct->UTIMER_Trigger_En = 0;
	ADC_InitStruct->DAT0_TH_Type = 0;
	ADC_InitStruct->Gain0 = 0;
	ADC_InitStruct->Gain1 = 0;
	ADC_InitStruct->RefVol = 0;
                ADC_InitStruct->ADC_CLK_DIV = 0;
}

/**
 *@brief @b ��������:   void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
 *@brief @b ��������:   ADC�жϱ�־λ����
 *@see���������ݣ�      ��
 *@param���������
 * <table>              <tr><td>ADCx��    <td> ADC0
 *                      <tr><td>INT_flag: <td> ADC�жϱ�־
 * 						<tr><th>ADC_DAT0_OV_IRQ_IF       <td>ADC_DAT0����ֵ�жϱ�־λ \n
 * 						<tr><th>ADC_H_CONFLICT_IRQ_IF    <td>Ӳ��������ͻ��־ \n
 * 						<tr><th>ADC_S_CONFLICT_IRQ_IF    <td>���������ͻ��־ \n
 * 						<tr><th>ADC_EOS3_IRQ_IF          <td>���Ķ�ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS2_IRQ_IF          <td>������ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS1_IRQ_IF          <td>�ڶ���ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS0_IRQ_IF          <td>��һ��ɨ������жϱ�־ \n
 * 						<tr><th>ADC_ALL_IRQ_IF           <td>ADCȫ���жϱ�־λ���������ڳ�ʼ�� \n
 * </table>
 *@param���������   ��
 *@return�� �� ֵ��  ��
 *@note����˵����    ��
 *@warning          ��
 *@par ʾ�����룺
 *@code
		   if(ADC_GetIRQFlag(ADC0,ADC_EOS0_IRQ_IF))//��ȡ��һ��ɨ������жϱ�־
		   {
			  ADC_ClearIRQFlag(ADC0,ADC_EOS0_IRQ_IF)//��һ��ɨ������жϱ�־
		   }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void ADC_ClearIRQFlag(ADC_TypeDef *ADCx, uint16_t INT_flag)
{
	ADCx->IF = INT_flag;
}

/**
 *@brief @b ��������:   uint16_t ADC_GetIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
 *@brief @b ��������:   ��ȡADC�жϱ�־
 *@see���������ݣ�      ��
 *@param���������
 * <table>              <tr><td>ADCx��    <td> ADC0
 *                      <tr><td>INT_flag: <td> ADC�жϱ�־
 * 						<tr><th>ADC_DAT0_OV_IRQ_IF       <td>ADC_DAT0����ֵ�жϱ�־λ \n
 * 						<tr><th>ADC_H_CONFLICT_IRQ_IF    <td>Ӳ��������ͻ��־ \n
 * 						<tr><th>ADC_S_CONFLICT_IRQ_IF    <td>���������ͻ��־ \n
 * 						<tr><th>ADC_EOS3_IRQ_IF          <td>���Ķ�ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS2_IRQ_IF          <td>������ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS1_IRQ_IF          <td>�ڶ���ɨ������жϱ�־ \n
 * 						<tr><th>ADC_EOS0_IRQ_IF          <td>��һ��ɨ������жϱ�־ \n
 * 						<tr><th>ADC_ALL_IRQ_IF           <td>ADCȫ���жϱ�־λ���������ڳ�ʼ�� \n
 * </table>
 *@param���������   ��
 *@return�� �� ֵ��  ����Ӧ��־λ�Ƿ�Ϊ1��1λ��λ��0Ϊδ��λ
 *@note����˵����    ��
 *@warning           ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
		   if(ADC_GetIRQFlag(ADC0,ADC_EOS0_IRQ_IF))//��ȡ��һ��ɨ������жϱ�־
		   {
			 ADC_ClearIRQFlag(ADC0,ADC_EOS0_IRQ_IF)//��һ��ɨ������жϱ�־
		   }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
u16 ADC_GetIRQFlag(ADC_TypeDef *ADCx, uint16_t INT_flag)
{
	u8 ADCIRQFlag = 0;
	if (ADCx->IF & INT_flag & ADCx->IE)
	{
		ADCIRQFlag = 1;
	}
	else
	{
		ADCIRQFlag = 0;
	}
	return ADCIRQFlag;
}

/**
 *@brief @b ��������:   void ADC_ChannelConfig(ADC_TypeDef* ADCx,CHNx CHNum, uint16_t Channel_0 ,uint16_t Channel_1)
 *@brief @b ��������:   ADC����ͨ������
 *@see���������ݣ�      CHNx
 *@param���������
					   ADCx��08ϵ�й̶�ѡ��ADC0
 *                     CHNumΪö�����ͣ���Ӧ0-19��Channel_0Ϊ����ͨ����Ŷ�Ӧ0-19
 *                     nChannel��ADC_CHANNEL_0 ~ ADC_CHANNEL_19
 *@param���������     ��
 *@return�� �� ֵ��    ��
 *@note����˵����      ��
 *@warning            ��
 *@par ʾ�����룺
 *@code
		   ADC_ChannelConfig(ADC0,CHN0,ADC_CHANNEL_3);//����ADC��0�β���ADCͨ��3���ź�
		   ADC_ChannelConfig(ADC0,CHN1,ADC_CHANNEL_0);//����ADC��1�β���ADCͨ��0���ź�
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void ADC_ChannelConfig(ADC_TypeDef *ADCx, CHNx CHNum, uint16_t nChannel)
{
	u32 *p;
	u32 reg;
	p = (u32 *)&ADCx->CHN0;
	reg = p[CHNum >> 1];
	reg &= ~(0xff << (8 * (CHNum & 1)));
	reg |= nChannel << (8 * (CHNum & 1));
	p[CHNum >> 1] = reg;
}

/**
 *@brief @b ��������:   s16 ADC_GetConversionValue(DATNx DATNum)
 *@brief @b ��������:   ��ADC����ֵ
 *@see���������ݣ�      DATx
 *@param���������      DATx����ȡ�ڼ��β������ֵ
 *@param���������      ��
 *@return�� �� ֵ��     ��Ӧx�β������ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
		   s16 ADC_samp0 = 0��
		   ADC_samp0 = ADC_GetConversionValue(DAT0);//��ȡADC��0�β������
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
s16 ADC_GetConversionValue(DATx DATNum)
{
	if (DATNum == DAT0)
	{
		return ADC0_DAT0;
	}
	else if (DATNum == DAT1)
	{
		return ADC0_DAT1;
	}
	else if (DATNum == DAT2)
	{
		return ADC0_DAT2;
	}
	else if (DATNum == DAT3)
	{
		return ADC0_DAT3;
	}
	else if (DATNum == DAT4)
	{
		return ADC0_DAT4;
	}
	else if (DATNum == DAT5)
	{
		return ADC0_DAT5;
	}
	else if (DATNum == DAT6)
	{
		return ADC0_DAT6;
	}
	else if (DATNum == DAT7)
	{
		return ADC0_DAT7;
	}
	else if (DATNum == DAT8)
	{
		return ADC0_DAT8;
	}
	else if (DATNum == DAT9)
	{
		return ADC0_DAT9;
	}
	else if (DATNum == DAT10)
	{
		return ADC0_DAT10;
	}
	else if (DATNum == DAT11)
	{
		return ADC0_DAT11;
	}
	else if (DATNum == DAT12)
	{
		return ADC0_DAT12;
	}
	else if (DATNum == DAT13)
	{
		return ADC0_DAT13;
	}
	else if (DATNum == DAT14)
	{
		return ADC0_DAT14;
	}
	else if (DATNum == DAT15)
	{
		return ADC0_DAT15;
	}
	else if (DATNum == DAT16)
	{
		return ADC0_DAT16;
	}
	else if (DATNum == DAT17)
	{
		return ADC0_DAT17;
	}
	else if (DATNum == DAT18)
	{
		return ADC0_DAT18;
	}
	else //(DATNum == DAT19)
	{
		return ADC0_DAT19;
	}
}

/**
 *@brief @b ��������:   void ADC_SoftTrgEN(ADC_TypeDef* ADCx, FuncState state)
 *@brief @b ��������:   ADC�����������ʹ��
 *@see���������ݣ�      FuncState
 *@param���������      ADCx��08ϵ�й̶�ѡ��ADC0
 *                     state��ENABLE������DISABLE������
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ADC0_SWTд0x5AA5������һ�Σ���ɺ��Զ����㣬�ȴ��´��������
 *@warning             ��
 *@par ʾ�����룺
 *@code
		  ADC_SoftTrgEN(ADC0��ENABLE);//�������һ��ADC����
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void ADC_SoftTrgEN(ADC_TypeDef *ADCx, FuncState state)
{
	if (state == ENABLE)
	{
		ADC0_SWT = 0x5AA5;
	}
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
