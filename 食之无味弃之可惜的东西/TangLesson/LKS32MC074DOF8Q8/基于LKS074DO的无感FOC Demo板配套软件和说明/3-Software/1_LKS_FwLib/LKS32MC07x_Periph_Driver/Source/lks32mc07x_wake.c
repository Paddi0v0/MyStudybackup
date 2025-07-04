/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_wake.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� wake������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��23�� <td>1.0    <td>HuangMG    <td>����
 * </table>
 */

#include "lks32mc07x_wake.h"
#include "lks32mc07x.h"


/**
 * @brief �͹��ĳ�ʼ���ṹ�嶨��
 * 
 */
WAKE_InitTypeDef WAKE_InitStruct;

/**
 *@brief @b ��������:   static void SoftDelay(u32 cnt)
 *@brief @b ��������:   ��ʱ����
 *@see�����ú�����      ��
 *@param���������      cnt����ʱcnt�Ŀ�ָ��_NOP()
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       �ĺ���ֻ�ڴ��ļ�ʹ�ã���Ҫ����Switch2PLL������      
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SoftDelay(0xff);//��ʱ255��_NOP()ʱ��1��_NOP��Լ��һ��ϵͳʱ�����ڼ���
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
static void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;
    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __nop();
    }
}

/**
 *@brief @b ��������:   void Switch2PLL(void)
 *@brief @b ��������:   ���߻��Ѻ�ϵͳʱ���л���PLLʱ�Ӻ���
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ϵͳ�͹��Ļ���ʹ��  
 *@warning             �� 
 *@par ʾ�����룺
 *@code    
           Switch2PLL();
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void Switch2PLL(void)
{
	  SYS_WR_PROTECT = 0x7a83;                        /*�����Ĵ���дʹ��*/
    SYS_AFE_REG5 = WAKE_InitStruct.AFE_REG5_RECORD; /*���� PLL,BGP*/
    SoftDelay(200);
    SYS_CLK_CFG = WAKE_InitStruct.CLK_CFG_RECORD;   /*����ʱ���л���PLLʱ��*/
	  SoftDelay(200);
	  SYS_WR_PROTECT = 0x0; 	  
}

/**
 *@brief @b ��������:   void Switch2HRC(void)
 *@brief @b ��������:   ���߻���ǰһʱ�̣��رո���ʱ����BGP�͵������·����
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ϵͳ�͹�������ʹ��   
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           Switch2HRC();
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void Switch2HRC(void)
{
	  SYS_WR_PROTECT = 0x7a83; /*�����Ĵ���дʹ��*/
    WAKE_InitStruct.AFE_REG5_RECORD = SYS_AFE_REG0;
	  WAKE_InitStruct.CLK_CFG_RECORD = SYS_CLK_CFG;
	  SYS_CLK_CFG &= 0;         /* �л���HRCʱ��*/
	  SYS_AFE_REG5 = 0x0500;    /* ׼���ر� PLL,BGP,������ģ���·ʹ��*/  
	  SYS_WR_PROTECT = 0x0; 
}

/**
 *@brief @b ��������:    void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint32_t flt , uint8_t ena)
 *@brief @b ��������:    �������߻���IO
 *@see���ѵ�ƽѡ��ö�٣�  enumWakeTrigLevel 
 *@param���������                           
 * <table>              <tr><td>
                                wakeIO = BIT11: CLUOUT3_EN  CLUOUT3 ��Ϊ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT10: CLUOUT2_EN  CLUOUT2 ��Ϊ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT9:  CLUOUT1_EN  CLUOUT1 ��Ϊ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT8:  CLUOUT0_EN  CLUOUT0 ��Ϊ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT7:  GPIO 2[15] / P2[4]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT6:  GPIO 2[7]  / P2[4]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
                                wakeIO = BIT5:  GPIO 2[4]  / P2[4]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
 *                              wakeIO = BIT4:  GPIO 0[14] / P0[14] �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
 *                              wakeIO = BIT3:  GPIO 0[11] / P0[11] �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
 *                              wakeIO = BIT2:  GPIO 0[6]  / P0[6]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
 *                              wakeIO = BIT1:  GPIO 0[2]  / P0[2]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á� \n
 *                              wakeIO = BIT0:  GPIO 0[0]  / P0[0]  �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0�����á�
 *                      <tr><td>enumWakeTrigLevel : \n
 *                              ���ѵ�ƽѡ�� \n 
 *                              0:�͵�ƽ���� \n
 *                              1:�ߵ�ƽ���� 
 *                      <tr><td>flt: \n
 *                              ENABLE�� ����IO�˲� \n
 *                              DISABLE���ر�IO�˲� 
 *                      <tr><td>ena: \n
 *                              ENABLE�� ����IO���� \n
 *                              DISABLE���ر�IO���� 
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       �͹���IO����ʹ�ö����������IO�����ź�ʹ��ͬһ������ѡ��    
 *@warning             �� 
 *@par ʾ�����룺
 *@code    
           SetWakeIO(WAKEIO_P0_0,WAKETRIG_LOW,ENABLE,ENABLE);//ϵͳ����ʱ,ʹ��P0.0��Ϊ�͹��Ļ���IO��
           �͵�ƽ����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint32_t flt , uint8_t ena)
{
	if(ena == ENABLE)
	{
		AON_IO_WAKE_EN |= wakeIO;
	}
	else
	{
		AON_IO_WAKE_EN &= ~wakeIO;
	}
	
	if (trigLevel == WAKETRIG_HIGH)
	{
		AON_IO_WAKE_POL = 0x01;
	}
	else
	{
		AON_IO_WAKE_POL = 0x00;
	}
	
	if(flt == IOWK_FLT_EN)
	{
	  AON_PWR_CFG = BIT1;
	}else{
		AON_PWR_CFG = 0;
	}
}

/**
 *@brief @b ��������:   void SYS_FallSleep(void)
 *@brief @b ��������:   ����ϵͳ��������״̬
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       �� 
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_FallSleep();
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_FallSleep(void)
{
	IWDG_PSW = 0xA6B4;
	IWDG_CLR = 0x798D;
	SCB->SCR |= (1UL << 2);
	 __WFI();	  /*PCָ��ֹͣ���У��ȴ��ж�*/
}

/**
 *@brief @b ��������:   void SYS_EVTCLRSleep(void)
 *@brief @b ��������:   �������ȫ���¼���¼
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       �� 
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_EVTCLRSleep();
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_EVTCLRSleep(void)
{
	 AON_EVT_RCD = PSW_EVT_CLR;/* ���ȫ���¼���¼*/
}

/**
 *@brief @b ��������:    void SYS_SetWakeInterval(uint32_t rth , uint32_t wth , uint8_t ena)
 *@brief @b ��������:    �������߻�������
 *@see�����ú�����  ��
 *@param���������                           
 * <table>
 *                     <tr><th>rth <td>���Ź���ʱ��λ����ֵ��Ҳ������װ��ֵ(���ֵ0x1FF000)
 *                     <tr><th>wth <td>���Ź���ʱ��������ֵ(���ֵС��rth)
 *                     <tr><th>ena <td>ENABLE��������ʱ����  DISABLE���رն�ʱ����  
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       
 *               ����ʱ����㹫ʽ��
 *               WakeupIWDG = tLRC * (rth - Iwth) 
 *               tLRC Ϊ LRCʱ�����ڣ�1/32kHz=31.25uS
 *</table>
 *               <tr><th>rth - wth  * ��tLRC�� =  <td>����ʱ��
 *               <tr><th>   8000    * 31.25uS  =  <td>0.25S
 *               <tr><th>   16000   * 31.25uS  =  <td>0.5S
 *               <tr><th>   32000   * 31.25uS  =  <td>1S
 *               <tr><th>   64000   * 31.25uS  =  <td>2S
 *               <tr><th>   128000  * 31.25uS  =  <td>4S  
 *</table> 
 *@warning             �� 
 *@par ʾ�����룺
 *@code    
           SetWakeIO(WAKEIO_P0_0,WAKETRIG_LOW,ENABLE,ENABLE);//ϵͳ����ʱ,ʹ��P0.0��Ϊ�͹��Ļ���IO��
           �͵�ƽ����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��22�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_SetWakeInterval(uint32_t rth , uint32_t wth , uint8_t ena)
{ 
	 if(ena == ENABLE)
	 { 
	   IWDG_CFG |= 0x01 << 4; /*������߶�ʱ����ʹ��*/
	 }else{
		 IWDG_CFG &= ~(0x01 << 4); /*�ر�������߶�ʱ����*/
	 }
	  IWDG_PSW = 0xA6B4;     /*ʹ�ܿ��Ź����ּĴ���д����*/
	  IWDG_RTH = rth;
	  IWDG_WTH = wth;
}







