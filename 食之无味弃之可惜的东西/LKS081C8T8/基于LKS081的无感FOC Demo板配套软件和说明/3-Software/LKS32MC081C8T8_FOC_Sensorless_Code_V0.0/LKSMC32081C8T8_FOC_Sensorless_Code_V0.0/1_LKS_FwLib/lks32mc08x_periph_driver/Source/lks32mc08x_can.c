/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_can.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CAN������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��4��29�� <td>1.0     <td>HuangMG      <td>����
 * </table>
 */

#include "lks32mc08x_can.h"
 

/**
 *@brief @b ��������:   void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
 *@brief @b ��������:   ADC��ʼ������
 *@see�����ú�����      SYS_ModuleClockCmd()
 *@param���������      CANx��CAN , CAN_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			CAN_InitTypeDef CAN_InitStruct;
			CAN_StructInit(&CAN_InitStruct);
			//Can������:500Kbps
			CAN_InitStruct.CAN_Btr0 = 0x05;
			CAN_InitStruct.CAN_Btr1 = 0x1C;
			CAN_InitStruct.CAN_DMAEn = DISABLE;              //��DMA��������
			CAN_InitStruct.CAN_WorkMode = CAN_NORMAL_MODE;   //CAN�����շ�����ģʽ
			CAN_InitStruct.CAN_ErrWarThre = 50;              //�����뾯������ֵΪ50
			//���������ж�ʹ��,������ϵ�ǰ֡�ж�ʹ��,���󱨾��ж�ʹ��
	        CAN_InitStruct.IRQEna = CAN_IE_RFIFONOEMPTY | CAN_IE_TXDONE | CAN_IE_WERR;
			CAN_Init(CAN, &CAN_InitStruct);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct)
{
	SYS_ModuleClockCmd(SYS_Module_CAN, ENABLE);
	CAN->MOD |= 1 << 0; // ��λģʽ
	if (CAN_InitStruct->CAN_DMAEn)
	{
		CANx->CMR |= BIT5;
	}
	else
	{
		CANx->CMR &= ~BIT5;
	}
	CANx->EWLR = CAN_InitStruct->CAN_ErrWarThre;
	CANx->BTR0 = CAN_InitStruct->CAN_Btr0;
	CANx->BTR1 = CAN_InitStruct->CAN_Btr1;
	CANx->MOD = CAN_InitStruct->CAN_WorkMode;
	CANx->EIR = CAN_InitStruct->IRQEna;
}
/**
 *@brief @b ��������:   void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct)
 *@brief @b ��������:   CAN�ṹ���ʼ��
 *@see�����ú�����       ��
 *@param���������      CAN_InitTypeDef
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��   
 *@par ʾ�����룺
 *@code    
           CAN_InitTypeDef CAN_InitStructure;
		   CAN_StructInit(&CAN_InitStructure); //��ʼ���ṹ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct)
{
	CAN_InitStruct->CAN_DMAEn = DISABLE;
	CAN_InitStruct->CAN_WorkMode = CAN_NORMAL_MODE;
	CAN_InitStruct->CAN_ErrWarThre = 120;
	/*������ѡ��500K*/
	CAN_InitStruct->CAN_Btr0 = 0x05;
	CAN_InitStruct->CAN_Btr1 = 0x1C;
	CAN_InitStruct->IRQEna = 0x0;
}

/**
 *@brief @b ��������:   void CAN_Sleep(u32 Baud)
 *@brief @b ��������:   CAN�������ߺ���
 *@see�����ú�����       ��
 *@param���������       Baud�������ʣ���λKbps
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ���߻��������֣�1���������CAN_MOD[4]�����Զ��˳�����ģʽ����2��һ����
			           �⵽CAN���ߵĻ�����Զ��˳�����ģʽ��
 *@warning             ��   
 *@par ʾ�����룺
 *@code    
           CAN_Sleep(500);//����CAN���ߣ�CAN������500kbps
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_Sleep(u32 Baud)
{
	u32 rate = 0 , t_cnt = 0;
	CAN_MODE &= ~BIT0;//�������ģʽ
	rate = 1152000 / Baud; //��ֵΪ��Ƶ96MHz���м��㣬��Ƶ48MHzд���Baud�ʵ��Ӵ�
	for (t_cnt = 0; t_cnt < rate; t_cnt++)//�ȴ�11������������,������������ģʽ
	{
		__NOP();
	}
	CAN_MODE |= BIT4;
	rate = 1632000 / Baud; //��ֵΪ��Ƶ96MHz���м��㣬��Ƶ48MHzд���Baud�ʵ��Ӵ�
	// �ȴ�16�����������ڣ���ʽ�������ߵȴ�16�����������ڣ���ʽ��������
	for (t_cnt = 0; t_cnt < rate; t_cnt++)
	{
	   __NOP();
	}
}

/**
 *@brief @b ��������:   void CAN_Manual_Awake(void)
 *@brief @b ��������:   CAN�ֶ����߻��Ѻ���
 *@see�����ú�����       ��
 *@param���������       ��
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
           CAN_Manual_Awake();//����CAN����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_Manual_Awake(void)
{
	CAN_MODE &= ~BIT0;
	CAN_MODE &= ~BIT4;
}

/**
 *@brief @b ��������:   void  SFF_ID_Filter(u32 acr,u32 amr,u8 rtr_acr,u8 rtr_amr,
						   u8 Byte1_acr,u8 Byte1_amr,u8 Byte2_acr,u8 Byte2_amr)
 *@brief @b ��������:   CAN��׼֡��ID����,����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       acr: IDֵ�� \n
 *                      amr������λ                     
 *                      rtr_acr:   ���գ�1Զ��/0����֡��֡����ѡ�� \n
 *                      rtr_amr��  Զ��/����֡�˲�����λ��0��ʾֻ����rtr_acr����֡��Ϊ1��ʾԶ��/0��־֡������ \n
 *                      Byte1_acr�����ݵ�һ���ֽ� \n
 *                      Byte1_amr����һ���ֽ�����λ \n
 *                      Byte2_acr�����ݵڶ����ֽ� \n
 *                      Byte2_amr���ڶ����ֽ�����λ \n
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����         
                        1����׼֡  11λID���� \n
                        2��amr������λ��Ӧλ0��ʾ��Ϊ��ӦID����ƥ��Ż����
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
         ʾ��1�� 
           //��׼֡���յ��˲��� ID:0x5A ,����֡��BYTE1:0x81,BYTE:0x23
            SFF_ID_Filter(0X5A, 0x00, 0x0, 0x0, 0x81, 0x00, 0x23, 0x00);
		   //���յı�׼ֻ֡������ID��ַΪ0x5A������֡�ҵ�һ�������ֽ�Ϊ0x81���ڶ����ֽ�Ϊ0x23�Ż����

	     ʾ��2�� 
           //��׼֡���յ��˲��� ID:0x5A��0x5B(��Ϊamr��һλΪ1�����Ե�һλ����Ϊ1��0��
		                       ����ƥ�伴�ɽ���) ,����֡��BYTE1:0x81,BYTE:0x23
            SFF_ID_Filter(0X5A, 0x01, 0x0, 0x0, 0x81, 0x00, 0x23, 0x00);
		   //���յı�׼ֻ֡������ID��ַΪ0x5A��0x5B������֡�ҵ�һ�������ֽ�Ϊ0x81���ڶ����ֽ�Ϊ0x23�Ż����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void SFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr, u8 Byte1_acr, u8 Byte1_amr, u8 Byte2_acr, u8 Byte2_amr)
{
	CAN->MOD |= 1 << 0; // ��λģʽ
	CAN->MOD |= 1 << 3; // ���˲�ID,һ�� 32 λ�����˲�ID

	acr &= 0x7ff;
	amr &= 0x7ff;

	rtr_acr &= 0x1;
	rtr_amr &= 0x1;

	CAN_ACR0 = (u8)(acr >> 3);
	CAN_ACR1 = (u8)(acr << 5) | (rtr_acr << 4);
	CAN_ACR2 = Byte1_acr;
	CAN_ACR3 = Byte2_acr;

	CAN_AMR0 = (u8)(amr >> 3);
	CAN_AMR1 = (u8)(amr << 5) | (rtr_amr << 4) | 0x0f;
	CAN_AMR2 = Byte1_amr;
	CAN_AMR3 = Byte2_amr;

	CAN->MOD &= ~(1 << 0); //�������ģʽ
}

/**
 *@brief @b ��������:   void EFF_ID_Filter(u32 acr,u32 amr,u8 rtr_acr,u8 rtr_amr)
 *@brief @b ��������:   CAN��չ֡��ID����,����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       acr: IDֵ�� \n
 *                      amr������λ           
 *                      rtr_acr:   ���գ�1Զ��/0����֡��֡����ѡ�� \n
 *                      rtr_amr��  Զ��/����֡�˲�����λ��0��ʾֻ����rtr_acr����֡��Ϊ1��ʾԶ��/0��־֡������ \n
 *                 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        
                        1����չ֡  29λID���� \n
                        2��amr������λ��Ӧλ0��ʾ��Ϊ��ӦID����ƥ��Ż����
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
      ʾ��1�� 
           //��չ֡�����˲��� ID:0X10005A5A ,����֡
           EFF_ID_Filter(0X10005A5A, 0x00, 0x0, 0x0);
		   //���յ���չֻ֡������ID��ַΪ0X10005A5A��Ϊ����֡�Ż����
      ʾ��2��
           //��չ֡�����˲��� ID:0X10005A5A ,����֡��Զ��֡����Ϊrtr_amrΪ1�����Խ���֡���Ͳ���Ҫƥ��Ҳ���Խ��գ�
		   EFF_ID_Filter(0X10005A5A, 0x00, 0x0, 0x1);
		   //���յ���չֻ֡������ID��ַΪ0X10005A5A������֡��Զ��֡�Ż����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void EFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr)
{
	CAN->MOD |= 1 << 0; // ��λģʽ
	CAN->MOD |= 1 << 3; // ���˲�ID,һ�� 32 λ�����˲�ID

	acr &= 0x1fffffff;
	amr &= 0x1fffffff;
	rtr_acr &= 0x1;
	rtr_amr &= 0x1;

	/*�ɽ��յ�ID�Ĵ�����������֡�� ID ͬ��ƥ�䣬�� CAN ģ�����
	  ��� AMR �Ĵ�������ͬһ�� ID ƥ�䡣*/
	CAN_ACR3 = (u8)(acr << 3) | (rtr_acr << 2);
	CAN_ACR2 = (u8)(acr >> 5);
	CAN_ACR1 = (u8)(acr >> 13);
	CAN_ACR0 = (u8)(acr >> 21);
	/*��� ACR �Ĵ�����ͬ���� ID ����ƥ�䡣AMR �Ĵ���ĳһλΪ 0����ʶ
   ID ��Ӧλ��Ҫͬ ACR ƥ�䣻Ϊ 1����ʶ ID ��Ӧλ����Ҫͬ ACR ƥ��*/
	CAN_AMR3 = (u8)((amr << 3) | (rtr_amr << 2) | 0x03);
	CAN_AMR2 = (u8)(amr >> 5);
	CAN_AMR1 = (u8)(amr >> 13);
	CAN_AMR0 = (u8)(amr >> 21);

	CAN->MOD &= ~(1 << 0); //�������ģʽ
}


/**
 *@brief @b ��������:   void  ID1_Filter_Dual(u32 acr1 ,u32 amr1,u8 rtr_acr1,
							   u8 rtr_amr1,u8 Byte_acr1,u8 Byte_amr1,u8 ide)
 *@brief @b ��������:   CAN˫ID������ID1�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      acr1��ID1ֵ����־֡ID11λ����չ֡ID��Χ1D28~ID13��ID��16λ��Ч�� \n 
 *                      amr1��ID1����λ                     
 *		                rtr_acr1�� ���գ�1Զ��/0����֡��֡����ѡ��ֻ֧��SFFģʽ����չ֡������Ч�� \n
 *		                rtr_amr1�� Զ��/��־֡�˲�����λ��0��ʾֻ����rtr_acr����֡��Ϊ1��ʾԶ��/0��־֡������
                                  ��ֻ֧��SFFģʽ����չ֡������Ч�� \n
 *		                Byte_acr1������֡��1��Byte��4λ�˲�ѡ��ֻ֧��SFFģʽ����չ֡������Ч�� \n
 *		                Byte_amr1������֡��1��Byte��4λ����λ��ֻ֧��SFFģʽ����չ֡������Ч�� \n
 *                      ide��0��׼(SFFģʽ)/1��չ֡ \n
 *                 
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        
 *                      1��SFFģʽΪ��׼֡ģʽ \n
 *                      2����չ֡ʱ��ֻ��ID��bit13- bit28��ID��16λ����ɸѡ���� \n
 *                      3����Ϊ˫�˲�ID1�������ݱ�־׼����չ֡�˲�����׼֡����չ֡��˫�˲������𣩣����Ըú�������
 *                         �β���ֻ֧�ֱ�׼֡ģʽ����ʹ�ã���չ֡ģʽ������Ч��
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
      ʾ��1�� 
           //��׼֡ID1�����˲��� ID:0X75A ,����֡
           ID1_Filter_Dual(0X75A, 0x00, 0x0, 0x0��0x0F, 0x00, 0);
		   //���յı�׼ֻ֡������ID��ַΪ0X75A��Ϊ����֡�ҽ������ݣ�����֡�ĵ�һ�����ݣ������������ĵ�һ�����ݣ��ĵ�1��
		     Byte��4λΪ0XF�Ż���ա�
      ʾ��2��
           //��չ֡ID1�����˲��� ID:0XFECB����չ֡
		   ID1_Filter_Dual(0XFECB, 0x00, 0x0, 0x0��0x00, 0x00,1);
		   //���յ���չֻ֡������ID��ַΪ0XFECB�Ż����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID1_Filter_Dual(u32 acr1, u32 amr1, u8 rtr_acr1, u8 rtr_amr1, u8 Byte_acr1, u8 Byte_amr1, u8 ide)
{
	CAN->MOD |= 1 << 0;	   // RESET MODE
	CAN->MOD &= ~(1 << 3); // ˫�˲�ID
	rtr_acr1 &= 0x1;
	rtr_amr1 &= 0x1;

	if (ide) /*��չ֡*/
	{
		acr1 &= 0x1fffffff;
		CAN_ACR1 = (u8)(acr1 >> 13);
		CAN_ACR0 = (u8)(acr1 >> 21);

		CAN_AMR1 = (u8)(amr1 >> 13);
		CAN_AMR0 = (u8)(amr1 >> 21);
	}
	else /*��׼֡*/
	{
		acr1 &= 0x7ff;
		CAN_ACR0 = (u8)(acr1 >> 3);
		CAN_ACR1 = (u8)(acr1 << 5) | (rtr_acr1 << 4) | (Byte_acr1 >> 4);
		CAN_ACR3 &= 0xF0;
		CAN_ACR3 |= (u8)(Byte_acr1 & 0x0F);

		CAN_AMR0 = (u8)(amr1 >> 3);
		CAN_AMR1 = (u8)(amr1 << 5) | (rtr_amr1 << 4) | (Byte_amr1 >> 4);
		CAN_AMR3 &= 0xF0;
		CAN_AMR3 |= (u8)(Byte_amr1 & 0x0F);
	}
	CAN->MOD &= ~(1 << 0); //�������ģʽ
}

/**
 *@brief @b ��������:   void ID2_Filter_Dual(u32 acr2 ,u32 amr2,u8 rtr_acr2,u8 rtr_amr2 ,u8 ide)
 *@brief @b ��������:   CAN˫ID������ID2�˲����� ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      acr2��ID2ֵ����Χ1D28~ID13��ID��16λ��Ч�� \n 
 *                      amr2��ID2����λ                      
 *		                rtr_acr2: ���գ�1Զ��/0����֡��֡����ѡ��ֻ֧��SFF��׼֡ģʽ����չ֡������Ч�� \n
 *		                rtr_amr2��Զ��/��־֡�˲�����λ��0��ʾֻ����rtr_acr����֡��Ϊ1��ʾԶ��/0��־
 *					              ֡�����գ�ֻ֧��SFF��׼֡ģʽ����չ֡������Ч�� \n             
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        
 *                      1��SFFģʽΪ��׼֡ģʽ \n
 *                      2����չ֡ʱ��ֻ��ID��bit13- bit28��ID��16λ����ɸѡ���� \n
 *                      3����Ϊ˫�˲�ID1�������ݱ�־׼����չ֡�˲�����׼֡����չ֡��˫�˲������𣩣����Ըú�������
 *                         �β���ֻ֧�ֱ�׼֡ģʽ����ʹ�ã���չ֡ģʽ������Ч��
 *@warning              ��   
 *@par ʾ�����룺
 *@code   
      ʾ��1�� 
           //��׼֡ID2�����˲��� ID:0X75A ,����֡
           ID2_Filter_Dual(0X75A, 0x00, 0x0, 0x0, 0);
		   //���յı�׼ֻ֡������ID��ַΪ0X75A��Ϊ����֡�Ż���ա�
      ʾ��2��
           //��չ֡ID2�����˲��� ID:0XFECB����չ֡
		   ID2_Filter_Dual(0XFECB, 0x00, 0x0, 0x0,1);
		   //���յ���չֻ֡������ID��ַΪ0XFECB�Ż����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void ID2_Filter_Dual(u32 acr2, u32 amr2, u8 rtr_acr2, u8 rtr_amr2, u8 ide)
{
	CAN->MOD |= 1 << 0;	   // RESET MODE
	CAN->MOD &= ~(1 << 3); //˫�˲�ID
	rtr_acr2 &= 0x1;
	rtr_amr2 &= 0x1;
	if (ide) // externed
	{
		acr2 &= 0x1fffffff;
		CAN_ACR3 = (u8)(acr2 >> 13);
		CAN_ACR2 = (u8)(acr2 >> 21);

		CAN_AMR3 = (u8)(amr2 >> 13);
		CAN_AMR2 = (u8)(amr2 >> 21);
	}
	else
	{
		acr2 &= 0x7ff;
		CAN_ACR3 &= 0x0f;
		CAN_ACR2 = (u8)(acr2 >> 3);
		CAN_ACR3 |= (u8)(acr2 << 5) | (rtr_acr2 << 4);

		CAN_AMR3 &= 0x0f;
		CAN_AMR2 = (u8)(amr2 >> 3);
		CAN_AMR3 |= (u8)(amr2 << 5) | (rtr_amr2 >> 4);
	}
	CAN->MOD &= ~(1 << 0); //�������ģʽ
}

/**
 *@brief @b ��������:   u8 My_CAN_Send_Msg(u32 id,u8 ide,u8 rtr,const u8 *msg,u8 len)
 *@brief @b ��������:   CAN��ѯ�������ݳ���  ����2.0BЭ��
 *@see�����ú�����       ��
 *@param���������       
 *                      id: ����id \n 
 *                      ide: ��׼/��չ֡                      
 *		                rtr��Զ��֡/����֡ \n
 *		                *msg: ���͵�����    
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
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2018��08��10�� <td>1.0      <td>ZKW        <td>����
 * </table>
 *@par �޸���־:�����Բ�ģʽ�������ݹ���
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��01��16�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */

u8 My_CAN_Send_Msg(u32 id, u8 ide, u8 rtr, u8 *msg, u8 len)
{
	u8 frame_inf = 0;
	u16 i = 0;

	if (CAN->SR & 0X04) // TFIFO ��
	{
		CAN->MOD &= ~(1 << 0);	//�������ģʽ
		frame_inf |= ide << 7;	// 0��׼/1��չ֡
		frame_inf |= rtr << 6;	// 0,����֡;1,ң��֡
		frame_inf |= len << 0;	//�������ݳ���
		CAN->TXRX0 = frame_inf; //����TX֡��Ϣ
		if (0 == ide)			//��׼֡
		{
			id &= 0x7ff;
			//���ͽ��ռĴ���1
			CAN->TXRX1 = id >> 3;		   // TX ID0
			CAN->TXRX2 = (id & 0X07) << 5; // TX ID1
			CAN->TXRX3 = msg[0];		   // TX DATA0
			CAN->TXRX4 = msg[1];		   // TX DATA1
			CAN->TXRX5 = msg[2];		   // TX DATA2
			CAN->TXRX6 = msg[3];		   // TX DATA3
			CAN->TXRX7 = msg[4];		   // TX DATA4
			CAN->TXRX8 = msg[5];		   // TX DATA5
			CAN->TXRX9 = msg[6];		   // TX DATA6
			CAN->TXRXA = msg[7];		   // TX DATA7
		}
		else //��չ֡
		{
			id &= 0X1FFFFFFF;
			CAN->TXRX1 = (u8)(id >> 21);		  // TX ID0
			CAN->TXRX2 = (u8)((id >> 13) & 0xFF); // TX ID1
			CAN->TXRX3 = (u8)((id >> 5) & 0xFF);  // TX ID2
			CAN->TXRX4 = (u8)((id & 0xff) << 3);  // TX ID3
			CAN->TXRX5 = msg[0];				  // TX DATA0
			CAN->TXRX6 = msg[1];				  // TX DATA1
			CAN->TXRX7 = msg[2];				  // TX DATA2
			CAN->TXRX8 = msg[3];				  // TX DATA3
			CAN->TXRX9 = msg[4];				  // TX DATA4
			CAN->TXRXA = msg[5];				  // TX DATA5
			CAN->TXRXB = msg[6];				  // TX DATA6
			CAN->TXRXC = msg[7];				  // TX DATA7
		}
		if ((CAN->MOD) & CAN_SELFTEST_MODE) /*CAN�������Բ�ģʽ*/
		{
			CAN->CMR |= 1 << 4; // CAN�������ݵ�ͬʱҲ�����ݽ��ջ���
		}
		else
		{
			CAN->CMR |= 1 << 0; // CAN���ʹ�������
		}
		while (!(CAN->SR & 0X08)) //���һ���Ƿ������
		{
			if ((i++) >= 0xfff)
			{
				return 0xff; //����ʧ��
			}
		};
		return 0; //�������
	}
	else // TFIFO�ǿ�
	{
		return 0xff; //����ʧ��
	}
}


/**
 *@brief @b ��������:   u8 CAN_Receive_Msg(uint32_t* id,u8* ide,u8* rtr,u8 *buf)
 *@brief @b ��������:   CAN��ѯ�������ݳ���
 *@see�����ú�����       ��
 *@param���������       ��                                
 *@param���������       id:  ����֡id \n 
 *                      ide: ����֡�����ͣ���׼/��չ֡                      
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
        //can_par.id�洢����֡ID��can_par.ide�洢����֡���ͱ�׼/��չ֡��
		  can_par.rtr�洢����֡����Զ��֡/����֡��can_par.RX�洢����֡����
        CAN_Receive_Msg(&( can_par.id),&(can_par.ide),&(can_par.rtr),can_par.RX) ;
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2018��08��10�� <td>1.0      <td>ZKW        <td>����
 * </table>
 */

u8 CAN_Receive_Msg(uint32_t *id, u8 *ide, u8 *rtr, u8 *buf)
{
	u8 reg_inf;

	if (CAN->SR & 0X01) // RFIFO ����һ֡���֡���ݣ�����ͨ�� RFIFO �Ĵ�����ȡ
	{
		CAN->MOD &= ~(1 << 0); //�������ģʽ
		reg_inf = CAN->TXRX0;  //��ȡ��һ֡���֡����
		if (reg_inf & 0x80)	   // EFF����չ��֡
		{
			*ide = 1;
			*id = (CAN->TXRX1 << 21) | (CAN->TXRX2 << 13) | (CAN->TXRX3 << 5) | (CAN->TXRX4 >> 3); //��ȡID
			if (reg_inf & 0x40)
			{
				*rtr = 1; //ң��֡
			}
			else
			{
				*rtr = 0; //����֡
			}
			buf[0] = CAN->TXRX5; //��ȡ����
			buf[1] = CAN->TXRX6;
			buf[2] = CAN->TXRX7;
			buf[3] = CAN->TXRX8;
			buf[4] = CAN->TXRX9;
			buf[5] = CAN->TXRXA;
			buf[6] = CAN->TXRXB;
			buf[7] = CAN->TXRXC;
		}
		else // SFF����׼��֡
		{
			*ide = 0;
			if (reg_inf & 0x40)
			{
				*rtr = 1;
			}
			else
			{
				*rtr = 0;
			}

			*id = ((CAN->TXRX1 << 3) | (CAN->TXRX2 >> 5));
			buf[0] = CAN->TXRX3;
			buf[1] = CAN->TXRX4;
			buf[2] = CAN->TXRX5;
			buf[3] = CAN->TXRX6;
			buf[4] = CAN->TXRX7;
			buf[5] = CAN->TXRX8;
			buf[6] = CAN->TXRX9;
			buf[7] = CAN->TXRXA;
		}

		CAN->CMR |= 0x04; //�ͷŽ��ջ�����

		return 0; //�������
	}
	else // fifo is empty
	{
		return 0xff; //�����쳣
	}
}


/**
 *@brief @b ��������:   uint8_t CAN_ReadState(CAN_TypeDef* CANx , uint32_t sr)
 *@brief @b ��������:   ��ȡCAN״̬
 *@see�����ú�����       ��
 *@param���������       sr:��ȡָ��״̬��������״̬��� \n 
 * <table>              <tr><th>CAN_ONBUS 	        <td>1��CAN ģ�飬���� BUS OFF ״̬�������ݷ��ͽ��ն��� \n 
 *                                                      0��CAN ģ�飬���Է������ݵ� CAN ���ߣ������ CAN ��������                          
 *						<tr><th>CAN_ERROV 			<td>1��CAN ��������Ĵ��������ﵽ�򳬹� CAN_EWL �涨ֵ \n 
 *                                                      0��CAN ��������Ĵ����������� CAN_EWL �涨ֵ
 *						<tr><th>CAN_TXING 			<td>1��CAN ģ�����ڷ���һ֡���� \n 
 *                                                      0��CAN ģ��û�з������ݵĶ���
 *						<tr><th>CAN_RXING 			<td>1��CAN ģ�����ڽ���һ֡���� \n
 *                                                      0��CAN ģ��û�н������ݵĶ���
 *						<tr><th>CAN_TRNASDONE 	    <td>1�����һ�δ�������� \n 
 *                                                      0�����һ�δ���δ���
 *						<tr><th>CAN_TFIFOEMPTY      <td>1��TFIFO �գ�����д�뷢������ \n 
 *                                                      0��TFIFO �ǿգ��ڲ�����û�з������
 *						<tr><th>CAN_RFIFOEMPTY      <td>1��RFIFO �����̫֡�࣬�������������ݶ�ʧ \n 
 *                                                      0��RFIFO δ��
 *						<tr><th>CAN_DATAAVAIL 	    <td>1��RFIFO ����һ֡���֡���ݣ�����ͨ�� RFIFO �Ĵ�����ȡ \n 
 *                                                      0��RFIFO û����Ч֡����
 *						<tr><th>CAN_ALLSR 			<td>��ȡȫ��״̬
 * </table>     
 *@param���������       ��
 *@return�� �� ֵ��      ����ָ��״̬�����������״̬���
 * <table>              <tr><th>CAN_ONBUS 	        <td>1��CAN ģ�飬���� BUS OFF ״̬�������ݷ��ͽ��ն��� \n 
 *                                                      0��CAN ģ�飬���Է������ݵ� CAN ���ߣ������ CAN ��������                          
 *						<tr><th>CAN_ERROV 			<td>1��CAN ��������Ĵ��������ﵽ�򳬹� CAN_EWL �涨ֵ \n 
 *                                                      0��CAN ��������Ĵ����������� CAN_EWL �涨ֵ
 *						<tr><th>CAN_TXING 			<td>1��CAN ģ�����ڷ���һ֡���� \n 
 *                                                      0��CAN ģ��û�з������ݵĶ���
 *						<tr><th>CAN_RXING 			<td>1��CAN ģ�����ڽ���һ֡���� \n
 *                                                      0��CAN ģ��û�н������ݵĶ���
 *						<tr><th>CAN_TRNASDONE 	    <td>1�����һ�δ�������� \n 
 *                                                      0�����һ�δ���δ���
 *						<tr><th>CAN_TFIFOEMPTY      <td>1��TFIFO �գ�����д�뷢������ \n 
 *                                                      0��TFIFO �ǿգ��ڲ�����û�з������
 *						<tr><th>CAN_RFIFOEMPTY      <td>1��RFIFO �����̫֡�࣬�������������ݶ�ʧ \n 
 *                                                      0��RFIFO δ��
 *						<tr><th>CAN_DATAAVAIL 	    <td>1��RFIFO ����һ֡���֡���ݣ�����ͨ�� RFIFO �Ĵ�����ȡ \n 
 *                                                      0��RFIFO û����Ч֡����
 *						<tr><th>CAN_ALLSR 			<td>��ȡȫ��״̬
 * </table>   
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        //�ж�CAN��RFIFO�Ƿ����һ֡���֡���ݣ���ʱ�Ƿ����ͨ��RFIFO�Ĵ�����ȡ 
		if(CAN_ReadState(CAN , CAN_DATAAVAIL) == CAN_DATAAVAIL)
		{
		}
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
uint8_t CAN_ReadState(CAN_TypeDef *CANx, uint32_t sr)
{
	return (uint8_t)(CANx->SR & sr);
}

/**
 *@brief @b ��������:   enumLossposition CAN_ReadALC(void)
 *@brief @b ��������:   ��ȡ�ٲö�ʧ�ľ���λ��
 *@see��Ӧ��ö�٣�       enumLossposition
 *@param���������       ��                                
 *@param���������       ��   
 *@return�� �� ֵ��      �����ٲö�ʧ����λ�� enumLossposition
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        if(CAN_ReadALC() == CAN_LOST_ID0)//�����ٲö�ʧ��ID�ĵ�һλ
		{
		}
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
enumLossposition CAN_ReadALC(void)
{
	return (enumLossposition)CAN_ALC;
}

/**
 *@brief @b ��������:   uint8_t CAN_ReadRXERRCNT(void)
 *@brief @b ��������:   ��ȡ���մ������ֵ
 *@see��Ӧ��ö�٣�       enumLossposition
 *@param���������       ��                                
 *@param���������       ��   
 *@return�� �� ֵ��      ���ؽ��մ������ֵ
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        u8 err_value = 0;
		err_value = CAN_ReadRXERRCNT();//��ȡ�������ֵ����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
uint8_t CAN_ReadRXERRCNT(void)
{
	return CAN_RXERR;
}

/**
 *@brief @b ��������:   uint8_t CAN_ReadTXERRCNT(void)
 *@brief @b ��������:   ��ȡ���ʹ������ֵ
 *@see��Ӧ��ö�٣�       enumLossposition
 *@param���������       ��                                
 *@param���������       ��   
 *@return�� �� ֵ��      ���ط��ʹ������ֵ
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        u8 err_value = 0;
		err_value = CAN_ReadTXERRCNT();//��ȡ�������ֵ����
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
uint8_t CAN_ReadTXERRCNT(void)
{
	return CAN_TXERR;
}

/**
 *@brief @b ��������:   void CAN_ReceFramInfor(uint8_t* cnt , uint8_t* rbsa )
 *@brief @b ��������:   ��ȡ���ջ�������Ч����֡�����һ֡��Ч���������� RFIFO��λ��
 *@see��Ӧ��ö�٣�       enumLossposition
 *@param���������       ��                                
 *@param���������       cnt����Ч֡���� \n
			            rbsa��RFIFO �Ĵ�СΪ 32 �ֽڣ�rbsa�洢��һ֡��Ч���������� RFIFO��λ��0-31   
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        u8 frames_number = 0 �� RFIFO_location = 0;//�洢��������
		CAN_ReceFramInfor(frames_number��RFIFO_location);
		//frames_number���ջ������洢֡������RFIFO_location�����������ȡ��֡λ��
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_ReceFramInfor(uint8_t *cnt, uint8_t *rbsa)
{
	*cnt = CAN_RMC;
	*rbsa = CAN_RBSA;
}

/**
 *@brief @b ��������:   void CAN_ReadErrorCode(enumErrortype* errtype , enumErrortime* errtim
									  , enumErrposition* errpos)
 *@brief @b ��������:   ��ȡ���ߴ������ͺʹ���λ��
 *@see��Ӧ��ö�٣�       enumErrortype �� enumErrortime �� enumErrposition
 *@param���������       errtype�����ߴ�������     \n
						errtim�� ���ߴ�����ʱ�� \n
						errpos�� ��������λ��   \n                               
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        u8 err_type = 0 �� err_time = 0 , err_location = 0;//�洢��������
		CAN_ReadErrorCode(err_type,err_time,err_location);
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
void CAN_ReadErrorCode(enumErrortype *errtype, enumErrortime *errtim, enumErrposition *errpos)
{
	*errtype = (enumErrortype)((CAN_ECC & 0xC0) >> 6);
	*errtim = (enumErrortime)((CAN_ECC & 0x20) >> 5);
	*errpos = (enumErrposition)(CAN_ECC & 0x1f);
}

/**
 *@brief @b ��������:   uint32_t CAN_GetIRQFlag(void)
 *@brief @b ��������:   ��ȡ�жϱ�־������жϱ�־λ
 *@see��Ӧ��ö�٣�       enumErrortype �� enumErrortime �� enumErrposition
 *@param���������       ��                              
 *@param���������       ��
 *@return�� �� ֵ��      TIM�жϱ�־
 *@note����˵����        CAN�жϱ�־λΪ�����
 *@warning              ��   
 *@par ʾ�����룺
 *@code    
        u8 CAN_SR 0;
		CAN_SR = CAN_GetIRQFlag();//��ȡCAN�жϱ�־λ�����CAN�жϱ�־λ
  @endcode    
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author     <th>Description
 * <tr><td>2022��04��29�� <td>1.0      <td>HuangMG        <td>����
 * </table>
 */
uint32_t CAN_GetIRQFlag(void)
{
	return CAN_INTR;
}



