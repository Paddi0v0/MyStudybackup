/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_mcpwm.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� MCPWM������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2019��05��21�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */
#include "lks32mc08x_MCPWM.h"
#include "lks32mc08x.h"
#include "lks32mc08x_sys.h"
#include "string.h"

/**
 *@brief @b ��������:   void PWMOutputs(FuncState t_state)
 *@brief @b ��������:   MCPWM�������ʹ����ʧ�ܺ���
 *@see���������ݣ�      ��
 *@param���������      FuncState t_state ENABLE��ʹ�� �� DISABLE:ʹ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
		   PWMOutputs(ENABLE) //ʹ��MCPWM���
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2019��05��21�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */
void PWMOutputs(FuncState t_state)
{
    MCPWM_PRT       = 0x0000DEAD;

    if(t_state == ENABLE)
    {
        MCPWM_FAIL |= MCPWM_MOE_ENABLE_MASK;
    }
    else
    {
        MCPWM_FAIL &= MCPWM_MOE_DISABLE_MASK;
    }
    MCPWM_PRT       = 0x0000CAFE;
}


/**
 *@brief @b ��������:   void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct)
 *@brief @b ��������:   MCPWM�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       MCPWM_InitTypeDef *MCPWM_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
            MCPWM_InitTypeDef MCPWM_InitStructure;
            MCPWM_StructInit(&MCPWM_InitStructure);//��ʼ��MCPWM��ؽṹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2019��05��21�� <td>1.0     <td>Howlet      <td>����
 * </table>
 */
void MCPWM_StructInit(MCPWM_InitTypeDef* MCPWM_InitStruct)
{

    memset(MCPWM_InitStruct, 0, sizeof(MCPWM_InitTypeDef));
}


/**
 *@brief @b ��������:   void MCPWM_Init(MCPWM_InitTypeDef *MCPWM_InitStruct)
 *@brief @b ��������:   MCPWM��ʼ������
 *@see���������ݣ�      SYS_ModuleClockCmd()
 *@param���������      MCPWM_InitTypeDef *MCPWM_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
            MCPWM_InitTypeDef MCPWM_InitStructure;
            MCPWM_StructInit(&MCPWM_InitStructure);//��ʼ��MCPWM��ؽṹ��
            MCPWM_InitStructure.CLK_DIV = PWM_PRSC;                   // MCPWMʱ�ӷ�Ƶ���� 
            MCPWM_InitStructure.MCLK_EN = ENABLE;                     // ģ��ʱ�ӿ��� 
            MCPWM_InitStructure.MCPWM_Cnt_EN = ENABLE;                // ����������ʼ����ʹ�ܿ��� 
            MCPWM_InitStructure.MCPWM_WorkModeCH0 = CENTRAL_PWM_MODE; // MCPWM CH0����ģʽ�����Ķ���PWMģʽ
            MCPWM_InitStructure.MCPWM_WorkModeCH1 = CENTRAL_PWM_MODE; // ͨ������ģʽ���ã����Ķ������ض���
            MCPWM_InitStructure.MCPWM_WorkModeCH2 = CENTRAL_PWM_MODE;
            MCPWM_InitStructure.MCPWM_WorkModeCH3 = CENTRAL_PWM_MODE;
            MCPWM_Init(&MCPWM_InitStructure);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2019��05��21�� <td>1.0       <td>Howlet         <td>����
 * </table>
 */
void MCPWM_Init(MCPWM_InitTypeDef* MCPWM_InitStruct)
{
    SYS_ModuleClockCmd(SYS_Module_MCPWM,ENABLE);//MCPWMʱ��ʹ��
    
    MCPWM_PRT = 0x0000DEAD; /*enter password to unlock write protection */
    MCPWM_TCLK =    MCPWM_InitStruct->CLK_DIV                | (MCPWM_InitStruct->MCLK_EN          << 2)\
               	 | (MCPWM_InitStruct->MCPWM_Cnt_EN    << 3)  | (MCPWM_InitStruct->GPIO_BKIN_Filter << 8)\
                 | (MCPWM_InitStruct->CMP_BKIN_Filter << 12);
	
    MCPWM_TH = MCPWM_InitStruct->MCPWM_PERIOD;

    MCPWM_TMR0 = MCPWM_InitStruct->TriggerPoint0;
    MCPWM_TMR1 = MCPWM_InitStruct->TriggerPoint1;
    MCPWM_TMR2 = MCPWM_InitStruct->TriggerPoint2;
    MCPWM_TMR3 = MCPWM_InitStruct->TriggerPoint3;

    MCPWM_IO01 =    MCPWM_InitStruct->CH0N_Polarity_INV        | (MCPWM_InitStruct->CH0P_Polarity_INV << 1)  \
	               | (MCPWM_InitStruct->CH0_NS            << 2)  | (MCPWM_InitStruct->CH0_PS            << 3)  \
                 | (MCPWM_InitStruct->CH0N_SCTRLN       << 4)  | (MCPWM_InitStruct->CH0P_SCTRLP       << 5)  \
                 | (MCPWM_InitStruct->Switch_CH0N_CH0P  << 6)  | (MCPWM_InitStruct->MCPWM_WorkModeCH0 << 7)  \
                 | (MCPWM_InitStruct->CH1N_Polarity_INV << 8)  | (MCPWM_InitStruct->CH1P_Polarity_INV << 9)  \
	               | (MCPWM_InitStruct->CH1_NS            << 10) | (MCPWM_InitStruct->CH1_PS            << 11) \
	               | (MCPWM_InitStruct->CH1N_SCTRLN       << 12) | (MCPWM_InitStruct->CH1P_SCTRLP       << 13) \
                 | (MCPWM_InitStruct->Switch_CH1N_CH1P  << 14) | (MCPWM_InitStruct->MCPWM_WorkModeCH1 << 15) ;
								 
		MCPWM_IO23 =    MCPWM_InitStruct->CH2N_Polarity_INV        | (MCPWM_InitStruct->CH2P_Polarity_INV << 1)  \
	               | (MCPWM_InitStruct->CH2_NS            << 2)  | (MCPWM_InitStruct->CH2_PS            << 3)  \
                 | (MCPWM_InitStruct->CH2N_SCTRLN       << 4)  | (MCPWM_InitStruct->CH2P_SCTRLP       << 5)  \
                 | (MCPWM_InitStruct->Switch_CH2N_CH2P  << 6)  | (MCPWM_InitStruct->MCPWM_WorkModeCH2 << 7)  \
                 | (MCPWM_InitStruct->CH3N_Polarity_INV << 8)  | (MCPWM_InitStruct->CH3P_Polarity_INV << 9)  \
	               | (MCPWM_InitStruct->CH3_NS            << 10) | (MCPWM_InitStruct->CH3_PS            << 11) \
	               | (MCPWM_InitStruct->CH3N_SCTRLN       << 12) | (MCPWM_InitStruct->CH3P_SCTRLP       << 13) \
                 | (MCPWM_InitStruct->Switch_CH3N_CH3P  << 14) | (MCPWM_InitStruct->MCPWM_WorkModeCH3 << 15) ;
		
		MCPWM_PP   =     MCPWM_InitStruct->IO0_PPE        | ( MCPWM_InitStruct->IO1_PPE << 1 ) \
		             | ( MCPWM_InitStruct->IO2_PPE << 2 ) | ( MCPWM_InitStruct->IO3_PPE << 3 ) ;

    MCPWM_FAIL =     MCPWM_InitStruct->FAIL0_Signal_Sel           |  (MCPWM_InitStruct->FAIL1_Signal_Sel   << 1) \
		              | (MCPWM_InitStruct->FAIL0_Polarity  << 2)      |  (MCPWM_InitStruct->FAIL1_Polarity     << 3) \
                  | (MCPWM_InitStruct->FAIL0_INPUT_EN  << 4)      |  (MCPWM_InitStruct->FAIL1_INPUT_EN     << 5) \
                  
                  |                                                 (MCPWM_InitStruct->DebugMode_PWM_out   << 7) \
                  | (MCPWM_InitStruct->CH0P_default_output << 8)  | (MCPWM_InitStruct->CH0N_default_output << 9) \
                  | (MCPWM_InitStruct->CH1P_default_output << 10) | (MCPWM_InitStruct->CH1N_default_output << 11)\
                  | (MCPWM_InitStruct->CH2P_default_output << 12) | (MCPWM_InitStruct->CH2N_default_output << 13)\
                  | (MCPWM_InitStruct->CH3P_default_output << 14) | (MCPWM_InitStruct->CH3N_default_output << 15);

    MCPWM_SDCFG =    MCPWM_InitStruct->MCPWM_UpdateInterval       | (MCPWM_InitStruct->MCPWM_T0_UpdateEN << 4) \
                  | (MCPWM_InitStruct->MCPWM_T1_UpdateEN   << 5)  | (MCPWM_InitStruct->MCPWM_Auto_ERR_EN << 6) ;

    MCPWM_DTH00 = MCPWM_InitStruct->DeadTimeCH0N;
    MCPWM_DTH01 = MCPWM_InitStruct->DeadTimeCH0P;
    MCPWM_DTH10 = MCPWM_InitStruct->DeadTimeCH1N;
    MCPWM_DTH11 = MCPWM_InitStruct->DeadTimeCH1P;
    MCPWM_DTH20 = MCPWM_InitStruct->DeadTimeCH2N;
    MCPWM_DTH21 = MCPWM_InitStruct->DeadTimeCH2P;
    MCPWM_DTH30 = MCPWM_InitStruct->DeadTimeCH3N;
    MCPWM_DTH31 = MCPWM_InitStruct->DeadTimeCH3P;

    MCPWM_EIF   = 0xffff;
    MCPWM_IE    = MCPWM_InitStruct->T0_Update_INT_EN         | (MCPWM_InitStruct->T1_Update_INT_EN  << 1)  \
		           | (MCPWM_InitStruct->TH00_Match_INT_EN << 2)  | (MCPWM_InitStruct->TH01_Match_INT_EN << 3)  \
							 | (MCPWM_InitStruct->TH10_Match_INT_EN << 4)  | (MCPWM_InitStruct->TH11_Match_INT_EN << 5)  \
							 | (MCPWM_InitStruct->TH20_Match_INT_EN << 6)  | (MCPWM_InitStruct->TH21_Match_INT_EN << 7)  \
							 | (MCPWM_InitStruct->TH30_Match_INT_EN << 8)  | (MCPWM_InitStruct->TH31_Match_INT_EN << 9)  \
							 | (MCPWM_InitStruct->TMR0_Match_INT_EN << 10) | (MCPWM_InitStruct->TMR1_Match_INT_EN << 11) \
							 | (MCPWM_InitStruct->TMR2_Match_INT_EN << 12) | (MCPWM_InitStruct->TMR3_Match_INT_EN << 13) \
							 | (MCPWM_InitStruct->MCPWM_REG_UPDATE_INT_EN << 14);

    MCPWM_EIE = (MCPWM_InitStruct->FAIL0_INT_EN << 4) | (MCPWM_InitStruct->FAIL1_INT_EN << 5);

    MCPWM_UPDATE = 0xffff;    /* write whatever value to trigger update */

    MCPWM_UPDATE = 0x00;

    MCPWM_PRT = 0x0000CAFE;  /* write any value other than 0xDEAD to enable write protection */
}

/**
 *@brief @b ��������:   u16 MCPWM_GetIRQFlag(uint32_t INT_flag)
 *@brief @b ��������:   ��ȡMCPWM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������
 * <table>             <tr><th> MCPWM_T0_IRQ_IF       <td>BIT0  <td> ����������ֵ�ص�MCPWM_TH�жϱ�־λ 
 *                     <tr><th> MCPWM_T1_IRQ_IF       <td>BIT1  <td> ����������ֵ�ص�0�жϱ�־λ 
 *                     <tr><th> MCPWM_TH00_IRQ_IF     <td>BIT2  <td> ����������ֵ�ص�MCPWM_TH00�жϱ�־λ 
 *                     <tr><th> MCPWM_TH01_IRQ_IF     <td>BIT3  <td> ����������ֵ�ص�MCPWM_TH01�жϱ�־λ 
 *                     <tr><th> MCPWM_TH10_IRQ_IF     <td>BIT4  <td> ����������ֵ�ص�MCPWM_TH10�жϱ�־λ 
 *                     <tr><th> MCPWM_TH11_IRQ_IF     <td>BIT5  <td> ����������ֵ�ص�MCPWM_TH11�жϱ�־λ 
 *                     <tr><th> MCPWM_TH20_IRQ_IF     <td>BIT6  <td> ����������ֵ�ص�MCPWM_TH20�жϱ�־λ 
 *                     <tr><th> MCPWM_TH21_IRQ_IF     <td>BIT7  <td> ����������ֵ�ص�MCPWM_TH21�жϱ�־λ 
 *                     <tr><th> MCPWM_TH30_IRQ_IF     <td>BIT8  <td> ����������ֵ�ص�MCPWM_TH30�жϱ�־λ 
 *                     <tr><th> MCPWM_TH31_IRQ_IF     <td>BIT9  <td> ����������ֵ�ص�MCPWM_TH31�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR0_IRQ_IF     <td>BIT10 <td> ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR1_IRQ_IF     <td>BIT11 <td> ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR2_IRQ_IF     <td>BIT12 <td> ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR3_IRQ_IF     <td>BIT13 <td> ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ 
 *                     <tr><th> MCPWM_SHADE_IRQ_IF    <td>BIT14 <td> MCPWM_TH/MCPWM_TH00-MCPWM_TH31/MCPWM_TR0-MCPWM_TR3�ȼĴ������µ�MCPWMʵ������ϵͳ�жϱ�־λ
 * </table>
 *@param���������   ��
 *@return�� �� ֵ��  ����Ӧ��־λ�Ƿ�Ϊ1��1λ��λ��0Ϊδ��λ
 *@note����˵����    ��
 *@warning           ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
		   if(MCPWM_GetIRQFlag(MCPWM_T0_IRQ_EN))//��ȡMCPWM�ļ���������ֵ�ص�MCPWM_TH�жϱ�־
		   {
		   }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
u16 MCPWM_GetIRQFlag(uint32_t INT_flag)
{
	  u8 MCPWMIRQFlag = 0;
	 if(INT_flag <= BIT14)
	 {
	     if(MCPWM_IF & INT_flag & MCPWM_IE)
				{
						MCPWMIRQFlag = 1;
				}
				else
				{
						MCPWMIRQFlag = 0;
				}
	 }
	 else
	 {
	     if(MCPWM_EIF & (INT_flag >> 15) & MCPWM_EIE)
				{
						MCPWMIRQFlag = 1;
				}
				else
				{
						MCPWMIRQFlag = 0;
				}
	 }
    return MCPWMIRQFlag;
}


/**
 *@brief @b ��������:   void MCPWM_ClearIRQFlag(uint32_t INT_flag)
 *@brief @b ��������:   ���MCPWM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������
 * <table>             <tr><th> MCPWM_T0_IRQ_IF       <td>BIT0  <td> ����������ֵ�ص�MCPWM_TH�жϱ�־λ 
 *                     <tr><th> MCPWM_T1_IRQ_IF       <td>BIT1  <td> ����������ֵ�ص�0�жϱ�־λ 
 *                     <tr><th> MCPWM_TH00_IRQ_IF     <td>BIT2  <td> ����������ֵ�ص�MCPWM_TH00�жϱ�־λ 
 *                     <tr><th> MCPWM_TH01_IRQ_IF     <td>BIT3  <td> ����������ֵ�ص�MCPWM_TH01�жϱ�־λ 
 *                     <tr><th> MCPWM_TH10_IRQ_IF     <td>BIT4  <td> ����������ֵ�ص�MCPWM_TH10�жϱ�־λ 
 *                     <tr><th> MCPWM_TH11_IRQ_IF     <td>BIT5  <td> ����������ֵ�ص�MCPWM_TH11�жϱ�־λ 
 *                     <tr><th> MCPWM_TH20_IRQ_IF     <td>BIT6  <td> ����������ֵ�ص�MCPWM_TH20�жϱ�־λ 
 *                     <tr><th> MCPWM_TH21_IRQ_IF     <td>BIT7  <td> ����������ֵ�ص�MCPWM_TH21�жϱ�־λ 
 *                     <tr><th> MCPWM_TH30_IRQ_IF     <td>BIT8  <td> ����������ֵ�ص�MCPWM_TH30�жϱ�־λ 
 *                     <tr><th> MCPWM_TH31_IRQ_IF     <td>BIT9  <td> ����������ֵ�ص�MCPWM_TH31�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR0_IRQ_IF     <td>BIT10 <td> ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR1_IRQ_IF     <td>BIT11 <td> ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR2_IRQ_IF     <td>BIT12 <td> ����������ֵ�ص�MCPWM_TMR0�жϱ�־λ 
 *                     <tr><th> MCPWM_TMR3_IRQ_IF     <td>BIT13 <td> ����������ֵ�ص�MCPWM_TMR1�жϱ�־λ 
 *                     <tr><th> MCPWM_SHADE_IRQ_IF    <td>BIT14 <td> MCPWM_TH/MCPWM_TH00-MCPWM_TH31/MCPWM_TR0-MCPWM_TR3�ȼĴ������µ�MCPWMʵ������ϵͳ�жϱ�־λ
 * </table>
 *@param���������   ��
 *@return�� �� ֵ��  ��
 *@note����˵����    ��
 *@warning          ��
 *@par ʾ�����룺
 *@code
		   if(MCPWM_GetIRQFlag(MCPWM_T0_IRQ_EN))//��ȡMCPWM�ļ���������ֵ�ص�MCPWM_TH�жϱ�־
		   {
            MCPWM_ClearIRQFlag(MCPWM_T0_IRQ_EN);//���MCPWM�ļ���������ֵ�ص�MCPWM_TH�жϱ�־
		   }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��04��20�� <td>1.0     <td>Zhu Jie      <td>����
 * </table>
 */
void MCPWM_ClearIRQFlag(uint32_t INT_flag)
{
	 if(INT_flag <= BIT14)
	 {
	     MCPWM_IF = INT_flag;

	 }
	 else
	 {
	     MCPWM_EIF = (INT_flag >> 15);
	 }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
