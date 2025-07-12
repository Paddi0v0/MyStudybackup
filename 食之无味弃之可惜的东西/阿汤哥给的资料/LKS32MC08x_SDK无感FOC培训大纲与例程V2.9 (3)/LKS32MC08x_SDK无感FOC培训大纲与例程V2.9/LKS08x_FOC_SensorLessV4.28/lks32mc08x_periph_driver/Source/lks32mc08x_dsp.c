/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_dsp.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DSP�������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2015��11��5�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:DSP���ú����������жϴ�����¸�λDSP����
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��18�� <td>2.0     <td>Zhu Jie   <td>�޸�
 * </table>
 */

#include "lks32mc08x_dsp.h"



/**
 *@brief @b ��������:   void DSP_Cmd(FuncState state)
 *@brief @b ��������:   DSPʹ�ܺ͹ر�
 *@see���������ݣ�       SYS_ModuleClockCmd()
 *@param���������       state��ENABLE��ʹ�� �� DISABLE:ʧ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			DSP_Cmd(ENABLE);//ʹ��DSPʱ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>1.0     <td>Zhu Jie          <td>����
 * </table>
 */
void DSP_Cmd(FuncState state)
{
   SYS_ModuleClockCmd(SYS_Module_DSP, state); /* DSPʱ��ʹ�� */
}

/**
 *@brief @b ��������:   void DSP_CalcDivision(stru_DiviComponents * stru_Divi)
 *@brief @b ��������:   DSPʹ�ܺ͹ر�
 *@see���������ݣ�       stru_DiviComponents ��SYS_SoftResetModule()
 *@param���������       .nDID,������   .nDIS����
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        
                        1������10������������� \n
                        2��������DividendΪ32λ��С������Χ�� -(2^31 - 1) ~ (2^31 - 1) \n
                        3������DivisorΪ16λ��С������Χ�� -(2^15 - 1) ~ (2^15 - 1) \n
                        4����������֧�ָ�ֵΪ-2^31��������֧�ָ�ֵΪ-2^15 \n
                        5����QuotientΪ32λ��С \n
                        6������RemainderΪ16λ��С
 *@warning              ��
 *@par ʾ�����룺
 *@code    
            stru_DiviComponents DiviData; //���������������ṹ�����
            DiviData.Dividend = 100;//��������ֵ
            DiviData.Divisor = 10;  //������ֵ
			DSP_CalcDivision(&DiviData);
            �������̴���λ�ã�DiviData.Quotient
            ��������������λ�ã�DiviData.Remainder
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��5��25�� <td>1.0         <td>cfwu          <td>����
 * </table>
 *@par �޸���־:ʹ�ýṹ����������ӷ�ֹDSP���㱻����߼����    
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>2.0     <td>Zhu Jie          <td>�޸�
 * </table>
 */

static volatile u8 dsp_flg; /**< ��־λ�������жϳ����Ƿ񱻴��*/
void DSP_CalcDivision(stru_DiviComponents * stru_Divi)
{
	  uint8_t dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        DSP_DID = stru_Divi->Dividend; /* �������Ĵ�����ֵ */
        DSP_DIS = stru_Divi->Divisor;  /* �����Ĵ�����ֵ   */
	
	      stru_Divi->Quotient = DSP_QUO; /* ȡ���̼Ĵ���ֵ */
	      stru_Divi->Remainder = DSP_REM;/* ȡ�������Ĵ���ֵ */
    } while (dsp_flg_old != dsp_flg);  /* ��ֹDSP���㱻��� */
}

/**
 *@brief @b ��������:   void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise)
 *@brief @b ��������:   DSP��������
 *@see���������ݣ�       stru_CordicComponents ��Arc_or_SinCFuction ��SYS_SoftResetModule()
 *@param���������       Cordic���ǶȻ�����X��Y��choise������ģʽѡ�� 0 Arctan;1 sin/cos
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        
                        1������8������������� \n
 *                      2��Theta(�Ƕ�)Ϊ16λ�з��Ŷ���������ʾ��Χ (-(32768) ~ 32767 ) ��Ӧ(-(��) ~ ��) \n
 *                      3��SinValueֵΪ16λ�з��Ŷ����������� 1bit ����λ��15bitС��λ����ʾ��Χ(-(1) ~ 1)  \n
 *                      4��CosValueֵΪ16λ�з��Ŷ����������� 1bit ����λ��15bitС��λ����ʾ��Χ(-(1) ~ 1 ) \n
 *                      5��ArctanXֵΪ16λ�з��Ŷ�������Q15 ��ʽ������ 1bit ����λ��15bit����λ����ʾ��Χ(-(32768)~32767) \n
 *                      6��ArctanYֵΪ16λ�з��Ŷ�������Q15 ��ʽ������ 1bit ����λ��15bit����λ����ʾ��Χ(-(32768)~32767) \n
 *                      7��ArctanValueֵΪ16λ�з��Ŷ���������ʾ��Χ (-(32768) ~ 32767)��Ӧ(-(��) ~ ��)
 *@warning              ��
 *@par ʾ�����룺
 *@code    
 *           stru_CordicComponents CordicData;// �������Ǻ�������ṹ�����
 *       ʾ��1��
 *           CordicData.ArctanX = 100; //����X��ֵ  ȡֵ��Χ(-(32768)~32767)
 *           CordicData.ArctanY = 10;  //����Y��ֵ  ȡֵ��Χ(-(32768)~32767)
 *			 DSP_CalcDivision(&CordicData,ARCTAN);
 *           arctan����λ�ã� CordicData.ArctanValue �������Χ-(32768) ~ 32767��Ӧ(-(��) ~ ��)��
 *       ʾ��2��
 *           CordicData.Theta = 0;      //�Ƕȸ�ֵ  ȡֵ��Χ(-(32768) ~ 32767) ��Ӧ(-(��) ~ ��)
 *			 DSP_CalcDivision(&CordicData,SINCOS); 
 *           sin�������λ�ã� CordicData.SinValue   �������Χ-(32768) ~ 32767��Ӧ(-(1) ~ 1)��
 *           cos�������λ�ã� CordicData.CosValue   �������Χ-(32768) ~ 32767��Ӧ(-(1) ~ 1)��
 *          
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��5��25�� <td>1.0         <td>cfwu          <td>����
 * </table>
 *@par �޸���־:ʹ�ýṹ����������ӷ�ֹDSP���㱻����߼����    
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>2.0     <td>Zhu Jie          <td>�޸�
 * </table>
 */
void DSP_CalcCordic(stru_CordicComponents *Cordic, Arc_or_SinCFuction choise)
{
	  uint8_t dsp_flg_old;
    do
    {
        dsp_flg++;
        dsp_flg_old = dsp_flg;
        SYS_SoftResetModule(SYS_Module_DSP);
        if(choise)/* Sin Cos ֵ����*/
				{
				 DSP_SC |= BIT2;/* ����ģʽѡ�� Sin��Cos */
				 DSP_CORDIC_THETA = Cordic->Theta;   /* �ǶȼĴ�����ֵ */
				 Cordic->SinValue = DSP_CORDIC_SIN;  /* ȡ��sin�Ĵ���ֵ */
				 Cordic->CosValue = DSP_CORDIC_COS;  /* ȡ��cos�Ĵ���ֵ */
				}
				else /* Arctan ֵ����*/
				{
				 DSP_SC &= (~BIT2);/* ����ģʽѡ�� Arctan */
				 DSP_CORDIC_X = Cordic->ArctanX; /* ����X�Ĵ�����ֵ */
				 DSP_CORDIC_Y = Cordic->ArctanY; /* ����Y�Ĵ�����ֵ */
				 Cordic->ArctanValue = DSP_CORDIC_ARCTAN;/* ȡ��arctan�Ĵ���ֵ */
				}
    } while (dsp_flg_old != dsp_flg);  /* ��ֹDSP���㱻��� */
	
}

/**
 *@brief @b ��������:   s16 DSP_CalcSprt(s16 X,s16 Y)
 *@brief @b ��������:   DSP������ƽ���󿪷�����
 *@see���������ݣ�       ��
 *@param���������       X��Y
 *@param���������       ��
 *@return�� �� ֵ��      sqrt(X^2+Y^2)���ֵ
 *@note����˵����        
                        1������8�������������
                        2��XֵΪ16λ�з��Ŷ�������Q15 ��ʽ������ 1bit ����λ��15bit����λ����ʾ��Χ(-(32768)~32767) \n
                        3��YֵΪ16λ�з��Ŷ�������Q15 ��ʽ������ 1bit ����λ��15bit����λ����ʾ��Χ(-(32768)~32767) \n
                        4��������sqrt(X^2+Y^2)��Ϊ16λ�з��Ŷ�����������1bit����λ��15bit����λ����ʾ��Χ(-(32768) ~ 32767)
 *@warning              ��
 *@par ʾ�����룺
 *@code    
            s16 DSP_Value = 0;
            DSP_Value = DSP_CalcSprt(50,100);//sqrt(50^2+100^2) 50��ƽ����100��ƽ���󿪸���
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��5��25�� <td>1.0         <td>cfwu          <td>����
 * </table>
 *@par �޸���־:ʹ�ýṹ����������ӷ�ֹDSP���㱻����߼����    
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>2.0     <td>Zhu Jie          <td>�޸�
 * </table>
 */
static volatile u8 dsp_flg1; /**< ��־λ�������жϳ����Ƿ񱻴��*/
s16 DSP_CalcSprt(s16 X,s16 Y)
{
	 uint8_t dsp_flg_old;
     s16  dsp_mod;
    do
    {
        dsp_flg1 ++;
        dsp_flg_old = dsp_flg1; 
        DSP_CORDIC_X = X;
	    DSP_CORDIC_Y = Y;
      dsp_mod = DSP_CORDIC_MOD;;
    } while (dsp_flg_old != dsp_flg1); /* ��ֹDSP���㱻��� */
	return dsp_mod; 
}

/**
 *@brief @b ��������:   uint32_t DSP_GetSqrt(uint32_t Data)
 *@brief @b ��������:   DSP��ƽ������
 *@see���������ݣ�       ��
 *@param���������       ��������
 *@param���������       ��
 *@return�� �� ֵ��      ������
 *@note����˵����        
                        1������8������������� \n
                        2����������DataΪ32λ�޷�����������Χ��(0 ~ (2^31)) \n
                        3��������sqrt(Data)Ϊ16λ�޷�����������Χ��(0 ~ (2^15)) \n
 *@warning              ��
 *@par ʾ�����룺
 *@code    
            u16 GetSqrt_Value = 0;
            GetSqrt_Value = DSP_GetSqrt(100);//sqrt(100) 100�����Ž��Ϊ10
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��5��25�� <td>1.0         <td>cfwu          <td>����
 * </table>
 *@par �޸���־:ʹ�ýṹ����������ӷ�ֹDSP���㱻����߼����   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��18�� <td>2.0     <td>Zhu Jie          <td>�޸�
 * </table>
 */
static volatile u8 dsp_flg2; /**< ��־λ�������жϳ����Ƿ񱻴��*/
uint32_t DSP_GetSqrt(uint32_t Data)
{
	  uint8_t dsp_flg_old;
      uint32_t  tdsp_sqrt;
    do
    {
        dsp_flg2++;
        dsp_flg_old = dsp_flg2;
        DSP_RAD = Data;  /*�������Ĵ�����ֵ*/
       tdsp_sqrt = DSP_SQRT;
    } while (dsp_flg_old != dsp_flg2);  /* ��ֹDSP���㱻��� */
	 
   return tdsp_sqrt ;
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
