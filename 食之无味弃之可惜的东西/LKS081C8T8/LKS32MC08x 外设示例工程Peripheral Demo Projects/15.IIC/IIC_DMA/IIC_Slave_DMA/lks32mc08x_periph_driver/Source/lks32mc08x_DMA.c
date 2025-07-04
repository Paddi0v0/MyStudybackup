/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC08x_dma.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� DMA������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2020��8��1�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:����DMAͨ��ʹ�ܺ���,��ȡ�����DAM�жϱ�־λ����
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��13�� <td>1.1     <td>HuangMG   <td>�޸�
 * </table>
 */

#include "lks32mc08x_dma.h"
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
 * <tr><td>2015��11��26�� <td>1.0     <td>Howlet     <td>�޸�
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
			u8 memaddr[25] ={0x11,0x12,0x13};//�������ݻ�����

			DMA_InitTypeDef DMA_InitStruct;
			DMA_StructInit(&DMA_InitStruct);
			DMA_InitStruct.DMA_IRQ_EN = DMA_TCIE;           // DMA ��������ж�ʹ�� 
			DMA_InitStruct.DMA_DIR = MEMORY2PERI;           // �ڴ������� 
			DMA_InitStruct.DMA_CIRC = DISABLE;              // �ر�ѭ������ģʽ 
			DMA_InitStruct.DMA_PINC = ENABLE;               // �����ַÿ�����Ƿ����,����Ч 
			DMA_InitStruct.DMA_MINC = ENABLE;               // �ڴ��ַ�ڶ����Ƿ��ڵ�һ�ֵ�ַ�Ļ����ϵ��������ڵ�ַһ��������,����Ч 
			DMA_InitStruct.DMA_PBTW = DMA_BYTE_TRANS;       // �������λ�� 0:byte, 1:half-word, 2:word 
			DMA_InitStruct.DMA_MBTW = DMA_BYTE_TRANS;       // �ڴ����λ�� 0:byte, 1:half-word, 2:word 
			DMA_InitStruct.DMA_REQ_EN = DMA_CH3_CAN_REQ_EN; // CAN DMA����ʹ�ܣ�����Ч 
			DMA_InitStruct.DMA_TIMES = 5;                   // DMA ͨ�� x ÿ�����ݰ��˴��� 1~511 
			DMA_InitStruct.DMA_ROUND = 5;                   // DMA ͨ�� x �������� 1~255 
			DMA_InitStruct.DMA_CPAR = (u32)&CAN_TXRX0;      // DMA ͨ�� x �����ַ
			DMA_InitStruct.DMA_CMAR = (u32)memaddr;         // DMA ͨ�� x �ڴ��ַ 
			DMA_Init(DMA_CH3, &DMA_InitStruct);
			DMA_CHx_EN(DMA_CH3,ENABLE);                     //ʹ��DMA_CH3ͨ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2015��11��26�� <td>1.0     <td>Howlet Li       <td>����
 * </table>
 *@par �޸���־:ȥ����ʼ��ʹ��DMAͨ������ΪDMA_CHx_EN��������ʹ��DMAͨ��
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��4��13�� <td>1.1       <td>HuangMG        <td>�޸�
 * </table>
 */
void DMA_Init(DMA_RegTypeDef *DMAx, DMA_InitTypeDef *DMAInitStruct)
{
    /* ͨ�����üĴ��� ��ֵ */
    DMAx->DMA_CCR = 0;
    DMAx->DMA_CTMS = DMAInitStruct->DMA_TIMES | (DMAInitStruct->DMA_ROUND<<16); /* ��������Ĵ��� */
    DMAx->DMA_CPAR = DMAInitStruct->DMA_CPAR;   /* �����ַ�Ĵ��� */
    DMAx->DMA_CMAR = DMAInitStruct->DMA_CMAR;   /* �ڴ��ַ�Ĵ��� */
    DMAx->DMA_CCR  =  (DMAInitStruct->DMA_IRQ_EN<<1) | (DMAInitStruct->DMA_DIR<<4) |\
                     (DMAInitStruct->DMA_CIRC<<5) | (DMAInitStruct->DMA_PINC<<6) | (DMAInitStruct->DMA_MINC<<7) |\
                     (DMAInitStruct->DMA_PBTW<<8) | (DMAInitStruct->DMA_MBTW<<10) | (DMAInitStruct->DMA_REQ_EN<<12);

    DMA_CTRL  = 0x0001;  /*enable dma, mcu has higher priorit */
}  

/**
 *@brief @b ��������:   void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
 *@brief @b ��������:   ʹ��DMAͨ��
 *@see���������ݣ�       DMAx��ѡ�� DMA_CH0 �� DMA_CH1 �� DMA_CH2 �� DMA_CH3
 *@param���������       DMAx��DMAͨ��ѡ�� \n 
                        Channel_EN��ENABLE��ʹ��DMAͨ����DISABLE���ر�DMAͨ��ʹ�� \n         
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           DMA_CHx_EN(DMA_CH0,ENABLE);//ʹ��DMAͨ��0
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��13�� <td>1.1       <td>HuangMG    <td>�޸�
 * </table>
 */
void DMA_CHx_EN(DMA_RegTypeDef *DMAx,u32 Channel_EN)
{
   if(Channel_EN)
	 {
	   DMAx->DMA_CCR  |= BIT0;/*ʹ��DMA����*/
	 }else{
	   DMAx->DMA_CCR  &= ~BIT0; /*�ر�ͨ��ʹ��*/
	 }
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
 * <tr><td>2022��4��13�� <td>1.1       <td>HuangMG    <td>�޸�
 * </table>
 */
uint32_t DMA_GetIRQFlag(DMA_RegTypeDef *DMAx  , u32 timer_if)
{
	 u8 reg = 0;	 
   if((DMAx->DMA_CCR) & BIT1)/*��������ж�ʹ��*/
	 {
		  reg = timer_if & 0x0f;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
	 }
   if((DMAx->DMA_CCR) & BIT2)/*�����ж�ʹ��*/
	 {
		  reg = timer_if & 0xf0;
	    if(DMA_IF & reg)
			{
			  return 1;
			}
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
 * <tr><td>2022��4��13��   <td>1.1      <td>HuangMG      <td>�޸�
 * </table>
 */
void DMA_ClearIRQFlag(DMA_RegTypeDef *DMAx , uint32_t tempFlag)
{
    DMA_IF = tempFlag;
}
