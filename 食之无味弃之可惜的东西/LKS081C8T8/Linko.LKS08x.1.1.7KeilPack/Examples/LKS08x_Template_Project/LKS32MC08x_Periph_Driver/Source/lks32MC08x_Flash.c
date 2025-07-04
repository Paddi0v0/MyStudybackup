/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� lks32mc081_flash.c
 * �ļ���ʶ��
 * ����ժҪ�� Flash������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2019��3��5��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2019��3��5��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�Howlet
 * �޸����ݣ�����
 *
 * �޸ļ�¼2��
 * �޸����ڣ�
 * �� �� �ţ�
 * �� �� �ˣ�
 * �޸����ݣ�
 *
 *******************************************************************************/
#include "basic.h"
#include "lks32mc08x.h"

void EraseSector(unsigned long adr)
{


	  FLASH_CFG &=~0x88008800;
	  FLASH_ADDR   = adr;
    FLASH_CFG   |= 0x88000000;//enable erase & prog
    FLASH_ERASE  = 0x7654DCBA;//trig sector erase
	  FLASH_CFG &=~0x88008800;

}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{

    /* Add your Code */
    volatile unsigned int Addr, size;
    volatile unsigned char *p;
    volatile unsigned t_rlt, t_Cnt;


    t_Cnt = 0;


ReErase:
    p = buf;
    size = sz;
    Addr = adr;

    EraseSector(adr);

    FLASH_CFG &=~0x88008800;
    FLASH_CFG   |= 0x08000000;

    size = (size + 3) & ~3;                     // align the word
    while(size)
    {
        FLASH_ADDR = (Addr & ~0x00000003);      // Address alignment
        FLASH_WDATA= *((u32 *)p);             // Write Data to Flash
			

        p += 4;
        size -= 4;                       // Go to next Word
        Addr += 4;
    }

		for(Addr=0; Addr<0xffff; Addr++);
    p = buf;
    size = sz;
    Addr = adr;
    t_rlt = 0;
    while(size)
    {

        if(*((u32 *)p) != *((u32 *)Addr))
        {
            t_rlt = 1;
        }

        p += 4;
        size -= 4;
        Addr+=4;
    }

    t_Cnt++;

    if(t_Cnt < 3)
    {
        if(t_rlt)
        {
            goto ReErase;
        }
    }

    FLASH_CFG &=~0x88008800;
    return (0);                             // Finished without Errors
}



