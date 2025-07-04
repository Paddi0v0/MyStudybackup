/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_sys.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� SYS������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */

#include "lks32mc07x_sys.h"

/**
 * @brief ȫ�ֱ����洢��ʱ��Ƶ��ֵ
 * 
 */
static u32 sys_clk = 96000000;

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
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */void SYS_Init(SYS_InitTypeDef* SYS_InitStruct)
{
    uint32_t RCHPd, XTALPdn;
    uint32_t tmp1, tmp2;

    tmp1 = AFE->REG6;
    tmp1 &= 0x7FFF;
    tmp1 |= (SYS_InitStruct->PLL_SrcSel << 15);
    AFE->REG6 = tmp1;

    if (SYS_InitStruct->PLL_SrcSel == SYS_PLLSRSEL_CRYSTAL)
    {
        RCHPd = DISABLE;         /*RCH(4MHz)ʱ�ӹر�*/
        XTALPdn = ENABLE;       /*���������·����*/
    }
    else
    {
        RCHPd = ENABLE;        /*RCH(4MHz)ʱ�Ӵ�*/
        XTALPdn = DISABLE;      /*���������·�ر�*/
    }

    tmp2 = AFE->REG5;
    tmp2 &= 0xBBFF; // 1010 1010 1111 1111
    tmp2 |= (RCHPd << 10);
    tmp2 |= (XTALPdn << 14);
    AFE->REG5 = tmp2;

    SYS->CLK_DIV0 = SYS_InitStruct->Clk_DivSPI;
    SYS->CLK_DIV2 = SYS_InitStruct->Clk_DivUART;
    SYS->CLK_FEN = SYS_InitStruct->Clk_FEN;
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
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_StructInit(SYS_InitTypeDef* SYS_InitStruct)
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
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_ClearRst(void)
{
      SYS->WR_PROTECT = 0x7a83;
      SYS->SFT_RST = 0x0;
      SYS->WR_PROTECT = 0;
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
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_ModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS->WR_PROTECT = 0x7A83;    //���ϵͳ�Ĵ���д����
    
    if (state == ENABLE)
    {
        SYS->CLK_FEN |= nModule;
    }
    else
    {
        SYS->CLK_FEN &= ~nModule;
    }
    SYS->WR_PROTECT = 0x0;    //ϵͳ�Ĵ���д����
    
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
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_AnalogModuleClockCmd(uint32_t nModule, FuncState state)
{
    SYS->WR_PROTECT = 0x7A83;    //���ϵͳ�Ĵ���д����
    
    if (state != DISABLE)
    {
        AFE->REG5 |= nModule;
    }
    else
    {
        AFE->REG5 &= ~nModule;
    }
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
*@par �޸���־:������ʱ�������ȴ��Ĵ�����λ������ͷŸ�λ������
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_SoftResetModule(uint32_t nModule)
{
    SYS->WR_PROTECT = 0x7A83;
    SYS->SFT_RST = nModule;
    SYS->WR_PROTECT = 1;
}

/**
 *@brief @b ��������:   void SYS_SoftReset_all(uint32_t state)
 *@brief @b ��������:   ����ģ����λ
 *@see������ö�٣�      ��
 *@param���������   
 *                     state�� ENABLEʹ�� , DISABLE�ر�     
 *                     nModule��������ѡ�� \n 
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        //��λ����ģ�飬����������Ĵ������ã�PCָ�븴λ
        SYS_SoftResetModule(ENABLE);
  @endcode    
*@par �޸���־:������ʱ�������ȴ��Ĵ�����λ������ͷŸ�λ������
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_SoftReset_all(uint32_t state)
{
	if(state == 1)
	{
    SYS->WR_PROTECT = 0x7A83;
    SYS->SFT_RST = 0xffffff;
    SYS->WR_PROTECT = 1;
	}
	else
	{
    SYS->WR_PROTECT = 0x7A83;
    SYS->SFT_RST = 0x0;
    SYS->WR_PROTECT = 1;
	}
}

/**
 *@brief @b ��������:   void SYS_MclkChoice(u8 SYS_MCLK_x)
 *@brief @b ��������:   ��ʱ��ѡ��
 *@see������ö�٣�      ��
 *@param���������      
 *                     nModule��������ѡ�� \n 
 * <table> 
 *                     <tr><td> �궨��                   <td>ԭʼֵ   <td>˵��
 * 							
 * 						<tr><th> SYS_MCLK_96M_RC			0x01	96MHz_PLL_RC
 *						<tr><th> SYS_MCLK_48M_RC			0x02	48MHz_PLL_RC
 *						<tr><th> SYS_MCLK_24M_RC    		0x03	24MHz_PLL_RC
 *						<tr><th> SYS_MCLK_12M_RC    		0x04	12MHz_PLL_RC
 *						<tr><th> SYS_MCLK_32K_RC    		0x05	32kHz_LSI
 *						<tr><th> SYS_MCLK_96M_XTAL          0x11	96MHz_PLL_XTAL
 *						<tr><th> SYS_MCLK_48M_XTAL          0x12	48MHz_PLL_XTAL
 *						<tr><th> SYS_MCLK_24M_XTAL          0x13	24MHz_PLL_XTAL
 *						<tr><th> SYS_MCLK_12M_XTAL          0x14	12MHz_PLL_XTAL
 * </table>
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
        //��λSPIģ�飬���SPI�Ĵ������ã�PCָ�븴λ
        SYS_MclkChoice(SYS_MCLK_96M_RC);
  @endcode    
*@par �޸���־:������ʱ�������ȴ��Ĵ�����λ������ͷŸ�λ������
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_MclkChoice(u8 SYS_MCLK_x)
{
	u32 reg = 0;
	SYS->WR_PROTECT = 0x7a83;
	switch(SYS_MCLK_x)
	{
		case SYS_MCLK_96M_RC:
		{
			AFE->REG5 |= (BIT15);  //��PLL RCH default open
			AFE->REG6 &= ~BIT15; // PLL��Դ���ڲ�8Mhz
			reg = (1 << 8) | (0xff);
			sys_clk = 96000000;
			break;
		}
		case SYS_MCLK_48M_RC:{
			AFE->REG5 |= (BIT15);  //��PLL RCH default open
			AFE->REG6 &= ~BIT15 ;// PLL��Դ���ڲ�8Mhz
			reg = (1 << 8) | (0x55);
			sys_clk = 48000000;
			break;
		}
		case SYS_MCLK_24M_RC:{
			AFE->REG5 |= (BIT15);//��PLL RCH default open
			AFE->REG6 &= ~BIT15 ;// PLL��Դ���ڲ�8Mhz
			reg = (1 << 8) | (0x11);
			sys_clk = 24000000;
			break;
		}
		case SYS_MCLK_12M_RC:{
			AFE->REG5 |= (BIT15);  //��PLL RCH default open
			AFE->REG6 &= ~BIT15 ;// PLL��Դ���ڲ�8Mhz
			reg = (2 << 8) | (0xFF);
			sys_clk = 12000000;
			break;
		}
		case SYS_MCLK_32K_RC:{
			AFE->REG5 |= (BIT15);  //��PLL RCH default open
			AFE->REG6 &= ~BIT15 ;// PLL��Դ���ڲ�8Mhz
			reg = (1 << 8) | (0x01); //main clk from LSI
			sys_clk = 12000000;
			break;
		}
		case SYS_MCLK_96M_XTAL:{
			AFE->REG5 |= (BIT15 | BIT14 | BIT10);  //��PLL ���񿪹�
			AFE->REG6 |= BIT15 ;// PLL��Դ���ⲿ����
			reg = (1 << 8) | (0xFF);
			sys_clk = 96000000;
			break;
		}
		case SYS_MCLK_48M_XTAL:{
			AFE->REG5 |= (BIT15 | BIT14 | BIT10);  //��PLL ���񿪹�
			AFE->REG6 |= BIT15 ;// PLL��Դ���ⲿ����
			reg = (1 << 8) | (0x55);
			sys_clk = 48000000;
			break;
		}
		case SYS_MCLK_24M_XTAL:{
			AFE->REG5 |= (BIT15 | BIT14 | BIT10);  //��PLL ���񿪹�
			AFE->REG6 |= BIT15 ;// PLL��Դ���ⲿ����
			reg = (1 << 8) | (0x11);
			sys_clk = 24000000;
			break;
		}
		case SYS_MCLK_12M_XTAL:{
			AFE->REG5 |= (BIT15 | BIT14 | BIT10);//��PLL ���񿪹�
			AFE->REG6 |= BIT15 ; // PLL��Դ���ⲿ����
			reg = (1 << 8) | (0x01);
			sys_clk = 12000000;
			break;
		}
	}
	SYS->CLK_CFG = reg;
	SYS->WR_PROTECT = 0x0;

}

/**
 *@brief @b ��������:  u32 SYS_ReadMcuClk(void)
 *@brief @b ��������:   ��ȡ��ǰʱ��
 *@see������ö�٣�      ��
 *@param���������   
 *@param���������      ��
 *@return�� �� ֵ��     ʱ��Ƶ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
  @endcode    
*@par �޸���־:������ʱ�������ȴ��Ĵ�����λ������ͷŸ�λ������
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
u32 SYS_ReadMcuClk(void)
{
    return sys_clk;
}


/**
 *@brief @b ��������:   void SYS_UartClkDiv(u16 div)
 *@brief @b ��������:   ���ڷ�Ƶ����
 *@see������ö�٣�      ��
 *@param���������   
 *                     div�� ��Ƶϵ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
  @endcode    
*@par �޸���־
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_UartClkDiv(u16 div)
{
    SYS->WR_PROTECT = 0x7a83;
    SYS->CLK_DIV2 = div;
    SYS->WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:   void SYS_I2CClkDiv(u16 div)
 *@brief @b ��������:   I2C��Ƶ����
 *@see������ö�٣�      ��
 *@param���������   
 *                     div�� ��Ƶϵ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
  @endcode    
*@par �޸���־
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_I2CClkDiv(u16 div)
{
    SYS->WR_PROTECT = 0x7a83;
    SYS->CLK_DIV0 = div;
    SYS->WR_PROTECT = 0x0;
}

/**
 *@brief @b ��������:   void SYS_SPIClkDiv(u16 div)
 *@brief @b ��������:   SPI��Ƶ����
 *@see������ö�٣�      ��
 *@param���������   
 *                     div�� ��Ƶϵ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
  @endcode    
*@par �޸���־
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author     <th>Description
 * <tr><td>2023��02��12�� <td>1.0     <td>ywx      <td>����
 * </table>
 */
void SYS_SPIClkDiv(u16 div)
{
    SYS->WR_PROTECT = 0x7a83;
    SYS->CLK_DIV0 = div;
    SYS->WR_PROTECT = 0x0;
}


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
            IWDG_StrutInit(&IWDG_InitStruct);
            IWDG_InitStruct.WDG_EN  = ENABLE;      //ʹ�ܿ��Ź�
            IWDG_InitStruct.RTH     = SECOND2IWDGCNT(2.0);//���ÿ��Ź�
            IWDG_InitStruct.DWK_EN  = DISABLE;     // ������߶�ʱ���ѹر�   
            IWDG_InitStruct.WTH     = 0;           // ���Ź���ʱ����ʱ�䣨21λ������������12��λ0��
            IWDG_Init(&IWDG_InitStruct);
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��28�� <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
void IWDG_Init(IWDG_InitTypeDef *this)
{
    this->RTH = (this->RTH > BIT21) ? BIT21 - 1 : this->RTH;
    this->WTH = (this->WTH > BIT21) ? BIT21 - 1 : this->WTH;
    IWDG->CFG = (this->DWK_EN<<4) | (this ->WDG_EN);
    IWDG->PSW = PSW_IWDG_PRE;
    IWDG->RTH = this->RTH;
    if(this->RTH > this->WTH)
    {
        IWDG->WTH = this->RTH-this->WTH;
    }
    else
    {
        IWDG->WTH = 0x001000;
    }
    IWDG->PSW = PSW_IWDG_PRE;
    IWDG->CLR = PSW_IWDG_CLR;
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
 * <tr><td>2023��02��28�� <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
void IWDG_StrutInit(IWDG_InitTypeDef *this)
{
    this->DWK_EN = DISABLE;             // ������߶�ʱ����ʹ��
    this->WDG_EN = DISABLE;             // �������Ź�ʹ��
    this->WTH = SECOND2IWDGCNT(1);      // ���Ź���ʱ����ʱ��,���ڸ�λʱ�����Ч
    this->RTH = SECOND2IWDGCNT(2);      // ���Ź���ʱ��λʱ�䣨21λ������������12��λ0��
                                        // SECOND2IWDGCNT�����������
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
 * <tr><td>2023��02��28�� <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
void IWDG_DISABLE(void)
{
    IWDG->CFG = 0x3c00;
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
 * <tr><td>2023��02��28�� <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
void IWDG_ENABLE(void)
{
    IWDG->CFG = 0x1;
}

/**
 *@brief @b ��������:   void IWDG_Feed(void)
 *@brief @b ��������:   ���Ź�ι��
 *@see���������ݣ�       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
                IWDG_Feed();    //���Ź�ι��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��02��28�� <td>1.0     <td>cakezhang      <td>����
 * </table>
 */
void IWDG_Feed(void)
{
    IWDG->PSW = PSW_IWDG_PRE;
    IWDG->CLR = PSW_IWDG_CLR;
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
