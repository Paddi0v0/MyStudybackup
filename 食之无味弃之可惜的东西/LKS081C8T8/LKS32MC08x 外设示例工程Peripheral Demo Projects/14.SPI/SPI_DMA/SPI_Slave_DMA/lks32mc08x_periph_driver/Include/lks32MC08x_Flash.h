/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC08x_flash.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� FLASH������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2018��07��25�� <td>1.0     <td>Howlet      <td>����
 * </table>
 *@par �޸���־:���ӳ����ܷ�ʱ����Flash�ı���
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2020��12��15�� <td>1.1     <td>DengT      <td>�޸�
 * </table>
 */

#ifndef __LKS32MC08x_FLASH__
#define __LKS32MC08x_FLASH__

#include "lks32mc08x_lib.h"

#define Flash_MAIN 0x000 /**< Flash��mian������*/
#define Flash_NVR  0x800 /**< Flash��nvr������*/

extern volatile u32 erase_flag; 
extern volatile u32 progm_flag;
int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr);
extern void EraseSector(u32 adr, u16 nvr);
void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr);

#endif

