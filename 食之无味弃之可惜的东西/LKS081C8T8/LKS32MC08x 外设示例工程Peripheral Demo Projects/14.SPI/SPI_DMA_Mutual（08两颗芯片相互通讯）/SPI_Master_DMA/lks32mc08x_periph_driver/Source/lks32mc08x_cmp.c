/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_cmp.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� cmp������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2019��3��5�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */
#include "lks32mc08x_cmp.h"

/**
 *@brief @b ��������:   CMP_Init(CMP_InitTypeDef* this)
 *@brief @b ��������:   CMP��ʼ��
 *@see�����ú�����      SYS_AnalogModuleClockCmd()
 *@param���������      CMP_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            CMP_InitTypeDef CMP_InitStruct;
            CMP_StructInit(&CMP_InitStruct);
            CMP_InitStruct.CLK10_EN             = ENABLE        ;   // �Ƚ��� 1/0 �˲�ʱ��ʹ��
            CMP_InitStruct.FIL_CLK10_DIV16      = 15            ;   // �Ƚ��� 1/0 �˲�ϵ��       0-0xF
            CMP_InitStruct.FIL_CLK10_DIV2       = 0             ;   // �Ƚ��� 1/0 �˲���λϵ��   0-0x3   ���շ�Ƶϵ��Ϊ (DIV16+1)<<DIV2
            CMP_InitStruct.CMP_FT               = DISABLE       ;   // �Ƚ������ٱȽ�ʹ��
            CMP_InitStruct.CMP_HYS              = 0             ;   // �Ƚ����ز�ѡ��            0���ز�20mV 1���ز�0mV
            
            CMP_InitStruct.CMP1_EN              = ENABLE        ;   // �Ƚ��� 1 ʹ��
            CMP_InitStruct.CMP1_IN_EN           = ENABLE        ;   // �Ƚ��� 1 �ź�����ʹ�� Ӱ��Ƚ�������ģ���źţ�ģ�ⲿ�ֲ���Ӱ��
            CMP_InitStruct.CMP1_SELP            = CMP1_SELP_IP2 ;   // �Ƚ��� 1 �ź�����ѡ�� �� CMP1_SELP_CMP1_IP0 �Ƚ�������
            CMP_InitStruct.CMP1_SELN            = CMP_SELN_DAC  ;   // �Ƚ��� 1 �źŸ���ѡ��
            CMP_InitStruct.CMP1_IE              = ENABLE        ;   // �Ƚ��� 1 �ж�ʹ��
            CMP_InitStruct.CMP1_POL             = 0             ;   // �Ƚ��� 1 ����ѡ��     0���ߵ�ƽ��Ч 1���͵�ƽ��Ч ֻӰ��Ƚ����ж�
            CMP_InitStruct.CMP1_RE              = DISABLE       ;   // �Ƚ��� 1 DMA ����ʹ��
            CMP_InitStruct.CMP1_IRQ_TRIG        = DISABLE       ;   // �Ƚ��� 1 ���ش���ʹ��
            CMP_InitStruct.CMP1_CHN3P_WIN_EN    = DISABLE       ;   // MCPWM ģ�� CHN3_P ͨ��ʹ�ܱȽ��� 1 ����
            CMP_InitStruct.CMP1_CHN2P_WIN_EN    = DISABLE       ;   // MCPWM ģ�� CHN2_P ͨ��ʹ�ܱȽ��� 1 ����
            CMP_InitStruct.CMP1_CHN1P_WIN_EN    = DISABLE       ;   // MCPWM ģ�� CHN1_P ͨ��ʹ�ܱȽ��� 1 ����
            CMP_InitStruct.CMP1_CHN0P_WIN_EN    = DISABLE       ;   // MCPWM ģ�� CHN0_P ͨ��ʹ�ܱȽ��� 1 ����
            CMP_InitStruct.CMP1_W_PWM_POL       = DISABLE       ;   // �Ƚ��� 1 ���� PWM �źż���ѡ��  0:�ߵ�ƽ��Ч��1:�͵�ƽ��Ч
            CMP_Init(&CMP_InitStruct);
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2018��11��26�� <td>1.0     <td>Howlet Li       <td>����
 * </table>
 */
void CMP_Init(CMP_InitTypeDef* this)
{
    // Comparator��ʼ��
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP1,this->CMP1_EN);
    SYS_AnalogModuleClockCmd(SYS_AnalogModule_CMP0,this->CMP0_EN);
    SYS_WR_PROTECT = 0x7A83;
    
    SYS_AFE_REG3 |= (this->CMP0_SELN << 2) | (this->CMP1_SELN << 4) | (this->CMP0_SELP << 8) |
                    (this->CMP1_SELP << 12) | (this->CMP_HYS << 7);

    /* CMP_HYS �Ƚ����ͻع��ܣ�Ĭ��20mV, д0�ر� */

    SYS_AFE_REG1 |= this->CMP_FT; /* ���ٱȽϣ�Enable�� 30nS */

    SYS_AFE_REG5 |= (this->CMP0_EN << 6) | (this->CMP1_EN << 7);
    
    SYS_WR_PROTECT = 0;
    
    CMP -> IE      = (this -> CMP1_IE           <<  1) | (this -> CMP0_IE);
    if(this->CLK10_EN == DISABLE)
    {
        if(this->CMP1_IN_EN || this->CMP0_IN_EN)
        {
            CMP->TCLK = BIT3;
        }
        else
        {
            CMP->TCLK = 0;
        }
    }
    else
    {
        CMP->TCLK  = (this -> FIL_CLK10_DIV16   <<  4) | (this -> CLK10_EN          <<  3) |
                     (this -> FIL_CLK10_DIV2);
    }
    CMP -> CFG     = (this -> CMP1_W_PWM_POL    <<  7) | (this -> CMP1_IRQ_TRIG     <<  6) |
                     (this -> CMP1_IN_EN        <<  5) | (this -> CMP1_POL          <<  4) |
                     (this -> CMP0_W_PWM_POL    <<  3) | (this -> CMP0_IRQ_TRIG     <<  2) |
                     (this -> CMP0_IN_EN        <<  1) | (this -> CMP0_POL);           
    CMP -> BLCWIN  = (this -> CMP1_CHN3P_WIN_EN <<  7) | (this -> CMP1_CHN2P_WIN_EN <<  6) |
                     (this -> CMP1_CHN1P_WIN_EN <<  5) | (this -> CMP1_CHN0P_WIN_EN <<  4) |
                     (this -> CMP0_CHN3P_WIN_EN <<  3) | (this -> CMP0_CHN2P_WIN_EN <<  2) |
                     (this -> CMP0_CHN1P_WIN_EN <<  1) | (this -> CMP0_CHN0P_WIN_EN);
    CMP -> IF = 3;
}
/**
 *@brief @b ��������:   void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
 *@brief @b ��������:   ADC�ṹ���ʼ��
 *@see�����ú�����       ��
 *@param���������       CMP_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            CMP_StructInit CMP_InitStructure;
		    CMP_StructInit(&CMP_InitStructure); //��ʼ���ṹ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2018��11��26�� <td>1.0     <td>Howlet Li       <td>����
 * </table>
 */
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{
    memset(CMP_InitStruct, 0, sizeof(CMP_InitTypeDef));
}

/**
 *@brief @b ��������:   u8 CMP_GetIRQFlag(u8 CMPx)
 *@brief @b ��������:   ��ȡ�Ƚ����ж�
 *@see�����ú�����       ��
 *@param���������       CMP0��CMP1����ȡCMP0��CMP1�жϱ�־λ
 *@param���������       ��
 *@return�� �� ֵ��      TIM�жϱ�־0/1
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code    
             if(CMP_GetIRQFlag(CMP1))//��ȡCMP1�жϱ�־λ
              {
                CMP_ClearIRQFlag(CMP1);//���CMP1�жϱ�־λ
              }
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��21�� <td>1.0        <td>Yangzj         <td>����
 * </table>
 */
u8 CMP_GetIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        if(CMP_IF&BIT0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(CMPx == CMP1)
    {
        if(CMP_IF&BIT1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

/**
 *@brief @b ��������:   void CMP_ClearIRQFlag(u8 CMPx)
 *@brief @b ��������:   ���TIM�жϱ�־
 *@see�����ú�����       ��
 *@param���������       CMP0��CMP1����ȡCMP0��CMP1�жϱ�־λ
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code    
             if(CMP_GetIRQFlag(CMP1))//��ȡCMP1�жϱ�־λ
              {
                CMP_ClearIRQFlag(CMP1);//���CMP1�жϱ�־λ
              }
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��11�� <td>1.0       <td>HuangMG         <td>����
 * </table>
 */
void CMP_ClearIRQFlag(u8 CMPx)
{
	if(CMPx == CMP0)
    {
        CMP_IF=BIT0;
    }
    else if(CMPx == CMP1)
    {
        CMP_IF=BIT1;
    }
}
