/*******************************************************************************
 * ��Ȩ���� (C)2015, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� LKS32MC_061_flash.h
 * �ļ���ʶ��
 * ����ժҪ�� flash������������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� Howlet
 * ������ڣ� 2016��3��25��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2016��3��25��
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
#ifndef __lks32mc08x_FLASH__
#define __lks32mc08x_FLASH__

void EraseSector (unsigned long adr);
void ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf);

#endif

