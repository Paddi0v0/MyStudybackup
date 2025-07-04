/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_adc.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ADC������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	       <th>Version   <th>Author  <th>Description
 * <tr><td>2023��2��27�� <td>1.0     <td>ZhuJie     <td>����
 * </table>
 */
#include "lks32mc07x_adc.h"
#include "lks32mc07x_sys.h"

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
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* this)
{
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_ADC, ENABLE); // ADCģ��ʹ��

    ADCx->IE   = this->IE | this->RE;
    ADCx->CHNT =   (this->IS1   << 8 ) | (this->S2          << 4) \
                   | (this->S1    << 0 );
    ADCx->TRIG = this->TRIG;
    ADCx->CFG  =   (this->NSMP  << 12) | (this->DATA_ALIGN  << 10) \
                   | (this->CSMP  << 8 ) | (this->TCNT        << 4 ) \
                   | (this->TROVS << 3 ) | (this->OVSR        << 0 ) ;

    ADCx->LTH = this->LTH;
    ADCx->HTH = this->HTH;
    ADCx->GEN = this->GEN;

    {
        ADC0->DC0  = Read_Trim(0x00001420) ;  /* 3.6V���̶�ӦDC offset    */
        ADC0->AMC0 = Read_Trim(0x00001424) ;  /* 3.6V���̶�Ӧ����У��ϵ�� */
        ADC0->DC1  = Read_Trim(0x00001428) ;  /* 7.2V���̶�ӦDC offset    */
        ADC0->AMC1 = Read_Trim(0x0000142c) ;  /* 7.2V���̶�Ӧ����У��ϵ�� */

        ADC1->DC0  = Read_Trim(0x00001430) ;  /* 3.6V���̶�ӦDC offset    */
        ADC1->AMC0 = Read_Trim(0x00001434) ;  /* 3.6V���̶�Ӧ����У��ϵ�� */
        ADC1->DC1  = Read_Trim(0x00001438) ;  /* 7.2V���̶�ӦDC offset    */
        ADC1->AMC1 = Read_Trim(0x0000143c) ;  /* 7.2V���̶�Ӧ����У��ϵ�� */
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
void ADC_StructInit(ADC_InitTypeDef* this)
{
    this->IE         = 0    ;   // �ж�ʹ��
    this->RE         = 0    ;   // DMA����ʹ��
    this->NSMP       = 0    ;   // ���β���ʹ��
    this->DATA_ALIGN = 0    ;   // DAT���뷽ʽ
    this->CSMP       = 0    ;   // ��������ʹ��
    this->TCNT       = 0    ;   // ����һ�β���������¼���
    this->TROVS      = 0    ;   // ����������ģʽ
    this->OVSR       = 0    ;   // ��������
    this->TRIG       = 0    ;   // �����ź�
    this->S1         = 1    ;   // ��һ�γ����������
    this->S2         = 1    ;   // �ڶ��γ����������
    this->IS1        = 1    ;   // ���в�������
    this->GAIN       = 0    ;   // ����Ĭ��3.6V
}

/**
 *@brief @b ��������:   void ADC_ClearIRQFlag(ADC_TypeDef* ADCx, uint16_t INT_flag)
 *@brief @b ��������:   ADC�жϱ�־λ����
 *@see���������ݣ�      ��
 *@param���������
 * <table>              <tr><td>ADCx��    <td> ADC0
 *                      <tr><td>INT_flag: 					 <td> ADC�жϱ�־
 * 											<tr><th>ADC_ISF_IF    			 <td>���в�������жϱ�־ \n
 * 											<tr><th>ADC_HERR_IF          <td>Ӳ�����������ڷǿ���״̬�жϱ�־ \n
 * 											<tr><th>ADC_SERR_IF          <td>������������ڷǿ���״̬�жϱ�־ \n
 * 											<tr><th>ADC_AWD0_IF          <td>��ֵ��� 0 �����жϱ�־ \n
 * 											<tr><th>ADC_SF2_IF           <td>�ڶ���ɨ������жϱ�־ \n
 * 											<tr><th>ADC_SF1_IF           <td>��һ��ɨ������жϱ�־ \n
 * 											<tr><th>ADC_ALL_IF           <td>����ADC�жϱ�־ \n
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
 * <tr><td>2023��02��27�� <td>1.0     <td>Zhu Jie      <td>����
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
 * <table>              <tr><td>ADCx��        			 <td> ADC0
 *                      <tr><td>INT_flag: 					 <td> ADC�жϱ�־
 * 											<tr><th>ADC_ISF_IF    			 <td>���в�������жϱ�־ \n
 * 											<tr><th>ADC_HERR_IF          <td>Ӳ�����������ڷǿ���״̬�жϱ�־ \n
 * 											<tr><th>ADC_SERR_IF          <td>������������ڷǿ���״̬�жϱ�־ \n
 * 											<tr><th>ADC_AWD0_IF          <td>��ֵ��� 0 �����жϱ�־ \n
 * 											<tr><th>ADC_SF2_IF           <td>�ڶ���ɨ������жϱ�־ \n
 * 											<tr><th>ADC_SF1_IF           <td>��һ��ɨ������жϱ�־ \n
 * </table>
 *@param���������   ��
 *@return�� �� ֵ��  ����Ӧ��־λ�Ƿ�Ϊ1��1λ��λ��0Ϊδ��λ
 *@note����˵����    ��
 *@warning           ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
		   if(ADC_GetIRQFlag(ADC0,ADC_SF1_IF))//��ȡ��һ��ɨ������жϱ�־
		   {
			     ADC_ClearIRQFlag(ADC0,ADC_SF1_IF) //��һ��ɨ������жϱ�־
		   }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��27�� <td>1.0     <td>Zhu Jie      <td>����
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
 *@brief @b ��������:   void ADC_CHN_GAIN_CFG(ADC_TypeDef *ADCx,CHNx CHNum,u8 passageway, ADC_GAINx GAIN3V6_or_7V2)
 *@brief @b ��������:   ADC����ͨ������
 *@see���������ݣ�      CHNx
 *@param���������
					             ADCx��07ϵ����ADC0��ADC1
 *                     CHNumΪö�����ͣ���Ӧ0-15��Channel_0Ϊ����ͨ����Ŷ�Ӧ0
 *                     nChannel��ADC_CHANNEL_0 ~ ADC_CHANNEL_15 ͨ��ѡ��
 *                     ADC_GAINx��ADC_GAIN3V6,ADC_GAIN7V2 ����ѡ��
 *@param���������     ��
 *@return�� �� ֵ��    ��
 *@note����˵����      ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
		   ADC_CHN_GAIN_CFG(ADC0,CHN0,ADC_CHANNEL_9,ADC_GAIN3V6);//����ADC0��0�β���ADCͨ��9���źţ�ѡ������Ϊ3.6V
		   ADC_CHN_GAIN_CFG(ADC1,CHN1,ADC_CHANNEL_9,ADC_GAIN7V2);//����ADC1��1�β���ADCͨ��9���źţ�ѡ������Ϊ7.2V
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��03��9�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void ADC_CHN_GAIN_CFG(ADC_TypeDef *ADCx, CHNx CHNum, u8 nChannel, ADC_GAINx GAIN3V6_or_7V2)
{
    u8 val;
    u32 reg;

    if(GAIN3V6_or_7V2 == 1)
    {
        ADCx->GAIN |= (GAIN3V6_or_7V2 << CHNum);
    }

    val = CHNum >> 2;
    val &= 3;
    CHNum &= 3;
    CHNum *= 4;
    reg = (&ADCx->CHN0)[val] & ~(0xf << CHNum);
    (&ADCx->CHN0)[val] = reg | (nChannel << CHNum);
}

/**
 *@brief @b ��������:   s16 ADC_GetConversionValue(ADC_TypeDef *ADCx,DATx DATNum)
 *@brief @b ��������:   ��ADC����ֵ
 *@see���������ݣ�      ADCx��ADC0��ADC1
 *@param���������      DATx����ȡ�ڼ��β������ֵ��ȡֵΪ0-15
 *@param���������      ��
 *@return�� �� ֵ��     ��Ӧx�β������ֵ
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
		   s16 ADC_samp0 = 0��
		   ADC_samp0 = ADC_GetConversionValue(ADC0,DAT3);//��ȡADC0��3�β������
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��27�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
s16 ADC_GetConversionValue(ADC_TypeDef *ADCx, DATx DATNum)
{
    if (DATNum == DAT0)
    {
        return ADCx->DAT0;
    }
    else if (DATNum == DAT1)
    {
        return ADCx->DAT1;
    }
    else if (DATNum == DAT2)
    {
        return ADCx->DAT2;
    }
    else if (DATNum == DAT3)
    {
        return ADCx->DAT3;;
    }
    else if (DATNum == DAT4)
    {
        return ADCx->DAT4;
    }
    else if (DATNum == DAT5)
    {
        return ADCx->DAT5;
    }
    else if (DATNum == DAT6)
    {
        return ADCx->DAT6;
    }
    else if (DATNum == DAT7)
    {
        return ADCx->DAT7;
    }
    else if (DATNum == DAT8)
    {
        return ADCx->DAT8;
    }
    else if (DATNum == DAT9)
    {
        return ADCx->DAT9;
    }
    else if (DATNum == DAT10)
    {
        return ADCx->DAT10;
    }
    else if (DATNum == DAT11)
    {
        return ADCx->DAT11;
    }
    else if (DATNum == DAT12)
    {
        return ADCx->DAT12;
    }
    else if (DATNum == DAT13)
    {
        return ADCx->DAT13;
    }
    else if (DATNum == DAT14)
    {
        return ADCx->IDAT0;
    }
    else if (DATNum == DAT15)
    {
        return ADCx->IDAT1;
    }
    else
    {
        return 0;
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
 * <tr><td>2023��02��27�� <td>1.0     <td>Zhu Jie      <td>����
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
