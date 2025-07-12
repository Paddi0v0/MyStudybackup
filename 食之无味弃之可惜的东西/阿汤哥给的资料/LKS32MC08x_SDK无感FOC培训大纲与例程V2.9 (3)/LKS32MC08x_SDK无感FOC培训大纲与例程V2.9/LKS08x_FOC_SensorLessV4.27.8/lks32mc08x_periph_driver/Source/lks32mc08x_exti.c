/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_exti.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� EXTI������������ \n
 * ����˵���� �� \n
 *@par 
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2021��11��10�� <td>1.0    <td>YangZJ      <td>����
 * </table>
 */
#include "lks32mc08x_exti.h"

/**
 *@brief @b ��������:   void EXTI_Trigger_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint16_t EXTI_Trigger)
 *@brief @b ��������:   GPIO�ⲿ��������ѡ��
 *@see���������ݣ�       GPIO_TypeDef
 *@param���������       
                        GPIOx:GPIO�����ѡ(GPIO0) \n
                        GPIO_PadSource:ָ����PadSource��ѡ(GPIO_PinSource_0~GPIO_PinSource_15) \n
                        EXTI_Trigger:ָ���Ĵ������� @see EXTI_Trigger������Դ��EXTIMode_TypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
		EXTI_Trigger_Config(GPIO0,GPIO_PinSource_0,EXTI_Trigger_Negedge);//P0.0�ź��½��ش����ⲿ�ж��¼�
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��10�� <td>1.0       <td>YangZJ        <td>����
 * </table>
 */
void EXTI_Trigger_Config(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource, uint16_t EXTI_Trigger)
{
    if(GPIOx == GPIO0)
    {
        if(GPIO_PinSource>16)
        {
            return;
        }
        if(GPIO_PinSource<8)
        {
            EXTI->CR0 = (EXTI->CR0 & 0xfffc) | (EXTI_Trigger << (2 * GPIO_PinSource));
        }
        else
        {
            EXTI->CR1 = (EXTI->CR0 & 0xfffc) | (EXTI_Trigger << (2 * (GPIO_PinSource - 8)));
        }
    }
}

/**
 *@brief @b ��������:   u8 EXTI_GetIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
 *@brief @b ��������:   ��ȡ�ⲿ�жϱ�־λ
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO�����ѡ(GPIO0~GPIO3) \n
                        GPIO_PadSource:ָ����PadSource��ѡ(GPIO_PinSource_0~GPIO_PinSource_15)
 *@param���������      ��
 *@return�� �� ֵ��     �жϱ�־������Ч
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
		if(EXTI_GetIRQFlag(GPIO0,GPIO_PinSource_0))//��ȡP0.0�ⲿ�жϱ�־λ
        {
        }
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��10�� <td>1.0       <td>YangZJ        <td>����
 * </table>
 */
u8 EXTI_GetIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
{
    if(GPIO_PinSource>=16)
    {
        return 0;
    }
    if(GPIOx == GPIO0)
    {
        if(EXTI_IF & (1<<GPIO_PinSource))
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
 *@brief @b ��������:   void EXTI_ClearIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
 *@brief @b ��������:   ����ⲿ�жϱ�־λ
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO�����ѡ(GPIO0~GPIO3) \n
                        GPIO_PadSource:ָ����PadSource��ѡ(GPIO_PinSource_0~GPIO_PinSource_15)
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
		if(EXTI_GetIRQFlag(GPIO0,GPIO_PinSource_0))//��ȡP0.0�ⲿ�жϱ�־λ
        {
            EXTI_ClearIRQFlag(GPIO0,GPIO_PinSource_0);//���P0.0�ⲿ�жϱ�־λ
        }
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��10�� <td>1.0       <td>YangZJ        <td>����
 * </table>
 */
void EXTI_ClearIRQFlag(GPIO_TypeDef *GPIOx, uint32_t GPIO_PinSource)
{
    if(GPIOx == GPIO0)
    {
        EXTI_IF = (1<<GPIO_PinSource);
    }
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/


