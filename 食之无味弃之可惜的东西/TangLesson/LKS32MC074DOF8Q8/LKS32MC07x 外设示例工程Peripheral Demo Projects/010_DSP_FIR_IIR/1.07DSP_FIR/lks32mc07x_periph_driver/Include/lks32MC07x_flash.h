/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� LKS32MC07x_flash.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� FLASH������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2023��2��22�� <td>1.0     <td>HuangMG    <td>����
 * </table>
 */
#ifndef __lks32mc07x_FLASH__
#define __lks32mc07x_FLASH__
#include "lks32mc07x_lib.h"

#define Flash_MAIN 0x000  /**< Flash��mian������*/
#define Flash_NVR  0x800  /**< Flash��mian������*/

extern volatile u32 erase_flag; 
extern volatile u32 progm_flag; 

int ProgramPage(u32 adr, u32 sz, u8 *buf, u16 nvr);
void EraseSector(u32 adr, u16 nvr);
void Read_More_Flash(u32 adr, u32 sz, u32 *buf, u16 nvr);

#endif

