/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_tim.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ��ʱ�������������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��06��06�� <td>1.0     <td>cfwu        <td>����
 * </table>
 */

#include "lks32mc08x_tim.h"


/**
 *@brief @b ��������:   void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct)
 *@brief @b ��������:   ��ʱ����ʼ������
 *@see���������ݣ�      SYS_ModuleClockCmd()
 *@param���������      TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 \n
                        TIM_TimerInitTypeDef  *TIM_TimerInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			          TIM_TimerInitTypeDef TIM_InitStruct;
                TIM_TimerStrutInit(&TIM_InitStruct);                   // Timer�ṹ���ʼ��
								TIM_InitStruct.EN = ENABLE;                            // ��ʱ��ģ��ʹ��
                TIM_InitStruct.Timer_ClockDiv = TIM_Clk_Div1;          // ����Timerģ��ʱ��1��Ƶϵ�� 
                TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP;  // ����Timer CH0 Ϊ�Ƚ�ģʽ
                TIM_InitStruct.Timer_CH0Output = 0;                    // ����������ʱ���Ƚ�ģʽ������Կ��� 
                TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP;  // ����Timer CH1 Ϊ�Ƚ�ģʽ 
                TIM_InitStruct.Timer_CH1Output = 0;                    // ����������ʱ���Ƚ�ģʽ������Կ��� 
                TIM_InitStruct.Timer_TH = 48000;                       // ��ʱ���������޳�ʼֵ500us
                TIM_InitStruct.Timer_CMP0 = 36000;                     // ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ25%ת�ձ� 
                TIM_InitStruct.Timer_CMP1 = 24000;                     // ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ24000
                TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0;        // ����Timerģ��Ƚ��ж�
                TIM_TimerInit(TIMER0, &TIM_InitStruct);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��06�� <td>1.0       <td>cfwu          <td>����
 * </table>
 *@par �޸���־: ȥ����ʱ��ʹ�ܽӿ�
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��04��13�� <td>1.0      <td>HuangMG       <td>�޸�
 * </table>
 */
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *TIM_TimerInitStruct)
{
    uint32_t tempIE;
    uint32_t tempVal;
    uint32_t tempFilter;
    uint32_t filter0;
    uint32_t filter1;

    SYS_ModuleClockCmd(SYS_Module_TIMER,ENABLE);     //��Timerʱ��

   TIM_TimerCmd(TIMERx,TIM_TimerInitStruct->EN);

    TIMERx->CFG = TIM_TimerInitStruct->Timer_CH0_CapMode | (TIM_TimerInitStruct->Timer_CH0_WorkMode << 2)
                  | (TIM_TimerInitStruct->Timer_CH0Output << 3) | (TIM_TimerInitStruct->Timer_CH1_CapMode << 4)
                  | (TIM_TimerInitStruct->Timer_CH1_WorkMode << 6) | (TIM_TimerInitStruct->Timer_CH1Output << 7)
                  | (TIM_TimerInitStruct->Timer_ClockDiv << 8);
    TIMERx->TH = TIM_TimerInitStruct->Timer_TH;
    TIMERx->CMPT0 = TIM_TimerInitStruct->Timer_CMP0;
    TIMERx->CMPT1 = TIM_TimerInitStruct->Timer_CMP1;

    /*ʹ�ܶ�ʱ���ж�*/
    /*�˲�����*/
    tempIE = UTIMER_IE;
    filter0 = TIM_TimerInitStruct->Timer_Filter0 & 0x000F;    /*�˲�����ֻʹ�õ���λ*/
    filter1 = TIM_TimerInitStruct->Timer_Filter1 & 0x000F;
    if (TIMERx == TIMER0)
    {
        tempVal = 0x0007;
        tempIE &= ~tempVal;
        tempIE |= (TIM_TimerInitStruct->Timer_IRQEna);

        tempFilter = UTIMER_FLT_TH01;
        tempFilter &= 0xFF00;
        tempFilter |= filter0;
        tempFilter |= (filter1 << 4);
        UTIMER_FLT_TH01 = tempFilter;
    }
    else if (TIMERx == TIMER1)
    {
        tempVal = 0x0038;
        tempIE &= ~tempVal;
        tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 3);

        tempFilter = UTIMER_FLT_TH01;
        tempFilter &= 0x00FF;
        tempFilter |= (filter0 << 8);
        tempFilter |= (filter1 << 12);
        UTIMER_FLT_TH01 = tempFilter;
    }
    else if (TIMERx == TIMER2)
    {
        tempVal = 0x01C0;
        tempIE &= ~tempVal;
        tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 6);

        tempFilter = UTIMER_FLT_TH23;
        tempFilter &= 0xFF00;
        tempFilter |= filter0;
        tempFilter |= (filter1 << 4);
        UTIMER_FLT_TH23 = tempFilter;
    }
    else if (TIMERx == TIMER3)
    {
        tempVal = 0x0E00;
        tempIE &= ~tempVal;
        tempIE |= (TIM_TimerInitStruct->Timer_IRQEna << 9);

        tempFilter = UTIMER_FLT_TH23;
        tempFilter &= 0x00FF;
        tempFilter |= (filter0 << 8);
        tempFilter |= (filter1 << 12);
        UTIMER_FLT_TH23 = tempFilter;
    }
    UTIMER_IE = tempIE;
}

/**
 *@brief @b ��������:   void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct)
 *@brief @b ��������:   ��ʱ���ṹ���ʼ��
 *@see���������ݣ�      ��
 *@param���������      TIM_TimerInitTypeDef  *TIM_TimerInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			    TIM_TimerInitTypeDef TIM_InitStruct;
                TIM_TimerStrutInit(&TIM_InitStruct);  // Timer�ṹ���ʼ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��06�� <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *TIM_TimerInitStruct)
{
    TIM_TimerInitStruct->Timer_CH0_CapMode = TIMER_CapMode_None;
    TIM_TimerInitStruct->Timer_CH0_WorkMode = TIMER_OPMode_CMP;
    TIM_TimerInitStruct->Timer_CH0Output = 0;

    TIM_TimerInitStruct->Timer_CH1_CapMode = TIMER_CapMode_None;
    TIM_TimerInitStruct->Timer_CH1_WorkMode = TIMER_OPMode_CMP;
    TIM_TimerInitStruct->Timer_CH1Output = 0;

    TIM_TimerInitStruct->Timer_TH = 1000;
    TIM_TimerInitStruct->Timer_CMP0 = 500;
    TIM_TimerInitStruct->Timer_CMP1 = 500;
    TIM_TimerInitStruct->Timer_Filter0 = 0;
    TIM_TimerInitStruct->Timer_Filter1 = 0;
    TIM_TimerInitStruct->Timer_ClockDiv = TIM_Clk_Div1;
    TIM_TimerInitStruct->Timer_IRQEna = Timer_IRQEna_None;
    TIM_TimerInitStruct->EN = DISABLE;
}

/**
 *@brief @b ��������:  void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
 *@brief @b ��������:   ��ʱ��TIMERxʹ�ܺ�ֹͣ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 \n
                        FuncState state��ʹ����ʧ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			    TIM_TimerCmd(TIMER0, ENABLE); // Timer0ģ��ʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
{
    if (state != DISABLE)
    {
        if (TIMERx == TIMER0)
        {
            UTIMER_CFG |= BIT4;
        }
        else if (TIMERx == TIMER1)
        {
            UTIMER_CFG |= BIT5;
        }
        else if (TIMERx == TIMER2)
        {
            UTIMER_CFG |= BIT6;
        }
        else if (TIMERx == TIMER3)
        {
            UTIMER_CFG |= BIT7;
        }

    }
    else
    {
        if (TIMERx == TIMER0)
        {
            UTIMER_CFG &= ~BIT4;
        }
        else if (TIMERx == TIMER1)
        {
            UTIMER_CFG &= ~BIT5;
        }
        else if (TIMERx == TIMER2)
        {
            UTIMER_CFG &= ~BIT6;
        }
        else if (TIMERx == TIMER3)
        {
            UTIMER_CFG &= ~BIT7;
        }
    }
}


/**
 *@brief @b ��������:   uint32_t TIM_GetIRQFlag(TIM_TimerTypeDef *TIMERx , u32 timer_if)
 *@brief @b ��������:   ��ȡTIM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������      TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 \n
                        timer_if��
 * <table>              <tr><th>TIMER_IF_CH1   <td>Timer CH1�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_CH0   <td>Timer CH0�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_ZERO  <td>Timer �����ж�
 * </table> 
 *@param���������      ��
 *@return�� �� ֵ��     0��1����Ӧ�жϱ�־��λ����1��δ��λ����0
 *@note����˵����       ��
 *@warning             ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
			    if(TIM_GetIRQFlag(TIMER0,TIMER_IF_CH0)) //�ж�UTimer0��CH0�Ƿ����Ƚ��ж�
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
uint32_t TIM_GetIRQFlag(TIM_TimerTypeDef *TIMERx , u32 timer_if)
{
	if(TIMERx == TIMER0)
    {
			if((UTIMER_IF & timer_if & UTIMER_IE) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		
		if(TIMERx == TIMER1)
    {
			if(((UTIMER_IF>>3) & timer_if & (UTIMER_IE>>3)) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		
	  if(TIMERx == TIMER2)
    {
			if(((UTIMER_IF>>6) & timer_if & (UTIMER_IE>>6)) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		
    if(TIMERx == TIMER3)
    {
			if(((UTIMER_IF>>9) & timer_if & (UTIMER_IE>>9)) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		return 0;
}

/**
 *@brief @b ��������:   void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag)
 *@brief @b ��������:   ���TIM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������      TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 \n
                        timer_if��
 * <table>              <tr><th>TIMER_IF_CH1   <td>Timer CH1�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_CH0   <td>Timer CH0�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_ZERO  <td>Timer �����ж�
 * </table> 
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			    if(TIM_GetIRQFlag(TIMER0,TIMER_IF_CH0)) //�ж�UTimer0��CH0�Ƿ����Ƚ��ж�
                {
                    TIM_ClearIRQFlag(TIMER0,TIMER_IF_CH0); //���UTimer0ͨ��0�Ƚ��жϱ�־λ
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag)
{
	  if(TIMERx == TIMER0)
		{
			UTIMER_IF = tempFlag;
		}
		if(TIMERx == TIMER1)
		{
			UTIMER_IF = tempFlag<<3;
		}
	  if(TIMERx == TIMER2)
		{
			UTIMER_IF = tempFlag<<6;
		}
	  if(TIMERx == TIMER3)
		{
			UTIMER_IF = tempFlag<<9;
		}
}

/**
 *@brief @b ��������:   uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx)
 *@brief @b ��������:   ��ȡTimer����ֵ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_Value = 0;
                    TIMER0_Value = TIM_Timer_GetCount(TIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2016��06��06�� <td>1.0      <td>cfwu          <td>�޸�
 * </table>
 */
uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx)
{
    return TIMERx->CNT;
}

/**
 *@brief @b ��������:   uint32_t TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIM_TIMERx)
 *@brief @b ��������:   ��ȡ��ʱ��ͨ��0����ֵ
 *@see���������ݣ�       ��
 *@param���������      TIM_TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 
 *@param���������      ��
 *@return�� �� ֵ��     ��ʱ��ͨ��0����ֵ
 *@note����˵����       ����ʱ�����������¼�ʱ��������ʱ�̵�cntֵ�洢���üĴ�����
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_CAPValue = 0;
                    TIMER0_CAPValue = TIM_Timer_GetCMPT0(TIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2016��06��06�� <td>1.0      <td>cfwu          <td>�޸�
 * </table>
 */
uint32_t TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIM_TIMERx)
{
    return TIM_TIMERx->CMPT0;
}

/**
 *@brief @b ��������:   uint32_t TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIM_TIMERx)
 *@brief @b ��������:   ��ȡ��ʱ��ͨ��1����ֵ
 *@see���������ݣ�      ��
 *@param���������      TIM_TIMERx��TIMER0/TIMER1/TIMER2/TIMER3 
 *@param���������      ��
 *@return�� �� ֵ��     ��ʱ��ͨ��1����ֵ
 *@note����˵����       ����ʱ�����������¼�ʱ��������ʱ�̵�cntֵ�洢���üĴ�����
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_CAPValue = 0;
                    TIMER0_CAPValue = TIM_Timer_GetCMPT1(TIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2016��06��06�� <td>1.0      <td>cfwu          <td>�޸�
 * </table>
 */
uint32_t TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIM_TIMERx)
{
    return TIM_TIMERx->CMPT1;
}

/**
 *@brief @b ��������:   void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct)
 *@brief @b ��������:   ��������ʼ������
 *@see���������ݣ�      SYS_ModuleClockCmd();
 *@param���������      ECDx��ECD0/ECD1 \n
                       TIM_ECDInitTypeDef *TIM_ECDInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_ECDInitTypeDef TIM_ECDInitStruct;
                    TIM_ECDStructInit(&TIM_ECDInitStruct);
                    TIM_ECDInitStruct.ECD_ClockDiv = ECD_Clk_Div1;      //96M
                    TIM_ECDInitStruct.ECD_Filter0 = 10;                 //CH0�˲����˲����ΪTimer_Filter0*8
                                                                            ��ʱ������ ECD_Filter00~15
                    TIM_ECDInitStruct.ECD_Filter1 = 10;                 //CH1�˲�
                    TIM_ECDInitStruct.ECD_Mode = ECD_Mode_CCWSIGN;      //CCW+SIGN, �������ź�������
                    TIM_ECDInitStruct.ECD_TH = 4096;                    //��������
                    TIM_ECDInitStruct.ECD_IRQEna = ECD_IRQEna_OverFlow; //ʹ�������ж�
                    TIM_ECDInit(ECD0, &TIM_ECDInitStruct);
                    TIM_ECDCmd(ECD0,ENABLE);                            //ʹ��ECD0ģ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2016��06��06�� <td>1.0      <td>cfwu          <td>�޸�
 * </table>
 *@par �޸���־:���Ӷ�ʱ��ʱ��ʹ�ܺ���
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0      <td>HuangMG       <td>�޸�
 * </table>
 */
void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct)
{
    uint32_t tempIE;
    uint32_t tempFilter;
    uint32_t filter0;
    uint32_t filter1;
    uint32_t tempTimerCfg;

	  SYS_ModuleClockCmd(SYS_Module_TIMER,ENABLE);    /*��Timerʱ��*/
	
    ECDx->CFG = TIM_ECDInitStruct->ECD_Mode << 8;
    ECDx->TH = TIM_ECDInitStruct->ECD_TH;

    filter0 = TIM_ECDInitStruct->ECD_Filter0 & 0x000F;	/*�˲�����ֻʹ�õ���λ*/
    filter1 = TIM_ECDInitStruct->ECD_Filter1 & 0x000F;
    tempIE = UTIMER_IE;
    if (ECDx == ECD0)
    {
        tempFilter = UTIMER_FLT_TH23;
        tempFilter &= 0xFF00;
        tempFilter |= filter0;
        tempFilter |= (filter1 << 4);
        UTIMER_FLT_TH23 = tempFilter;

        tempIE |= (TIM_ECDInitStruct->ECD_IRQEna << 12);

        tempTimerCfg = TIMER2->CFG;
        tempTimerCfg &= ~(0x0300);
        tempTimerCfg |= (TIM_ECDInitStruct->ECD_ClockDiv << 8);
        TIMER2->CFG = tempTimerCfg;
        UTIMER_CFG |= BIT8;
    }
    else if (ECDx == ECD1)
    {
        tempFilter = UTIMER_FLT_TH23;
        tempFilter &= 0x00FF;
        tempFilter |= (filter0 << 8);
        tempFilter |= (filter1 << 12);
        UTIMER_FLT_TH23 = tempFilter;

        tempIE |= (TIM_ECDInitStruct->ECD_IRQEna << 14);

        tempTimerCfg = TIMER3->CFG;
        tempTimerCfg &= ~(0x0300);
        tempTimerCfg |= (TIM_ECDInitStruct->ECD_ClockDiv << 8);
        TIMER3->CFG = tempTimerCfg;
        UTIMER_CFG |= BIT9;
    }
    UTIMER_IE = tempIE;
}

/**
 *@brief @b ��������:   void TIM_ECDInit(TIM_ECDTypeDef *ECDx, TIM_ECDInitTypeDef *TIM_ECDInitStruct)
 *@brief @b ��������:   �������ṹ���ʼ��  
 *@see���������ݣ�      ��
 *@param���������      TIM_ECDInitTypeDef *TIM_ECDInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_ECDInitTypeDef TIM_ECDInitStruct;
                    TIM_ECDStructInit(&TIM_ECDInitStruct);//�������ṹ���ʼ��                               
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��06�� <td>1.0      <td>cfwu          <td>�޸�
 * </table>
 */
void TIM_ECDStructInit(TIM_ECDInitTypeDef *TIM_ECDInitStruct)
{
    TIM_ECDInitStruct->ECD_Mode = ECD_Mode_T1;
    TIM_ECDInitStruct->ECD_TH = 1000;
    TIM_ECDInitStruct->ECD_ClockDiv = TIM_Clk_Div1;
    TIM_ECDInitStruct->ECD_Filter0 = 0;
    TIM_ECDInitStruct->ECD_Filter1 = 0;
    TIM_ECDInitStruct->ECD_IRQEna = ECD_IRQEna_UnderFlow | ECD_IRQEna_OverFlow;
}

/**
 *@brief @b ��������:   void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state)
 *@brief @b ��������:   ������ECDxʹ�ܺ�ֹͣ
 *@see���������ݣ�      ��
 *@param���������      ECDx��ECD0/ECD1 \n
                       FuncState state��ʹ����ʧ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			    TIM_ECDCmd(ECD0, ENABLE); // ECD0ģ��ʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_ECDCmd(TIM_ECDTypeDef *ECDx, FuncState state)
{
    uint32_t tempEna;
    tempEna = UTIMER_CFG;

    if (state != DISABLE)
    {
        if (ECDx == ECD0)
        {
            tempEna |= BIT8;
        }
        else if (ECDx == ECD1)
        {
            tempEna |= BIT9;
        }
    }
    else
    {
        if (ECDx == ECD0)
        {
            tempEna &= ~BIT8;
        }
        else if (ECDx == ECD1)
        {
            tempEna &= ~BIT9;
        }
    }
    UTIMER_CFG = tempEna;
}

/**
 *@brief @b ��������:   uint32_t TIM_ECD_GetCount(TIM_ECDTypeDef *TIM_ECDx)
 *@brief @b ��������:   ����������ֵ
 *@see���������ݣ�       ��
 *@param���������       ECDx��ECD0/ECD1 \n
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			    uint32_t ECD0_Value = 0;
                ECD0_Value = TIM_ECD_GetCount(ECD0); //��ȡ������0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��06��06�� <td>1.0        <td>cfwu          <td>�޸�
 * </table>
 */
uint32_t TIM_ECD_GetCount(TIM_ECDTypeDef *TIM_ECDx)
{
    return TIM_ECDx->CNT;
}


/**
 *@brief @b ��������:   uint32_t ECD_GetIRQFlag(TIM_ECDTypeDef *ECDx , u32 timer_if)
 *@brief @b ��������:   ��ȡECD�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       ECDx��ECD0/ECD1 \n
                        timer_if��
 * <table>              <tr><th>ECD_IF_UNDERFLOW   <td>Encoder�����ж�
                        <tr><th>ECD_IF_OVERFLOW    <td>Encoder�����ж�                 
 * </table> 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
			    if(ECD_GetIRQFlag(ECD0,ECD_IF_OVERFLOW)) //��ȡ�����жϱ�־
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
uint32_t ECD_GetIRQFlag(TIM_ECDTypeDef *ECDx , u32 timer_if)
{
	if(ECDx == ECD0)
    {
			if(((UTIMER_IF>>12) & timer_if & (UTIMER_IE>>12)) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		
		if(ECDx == ECD1)
    {
			if(((UTIMER_IF>>14) & timer_if & (UTIMER_IE>>14)) == 0)
			{
			  return 0;
			}else{
				return 1;
			}    
    }
		return 0;
}


/*******************************************************************************
 �������ƣ�    void ECD_ClearIRQFlag(TIM_ECDTypeDef *ECDx , uint32_t tempFlag)
 ����������    ���ECD�жϱ�־
 �����ı�    ��
 ���������    ECD�жϱ�־
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2022/4/13      V1.0           HuangMG            ����
 *******************************************************************************/
/**
 *@brief @b ��������:   void ECD_ClearIRQFlag(TIM_ECDTypeDef *ECDx , uint32_t tempFlag)
 *@brief @b ��������:   ���ECD�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       ECDx��ECD0/ECD1 \n
                        timer_if��
 * <table>              <tr><th>ECD_IF_UNDERFLOW   <td>Encoder�����ж�
                        <tr><th>ECD_IF_OVERFLOW    <td>Encoder�����ж�                 
 * </table> 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			    if(ECD_GetIRQFlag(ECD0,ECD_IF_OVERFLOW)) //��ȡ�����������жϱ�־
                {
                    ECD_ClearIRQFlag(ECD0,ECD_IF_OVERFLOW);//��������������жϱ�־
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2022��04��13�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
void ECD_ClearIRQFlag(TIM_ECDTypeDef *ECDx , uint32_t tempFlag)
{
	  if(ECDx == ECD0)
		{
			UTIMER_IF = tempFlag<<12;
		}
		if(ECDx == ECD1)
		{
			UTIMER_IF = tempFlag<<14;
		}
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
