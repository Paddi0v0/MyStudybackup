#include "hardinit.h"




void SYS_VolSelModule(uint32_t Vol);
void SoftDelay(u32 cnt);

void Clock_Init(void)
{
	SYS_WR_PROTECT = 0x7a83;
	SYS_AFE_REG5 |= 0x8000;
	#if POWER_MODE
	SYS_VolSelModule(MCU_POWER_5v0);       /* MCU�����ѹ��1��5V��0;3.3V*/
	#else
	SYS_VolSelModule(MCU_POWER_3v3);       /* MCU�����ѹ��1��5V��0;3.3V*/
	#endif
	SoftDelay(100);                /* �ȴ�PLL�ȶ�*/
	SYS_WR_PROTECT = 0x7a83;       /* ���ϵͳ�Ĵ���д���� */
	
	SYS_CLK_CFG |= 0x000001ff;		 /* select fast clock */
	SYS_CLK_DIV2  = 0x0000;
	SYS_CLK_FEN = 0xff;
}

void SystemInit(void)
{
    Clock_Init();
}

void InitCom(void)
{
	GPIO0_PIE = BIT15;				// RX-P0.15
	GPIO1_POE = BIT0;				  // TX-P1.0 
	
	GPIO0_FFEDC = 0x4000;
	GPIO1_F3210 = 0x0004;
	
	UART0_DIVH = 0x27;
	UART0_DIVL = 0x0F;
	UART0_IE = 0x02;

	UTIMER_UNT0_CFG = 0x0400;
	UTIMER_UNT0_TH = 0xFB80;		//48000
	UTIMER_CFG = 0x10;
	UTIMER_IE = 0x01;
}

/*******************************************************************************
 �������ƣ�    void SoftDelay(void)
 ����������    �����ʱ����
 ���������    ��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 2020/8/5      V1.0           Howlet Li          ����
 *******************************************************************************/
void SoftDelay(u32 cnt)
{
    volatile u32 t_cnt;

    for(t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
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



