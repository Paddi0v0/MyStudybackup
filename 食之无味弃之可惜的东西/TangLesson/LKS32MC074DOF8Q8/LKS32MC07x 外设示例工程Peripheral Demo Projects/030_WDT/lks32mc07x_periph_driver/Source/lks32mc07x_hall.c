/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_hall.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� HALL������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0     <td>Olive Wang      <td>����
 * </table>
 */
 #include "lks32mc07x_hall.h"
 #include "lks32mc07x_lib.h"
/**
 *@brief @b ��������:   void HALL_Init(HALL_InitTypeDef* HALL_InitStruct)
 *@brief @b ��������:   HALL��ʼ������
 *@see���������ݣ�      ��
 *@param���������      HALL_InitTypeDef HALL_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code
	  HALL_InitTypeDef HALL_InitStruct;
   
    HALL_StructInit(&HALL_InitStruct);
	
    HALL_InitStruct.FilterLen = 512;                //Hall�ź������˲����� 512��ʱ������ 
    HALL_InitStruct.ClockDivision = HALL_CLK_DIV8;   //����Hallģ��ʱ�ӷ�Ƶϵ�� 
    HALL_InitStruct.Filter75_Ena = DISABLE;         // Hall�ź��˲���ʽ��7��5ģʽ����ȫ1��Чģʽ 
    HALL_InitStruct.HALL_Ena = ENABLE;              // ģ��ʹ�� 
    HALL_InitStruct.Capture_IRQ_Ena = ENABLE;       // ��׽�ж�ʹ�� 
    HALL_InitStruct.OverFlow_IRQ_Ena = ENABLE;      // ��ʱ�ж�ʹ�� 
    HALL_InitStruct.CountTH = 960000;              // Hallģ�����ģֵ����������ģֵ�������ʱ�ж� 
		HALL_InitStruct.softIE = ENABLE;								// ����ж�ʧ�� 
    HALL_Init(&HALL_InitStruct);
		
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��23�� <td>1.0       <td>Olive Wang           <td>����
 * </table>
 */void HALL_Init(HALL_InitTypeDef* HALL_InitStruct)
{
    HALL->CFG = (HALL_InitStruct->FilterLen) | (HALL_InitStruct->ClockDivision << 16)
                | (HALL_InitStruct->Filter75_Ena << 20) | (HALL_InitStruct->HALL_Ena << 24)
                | (HALL_InitStruct->Capture_IRQ_Ena << 28) | (HALL_InitStruct->OverFlow_IRQ_Ena << 29)
                | (HALL_InitStruct->softIE << 30);
    HALL->TH = HALL_InitStruct->CountTH;
    HALL->INFO = 0;
}

/**
 *@brief @b ��������:   void HALL_StructInit(HALL_InitTypeDef* HALL_InitStruct)
 *@brief @b ��������:   HALL�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       HALL_InitTypeDef  HALL_InitStruct
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    HALL_InitTypeDef HALL_InitStruct;
            HALL_StructInit(&HALL_InitStruct); //��ʼ���ṹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>Olive Wang     <td>����
 * </table>
 */
void HALL_StructInit(HALL_InitTypeDef* HALL_InitStruct)
{
    HALL_InitStruct->FilterLen = 1023;
    HALL_InitStruct->ClockDivision = HALL_CLK_DIV1;
    HALL_InitStruct->Filter75_Ena = ENABLE;
    HALL_InitStruct->HALL_Ena = ENABLE;
    HALL_InitStruct->Capture_IRQ_Ena = ENABLE;
    HALL_InitStruct->OverFlow_IRQ_Ena = DISABLE;
    HALL_InitStruct->CountTH = 1000;
    HALL_InitStruct->softIE = DISABLE;
}


/**
 *@brief @b ��������:   uint32_t HALL_GetFilterValue(void)
 *@brief @b ��������:   ȡ��HALLֵ���˲����
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      �����˲���HALL�źŽ��ֵ��ǰ��λ��Ч������λ��Ϊ0
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    uint32_t HALL_Value = 0;
            HALL_Value = HALL_GetFilterValue();//��ȡ�����˲���HALL���ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>Olive Wang     <td>����
 * </table>
 */
uint32_t HALL_GetFilterValue(void)
{
    return (HALL->INFO & 0x07);
}

/**
 *@brief @b ��������:   uint32_t HALL_GetCaptureValue(void)
 *@brief @b ��������:   ȡ��HALLֵ��δ�˲��������������ԭʼHALL״ֵ̬
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      δ�˲���HALL�źŽ��ֵ��ǰ��λ��Ч������λ��Ϊ0
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    uint32_t HALL_Value = 0;
            HALL_Value = HALL_GetCaptureValue();//��ȡδ�˲���HALL���ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>Olive Wang     <td>����
 * </table>
 */
uint32_t HALL_GetCaptureValue(void)
{
    return (HALL->INFO >> 8) & 0x07;
}

/**
 *@brief @b ��������:   uint32_t HALL_GetCount(void)
 *@brief @b ��������:   ��ȡʵʱHALL������ֵ
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ʵʱHALL������ֵ
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    uint32_t HALL_Value = 0;
            HALL_Value = HALL_GetCount();//��ȡʵʱHALL������ֵ
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0      <td>Olive Wang     <td>����
 * </table>
 */
uint32_t HALL_GetCount(void)
{
    return HALL->CNT;
}

/**
 *@brief @b ��������:   uint32_t HALL_IsCaptureEvent(void)
 *@brief @b ��������:   ��ȡ�Ƿ��������¼���־
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��     �����¼���־��ΪHALL_CAPTURE_EVENT����Ӧ��־λ��λ��Ϊ0����Ӧ��־λδ��λ
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    if(HALL_CAPTURE_EVENT == HALL_IsCaptureEvent())//��ȡHALL�źű仯��־λ
            {    
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */
uint32_t HALL_IsCaptureEvent(void)
{
    return (HALL->INFO & HALL_CAPTURE_EVENT);
}

/**
 *@brief @b ��������:   uint32_t HALL_IsCaptureEvent(void)
 *@brief @b ��������:   ��ȡ�Ƿ�����ʱ����¼���־
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��     �����¼���־��ΪHALL_OVERFLOW_EVENT����Ӧ��־λ��λ��Ϊ0����Ӧ��־λδ��λ
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    if( HALL_OVERFLOW_EVENT == HALL_IsOverFlowEvent())//��ȡ��ʱ�����־λ
            {    
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */
uint32_t HALL_IsOverFlowEvent(void)
{
    return (HALL->INFO & HALL_OVERFLOW_EVENT);
}

/**
 *@brief @b ��������:   uint32_t HALL_ClearIRQFlag(uint32_t tempFlag)
 *@brief @b ��������:   ���HALL�жϱ�־λ
 *@see���������ݣ�       ��
 *@param���������       
                        tempFlag: \n
 * <table>              <tr><th>HALL_CAPTURE_EVENT   <td>HALL�źű仯�ж� \n
 *                      <tr><th>HALL_OVERFLOW_EVENT  <td>HALL��������ж�
 * </table>
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    if(HALL_GetIRQFlag(HALL_CAPTURE_EVENT))//��ȡHALL�źű仯�жϱ�־λ
            {    
                HALL_ClearIRQFlag(HALL_CAPTURE_EVENT);//���HALL�źű仯�жϱ�־λ
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */void HALL_Clear_IRQ(uint32_t tempFlag)
{
    uint32_t temp = HALL->INFO & tempFlag;
    HALL->INFO = temp;
}
/**
 *@brief @b ��������:   uint32_t HALL_GetIRQFlag(uint32_t tempFlag)
 *@brief @b ��������:   ��ȡHALL�жϱ�־
 *@see���������ݣ�       ��
 *@param���������       
                        tempFlag: \n
 * <table>              <tr><th>HALL_CAPTURE_EVENT   <td>HALL�źű仯�ж� \n
 *                      <tr><th>HALL_OVERFLOW_EVENT  <td>HALL��������ж�
 * </table>
 *@param���������       ��
 *@return�� �� ֵ��      HALL�жϱ�־ ΪHALL_CAPTURE_EVENT����Ӧ��־λ��λ��Ϊ0����Ӧ��־λδ��λ
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
		    if(HALL_CAPTURE_EVENT == HALL_GetIRQFlag(HALL_CAPTURE_EVENT))//��ȡHALL�źű仯�жϱ�־λ
            {    
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */

uint32_t HALL_GetIRQFlag(uint32_t tempFlag)
{
	return (HALL->INFO&tempFlag);
}

/**
 *@brief @b ��������:   uint32_t HALL_SoftTrigger(void)
 *@brief @b ��������:   �������HALL�źű仯�ж�
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
							HALL_SoftTrigger();//�������HALL�źű仯�ж�
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */

void HALL_SoftTrigger(void)
{
    HALL->INFO |= BIT18;
}
/**
 *@brief @b ��������:  void HALL_Cmd(FuncState state)
 *@brief @b ��������:   HALLʱ��ʹ��
 *@see���������ݣ�       ��
 *@param���������       
                        state: \n
 * <table>              <tr><th>ENABLE   <td>HALLʹ�� \n
 *                      <tr><th>DISABLE  <td>HALLʧ��
 * </table>
 *@param���������       ��
 *@return�� �� ֵ��     ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
								HALL_Cmd(ENABLE);//HALLʹ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23��  <td>1.0     <td>Olive Wang     <td>����
 * </table>
 */
void HALL_Cmd(FuncState state)
{
    SYS_ModuleClockCmd(SYS_Module_HALL0,state);   //HALLʱ��ʹ��
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
