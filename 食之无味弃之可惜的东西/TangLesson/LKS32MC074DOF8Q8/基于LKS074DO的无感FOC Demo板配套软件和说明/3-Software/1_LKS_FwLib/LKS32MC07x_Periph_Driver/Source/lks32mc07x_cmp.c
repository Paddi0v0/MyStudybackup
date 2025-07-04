/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_cmp.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� cmp������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */
#include "lks32mc07x_cmp.h"
#include "lks32mc07x.h"
#include "string.h"
#include "lks32mc07x_sys.h"

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
	CMP_InitTypeDef CMP_InitStre;
	CMP_StructInit(&CMP_InitStre);

	CMP_InitStre.CLK_COM_DIV = 0;									// �Ƚ��������˲�ʱ�ӷ�Ƶ
	CMP_InitStre.FT = DISABLE;										// �Ƚ������ٱȽ�
	CMP_InitStre.HYS = CMP_HYS_20mV;							// �Ƚ����ͻص�ѹ

	CMP_InitStre.CMP0.SELP = CMP0_SELP_IP1; 		  // �Ƚ���0�����ź�ѡ��
	CMP_InitStre.CMP0.SELN = CMP_SELN_REF; 				// �Ƚ���0�����ź�ѡ��
	CMP_InitStre.CMP0.RE = DISABLE;								// �Ƚ���0DMAʧ��
	CMP_InitStre.CMP0.POL= CMP_HIGH_LEVEL;				// �Ƚ���0�ߵ�ƽ��Ч
	CMP_InitStre.CMP0.IRQ_TRIG = IRQ_LEVEL_TRIG_MODE; // �Ƚ���0��ƽ�����ж�ģʽ
	CMP_InitStre.CMP0.IN_EN = ENABLE; 			 			// �Ƚ���0�ź�����ʹ��
	CMP_InitStre.CMP0.IE = ENABLE;  							// �Ƚ���0�ź��ж�ʹ��
	CMP_InitStre.CMP0.FIL_CLK_DIV16 = 2; 					// ���˲����=tclk ����*16*CMP_FltCnt (CMP_FltCnt��Ƶϵ��,0~15)
	CMP_InitStre.CMP0.FIL_CLK_DIV2 = 2;  					// �Ƚ��� 2/1/0 �˲�ʱ��ʹ��
	CMP_InitStre.CMP0.CLK_EN = ENABLE;						//�Ƚ���ʱ��ʹ��
	CMP_InitStre.CMP0.EN = ENABLE;       				  // �Ƚ���0���� ����SYS_AFE_REG5

	CMP_Init(&CMP_InitStre);// �Ƚ�����ʼ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang       <td>����
 * </table>
 */
void CMP_Init(CMP_InitTypeDef *this)
{
    SYS_WR_PROTECT = 0x7A83;

    SYS_AFE_REG1 |= this->FT; /* ���ٱȽϣ�Enable�� 30nS */

    SYS_AFE_REG2 |= (this->CMP2.SELP << 14);

    SYS_AFE_REG3 |= (this->CMP1.SELP << 12) | (this->CMP1.SELN << 4) | (this->CMP0.SELP << 8) | (this->CMP0.SELN << 2) |
                    (this->HYS << 7) | (this->CMP2.SELN);

    SYS_AFE_REG5 |= (this->CMP2.EN << 11) | (this->CMP1.EN << 7) | (this->CMP0.EN << 6);

    SYS_WR_PROTECT = 0x0000;

    CMP->IE    = (this ->CMP2.RE << 10) | (this ->CMP1.RE << 9) | (this ->CMP0.RE << 8) |
                 (this ->CMP2.IE << 2) | (this -> CMP1.IE << 1) | (this -> CMP0.IE); //�ж�

    CMP->TCLK  = (this ->CLK_COM_DIV << 24) |
                 (this ->CMP2.FIL_CLK_DIV16 << 20) | (this -> CMP2.FIL_CLK_DIV2 << 16) |
                 (this ->CMP1.FIL_CLK_DIV16 << 12) | (this -> CMP1.FIL_CLK_DIV2  << 8) |
                 (this ->CMP0.FIL_CLK_DIV16 << 4) | (this -> CMP0.FIL_CLK_DIV2) |
                 (this -> CMP2.CLK_EN << 19) | (this -> CMP1.CLK_EN << 11) | (this -> CMP0.CLK_EN << 3);

    CMP -> CFG =  																	 (this -> CMP2.IRQ_TRIG << 10) |
            (this -> CMP2.IN_EN     <<  9) | (this -> CMP2.POL 		 <<  8)  |
            (this ->CMP1.W_PWM_POL <<  7) | (this -> CMP1.IRQ_TRIG <<  6) |
            (this -> CMP1.IN_EN     <<  5) | (this -> CMP1.POL      <<  4) |
            (this -> CMP0.W_PWM_POL <<  3) | (this -> CMP0.IRQ_TRIG <<  2) |
            (this -> CMP0.IN_EN     <<  1) | (this -> CMP0.POL);

    CMP -> BLCWIN  = (this -> CMP1.CHN3P_PWM_EN <<  7) | (this -> CMP1.CHN2P_PWM_EN <<  6) |
                     (this -> CMP1.CHN1P_PWM_EN <<  5) | (this -> CMP1.CHN0P_PWM_EN <<  4) |
                     (this -> CMP0.CHN3P_PWM_EN <<  3) | (this -> CMP0.CHN2P_PWM_EN <<  2) |
                     (this -> CMP0.CHN1P_PWM_EN <<  1) | (this -> CMP0.CHN0P_PWM_EN);

    CMP -> IF = 3;//�����־λ
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
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang       <td>����
 * </table>
 */
void CMP_StructInit(CMP_InitTypeDef* CMP_InitStruct)
{

    memset(CMP_InitStruct, 0, sizeof(CMP_InitTypeDef));
}

/**
 *@brief @b ��������:  void CMP_Cmd(u8 CMP_CHN_x,FuncState state)
 *@brief @b ��������:   CMPʱ��ʹ��
 *@see�����ú�����       ��
 *@param���������       CMP_CHN_x��CMP_CHN_0/CMP_CHN_1/CMP_CHN_2,state��ENABLE/DISABLE
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
            CMP_Cmd(CMP_CHN_0,ENABLE);//CMP0ʹ��
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>Olive Wang       <td>����
 * </table>
 */
void CMP_Cmd(u8 CMP_CHN_x, FuncState state)
{
    if(CMP_CHN_0 == CMP_CHN_x)
    {
        SYS_AnalogModuleClockCmd( SYS_AnalogModule_CMP0, state);   //CMP0ʹ��
    }
    else if(CMP_CHN_1 == CMP_CHN_x)
    {
        SYS_AnalogModuleClockCmd( SYS_AnalogModule_CMP1, state);    //CMP1ʹ��
    }
    else if(CMP_CHN_2 == CMP_CHN_x)
    {
        SYS_AnalogModuleClockCmd( SYS_AnalogModule_CMP2, state); //CMP2ʹ��
    }
		
		

}

/**
 *@brief @b ��������:   u8 CMP_GetIRQFlag(u8 CMP_CHN_x)
 *@brief @b ��������:   ��ȡ�Ƚ����жϱ�־
 *@see�����ú�����       ��
 *@param���������       CMP_CHN_0��CMP_CHN_1����ȡCMP0��CMP1�жϱ�־λ
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
 * <tr><td>2023��02��22�� <td>1.0        <td>Olive Wang         <td>����
 * </table>
 */
u8 CMP_GetIRQFlag(u8 CMP_CHN_x)
{
    if(CMP->IF & (BIT0 << CMP_CHN_x))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 *@brief @b ��������:   void CMP_ClearIRQFlag(u8 CMP_CHN_x)
 *@brief @b ��������:   ���TIM�жϱ�־
 *@see�����ú�����       ��
 *@param���������       CMP_CHN_0��CMP_CHN_1����ȡCMP0��CMP1�жϱ�־λ
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
             if(CMP_GetIRQFlag(CMP_CHN_1))//��ȡCMP1�жϱ�־λ
              {
                CMP_ClearIRQFlag(CMP_CHN_1);//���CMP1�жϱ�־λ
              }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��22�� <td>1.0       <td>Olive Wang         <td>����
 * </table>
 */

void CMP_ClearIRQFlag(u8 CMP_CHN_x)
{
    CMP->IF = (BIT0 << CMP_CHN_x);
}

/**
 *@brief @b ��������:   uint8_t CMP_GetFilterValue(void)
 *@brief @b ��������:   ��ȡ�˲����CMPֵ
 *@see�����ú�����       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
             u8 cmpVal;
							cmpVal = CMP_GetFilterValue();
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��22�� <td>1.0       <td>Olive Wang         <td>����
 * </table>
 */
uint8_t CMP_GetFilterValue(void)
{
    return ((CMP_DATA >> 8) & 0x0007);
}
/**
 *@brief @b ��������:   uint8_t CMP_GetValue(void)
 *@brief @b ��������:   ��ȡδ�˲���CMPֵ
 *@see�����ú�����       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
             u8 cmpVal;
							cmpVal = CMP_GetValue();
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��22�� <td>1.0       <td>Olive Wang         <td>����
 * </table>
 */
uint8_t CMP_GetValue(void)
{
    return (CMP_DATA & 0x0007);
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */
