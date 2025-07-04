/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_qep.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� ������������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0    <td>HuangMG     <td>����
 * </table>
 */


#include "lks32mc07x_lib.h"


/*******************************************************************************
 �������ƣ�    void TIM_QEPInit(TIM_QEP_TypeDef *TIM_QEPx, TIM_QEPInitTypeDef *TIM_QEPInitStruct)
 ����������    QEP��ʼ��
 �����ı�    ��
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2016/06/06      V1.0           cfwu              ����
 *******************************************************************************/

/**
 *@brief @b ��������:  void TIM_QEPInit(TIM_QEPTypeDef *QEPx, TIM_QEPInitTypeDef *TIM_QEPInitStruct)
 *@brief @b ��������:   ��������ʼ������
 *@see���������ݣ�      SYS_ModuleClockCmd();
 *@param���������      QEPx QEP0/QEP1 \n
                       TIM_QEPInitTypeDef *TIM_QEPInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_QEPInitTypeDef TIM_QEPInitStruct;  
                    TIM_QEPStructInit(&TIM_QEPInitStruct);                   // Timer�ṹ���ʼ��
                    TIM_QEPInitStruct.QEP_FE_CNT_EN = DISABLE;               // ʹ���½��ؽ��м�����ENABLE:ʹ�� DISABLE:ʧ��
                    TIM_QEPInitStruct.QEP_Mode = QEP_Mode_T1;                // ������ģʽѡ��
                    TIM_QEPInitStruct.QEP_ClockDiv = QEP_CLK_DIV1;           // ������ʱ�ӷ�Ƶ
                    TIM_QEPInitStruct.QEP_Filter = 0;                        // �����ź��˲�ϵ��
                    TIM_QEPInitStruct.QEP_TH = 65535;                        // ��������������
                    TIM_QEPInitStruct.QEP_ZEC = ENABLE;                      // Z �ź����㼫��ѡ�񣺵͵�ƽ/�½�������ʹ��
                    TIM_QEPInitStruct.QEP_ZLC = DISABLE;                     // Z �ź����㼫��ѡ�񣺸ߵ�ƽ/�½�������ʹ��
                    TIM_QEPInitStruct.QEP_ZNC = DISABLE;                     // Z �źŵ�ƽ���� QEP ������ʹ�� 
                    TIM_QEPInitStruct.QEP_ZPC = ENABLE;                      // Z �źű������� QEP ������ʹ�� 
                    TIM_QEPInitStruct.QEP_IRQEna = DISABLE;                  // �ж�ʹ��
                    TIM_QEPInit(QEP0, &TIM_QEPInitStruct);                 
                    TIM_QEPCmd(QEP0, ENABLE);                                // QEP0ģ��ʹ�� 
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
  * <tr><td>2023��02��23�� <td>1.0     <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_QEPInit(TIM_QEPTypeDef *QEPx, TIM_QEPInitTypeDef *TIM_QEPInitStruct)
{
    uint32_t tempTimerCfg;

    QEPx->CFG = TIM_QEPInitStruct->QEP_Mode << 8;
    QEPx->TH = TIM_QEPInitStruct->QEP_TH;

    if (QEPx == QEP0)
    {
			  SYS_ModuleClockCmd(SYS_Module_TIMER2, ENABLE);
			  SYS_ModuleClockCmd(SYS_Module_QEP0, ENABLE);
			
        UTIMER2_FLT = TIM_QEPInitStruct->QEP_Filter & 0x000F;	/*�˲�����ֻʹ�õ���λ*/
        tempTimerCfg = UTIMER2_CFG;
			  tempTimerCfg &= ~0x700000;
			  tempTimerCfg |= (TIM_QEPInitStruct->QEP_ClockDiv << 20);
			  UTIMER2_CFG  = tempTimerCfg;
           			
    }
    else if (QEPx == QEP1)
    {
			  SYS_ModuleClockCmd(SYS_Module_TIMER3, ENABLE);
			  SYS_ModuleClockCmd(SYS_Module_QEP1, ENABLE);
			
        UTIMER3_FLT = TIM_QEPInitStruct->QEP_Filter & 0x000F;	/*�˲�����ֻʹ�õ���λ*/
        tempTimerCfg = UTIMER3_CFG;
			  tempTimerCfg &= ~0x700000;
			  tempTimerCfg |= (TIM_QEPInitStruct->QEP_ClockDiv << 20);
			  UTIMER3_CFG  = tempTimerCfg;
    }
		
		QEPx->CFG = (TIM_QEPInitStruct->QEP_FE_CNT_EN)<<10 | (TIM_QEPInitStruct->QEP_Mode)<<8
			            | (TIM_QEPInitStruct->QEP_ZNC )<<3 | (TIM_QEPInitStruct->QEP_ZPC)<<2
                  | (TIM_QEPInitStruct->QEP_ZLC )<<1 | (TIM_QEPInitStruct->QEP_ZEC);
		QEPx->TH = TIM_QEPInitStruct->QEP_TH;
		
    QEPx->IE  = TIM_QEPInitStruct->QEP_IRQEna;
}

/**
 *@brief @b ��������:   void TIM_QEPStructInit(TIM_QEPInitTypeDef *TIM_QEPInitStruct)
 *@brief @b ��������:   �������ṹ���ʼ��  
 *@see���������ݣ�      ��
 *@param���������      TIM_QEPInitTypeDef *TIM_QEPInitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                    TIM_QEPInitTypeDef TIM_QEPInitStruct;
                    TIM_QEPStructInit(&TIM_QEPInitStruct);//�������ṹ���ʼ��                               
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_QEPStructInit(TIM_QEPInitTypeDef *TIM_QEPInitStruct)
{
    TIM_QEPInitStruct->QEP_Mode = QEP_Mode_T1;
    TIM_QEPInitStruct->QEP_ClockDiv = 0;
	TIM_QEPInitStruct->QEP_FE_CNT_EN = 0;
	TIM_QEPInitStruct->QEP_TH = 0;
	TIM_QEPInitStruct->QEP_ZEC = 0;
	TIM_QEPInitStruct->QEP_ZLC = 0;
	TIM_QEPInitStruct->QEP_ZNC = 0;
	TIM_QEPInitStruct->QEP_ZPC =0;
    TIM_QEPInitStruct->QEP_Filter = 0;
    TIM_QEPInitStruct->QEP_IRQEna = 0;
}

/**
 *@brief @b ��������:   void TIM_QEPCmd(TIM_QEPTypeDef *QEPx, FuncState state)
 *@brief @b ��������:   ������QEPxʹ�ܺ�ֹͣ
 *@see���������ݣ�      ��
 *@param���������      QEPx��QEP0/QEP1 \n
                       FuncState state��ʹ����ʧ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
			    TIM_QEPCmd(ECD0, ENABLE); // ECD0ģ��ʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
void TIM_QEPCmd(TIM_QEPTypeDef *QEPx, FuncState state)
{
    if (state != DISABLE)
    {
        QEPx ->CFG |= BIT15;
    }
    else
    {
        QEPx ->CFG &= ~BIT15;
    }
}

/**
 *@brief @b ��������:   uint16_t TIM_QEP_GetCount(TIM_QEPTypeDef *QEPx)
 *@brief @b ��������:   ����������ֵ
 *@see���������ݣ�       ��
 *@param���������       QEPx��QEP0/QEP1 \n
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			          uint32_t QEP0_Value = 0;
                QEP0_Value = TIM_QEP_GetCount(QEP0); //��ȡ������0����ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
uint16_t TIM_QEP_GetCount(TIM_QEPTypeDef *QEPx)
{
    return QEPx->CNT;
}

/**
 *@brief @b ��������:   void QEP_ClearIRQFlag(TIM_QEPTypeDef *QEPx,uint32_t tempFlag)
 *@brief @b ��������:   ���QEP�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       QEPx��QEP0/QEP1 \n
                        tempFlag��
 * <table>              <tr><th>QEP_IF_UNDERFLOW   <td>Encoder�����ж�
                        <tr><th>QEP_IF_OVERFLOW    <td>Encoder�����ж�                 
 * </table> 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			          if(QEP_GetIRQFlag(QEP0,QEP_IF_OVERFLOW)) //��ȡ�����������жϱ�־
                {
                    QEP_ClearIRQFlag(QEP0,QEP_IF_OVERFLOW);//��������������жϱ�־
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
void QEP_ClearIRQFlag(TIM_QEPTypeDef *QEPx,uint32_t tempFlag)
{
    QEPx->IF = tempFlag;
}

/**
 *@brief @b ��������:   u16 QEP_GetIRQFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x)
 *@brief @b ��������:   ��ȡQEP�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       QEPx��QEP0/QEP1 \n
                        QEP_IF_x��
 * <table>              <tr><th>QEP_IF_UNDERFLOW   <td>Encoder�����ж�
                        <tr><th>QEP_IF_OVERFLOW    <td>Encoder�����ж�                 
 * </table> 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
			          if(QEP_GetIRQFlag(QEP0,QEP_IF_OVERFLOW)) //��ȡ�����жϱ�־
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
u16 QEP_GetIRQFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x)
{
    if (QEPx->IF & QEPx->IE & QEP_IF_x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/**
 *@brief @b ��������:   u16 QEP_GetFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x)
 *@brief @b ��������:   ��ȡQEP�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       QEPx��QEP0/QEP1 \n
                        QEP_IF_x��
 * <table>              <tr><th>QEP_IF_UNDERFLOW   <td>Encoder�����ж�
                        <tr><th>QEP_IF_OVERFLOW    <td>Encoder�����ж�                 
 * </table> 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ����Ҫ��Ӧ�ж�ʹ�ܣ�Ҳ����������ȡ�жϱ�־λ
 *@par ʾ�����룺
 *@code
			          if(QEP_GetFlag(QEP0,QEP_IF_OVERFLOW)) //��ȡ�����жϱ�־
                {
                }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>HuangMG        <td>�޸�
 * </table>
 */
u16 QEP_GetFlag(TIM_QEPTypeDef *QEPx, u32 QEP_IF_x)
{
    if (QEPx->IF & QEP_IF_x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


