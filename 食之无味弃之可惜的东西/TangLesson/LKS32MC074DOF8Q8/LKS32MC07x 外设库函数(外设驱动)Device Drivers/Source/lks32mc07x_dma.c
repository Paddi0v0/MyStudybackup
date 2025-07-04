/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_dma.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DMA������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2020��8��1�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:�޸�dma��ʼ����غ���
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
#include "lks32mc07x_dma.h"
#include "string.h"

/*******************************************************************************
 �������ƣ�    void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
 ����������    DMA�ṹ���ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020.08.1      V1.0           howlet              ����
 *******************************************************************************/
/**
 *@brief @b ��������:   void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
 *@brief @b ��������:   DMA�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       DMA_InitTypeDef
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DMA_InitTypeDef DMA_InitStructure;
		   DMA_StructInit(&DMA_InitStructure); //��ʼ���ṹ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_StructInit(DMA_InitTypeDef *DMAInitStruct)
{
    memset(DMAInitStruct, 0, sizeof(DMA_InitTypeDef));
}

/**
 *@brief @b ��������:   void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
 *@brief @b ��������:   DMA��ʼ������
 *@see���������ݣ�       ��
 *@param���������       ADCx��ADC0 , ADC_InitTypeDef
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code    
	DMA_InitTypeDef DMA_InitStruct;
  DMA_StructInit(&DMA_InitStruct);
	DMA_InitStruct.EN = ENABLE;                                          // DMAͨ��ʹ��
	DMA_InitStruct.IRQ_EN = ENABLE;                                      // DMA ��������ж�ʹ�� 
	DMA_InitStruct.CIRC = DISABLE;                                       // DMA����ģʽ��ѭ��ģʽ������Ч 
	DMA_InitStruct.SINC = ENABLE;                                        // Դ��ַ����,  ����Ч,��ַ���� SBTW ��Ӧ��С���� 1/2/4
	DMA_InitStruct.DINC = DISABLE;                                       // Ŀ�ĵ�ַ����,����Ч,��ַ���� DBTW ��Ӧ��С���� 1/2/4
	DMA_InitStruct.SBTW = DMA_BYTE_TRANS;                                // Դ����λ�� 0:byte, 1:half-word, 2:word 
	DMA_InitStruct.DBTW = DMA_BYTE_TRANS;                                // Ŀ�ķ���λ�� 0:byte, 1:half-word, 2:word 
	DMA_InitStruct.REN.B.SIF_EN = ENABLE;                                // SIF DMA����ʹ�ܣ�����Ч  
	DMA_InitStruct.TMS = 12;                                             // DMA ͨ�� x ���ݰ��˴��� 1~255 
	DMA_InitStruct.RMODE = 1;                                            // 0:���ִ��䣬һ������������ / 1:���֣�ÿ�ֽ���һ�����ݴ��� 
	DMA_InitStruct.SADR = (u32)stru_yxtsenddata.byxt_send_data;          // DMA ͨ�� x Դ��ַ &CAN0_TXBUF0
	DMA_InitStruct.DADR = (u32)&SIF0_WDATA;                              // DMA ͨ�� x Ŀ�ĵ�ַ CAN0_TXRX0 CAN0_RFIFO00
	DMA_Init(DMA_CHN0, &DMA_InitStruct);
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2015��11��26�� <td>1.0     <td>Howlet Li       <td>����
 * </table>
 *@par �޸���־:ȥ����ʼ��ʹ��DMAͨ������ΪDMA_CHx_EN��������ʹ��DMAͨ��
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *this)
{
    // DMA0_CTRL  = 0x0001;  // DMA����ʹ��
    DMA_Enable();
    /* ͨ�����üĴ��� ��ֵ */
    DMAx->CCR = 0;
    DMAx->CTMS  =   this->TMS;
    DMAx->SADR  =   this->SADR;
    DMAx->DADR  =   this->DADR;
    DMAx->REN   =   this->REN.R;
    DMAx->CCR   =   (this->SBTW     <<  10) | (this->DBTW   <<  8 ) |
                    (this->SINC     <<  6 ) | (this->DINC   <<  4 ) |
                    (this->CIRC     <<  3 ) | (this->RMODE  <<  1 ) |
                    (this->EN);
    if (this->IRQ_EN)
        switch ((u32)DMAx)
        {
        case DMA0_BASE:
            DMA0_IE |= BIT0;
            break;
        case DMA0_BASE + 0x20:
            DMA0_IE |= BIT1;
            break;
        case DMA0_BASE + 0x40:
            DMA0_IE |= BIT2;
            break;
        case DMA0_BASE + 0x60:
            DMA0_IE |= BIT3;
            break;
        }
    DMA0_CTRL = 0x0001; /*enable dma, mcu has higher priorit */
}  

/**
 *@brief @b ��������:   uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
 *@brief @b ��������:   ��ȡDMA�жϱ�־
 *@see���������ݣ�       DMAx��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *@param���������       DMAx��DMAͨ��ѡ��  \n
                        timer_if������ѡ�� 
 * <table>              <tr><td> �궨��          <td>˵��
                        <tr><th> DMA_CH0_FIF    <td>ͨ�� 0 ����жϱ�־
 *					    <tr><th> DMA_CH1_FIF    <td>ͨ�� 1 ����жϱ�־
 *						<tr><th> DMA_CH2_FIF	<td>ͨ�� 2 ����жϱ�־
 *						<tr><th> DMA_CH3_FIF    <td>ͨ�� 3 ����жϱ�־
 *						<tr><th> DMA_CH0_EIF    <td>ͨ�� 0 �����жϱ�־
 *						<tr><th> DMA_CH1_EIF    <td>ͨ�� 1 �����жϱ�־
 *						<tr><th> DMA_CH2_EIF    <td>ͨ�� 2 �����жϱ�־
 *						<tr><th> DMA_CH3_EIF    <td>ͨ�� 3 �����жϱ�־
 * </table>  
 *@see 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0   
 *@par ʾ�����룺
 *@code    
           if(DMA_GetIRQFlag(DMA_CH0,DMA_CH0_FIF))//��ȡDMAͨ��0����жϱ�־
		   {	
		   }
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
u32 DMA_GetIRQFlag(u32 DMA_IF_x)
{
    if (DMA0_IF & DMA_IF_x & DMA0_IE)
    {
        return 1;
    }
    return 0;
}

/**
 *@brief @b ��������:   void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
 *@brief @b ��������:   ���DMA�жϱ�־
 *@see ���������ݣ�      DMAx��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *@param ���������      DMAx��DMAͨ��ѡ��  \n 
                        timer_if������ѡ�� 
 * <table>              <tr><td> �궨��          <td>˵��
                        <tr><th> DMA_CH0_FIF    <td>ͨ�� 0 ����жϱ�־
 *					    <tr><th> DMA_CH1_FIF    <td>ͨ�� 1 ����жϱ�־
 *						<tr><th> DMA_CH2_FIF	<td>ͨ�� 2 ����жϱ�־
 *						<tr><th> DMA_CH3_FIF    <td>ͨ�� 3 ����жϱ�־
 *						<tr><th> DMA_CH0_EIF    <td>ͨ�� 0 �����жϱ�־
 *						<tr><th> DMA_CH1_EIF    <td>ͨ�� 1 �����жϱ�־
 *						<tr><th> DMA_CH2_EIF    <td>ͨ�� 2 �����жϱ�־
 *						<tr><th> DMA_CH3_EIF    <td>ͨ�� 3 �����жϱ�־
 * </table>   
 * 
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
           if(DMA_GetIRQFlag(DMA_CH0,DMA_CH0_FIF))//��ȡDMAͨ��0����жϱ�־
		   {	
			  DMA_ClearIRQFlag(DMA_CH0,DMA_CH0_FIF)//���DMAͨ��0����жϱ�־
		   }
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_ClearIRQFlag(u32 DMA_IF_x)
{
    DMA0_IF = DMA_IF_x;
}

/**
 *@brief @b ��������:   void DMA_Enable(void)
 *@brief @b ��������:   DMAģ�� ʹ�ܺ���
 *@see ���������ݣ�      DMAx��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *@param ���������   ��
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_Enable(void)
{
    DMA_TOP->CTRL = 1;
}
/**
 *@brief @b ��������:   void DMA_Enable(void)
 *@brief @b ��������:   DMAģ�� �رպ���
 *@see ���������ݣ�   
 *@param ���������   ��
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_Disable(void)
{
    DMA_TOP->CTRL = 0;
}

/**
 *@brief @b ��������:   void DMA_SetAddr(DMA_RegTypeDef *DMA_CHx, u32 addr1, u32 addr2)
 *@brief @b ��������:   DMA�����ַ����
 *@see ���������ݣ�    
 *@param ���������   
 *                      DMA_CHX :DMA ͨ��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *                      addr1 : Դ��ַ����
 *                      addr2 ��Ŀ�ĵ�ַ����
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_SetAddr(DMA_RegTypeDef *DMA_CHx, u32 addr1, u32 addr2)
{
    DMA_CHx->CCR = DMA_CHx->CCR & ~BIT0;
    DMA_CHx->SADR = addr1;
    DMA_CHx->DADR = addr2;
    DMA_CHx->CCR = DMA_CHx->CCR | BIT0;
}

/**
 *@brief @b ��������:   void DMA_SetLength(DMA_RegTypeDef *DMA_CHx, u32 len)
 *@brief @b ��������:   DMA���䳤������
 *@see ���������ݣ�     
 *@param ���������   
 *                      DMA_CHX :DMA ͨ��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *                      len : ���䳤������
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_SetLength(DMA_RegTypeDef *DMA_CHx, u32 len)
{
    DMA_CHx->CCR = DMA_CHx->CCR & ~BIT0;
    DMA_CHx->CTMS = len;
    DMA_CHx->CCR = DMA_CHx->CCR | BIT0;
}

/**
 *@brief @b ��������:   void DMA_Trigger(DMA_RegTypeDef *DMA_CHx)
 *@brief @b ��������:   DMA �����������
 *@see ���������ݣ�     
 *@param ���������   
 *                      DMA_CHX :DMA ͨ��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_Trigger(DMA_RegTypeDef *DMA_CHx)
{
    DMA_CHx->REN |= BIT31;
}

/**
 *@brief @b ��������:   void DMA_SetRequest(DMA_RegTypeDef *DMA_CHx, u32 DMA_REQ_x)
 *@brief @b ��������:   DMA ��������ʹ��
 *@see ���������ݣ�     
 *@param ���������   
 *                      DMA_CHX :DMA ͨ��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *                      DMA_REQ_x ��������ѡ: \n
 * <table> 
 *                       <tr><td> �궨��                   <td>ԭʼֵ   <td>˵��
 *                      <tr><th> DMA_REQ_UART1_TX <td> BIT19
 *                      <tr><th> DMA_REQ_UART1_RX <td> BIT18
 *                      <tr><th> DMA_REQ_UART0_TX <td> BIT17
 *                      <tr><th> DMA_REQ_UART0_RX <td> BIT16
 *                      <tr><th> DMA_REQ_I2C0_TX  <td>  BIT15 
 *                      <tr><th> DMA_REQ_I2C0_RX  <td>  BIT14 
 *                      <tr><th> DMA_REQ_MCPWM1   <td> BIT13  
 *                      <tr><th> DMA_REQ_MCPWM0   <td> BIT12  
 *                      <tr><th> DMA_REQ_SPI_TX   <td> BIT11  
 *                      <tr><th> DMA_REQ_SPI_RX    <td> BIT10  
 *                      <tr><th> DMA_REQ_TIMER4   <td> BIT7   
 *                      <tr><th> DMA_REQ_TIMER3    <td> BIT6   
 *                      <tr><th> DMA_REQ_TIMER2    <td> BIT5   
 *                      <tr><th> DMA_REQ_TIMER1    <td> BIT4   
 *                      <tr><th> DMA_REQ_TIMER0    <td> BIT3   
 *                      <tr><th> DMA_REQ_DSP    <td> BIT2      
 *                      <tr><th> DMA_REQ_ADC1   <td> BIT1     
 *                      <tr><th> DMA_REQ_ADC0   <td> BIT0      
 * </table>
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_SetRequest(DMA_RegTypeDef *DMA_CHx, u32 DMA_REQ_x)
{
    DMA_CHx->REN = DMA_REQ_x;
}


/**
 *@brief @b ��������:  void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
 *@brief @b ��������:   DMAͨ��ʹ��
 *@see ���������ݣ�     
 *@param ���������   
 *                      DMA_CHX :DMA ͨ��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *                      Channel_EN : ENABLE/DISABLE
 *@param ���������   ��
 *@return �� �� ֵ��  ��
 *@note ����˵����    ��
 *@warning           ��
 *@par ʾ�����룺
 *@code    
  @endcode 
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author    <th>Description
 * <tr><td>2023��2��11�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
{
   if(Channel_EN)
	 {
	   DMAx->CCR  |= BIT0;/*ʹ��DMA����*/
	 }else{
	   DMAx->CCR  &= ~BIT0; /*�ر�ͨ��ʹ��*/
	 }
}

