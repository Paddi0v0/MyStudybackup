/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc07x_can.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CAN������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	       <th>Version   <th>Author  <th>Description
 * <tr><td>2023��2��23�� <td>1.0     <td>HuangMG     <td>����
 * </table>
 */
#include "lks32mc07x_can.h"
#include "lks32mc07x_sys.h"
#include "string.h"


can_parameter can_par =
{
	0x00,
	0x00,
	0x00,
	0x00,
	{0,0},
	{0,0},
	0x00,
};


/*******************************************************************************
��������: void CAN_Initl(u8 BTR3,u8 BTR2,u8 BTR1,u8 BTR0)
��������: CAN��ʼ������,����2.0BЭ��
�����ı�:
  
	�������: BTR0    S_SEG_1:��ʱ��� 1 ʱ�䵥Ԫ�趨
           BTR1    S_SEG_2����ʱ��� 2 ʱ�䵥Ԫ�趨
           BTR2    S_SJW �� ��ͬ���������ʱ���趨
					 BTR3    S_PRESC ��TQ Ԥ��Ƶ���趨ֵ
�������: ��
����ֵ:   ��
����˵��:  
�޸�����     �汾��     �޸���    �޸�����
-----------------------------------------------------------------------------
2023/02/13   V1.0     HuangMG     ����
*******************************************************************************/
/**
 *@brief @b ��������:   void CAN_Init(u8 BTR3,u8 BTR2,u8 BTR1,u8 BTR0)
 *@brief @b ��������:   CAN��ʼ������,����2.0BЭ��
 *@see�����ú�����      ��
 *@param���������      
   ������    AN_SBAUD.PRESC[7:0]   CAN_SBAUD.SJW[6:0]  CAN_SBAUD.SEG2[6:0] CAN_SBAUD.SEG1[7:0] Sample Poin\n          
	1Mbps             0x05                0x02                0x05               0x08             63% \n 
	800Kbps           0x0B                0x01                0x03               0x04             60% \n 
	666Kbps           0x0B                0x01                0x03               0x06             67% \n 
	500Kbps           0x0B                0x02                0x05               0x08             63% \n 
	400Kbps           0x0B                0x02                0x06               0x0B             65% \n 
	250Kbps           0x17                0x02                0x05               0x08             63% \n 
	200Kbps           0x17                0x02                0x06               0x0B             65% \n 
	125Kbps           0x2F                0x02                0x05               0x08             63% \n 
	100Kbps           0x2F                0x02                0x06               0x0B             65% \n 
	80Kbps            0x35                0x02                0x06               0x0B             65% \n 
	50Kbps            0x5F                0x02                0x06               0x0B             65% \n 
	40Kbps            0x77                0x02                0x06               0x0B             65% \n 
	25Kbps            0xBF                0x02                0x06               0x0B             65% \n 
	20Kbps            0xEF                0x02                0x06               0x0B             65% \n 
	10Kbps            0xEF                0x06                0x0D               0x18             65% \n 
	05Kbps            0xEF                0x0D                0x1B               0x32             65%                 
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			 CAN_Init(0x0B,0x02,0x05,0x08);    //CAN������500Khz
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_Init(u8 BTR3,u8 BTR2,u8 BTR1,u8 BTR0)
{
		SYS_WR_PROTECT = 0x7a83;         // �ر�д���� 
	
	  SYS_SFT_RST |= BIT19;            // ��λCAN
	  SYS_SFT_RST &= ~BIT19;           // �ͷ�CAN��λ
	 
	  SYS_CLK_FEN |= BIT19;            // ʹ��CANʱ��  
	  
		CAN_CFG_STAT |= 1 << 7;          // ��λģʽ
	
	  CAN_ACFEN = 0x00;                // �ر�ID�˲�����
    CAN_SBAUD    = BTR0|(BTR1<<8)|(BTR2<<16)|(BTR3<<24);//����������
	  CAN_CFG_STAT &= ~(1 << 7);       // �������ģʽ
	  CAN_TCMD    = 0x00;
  	CAN_RTIE |= 1 << 7;              // RFIFO���µ����ݱ����յ����ж�Դʹ��
	  CAN_RTIE |= 1 << 1;					     // �����ж�ʹ��
	  SYS_WR_PROTECT = 0x0;            // ��д���� 
}


/**
 *@brief @b ��������:   u8 My_CAN_Send_Msg(u32 id,u8 ide,u8 rtr, u8 *msg,u8 len)
 *@brief @b ��������:   CAN��ѯ�������ݳ���  ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id: ����id \n 
 *                      ide: ��׼/��չ֡                      
 *		                  rtr��Զ��֡/����֡ \n
 *		                  *msg: ���͵�����    
 *                      len:�����ֽڳ��� \n          
 *@param���������       ��
 *@return�� �� ֵ��      0�����ͳɹ� !0������ʧ��
 *@note����˵����        
 *                      1��id:��׼ID(11λ)/��չID(11λ+18λ) \n
 *                      2��ide:0,��׼֡;1,��չ֡ \n
 *                      3��rtr:0,����֡;1,Զ��֡��
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
      ʾ��1�� 
	          u8 Can_TX[8] = {0x81, 0x23, 0x33, 0x44, 0x15, 0x26, 0x37, 0x48};
           // �������� ID:0x03,��׼֡,����֡�����ݣ����ݳ���8�ֽ�
		       My_CAN_Send_Msg(0x5A, 0, 0, Can_TX, 8);
      ʾ��2��
           u8 Can_TX[8] = {0x81, 0x23, 0x33, 0x44, 0x15, 0x26, 0x37, 0x48};
           // �������� ID:0x03,��չ֡,����֡�����ݣ����ݳ���8�ֽ�
		       My_CAN_Send_Msg(0X10005A5A, 1, 0, Can_TX, 8);
  @endcode    
 *@par �޸���־:�����Բ�ģʽ�������ݹ���
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
u8 My_CAN_Send_Msg(u32 id,u8 ide,u8 rtr, u8 *msg,u8 len)
{  
	u8 frame_inf = 0 ;
	u16 i = 0;
	CAN_CFG_STAT &= ~(1 << 7);        // �������ģʽ
	if(!(CAN_TCMD & BIT4))        // ��CAN�豸�����Ϳ���
  {
		frame_inf |= ide << 7;          //0��׼/1��չ֡
		frame_inf |= rtr << 6;          //0,����֡;1,ң��֡
		frame_inf |= len << 0;          //�������ݳ���
		CAN_TBUF_01 = frame_inf;        //����TX֡��Ϣ
		if(0 == ide)//��׼֡
		{
			id &= 0x7ff;		
			//���ͽ��ռĴ���1
			CAN_TBUF_00 = id;          //TX ID
			CAN_TBUF_02 =  msg[0]|msg[1]<<8|msg[2]<<16|msg[3]<<24;          //TX DATA0123
      CAN_TBUF_03 =  msg[4]|msg[5]<<8|msg[6]<<16|msg[7]<<24;          //TX DATA4567
			
		}else//��չ֡
		{
			id &= 0X1FFFFFFF;
			//���ͽ��ռĴ���1
			CAN_TBUF_00 = id;          //TX ID
			CAN_TBUF_02 =  msg[0]|msg[1]<<8|msg[2]<<16|msg[3]<<24;          //TX DATA0123
      CAN_TBUF_03 =  msg[4]|msg[5]<<8|msg[6]<<16|msg[7]<<24;          //TX DATA4567
		}
		CAN_TCMD |= 1 << 4;         //CAN ���ʹ�������
		while(CAN_TCMD & BIT4)        //���һ���Ƿ������
		{
			if((i++) >= 0xfff) 
			{
				return 0xff;             //����ʧ��
			}
		};
		   return 0;                   //�������
	}
	else //TFIFO�ǿ�
	{
		   return 0xff;               //����ʧ��
	}
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
static void Delay(u32 cnt)
{
    volatile u32 t_cnt;

    for(t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __NOP();
    }
}

/**
 *@brief @b ��������:   void ID0_Filter(u32 id, u32 MASK, u8 ide)
 *@brief @b ��������:   CAN ID0�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id��IDֵ \n 
 *                      MASK��ID����λ\n                   
 *		                ide:0,��׼֡;1,��չ֡��Ϊ0 11λID���� Ϊ1 29λID���ȣ�       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��                   
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
             ID0_Filter(0x02,0x000,1);    //����ID0�����˲�������id=0x02����չ֡
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID0_Filter(u32 id, u32 MASK, u8 ide)
{ 
	  CAN_CFG_STAT |= 1 << 7;          // ��λģʽ 
	 
	  CAN_ACFEN   |= BIT0;       //ACF0 is Enabled		  
    CAN_ACFCTRL  = 0x00000020; //SELMASK==1 MASK
	  CAN_ACF |= BIT30; 
		if(ide == 0)
		 {
			 CAN_ACF &= ~BIT29;//���ձ�׼֡
		 }else{
			 CAN_ACF |= BIT29; //������չ֡
		 }
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (MASK & 0x1FFFFFFF);//ACF0 Mask Disabled, All bits should be compared 
	  CAN_ACFCTRL = 0x00;      //ID�˲�����ַ ָ�� ACF_0
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (id & 0x1FFFFFFF);//ACF0 ID Enabled
    Delay(100);
	CAN_CFG_STAT &= ~(1 << 7);       // �������ģʽ
}
/**
 *@brief @b ��������:   void ID1_Filter(u32 id, u32 MASK, u8 ide)
 *@brief @b ��������:   CAN ID1�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id��IDֵ \n 
 *                      MASK��ID����λ\n                   
 *		                ide:0,��׼֡;1,��չ֡��Ϊ0 11λID���� Ϊ1 29λID���ȣ�       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��                   
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
             ID1_Filter(0x02,0x000,1);    //����ID0�����˲�������id=0x02����չ֡
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID1_Filter(u32 id, u32 MASK, u8 ide)
{ 
	  CAN_CFG_STAT |= 1 << 7;          // ��λģʽ

	  CAN_ACFEN   |= BIT1;//ACF1 is Enabled		  
    CAN_ACFCTRL  = 0x00000021;//SELMASK==1 MASK
	  CAN_ACF |= BIT30; 
		if(ide == 0)
		 {
			 CAN_ACF &= ~BIT29;//���ձ�׼֡
		 }else{
			 CAN_ACF |= BIT29; //������չ֡
		 }
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (MASK & 0x1FFFFFFF);//ACF0 Mask Disabled, All bits should be compared 
	  CAN_ACFCTRL = 0x01;//SELMASK==0 ID
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (id & 0x1FFFFFFF);//ACF0 ID Enabled
    Delay(100);
	CAN_CFG_STAT &= ~(1 << 7);       // �������ģʽ
}
/**
 *@brief @b ��������:   void ID2_Filter(u32 id, u32 MASK, u8 ide)
 *@brief @b ��������:   CAN ID2�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id��IDֵ \n 
 *                      MASK��ID����λ\n                   
 *		                ide:0,��׼֡;1,��չ֡��Ϊ0 11λID���� Ϊ1 29λID���ȣ�       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��                   
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
             ID2_Filter(0x02,0x000,1);    //����ID0�����˲�������id=0x02����չ֡
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID2_Filter(u32 id , u32 MASK , u8 ide)
{ 
	  CAN_CFG_STAT |= 1 << 7;          // ��λģʽ
	 
	  CAN_ACFEN   |= BIT2;//ACF2 is Enabled		  
    CAN_ACFCTRL  = 0x00000022;//SELMASK==1 MASK
	  CAN_ACF |= BIT30; 
		if(ide == 0)
		 {
			 CAN_ACF &= ~BIT29;//���ձ�׼֡
		 }else{
			 CAN_ACF |= BIT29; //������չ֡
		 }
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (MASK & 0x1FFFFFFF);//ACF0 Mask Disabled, All bits should be compared 
	  CAN_ACFCTRL = 0x02;//SELMASK==0 ID
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (id & 0x1FFFFFFF);//ACF2 ID Enabled
    Delay(100);
	CAN_CFG_STAT &= ~(1 << 7);       // �������ģʽ
}  

/**
 *@brief @b ��������:   void ID3_Filter(u32 id, u32 MASK, u8 ide)
 *@brief @b ��������:   CAN ID3�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id��IDֵ \n 
 *                      MASK��ID����λ\n                   
 *		                ide:0,��׼֡;1,��չ֡��Ϊ0 11λID���� Ϊ1 29λID���ȣ�       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��                   
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
             ID3_Filter(0x02,0x000,1);    //����ID0�����˲�������id=0x02����չ֡
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID3_Filter(u32 id , u32 MASK , u8 ide)
{ 
	  CAN_CFG_STAT |= 1 << 7;          // ��λģʽ
	 
	  CAN_ACFEN   |= BIT3;//ACF3 is Enabled		  
    CAN_ACFCTRL  = 0x00000023;//SELMASK==1 MASK
	  CAN_ACF |= BIT30; 
		if(ide == 0)
		 {
			 CAN_ACF &= ~BIT29;//���ձ�׼֡
		 }else{
			 CAN_ACF |= BIT29; //������չ֡
		 }
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (MASK & 0x1FFFFFFF);//ACF0 Mask Disabled, All bits should be compared 
	  CAN_ACFCTRL = 0x03;//SELMASK==0 ID
		CAN_ACF &= ~0x1FFFFFFF;
    CAN_ACF |= (id & 0x1FFFFFFF);//ACF3 ID Enabled
    Delay(100);
	CAN_CFG_STAT &= ~(1 << 7);       // �������ģʽ
}  

/**
 *@brief @b ��������:   u8 CAN_Receive_Msg(u32* id,u8* len ,u8* ide,u8* rtr,u8 *buf)
 *@brief @b ��������:   CAN��ѯ�������ݳ���
 *@see�����ú�����       ��
 *@param���������       ��                                
 *@param���������       id:  ����֡id \n 
                        len: �������ݳ��� \n
 *                      ide: ����֡�����ͣ���׼/��չ֡ \n                      
 *		                rtr������֡�����ͣ�Զ��֡/����֡ \n 
 *		                *buf: ����֡�����ݴ洢������    
 *@return�� �� ֵ��      0��������� !0������ʧ��
 *@note����˵����        
 *                      1��id:��׼ID(11λ)/��չID(11λ+18λ) \n
 *                      2��ide:0,��׼֡;1,��չ֡ \n
 *                      3��rtr:0,����֡;1,Զ��֡��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
         //can_par.id�洢����֡ID��can_par.len�������ݸ���
		     can_par.ide�洢����֡���ͱ�׼/��չ֡��
		     can_par.rtr�洢����֡����Զ��֡/����֡��can_par.RX�洢����֡����
         CAN_Receive_Msg(&( can_par.id),&(can_par.len),&(can_par.ide),&(can_par.rtr),can_par.RX);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2023��2��23��  <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
u8 CAN_Receive_Msg(u32* id,u8* len ,u8* ide,u8* rtr,u8 *buf)
{
	  u8 reg_inf;
	  u8 tmep = 0;
	  if(CAN_RCTRL != 0x00)/*Rx Buffer����һ֡���֡���ݣ�����ͨ�� RFIFO �Ĵ�����ȡ*/
		{
	     reg_inf = CAN_RBUF_01;
			 if(reg_inf & BIT6)
			 {
			    *rtr = 1;
			 }else{
				  *rtr = 0;
			 }
			 *len = reg_inf & 0xf;
			 if(reg_inf & BIT7)/*��չ֡*/
			 {
			    *ide = 1;
				  *id = CAN_RBUF_00 & 0x1FFFFFFF;  
			 }else{             /*��׼֡*/
				  *ide = 0;
				  *id = CAN_RBUF_00 & 0x7ff;			 
			 }
			 	  buf[0] = CAN_RBUF_02;
				  buf[1] = CAN_RBUF_02>>8;
				  buf[2] = CAN_RBUF_02>>16;
				  buf[3] = CAN_RBUF_02>>24;
				  buf[4] = CAN_RBUF_03;
				  buf[5] = CAN_RBUF_03>>8;
				  buf[6] = CAN_RBUF_03>>16;
				  buf[7] = CAN_RBUF_03>>24;	
			    for(tmep = *len ; tmep < 8 ;tmep++)
			    {
			      buf[tmep] = 0;
			    }
			    CAN_RCTRL |= BIT4; /*�ͷ� Rx Buffer Slot*/
					return 0;
		}
	   return 1 ;
}








