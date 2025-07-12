/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_sys.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� SYS������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��05��21�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */


#include "lks32mc08x_sys.h"

/**
 * @brief �͹��ĳ�ʼ���ṹ�嶨��
 * 
 */
WAKE_InitTypeDef WAKE_InitStruct;
/**
 *@brief @b ��������:   void SYS_Init(SYS_InitTypeDef* SYS_InitStruct)
 *@brief @b ��������:   SYSģ���ʼ������
 *@see�����ú�����      ��
 *@param���������      SYS_InitTypeDef* SYS_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       һ�㲻ʹ�øú�������Ϊ�漰ģ��϶࣬�Ƚ����ң���Ӧ����������ڸ�ģ��
 *                     ��ʼ���������á�
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_InitTypeDef SYS_InitStruct;
           SYS_StructInit(&SYS_InitStruct);
           SYS_InitStruct.PLL_SrcSel = SYS_PLLSRSEL_RCH;    // ʹ���ڲ�4MHz������Ϊʱ������Դ
           SYS_Init(&SYS_InitStruct);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
void SYS_Init(SYS_InitTypeDef *SYS_InitStruct)
{
    uint32_t RCHPd, XTALPdn;
    uint32_t tmp1, tmp2;
    SYS_WR_PROTECT = 0x7a83; /*�����Ĵ���дʹ��*/
    tmp1 = SYS_AFE_REG6;
    tmp1 &= 0x7FFF;
    tmp1 |= (SYS_InitStruct->PLL_SrcSel << 15);
    SYS_AFE_REG6 = tmp1;

    if (SYS_InitStruct->PLL_SrcSel == SYS_PLLSRSEL_CRYSTAL)
    {
        RCHPd = ENABLE;   /*RCH(4MHz)ʱ�ӹر�*/
        XTALPdn = ENABLE; /*���������·����*/
    }
    else 
    {
        RCHPd = DISABLE;   /*RCH(4MHz)ʱ�Ӳ��ر�*/
        XTALPdn = DISABLE; /*���������·������*/
    }

    tmp2 = SYS_AFE_REG5;
    tmp2 &= 0xBBFF;
    tmp2 |= (RCHPd << 10);
    tmp2 |= (XTALPdn << 14);
    SYS_AFE_REG5 = tmp2;

    SYS_CLK_CFG = SYS_InitStruct->PLL_DivSel + (SYS_InitStruct->PLL_ReDiv << 8);
    SYS_RST_CFG = SYS_InitStruct->WDT_Ena | (SYS_InitStruct->PORFilter_Ena << 1);

    SYS_CLK_DIV0 = SYS_InitStruct->Clk_DivSPI;
    SYS_CLK_DIV2 = SYS_InitStruct->Clk_DivUART;
    SYS_CLK_FEN = SYS_InitStruct->Clk_FEN;
    SYS_WR_PROTECT = 0x0;
}


/**
 *@brief @b ��������:   void SYS_StructInit(SYS_InitTypeDef* SYS_InitStruct)
 *@brief @b ��������:   SYS�ṹ���ʼ��
 *@see�����ú�����      ��
 *@param���������      SYS_InitTypeDef* SYS_InitStruct
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��   
 *@par ʾ�����룺
 *@code    
           SYS_InitTypeDef SYS_InitStruct;
           SYS_StructInit(&SYS_InitStruct);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
void SYS_StructInit(SYS_InitTypeDef *SYS_InitStruct)
{
    SYS_InitStruct->PLL_SrcSel = SYS_PLLSRSEL_RCH;
    SYS_InitStruct->PLL_DivSel = 0xFF;
    SYS_InitStruct->PLL_ReDiv = SYS_PLLREDIV_1;

    SYS_InitStruct->Clk_DivSPI = SYS_Clk_SPIDiv1;
    SYS_InitStruct->Clk_DivUART = SYS_Clk_UARTDiv1;
    SYS_InitStruct->Clk_FEN = 0;

    SYS_InitStruct->WDT_Ena = DISABLE;
    SYS_InitStruct->PORFilter_Ena = DISABLE;
}

/**
 *@brief @b ��������:   void SYS_ClearRst(void)
 *@brief @b ��������:   SYS�����λ��־��¼
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��ע�����ڸ�λ��¼�����ڵ���ʱ�������ִ�������Ҫһ��ʱ�䣬��Ӧ��
                       ������������¼״̬ 
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           u32 RSET_VALUE = 0;
           SYS_ClearRst();//�����λ��־��¼
           delay_ms(1);
           RSET_VALUE = SYS_GetRstSource();//��ȡ��λԴ��¼ֵ
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
void SYS_ClearRst(void)
{
	  SYS_WR_PROTECT = 0x7a83; /*�����Ĵ���дʹ��*/
      SYS_CLR_RST = 0xDEAD;
	  SYS_WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:   uint32_t SYS_GetRstSource(void)
 *@brief @b ��������:   ���SYS��λԴ�ź�
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��λԴ��¼ֵ
 *@note����˵����       ����ֵλ����  
 *@warning             ��     
 * <table>
 *                     <tr><th>BIT3 <td>���Ź���λ������־<td> ����Ч
 *                     <tr><th>BIT2 <td>������λ������־  <td> ����Ч
 *                     <tr><th>BIT1 <td>HPOR ��λ������־ <td> ����Ч
 *                     <tr><th>BIT0 <td>LPOR ��λ������־ <td> ����Ч
 * </table>
 *@par ʾ�����룺
 *@code    
           u32 RSET_VALUE = 0;
           RSET_VALUE = SYS_GetRstSource();//��ȡ��λԴ��¼ֵ
           �� RSET_VALUE = 0x01;��LPOR������λ
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
uint32_t SYS_GetRstSource(void)
{
    return SYS_RST_SRC;
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
           SYS_FallSleep();//����ϵͳ��������״̬
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
void SYS_FallSleep(void)
{
    SYS_WR_PROTECT = 0x7a83; /*�����Ĵ���дʹ��*/
    SYS_CLK_SLP = 0xDEAD;
    SYS_WR_PROTECT = 0x0;
    __WFI(); /*PCָ��ֹͣ���У��ȴ��ж�*/
}

/**
 *@brief @b ��������:    void SYS_SetWakeInterval(uint32_t nInterval)
 *@brief @b ��������:    ����ϵͳ���߻�������
 *@see�����ú�����       ��
 *@param���������       nInterval������ʱ��
 * <table>
 *                      <tr><td> �궨��         <td>ԭʼֵ    <td>˵��
 *                      <tr><th>SYS_WakeInterval_025s  <td>0x00     <td>0.25s  
 *                      <tr><th>SYS_WakeInterval_05s   <td>0x01     <td>0.5s
 *                      <tr><th>SYS_WakeInterval_1s    <td>0x02     <td>1s
 *                      <tr><th>SYS_WakeInterval_2s    <td>0x03     <td>2s
 *                      <tr><th>SYS_WakeInterval_4s    <td>0x04     <td>4s
 *                      <tr><th>SYS_WakeInterval_8s    <td>0x05     <td>8s
 *                      <tr><th>SYS_WakeInterval_16s   <td>0x06     <td>16s
 *                      <tr><th>SYS_WakeInterval_32s   <td>0x07     <td>32s
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��   
 *@warning             ��    
 *@par ʾ�����룺
 *@code    
           SYS_SetWakeInterval(SYS_WakeInterval_1s);//ϵͳ����ʱ,����ϵͳ��1s����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��07��02�� <td>1.0     <td>cfwu      <td>����
 * </table>
 */
void SYS_SetWakeInterval(uint32_t nInterval)
{
    uint32_t tempReg = SYS_RST_CFG;
	SYS_WR_PROTECT = 0x7a83;        /*�����Ĵ���дʹ��*/
    tempReg |= (nInterval << 2);
    SYS_RST_CFG = tempReg;
	SYS_WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:    void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint8_t ena)
 *@brief @b ��������:    �������߻���IO
 *@see���ѵ�ƽѡ��ö�٣�  enumWakeTrigLevel 
 *@param���������                           
 * <table>              <tr><td>wakeIO = WAKEIO_P1_1: P1.1 �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0������ \n
 *                              wakeIO = WAKEIO_P1_0: P1.0 �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0������ \n
 *                              wakeIO = WAKEIO_P0_1: P0.1 �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0������ \n
 *                              wakeIO = WAKEIO_P0_0: P0.0 �ⲿ����ʹ�ܡ�1��ʹ�ܣ�0������ 
 *                      <tr><td>enumWakeTrigLevel : \n
 *                              ���ѵ�ƽѡ��\n 
 *                              0:�͵�ƽ���� \n
 *                              1:�ߵ�ƽ���� 
 *                      <tr><td>ena: \n
 *                              ENABLE�� ����IO����\n
 *                              DISABLE���ر�IO����\n
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��    
 *@warning             �� 
 *@par ʾ�����룺
 *@code    
           SetWakeIO(WAKEIO_P0_0,WAKETRIG_LOW,ENABLE);//ϵͳ����ʱ,ʹ��P0.0��Ϊ�͹��Ļ���IO��
           �͵�ƽ����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2022��04��14�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SetWakeIO(uint32_t wakeIO, enumWakeTrigLevel trigLevel, uint8_t ena)
{
    if (ena == ENABLE)
    {
        GPIO_WAKE_EN |= wakeIO;
    }
    else
    {
        GPIO_WAKE_EN &= ~wakeIO;
    }

    if (trigLevel == WAKETRIG_HIGH)
    {
        GPIO_WAKE_POL |= wakeIO;
    }
    else
    {
        GPIO_WAKE_POL &= ~wakeIO;
    }
}
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
 * <tr><td>2022��04��14�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
static void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;
    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
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
 * <tr><td>2022��04��14�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void Switch2PLL(void)
{
    SYS_WR_PROTECT = 0x7a83;        /*�����Ĵ���дʹ��*/
    SYS_AFE_REG5 = WAKE_InitStruct.AFE_REG5_RECORD; /*���� PLL,HRC,BGP*/
    SoftDelay(200);
    SYS_AFE_REG6 = WAKE_InitStruct.AFE_REG6_RECORD; /*�����������·*/
    SYS_CLK_CFG = WAKE_InitStruct.CLK_CFG_RECORD;   /*����ʱ���л���PLLʱ��*/
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
 * <tr><td>2022��04��14�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void Switch2HRC(void)
{
    SYS_WR_PROTECT = 0x7a83; /*�����Ĵ���дʹ��*/
    WAKE_InitStruct.AFE_REG5_RECORD = SYS_AFE_REG5;
    WAKE_InitStruct.AFE_REG6_RECORD = SYS_AFE_REG6;
    WAKE_InitStruct.CLK_CFG_RECORD = SYS_CLK_CFG;
    SYS_CLK_CFG = 0;       /* �л���HRCʱ��*/
    SYS_AFE_REG5 = 0x0500; /* ׼���ر� PLL,HRC,BGP*/
    SYS_AFE_REG6 |= 1;     /* �رյ������·*/
    SYS_WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:   void SYS_WatchDogCmd(FuncState state)
 *@brief @b ��������:   ���Ź�ģ��ʹ�ܺ���
 *@see�����ú�����      ��
 *@param���������      state��ENABLEʹ�ܿ��Ź�/DISABLE�رտ��Ź�
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��   
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_WatchDogCmd(ENABLE);//ʹ�ܿ��Ź�ģ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void SYS_WatchDogCmd(FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //���д����

    if (state == ENABLE)
    {
        SYS_RST_CFG |= BIT0; //ʹ�ܿ��Ź�
    }
    else
    {
        SYS_RST_CFG &= (~BIT0); //�رտ��Ź�
    }
		SYS_WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:   void SYS_EnablePORFilter(void)
 *@brief @b ��������:   �����POR�����˲�����
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             @b �ú���Ŀǰ�����ţ��벻Ҫʹ��
 *@par ʾ�����룺
 *@code    
         SYS_EnablePORFilter();
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void SYS_EnablePORFilter(void)
{  
    uint32_t tempReg = SYS_RST_CFG;
	  SYS_WR_PROTECT = 0x7A83; //���ϵͳ�Ĵ���д����
    tempReg |= BIT1;
    SYS_RST_CFG = tempReg;
    SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
}

/**
 *@brief @b ��������:   void SYS_FeedDog(uint32_t timeOut)
 *@brief @b ��������:   ι��
 *@see�����ú�����      ��
 *@param���������      timeOut�����Ź���ʱʱ��
 * <table> 
 *                     <tr><th>SYS_WD_TimeOut2s  <td>0    <td>2s��λ
 *                     <tr><th>SYS_WD_TimeOut4s  <td>1    <td>4s��λ
 *                     <tr><th>SYS_WD_TimeOut8s  <td>2    <td>8s��λ
 *                     <tr><th>SYS_WD_TimeOut64s <td>3    <td>64s��λ
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        SYS_FeedDog(SYS_WD_TimeOut2s);//���Ź�ι����������һ�����2s��ι����оƬ��λ
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void SYS_FeedDog(uint32_t timeOut)
{
    SYS_WR_PROTECT = 0xCAFE; // 0xCAFE;

    if (timeOut == (uint32_t)SYS_WD_TimeOut64s)
    {
        SYS_WDT_CLR = 0x7989; // 1 00 1
    }
    else if (timeOut == (uint32_t)SYS_WD_TimeOut8s)
    {
        SYS_WDT_CLR = 0x798B; // 1 01 1
    }
    else if (timeOut == (uint32_t)SYS_WD_TimeOut4s)
    {
        SYS_WDT_CLR = 0x798D; // 1 10 1
    }
    else // SYS_WD_TimeOut2s
    {
        SYS_WDT_CLR = 0x798F; // 1 11 1
    }

    SYS_WR_PROTECT = 0;
}

/**
 *@brief @b ��������:   void SYS_ModuleClockCmd(uint32_t nModule, FuncState state)
 *@brief @b ��������:   ����ģ��ʱ��ʹ�ܺ�ֹͣ
 *@see������ö�٣�      FuncState
 *@param���������   
 *                     state�� ENABLEʹ�� , DISABLE�ر�     
 *                     nModule��������ѡ�� \n 
 * <table> 
 *                     <tr><td> �궨��                   <td>ԭʼֵ   <td>˵��
 *                     <tr><th> SYS_Module_I2C          <td> BIT0    <td>I2Cģ��
 *                     <tr><th> SYS_Module_HALL         <td> BIT1    <td>HALLģ��
 *                     <tr><th> SYS_Module_TIMER        <td> BIT2    <td>UTIMERģ��
 *                     <tr><th> SYS_Module_MCPWM        <td> BIT3    <td>MCPWMģ��
 *                     <tr><th> SYS_Module_UART0        <td> BIT4    <td>UART0ģ��
 *                     <tr><th> SYS_Module_UART1        <td> BIT5    <td>UART1ģ��
 *                     <tr><th> SYS_Module_DSP          <td> BIT6    <td>DSPģ��
 *                     <tr><th> SYS_Module_CAN          <td> BIT7    <td>CANģ��
 *                     <tr><th> SYS_Module_SPI          <td> BIT8    <td>SPIģ��                  
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        SYS_ModuleClockCmd(SYS_Module_SPI,ENABLE);//ʹ��SPIʱ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void SYS_ModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //���ϵͳ�Ĵ���д����

    if (state != DISABLE)
    {
        SYS_CLK_FEN |= nModule;
    }
    else
    {
        SYS_CLK_FEN &= ~nModule;
    }
		
	  SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
}

/**
 *@brief @b ��������:   void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state)
 *@brief @b ��������:   ģ��ģ��ʹ�ܺ�ֹͣ
 *@see������ö�٣�      FuncState
 *@param���������      
 *                     state�� ENABLEʹ�� , DISABLE�ر�  
 *                     nModule��������ѡ�� \n 
 * <table> 
 *                     <tr><td> �궨��                  <td>˵��
 *                     <tr><th> SYS_AnalogModule_ADC    <td>  BIT0  <td> ADCģ��  
 *                     <tr><th> SYS_AnalogModule_OPA0   <td>  BIT2  <td> OPA0ģ�� 
 *                     <tr><th> SYS_AnalogModule_OPA1   <td> BIT3   <td> OPA1ģ�� 
 *                     <tr><th> SYS_AnalogModule_OPA2   <td>  BIT4  <td> OPA2ģ�� 
 *                     <tr><th> SYS_AnalogModule_OPA3   <td>  BIT5  <td> OPA3ģ��
 *                     <tr><th> SYS_AnalogModule_CMP0   <td>  BIT6  <td> CMP0ģ�� 
 *                     <tr><th> SYS_AnalogModule_CMP1   <td>  BIT7  <td> CMP1ģ�� 
 *                     <tr><th> SYS_AnalogModule_BGP    <td>  BIT8  <td> BGPģ��  
 *                     <tr><th> SYS_AnalogModule_RCH    <td>  BIT10 <td> RCHģ��  
 *                     <tr><th> SYS_AnalogModule_DAC    <td>  BIT12 <td> DACģ��  
 *                     <tr><th> SYS_AnalogModule_TMP    <td>  BIT13 <td> �¶ȴ����� 
 *                     <tr><th> SYS_AnalogModule_XTAL   <td>  BIT14 <td> ���� 
 *                     <tr><th> SYS_AnalogModule_PLL    <td>  BIT15 <td> PLLģ��                 
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        SYS_AnalogModuleClockCmd(SYS_AnalogModule_PLL,ENABLE);//ʹ��PLLģ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS_WR_PROTECT = 0x7A83; //���ϵͳ�Ĵ���д����

    if (state != DISABLE)
    {
        SYS_AFE_REG5 |= nModule;
    }
    else
    {
        SYS_AFE_REG5 &= ~nModule;
    }
		SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
}

/**
 *@brief @b ��������:   void SYS_SoftResetModule(uint32_t nModule)
 *@brief @b ��������:   ����ģ����λ
 *@see������ö�٣�      ��
 *@param���������   
 *                     state�� ENABLEʹ�� , DISABLE�ر�     
 *                     nModule��������ѡ�� \n 
 * <table> 
 *                     <tr><td> �궨��                   <td>ԭʼֵ   <td>˵��
 *                     <tr><th> SYS_Module_I2C          <td> BIT0    <td>I2Cģ��
 *                     <tr><th> SYS_Module_HALL         <td> BIT1    <td>HALLģ��
 *                     <tr><th> SYS_Module_TIMER        <td> BIT2    <td>UTIMERģ��
 *                     <tr><th> SYS_Module_MCPWM        <td> BIT3    <td>MCPWMģ��
 *                     <tr><th> SYS_Module_UART0        <td> BIT4    <td>UART0ģ��
 *                     <tr><th> SYS_Module_UART1        <td> BIT5    <td>UART1ģ��
 *                     <tr><th> SYS_Module_DSP          <td> BIT6    <td>DSPģ��
 *                     <tr><th> SYS_Module_CAN          <td> BIT7    <td>CANģ��
 *                     <tr><th> SYS_Module_SPI          <td> BIT8    <td>SPIģ��              
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        //��λSPIģ�飬���SPI�Ĵ������ã�PCָ�븴λ
        SYS_SoftResetModule(SYS_Module_SPI);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
*@par �޸���־:������ʱ�������ȴ��Ĵ�����λ������ͷŸ�λ������
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2022��04��14�� <td>1.1     <td>HuangMG      <td>�޸�
 * </table>
 */
void SYS_SoftResetModule(uint32_t nModule)
{
    SYS_WR_PROTECT = 0x7A83;
    SYS_SFT_RST = nModule;     /*ģ�鸴λ*/
    SYS_SFT_RST &= ~nModule;   /*�ͷŸ�λ����*/
	  SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
}

/**
 *@brief @b ��������:   void DSP_Init(void)
 *@brief @b ��������:   DSPʱ��ʹ��
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
         DSP_Init();//DSPʱ��ʹ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2016��06��28�� <td>1.0     <td>cfwu         <td>����
 * </table>
 */
void DSP_Init(void)
{
    SYS_ModuleClockCmd(SYS_Module_DSP, ENABLE); /* DSPʱ��ʹ��*/
}


/**
 *@brief @b ��������:   void SYS_VolSelModule(uint32_t Vol)
 *@brief @b ��������:   MCU�����ѹ�ȶ��жϺ���
 *@see�����ú�����      ��
 *@param���������      Vol��0 3.3 Voltage��1 5.0 Voltage
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��     
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_VolSelModule(MCU_POWER_5v0);//MCU����ϵͳΪ5Vϵͳ
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��10��19�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_VolSelModule(uint32_t Vol)
{
	if(Vol == MCU_POWER_5v0)  // 5.0 Voltage
	 {
		 SYS_WR_PROTECT = 0x7a83;       /* ���ϵͳ�Ĵ���д���� */
		 SYS_AFE_REG6 &=~(BIT8|BIT9);  //main init set to 3.6v, this clause reset to 4.5v 
		 SoftDelay(100);              /* ��ʱ100us*/
		 SYS_AFE_REG6 |= POWER_4V2 << 8;//����Ϊ4.2V��أ��ɸ���ʵ������������λ��3.6V/3.9V/4.2V/4.5V
		 SoftDelay(100);              /* ��ʱ100us, �ȴ���Դ����������*/
		 while(SYS_AFE_CMP & BIT13){};  //�ȴ��ⲿ5VOK
		 SYS_AFE_REG6 &=~(BIT8|BIT9);  //reset to 4.5V
		 SoftDelay(100);             /* ��ʱ100us*/
		 SYS_AFE_REG6 |= POWER_3V6<< 8; //����Ϊ3.6V����ֹ��Դ��Ƿѹ�ȽϵĶ���
		 SoftDelay(100);             /* ��ʱ100us*/
		 NVIC_ClearPendingIRQ(PWRDN_IRQn);
		 SYS_WR_PROTECT = 0x0;          /*�ر�ϵͳ�Ĵ���д����*/
		}
}

/**
 *@brief @b ��������:   void SYS_VolSelModuleIRQ(void)
 *@brief @b ��������:   MCU�����ѹ�ȶ��ж��жϺ���
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��     
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_VolSelModuleIRQ();
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��10��19�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_VolSelModuleIRQ(void)
{
		NVIC_DisableIRQ(PWRDN_IRQn);       /* �رյ�Դ����ж� */ 
		NVIC_ClearPendingIRQ(PWRDN_IRQn);
		//turn off pwm
		MCPWM_PRT   = 0xDEAD;
		MCPWM_FAIL &= ~BIT6 ;
		MCPWM_TCLK &=~0x0000000C;
		MCPWM_PRT   = 0x0000;
		SYS_WR_PROTECT = 0x7A83;   /* ���ϵͳ�Ĵ���д���� */
		SYS_SFT_RST = 0xff;        //�������踴λ 
		SYS_CLK_CFG &= ~BIT8;      //ϵͳʱ��ѡ��RCH
		SYS_AFE_REG5 &= ~BIT15;    //�ر�PLL
		SYS_SFT_RST = 0x0;         // �ͷ����踴λ
		SYS_WR_PROTECT = 0x0;      /*�ر�ϵͳ�Ĵ���д����*/
		NVIC_SystemReset();//��λ 

}

/**
 *@brief @b ��������:   void SYS_VolSelModuleEnableIRQ(uint32_t Vol)
 *@brief @b ��������:   MCU�����ѹ�ȶ��ж��ж�ʹ�ܺ���
 *@see�����ú�����      ��
 *@param���������      ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��     
 *@warning             ��
 *@par ʾ�����룺
 *@code    
           SYS_VolSelModuleIRQ(MCU_POWER_5v0);
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��10��19�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */
void SYS_VolSelModuleEnableIRQ(uint32_t Vol)
{
     if(Vol == MCU_POWER_5v0)
     {
       NVIC_SetPriority(PWRDN_IRQn, 0);  /*PWRDN_IRQn�ж����ȼ�����Ϊ0*/
       NVIC_EnableIRQ(PWRDN_IRQn);       /* �򿪵�Դ����ж� */
     }else{ 

       NVIC_DisableIRQ(PWRDN_IRQn);       /* �رյ�Դ����ж� */ 
     }
}




/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
