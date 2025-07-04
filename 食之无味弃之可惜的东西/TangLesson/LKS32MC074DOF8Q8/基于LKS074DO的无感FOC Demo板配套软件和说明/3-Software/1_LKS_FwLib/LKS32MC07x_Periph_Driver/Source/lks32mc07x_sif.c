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

/**
 *@brief @b ��������:   void SIF_Init(STF_Typedef* SIFx ,SIF_InitTypeDef* SIF_InitStruct)
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
		SIF_InitStruct.EN = 1;
		SIF_InitStruct.MSB = 0;
		SIF_InitStruct.RATIO = 0;
		SIF_InitStruct.SYS_VLD = 1;
		SIF_InitStruct.DONE_VLD = 1;
		SIF_InitStruct.SYSD = 0;
		SIF_InitStruct.DONED =0;
		SIF_InitStruct.SYS_PLUSE = 0;
		SIF_Init(SIF0,&SIF_InitStruct);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_Init(STF_Typedef* SIFx ,SIF_InitTypeDef* SIF_InitStruct)
{
    SYS_ModuleClockCmd(SYS_Module_SIF0,ENABLE);
    SIFx-> CFG = (SIF_InitStruct->EN | SIF_InitStruct->MSB <<1 \
                 | SIF_InitStruct->RATIO <<2                    \
                 | SIF_InitStruct->SYS_VLD <<3                 \
                 | SIF_InitStruct->DONE_VLD    <<4              \
                 | SIF_InitStruct->SYSD   <<5                  \
                 | SIF_InitStruct->DONED   <<6               \
                 | SIF_InitStruct->SYS_PLUSE << 7);               
}

/**
 *@brief @b ��������:    void SIF_StructInit(SIF_InitTypeDef* SIF_InitStruct)
 *@brief @b ��������:    SIF�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       SIF_InitTypeDef *SIF_InitStruct
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			SIF_InitTypeDef SIF_InitStruct;
            SIF_StructInit(&SIF_InitStruct);              //SIF�ṹ���ʼ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��12�� <td>1.1     <td>ywx   <td>�޸�
 * </table>
 */
void SIF_StructInit(SIF_InitTypeDef* SIF_InitStruct)
{
    SIF_InitStruct-> EN   = 0;
    SIF_InitStruct-> MSB  = 0;
    SIF_InitStruct->RATIO = 0;
    SIF_InitStruct->SYS_VLD = 0;
    SIF_InitStruct->DONE_VLD = 0;
    SIF_InitStruct->SYSD  = 0;
    SIF_InitStruct->DONED = 0;
    SIF_InitStruct->SYS_PLUSE = 0;
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
    SIF->TOSC = tosc;
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
    SIF-> STH1 = tsh;
    SIF-> DTH1 = tdh;
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
			SIF->IRQ |= SIF_DMA_Enable;
	}
	else
	{
			SIF->IRQ &= ~SIF_DMA_Enable;
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
			SIF->IRQ |= SIF_IE_Enable;
	}
	else
	{
			SIF->IRQ &= ~SIF_IE_Enable;
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
	SIF->IRQ |= BIT4;
}


