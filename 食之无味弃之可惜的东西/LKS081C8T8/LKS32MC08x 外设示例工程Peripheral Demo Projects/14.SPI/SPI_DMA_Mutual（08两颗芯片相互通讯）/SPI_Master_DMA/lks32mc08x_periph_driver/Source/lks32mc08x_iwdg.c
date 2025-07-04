/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_IWDG.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� IWDG������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */

#include "lks32mc08x_iwdg.h"

/**
 *@brief @b ��������:   void IWDG_Init(IWDG_InitTypeDef *this)
 *@brief @b ��������:   ���Ź���ʼ��
 *@see���������ݣ�       ��
 *@param���������       IWDG_InitTypeDef *this ���Ź����ýṹ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_InitTypeDef IWDG_InitStruct;
                IWDG_StrutInit(&IWDG_InitStruct);    //��ʼ���ṹ��
                IWDG_InitStruct.WDG_EN  = ENABLE;    //ʹ�ܿ��Ź�
                IWDG_InitStruct.RTH     = WDT_RTH_2S;//���ÿ��Ź�2S��λоƬ
                IWDG_Init(&IWDG_InitStruct);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
void IWDG_Init(IWDG_InitTypeDef *this)
{
    SYS_WR_PROTECT = 0xCAFE;
    SYS_WDT_CLR = 0x7988 | (this->RTH << 1) | (1);
    SYS_WR_PROTECT = 0x7a83;
    if (this->WDG_EN)
    {
        SYS_RST_CFG |= BIT0;
    }
    else
    {
        SYS_RST_CFG &= ~BIT0;
    }
    SYS_WR_PROTECT = 0;
}

/**
 *@brief @b ��������:   void IWDG_StrutInit(IWDG_InitTypeDef *this)
 *@brief @b ��������:   ���Ź����ýṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       IWDG_InitTypeDef *this ���Ź����ýṹ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_InitTypeDef IWDG_InitStruct;
                IWDG_StrutInit(&IWDG_InitStruct);    //��ʼ���ṹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
void IWDG_StrutInit(IWDG_InitTypeDef *this)
{
    this->WDG_EN = DISABLE; // �������Ź�ʹ��
    this->RTH = WDT_RTH_8S; // ���Ź���ʱ��λʱ�䣨21λ������������12��λ0��
                            // SECOND2IWDGCNT�����������
    //this->DWK_EN = DISABLE;             // ������߶�ʱ����ʹ��
    //this->WTH = SECOND2IWDGCNT(1);      // ���Ź���ʱ����ʱ��,���ڸ�λʱ�����Ч
}

/**
 *@brief @b ��������:   void IWDG_DISABLE(void)
 *@brief @b ��������:   �رտ��Ź�ģ��
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_DISABLE();    //�رտ��Ź�ģ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
void IWDG_DISABLE(void)
{
    SYS_RST_CFG &= ~BIT0;
}

/**
 *@brief @b ��������:   void IWDG_ENABLE(void)
 *@brief @b ��������:   �򿪿��Ź�ģ��
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_ENABLE();    //�򿪿��Ź�ģ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
void IWDG_ENABLE(void)
{
    SYS_RST_CFG |= BIT0;
}

/**
 *@brief @b ��������:   void IWDG_Feed(uint32_t timeOut)
 *@brief @b ��������:   ���Ź�ι��
 *@see���������ݣ�       ��
 *@param���������       
                        timeOut�����Ź���λʱ�� \n
                        WDT_RTH_2S    3s \n
                        WDT_RTH_4S    2s \n
                        WDT_RTH_8S    1s \n
                        WDT_RTH_64S   0s
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_Feed(WDT_RTH_2S);    //���Ź�ι������������һ�θ�λʱ��Ϊ2S
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2021��11��09�� <td>1.0     <td>YangZJ      <td>����
 * </table>
 */
void IWDG_Feed(uint32_t timeOut)
{
	
	SYS_WR_PROTECT = 0xCAFE; // 0xCAFE;

    if (timeOut == (uint32_t)WDT_RTH_64S)
    {
        SYS_WDT_CLR = 0x7989; // 1 00 1
    }
    else if (timeOut == (uint32_t)WDT_RTH_8S)
    {
        SYS_WDT_CLR = 0x798B; // 1 01 1
    }
    else if (timeOut == (uint32_t)WDT_RTH_4S)
    {
        SYS_WDT_CLR = 0x798D; // 1 10 1
    }
    else // WDT_RTH_2S
    {
        SYS_WDT_CLR = 0x798F; // 1 11 1
    }

    SYS_WR_PROTECT = 0;
}
/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
