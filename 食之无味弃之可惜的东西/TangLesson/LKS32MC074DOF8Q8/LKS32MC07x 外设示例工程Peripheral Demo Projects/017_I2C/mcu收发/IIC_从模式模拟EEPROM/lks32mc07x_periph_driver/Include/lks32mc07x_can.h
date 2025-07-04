/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_can.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CAN������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>HuangMG    <td>����
 * </table>
 */
#ifndef _LKS32MC07X_CAN_H_
#define _LKS32MC07X_CAN_H_

///< Includes ------------------------------------------------------------------
#include "lks32mc07x_lib.h"


typedef struct
{
	u32 id;   /**<����ID�� */
	u8 len;   /**<�������ݳ��� */
	u8 ide;   /**<������������0��׼/1��չ֡ */
	u8 rtr;   /**<������������0����֡/1Զ��֡*/
	u8 TX[8]; /**<���ݷ��������� */
	u8 RX[8]; /**<�������ݻ�����*/
	u32 rx_sta;/**<���մ����־λ*/
}can_parameter;

extern can_parameter can_par ;


void CAN_Init(u8 BTR3,u8 BTR2,u8 BTR1,u8 BTR0);
u8 My_CAN_Send_Msg(u32 id,u8 ide,u8 rtr, u8 *msg,u8 len);
void ID0_Filter(u32 id, u32 MASK, u8 ide);
void ID1_Filter(u32 id, u32 MASK, u8 ide);
void ID2_Filter(u32 id, u32 MASK, u8 ide);
void ID3_Filter(u32 id, u32 MASK, u8 ide);
u8 CAN_Receive_Msg(u32* id,u8* len ,u8* ide,u8* rtr,u8 *buf);

#endif



