/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_tim.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ��ʱ�������������� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>HungMG       <td>����
 * </table>
 */
#include "lks32mc07x_tim.h"
#include "lks32mc07x_sys.h"


/**
 *@brief @b ��������:   void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *this)
 *@brief @b ��������:   ��ʱ����ʼ������
 *@see���������ݣ�      SYS_ModuleClockCmd()
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
                        TIM_TimerInitTypeDef  *TIM_TimerInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
                    ����1�Ƚ�ģʽ���ã�
                        TIM_TimerInitTypeDef TIM_InitStruct;
                        TIM_TimerStrutInit(&TIM_InitStruct);                   // Timer�ṹ���ʼ��
                        TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CMP; // ����Timer CH0 Ϊ�Ƚ�ģʽ
                        TIM_InitStruct.Timer_CH0Output = 0;                   // ����������ʱ���Ƚ�ģʽ������Կ���
                        TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CMP; // ����Timer CH1 Ϊ�Ƚ�ģʽ 
                        TIM_InitStruct.Timer_CH1Output = 0;                   // ����������ʱ���Ƚ�ģʽ������Կ���
                        TIM_InitStruct.Timer_TH = 48000;    				  // ��ʱ���������޳�ʼֵ48000
                        TIM_InitStruct.Timer_CMP0 = 24000;  				  // ���ñȽ�ģʽ��CH0�Ƚϳ�ʼֵ24000 
                        TIM_InitStruct.Timer_CMP1 = 24000;  				  // ���ñȽ�ģʽ��CH1�Ƚϳ�ʼֵ24000 
                        TIM_InitStruct.Timer_ClockDiv = TIMER_CLK_DIV1;       // ����Timerģ��ʱ��2��Ƶϵ�� 
                        TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_CH0 | Timer_IRQEna_CH1 | Timer_IRQEna_ZC; // ����Timerģ��Ƚ��жϺ͹����ж�
                        TIM_TimerInit(UTIMER0, &TIM_InitStruct); 
                        TIM_TimerCmd(UTIMER0, ENABLE);                        //��ʱ��2ģ��ʹ��  
                    ����2����ģʽ���ã� 
                        TIM_TimerInitTypeDef TIM_InitStruct;  
                        TIM_TimerStrutInit(&TIM_InitStruct);                   // Timer�ṹ���ʼ��
                        TIM_InitStruct.Timer_SRC0 = CAP_SRC_TCH1;             // ��ʱ��0ͨ��0����ʱ��0ͨ��1�������ź�
                        TIM_InitStruct.Timer_CH0_WorkMode = TIMER_OPMode_CAP; // ����Timer CH0Ϊ����ģʽ 
                        TIM_InitStruct.Timer_CAP0_CLR_EN =ENABLE;             // ������CAP0�����¼�ʱ������Timer������
                        TIM_InitStruct.Timer_CH0_RE_CAP_EN = ENABLE;          // ʹ��Timerͨ��0�����ز���
                        TIM_InitStruct.Timer_CH0_FE_CAP_EN = DISABLE;         // �ر�Timerͨ��0�½��ز���
                                                                                                                                        
                        TIM_InitStruct.Timer_SRC1 = CAP_SRC_TCH1;             // ��ʱ��0ͨ��1����ʱ��0ͨ��1�������ź�
                        TIM_InitStruct.Timer_CH1_WorkMode = TIMER_OPMode_CAP; // ����Timer CH1Ϊ����ģʽ 
                        TIM_InitStruct.Timer_CAP1_CLR_EN = DISABLE;           // ������CAP1�����¼�ʱ������Timer������
                        TIM_InitStruct.Timer_CH1_RE_CAP_EN = DISABLE;         // ʹ��Timerͨ��1�����ز���
                        TIM_InitStruct.Timer_CH1_FE_CAP_EN = ENABLE;          // �ر�Timerͨ��1�½��ز���
                                                                                                                                    
                        TIM_InitStruct.Timer_TH = 96000000;    				  // ��ʱ���������޳�ʼֵ96000000 
                        TIM_InitStruct.Timer_FLT = 0;  						  // ���ò�׽ģʽ�������ģʽ�¶�Ӧͨ���������˲�ֵ 
                        TIM_InitStruct.Timer_ClockDiv = TIMER_CLK_DIV1;       // ����Timerģ��ʱ��2��Ƶϵ�� 
                        TIM_InitStruct.Timer_IRQEna = Timer_IRQEna_ZC;        // ����Timerģ������ж�
                        TIM_TimerInit(UTIMER3, &TIM_InitStruct);                
                        TIM_TimerCmd(UTIMER3, ENABLE);                        // Timer3 ģ��ʹ��    
  @endcode
 *@par �޸���־: ȥ����ʱ��ʹ�ܽӿ�
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_TimerInit(TIM_TimerTypeDef *TIMERx, TIM_TimerInitTypeDef *this)
{
    if (TIMERx == UTIMER0)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER0, ENABLE);
    }
    else if (TIMERx == UTIMER1)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER1, ENABLE);
    }
    else if (TIMERx == UTIMER2)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER2, ENABLE);
    }
    else if (TIMERx == UTIMER3)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER3, ENABLE);
    }

		TIMERx->CFG = (u32)(this->Timer_CAP1_CLR_EN<<29) | (u32)(this->Timer_CAP0_CLR_EN<<28)
                |	(u32)(this->Timer0_UPDATE<<27) | (u32)(this->Timer0_SHADOW<<26)		
		            |	(u32)(this->Timer_ONE_TRIG<<25) | (u32)(this->Timer_CENTER<<24)		
		            |	(u32)(this->Timer_DIR<<23) | (u32)(this->Timer_ClockDiv<<20)			
		            |	(u32)(this->Timer_ETON<<19) | (u32)(this->Timer_GATE_EN<<18)	
		            |	(u32)(this->Timer_RL_EN<<17) | (u32)(this->Timer_XCLK_EN<<16)	
		            |	(u32)(this->Timer_SRC1<<12) | (u32)(this->Timer_CH1Output<<11)	
		            |	(u32)(this->Timer_CH1_WorkMode<<10) | (u32)(this->Timer_CH1_FE_CAP_EN<<9)	
		            |	(u32)(this->Timer_CH1_RE_CAP_EN<<8) | (u32)(this->Timer_SRC0<<4)		
		            |	(u32)(this->Timer_CH0Output<<3) | (u32)(this->Timer_CH0_WorkMode<<2)	
			          |	(u32)(this->Timer_CH0_FE_CAP_EN<<1) | (u32)(this->Timer_CH0_RE_CAP_EN);							
   TIMERx->TH = this->Timer_TH;
	 TIMERx->CMP0 = this->Timer_CMP0;					
	 TIMERx->CMP1 = this->Timer_CMP1;							
   TIMERx->EVT = this->Timer_EVT;
	 TIMERx->FLT = this->Timer_FLT;
	 TIMERx->IE =  this->Timer_IRQEna;

	 TIMERx->IO = this->Timer0_CH1_DEFAULT<<9 | this->Timer0_CH0_DEFAULT<<8 
              | this->Timer0_HALT_PRT<<7 | this->Timer0_FAIL_SEL<<2
							| this->Timer0_FAIL_POL<<1 | this->Timer0_FAIL_EN;
}

/**
 *@brief @b ��������:   void TIM_TimerStrutInit(TIM_TimerInitTypeDef *this)
 *@brief @b ��������:   ��ʱ���ṹ���ʼ��
 *@see���������ݣ�      ��
 *@param���������      TIM_TimerInitTypeDef  *this
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
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_TimerStrutInit(TIM_TimerInitTypeDef *this)
{		
		this->Timer_CAP1_CLR_EN = DISABLE;
		this->Timer_CAP0_CLR_EN = DISABLE; 
		 
		this->Timer_ONE_TRIG = DISABLE;    

		this->Timer_CENTER = 0;      
												
		this->Timer_DIR = 0;         

		this->Timer_ClockDiv = TIMER_CLK_DIV1;     
		this->Timer_ETON = 0;        
		this->Timer_GATE_EN = 0;     							 
		this->Timer_RL_EN = 0;       
		this->Timer_XCLK_EN = 0;     

		this->Timer_SRC1 = 0;        
		this->Timer_CH1Output = 0;     
		this->Timer_CH1_WorkMode = 0;
		this->Timer_CH1_FE_CAP_EN = 0;
		this->Timer_CH1_RE_CAP_EN = 0; 

		this->Timer_SRC0 = 0;        
		this->Timer_CH0Output = 0;     
		this->Timer_CH0_WorkMode = 0;
		this->Timer_CH0_FE_CAP_EN = 0;
		this->Timer_CH0_RE_CAP_EN = 0;

		this->Timer_TH = 0;          
		this->Timer_CNT = 0;         
		this->Timer_CMP0 = 0;        
		this->Timer_CMP1 = 0;        

		this->Timer_EVT = 0;         
		this->Timer_FLT = 0;         
		 
		this->Timer_IRQEna = 0;      
		 
		this->Timer0_UPDATE = 0; 
		this->Timer0_SHADOW = 0; 
		
    this->Timer0_CH0_DEFAULT = 0;
	  this->Timer0_CH1_DEFAULT = 0;
		this->Timer0_HALT_PRT = 0;
	  this->Timer0_FAIL_SEL = 0;
	  this->Timer0_FAIL_POL = 0;	
	  this->Timer0_FAIL_EN = 	0;	                  
}    
                     
/**
 *@brief @b ��������:  void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
 *@brief @b ��������:   ��ʱ��TIMERxʹ�ܺ�ֹͣ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
                        FuncState state��ʹ����ʧ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			    TIM_TimerCmd(UTIMER0, ENABLE); // Timer0ģ��ʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0       <td>HungMG         <td>����
 * </table>
 */
void TIM_TimerCmd(TIM_TimerTypeDef *TIMERx, FuncState state)
{
    if (state != DISABLE)
    {      
         TIMERx->CFG |= BIT31;
    }
    else
    {
         TIMERx->CFG &= ~BIT31;
    }
}

/**
 *@brief @b ��������:   void TIM_CMP0(TIM_TimerTypeDef *TIMERx, uint32_t temp)
 *@brief @b ��������:   д�붨ʱ��ͨ��0�Ƚ�ֵ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
 *                     temp���Ƚ���ֵ
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_CMP0(UTIMER0��2000); //�趨��ʱ��0ͨ��0�Ƚ�ֵΪ2000
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_CMP0(TIM_TimerTypeDef *TIMERx, uint32_t temp)
{
    TIMERx->CMP0 = temp;
}

/**
 *@brief @b ��������:   void TIM_CMP1(TIM_TimerTypeDef *TIMERx, uint32_t temp)
 *@brief @b ��������:   д�붨ʱ��ͨ��1�Ƚ�ֵ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
 *                     temp���Ƚ���ֵ
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_CMP1(UTIMER0 ��2000); //�趨��ʱ��0ͨ��1�Ƚ�ֵΪ2000
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_CMP1(TIM_TimerTypeDef *TIMERx, uint32_t temp)
{
	
    TIMERx->CMP1 = temp;
}

/**
 *@brief @b ��������:   u32 TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIMERx)
 *@brief @b ��������:   ��ȡ��ʱ��ͨ��0����ֵ
 *@see���������ݣ�       ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 
 *@param���������      ��
 *@return�� �� ֵ��     ��ʱ��ͨ��0����ֵ
 *@note����˵����       ����ʱ�����������¼�ʱ��������ʱ�̵�cntֵ�洢���üĴ�����
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_CAPValue = 0;
                    TIMER0_CAPValue = TIM_Timer_GetCMPT0(UTIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
u32 TIM_Timer_GetCMPT0(TIM_TimerTypeDef *TIMERx)
{
    return TIMERx->CMP0;
}

/**
 *@brief @b ��������:   u32 TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIMERx)
 *@brief @b ��������:   ��ȡ��ʱ��ͨ��1����ֵ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 
 *@param���������      ��
 *@return�� �� ֵ��     ��ʱ��ͨ��1����ֵ
 *@note����˵����       ����ʱ�����������¼�ʱ��������ʱ�̵�cntֵ�洢���üĴ�����
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_CAPValue = 0;
                    TIMER0_CAPValue = TIM_Timer_GetCMPT1(UTIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
u32 TIM_Timer_GetCMPT1(TIM_TimerTypeDef *TIMERx)
{
    return TIMERx->CMP1;
}

/**
 *@brief @b ��������:   void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag)
 *@brief @b ��������:   ���TIM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
                        timer_if��
 * <table>              <tr><th>Timer_IRQEna_CH0_RE    <td>ͨ��0�Ƚ�/����DMA����ʹ��
                        <tr><th>Timer_IRQEna_CH1_RE    <td>ͨ��1�Ƚ�/����DMA����ʹ��
                        <tr><th>Timer_IRQEna_ZC_RE     <td>��������0 DMA����ʹ��
                        <tr><th>Timer0_IRQEna_FAIL_RE  <td>��ʱ��0��FAIL�¼�DMA����ʹ��
                        <tr><th>Timer_IRQEna_CH1       <td>Timer CH1�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>Timer_IRQEna_CH0       <td>Timer CH0�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>Timer_IRQEna_ZC        <td>Timer �����ж�
 * </table> 
 *@param���������      ��   
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			         if(TIM_GetIRQFlag(UTIMER0,Timer_IRQEna_CH0)) //�ж�UTimer0��CH0�Ƿ����Ƚ��ж�
                {
                    TIM_ClearIRQFlag(UTIMER0,Timer_IRQEna_CH0); //���UTimer0ͨ��0�Ƚ��жϱ�־λ
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_ClearIRQFlag(TIM_TimerTypeDef *TIMERx ,uint32_t tempFlag)
{
    TIMERx->IF = tempFlag;
}

/**
 *@brief @b ��������:   u16 TIM_GetIRQFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x)
 *@brief @b ��������:   ��ȡTIM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
                        TIMER_IF_x��
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
			         if(TIM_GetIRQFlag(UTIMER0,TIMER_IF_CH0)) //�ж�UTimer0��CH0�Ƿ����Ƚ��ж�
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
u16 TIM_GetIRQFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x)
{
    if (UTIMERx->IF & UTIMERx->IE & TIMER_IF_x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 *@brief @b ��������:   u16 TIM_GetFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x)
 *@brief @b ��������:   ��ȡTIM�жϱ�־
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0 \n
                        TIMER_IF_x��
 * <table>              <tr><th>TIMER_IF_CH1   <td>Timer CH1�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_CH0   <td>Timer CH0�жϣ��Ƚϻ򲶻��ж�
                        <tr><th>TIMER_IF_ZERO  <td>Timer �����ж�
 * </table> 
 *@param���������      ��
 *@return�� �� ֵ��     0��1����Ӧ�жϱ�־��λ����1��δ��λ����0
 *@note����˵����       ��
 *@warning             ����Ҫ��Ӧ�ж�ʹ�ܺ󣬾Ϳ��Զ�ȡ��Ӧ�жϱ�־λ
 *@par ʾ�����룺
 *@code
			         if(TIM_GetFlag(UTIMER0,TIMER_IF_CH0)) //�ж�UTimer0��CH0�Ƿ����Ƚ��ж�
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
u16 TIM_GetFlag(TIM_TimerTypeDef *UTIMERx, u32 TIMER_IF_x)
{
    if (UTIMERx->IF & TIMER_IF_x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 *@brief @b ��������:   uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx)
 *@brief @b ��������:   ��ȡTimer����ֵ
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
                    uint32_t TIMER0_Value = 0;
                    TIMER0_Value = TIM_Timer_GetCount(UTIMER0); //��ȡ��ʱ��0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
uint32_t TIM_Timer_GetCount(TIM_TimerTypeDef *TIMERx)
{
    return TIMERx->CNT;
}


/**
 *@brief @b ��������:   void TIM_Enable(TIM_TimerTypeDef *UTIMERx)
 *@brief @b ��������:   ��ʱ��ʱ��ʹ�ܺ���
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
                   TIM_Enable(UTIMER0); //��ʱ��0ʱ��ʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_Enable(TIM_TimerTypeDef *UTIMERx)
{
    if (UTIMERx == UTIMER0)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER0, ENABLE);
    }
    else if (UTIMERx == UTIMER1)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER1, ENABLE);
    }
    else if (UTIMERx == UTIMER2)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER2, ENABLE);
    }
    else if (UTIMERx == UTIMER3)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER3, ENABLE);
    }
}
/**
 *@brief @b ��������:   void TIM_Disable(TIM_TimerTypeDef *UTIMERx)
 *@brief @b ��������:   ��ʱ��ʱ��ʧ�ܺ���
 *@see���������ݣ�      ��
 *@param���������      TIMERx��UTIMER0/UTIMER0/UTIMER0/UTIMER0
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
                   TIM_Disable(UTIMER0); //��ʱ��0ʱ��ʧ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��22�� <td>1.0        <td>HungMG        <td>����
 * </table>
 */
void TIM_Disable(TIM_TimerTypeDef *UTIMERx)
{
    if (UTIMERx == UTIMER0)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER0, DISABLE);
    }
    else if (UTIMERx == UTIMER1)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER1, DISABLE);
    }
    else if (UTIMERx == UTIMER2)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER2, DISABLE);
    }
    else if (UTIMERx == UTIMER3)
    {
        SYS_ModuleClockCmd(SYS_Module_TIMER3, DISABLE);
    }
}



/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
