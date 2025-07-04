/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_sif.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� SIF������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>myx   <td>�޸�
 * </table>
 */
#include "lks32mc07x_sif.h"


void SIF_StructInit(SIF_InitTypeDef *SIFInitStruct)
{
    memset(SIFInitStruct, 0, sizeof(SIF_InitTypeDef));
}

/**
 *@brief @b ��������:   void SIF_Init(SIF_RegTypeDef *SIFx, SIF_InitTypeDef *SIFInitStruct)
 *@brief @b ��������:   SIF��ʼ������
 *@see���������ݣ�       
 *@param���������       SIFx��SIF , SIF_InitTypeDef *SIF_InitStruct
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
	SIF_InitTypeDef SIF_InitStruct;
    SIF_StructInit(&SIF_InitStruct); 
	
	SIF_InitStruct.SIF_DONE = DISABLE;			// ������ɺ�Ĭ�ϵ͵�ƽ 
	SIF_InitStruct.SIF_SYNC = DISABLE;			// ͬ���ź�Ĭ�ϵ͵�ƽ 
	SIF_InitStruct.SIF_SYNC_PULSE = ENABLE;		// ͬ���ź��е�ƽ��ת 
	SIF_InitStruct.SIF_DONE_VLD = DISABLE;		// �޽����ź� 
	SIF_InitStruct.SIF_SYNC_VLD = ENABLE;		// ��ͬ���ź�
	SIF_InitStruct.SIF_RATIO = DISABLE;			// ����ռ�ձ� 2��1 
	SIF_InitStruct.SIF_MSB = ENABLE;			// ���ݴ���˳�� ���ֽ���ǰ 
	SIF_InitStruct.SIF_EN = ENABLE;				// SIFģ��ʹ�� 
	SIF_InitStruct.SIF_TOSC = 46;				// ʱ������ 47 * 333 = 15.67US 
	SIF_InitStruct.SIF_TSTH1 = 999;				// ͬ��ʱ�� 1000 * SIF_TOSC * 32 
	SIF_InitStruct.SIF_TDTH1 = 0;				// �����ź�ʱ�� 1MS 
	SIF_InitStruct.SIF_IRQ_IF = ENABLE;			// ��SIF�жϱ�־λ 
	SIF_InitStruct.SIF_DMA_EN = ENABLE;			// DMA����ʹ�� 
	SIF_InitStruct.SIF_IRQ_EN = DISABLE;		// SIF�жϹر� 
	
	SIF_Init(SIF0, &SIF_InitStruct);
	
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */

void SIF_Init(SIF_RegTypeDef *SIFx, SIF_InitTypeDef *SIFInitStruct)
{
	  SIFx->SIFx_TOSC  =  SIFInitStruct->SIF_TOSC;
    SIFx->SIFx_TSTH1 = SIFInitStruct->SIF_TSTH1;    
    SIFx->SIFx_TDTH1 = SIFInitStruct->SIF_TDTH1;    
    SIFx->SIFx_IRQ   = (SIFInitStruct->SIF_IRQ_EN<<0) | (SIFInitStruct->SIF_DMA_EN<<1) | (SIFInitStruct->SIF_IRQ_IF<<4);
	
    SIFx->SIFx_CFG   = (SIFInitStruct->SIF_EN<<0) | (SIFInitStruct->SIF_MSB<<1) | (SIFInitStruct->SIF_RATIO<<2)| \
	                (SIFInitStruct->SIF_SYNC_VLD<<3) | (SIFInitStruct->SIF_DONE_VLD<<4) | \
					(SIFInitStruct->SIF_SYNC_PULSE<<5) | (SIFInitStruct->SIF_SYNC<<6) | (SIFInitStruct->SIF_DONE<<7);
	
	SYS_ModuleClockCmd(SYS_Module_SIF0,ENABLE);   //HALLʱ��ʹ��
}  


/**
 *@brief @b ��������:  void SIF_Timebase_set(u32 tosc)
 *@brief @b ��������:   ����SIF����ʱ��
 *@see���������ݣ�       
 *@param���������       tosc  ��ʱ��ϵ��   
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_Timebase_set(u32 tosc)
{
    SIF0->SIFx_TOSC = tosc;
}

/**
 *@brief @b ��������:  void SIF_Timebase_set(u32 tosc)
 *@brief @b ��������:   �����ź�ͨ�ŷ�ʽ
 *@see���������ݣ�       
 *@param���������       tsh tsl ��ͬ���ź�����
 *                      tdh tdl �������ź�����   
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_TH_set(u32 tsh,u32 tdh)
{
    SIF0->SIFx_TSTH1 = tsh;
    SIF0->SIFx_TDTH1 = tdh;
}

/**
 *@brief @b ��������:  void SIF_Senddata(u8 Data)
 *@brief @b ��������:   ������������
 *@see���������ݣ�       
 *@param���������       Data :���������   
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_Senddata(u8 Data)
{
    REG32(SIF0_BASE + 0x14) = Data;
}

/**
 *@brief @b ��������:  void SIF_DMA_Enable(void)
 *@brief @b ��������:   SIF DMA��������ʹ��
 *@see���������ݣ�       
*@param���������       state : ENABLE/DIABLE   
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_DMA_IsEnable(u8 state)
{
	if(state == ENABLE)
	{
			SIF0->SIFx_IRQ |= SIF_DMA_Enable;
	}
	else
	{
			SIF0->SIFx_IRQ &= ~SIF_DMA_Enable;
	}

}

/**
 *@brief @b ��������:  void SIF_DMA_Enable(void)
 *@brief @b ��������:   SIF DMA��������ʹ��
 *@see���������ݣ�       
*@param���������       state : ENABLE/DIABLE   
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_Irq_IsEnable(u8 state)
{
	if(state == ENABLE)
	{
			SIF0->SIFx_IRQ |= SIF_IE_Enable;
	}
	else
	{
			SIF0->SIFx_IRQ &= ~SIF_IE_Enable;
	}
}

/**
 *@brief @b ��������:  void SIF_Clear_Irq()
 *@brief @b ��������:   ���SIFģ���־λ
 *@see���������ݣ�       
*@param���������        �� 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_Clear_Irq(void)
{
	SIF0->SIFx_IRQ |= BIT4;
}
