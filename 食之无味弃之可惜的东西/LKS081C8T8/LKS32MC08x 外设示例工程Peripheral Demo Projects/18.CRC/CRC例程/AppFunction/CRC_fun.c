/*******************************************************************************
 * ��Ȩ���� (C)2021, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� CRC_fun.c
 * �ļ���ʶ��
 * ����ժҪ�� Ӳ��CRCУ�����
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ�
 * ������ڣ� 2021��3��12��
 *
 * �޸ļ�¼1��
 * �޸����ڣ�2021��3��12��
 * �� �� �ţ�V 1.0
 * �� �� �ˣ�
 * �޸����ݣ�

 * �޸ļ�¼2��
 * �޸����ڣ�2021��7��05��
 * �� �� �ţ�V 1.1
 * �� �� �ˣ�yangzj
 * �޸����ݣ���ȫcrc��ʽ
 
 * �޸ļ�¼2��
 * �޸����ڣ�2021��3��24��
 * �� �� �ţ�V 1.2
 * �� �� �ˣ�yangzj
 * �޸����ݣ��޸�CRC16/CRC32�������ݱ���Ϊu16/u32������
 *
 *******************************************************************************/
#include "lks32mc08x.h"
#include "CRC_fun.h"
#include "basic.h"
/*******************************************************************************
 �������ƣ�    u32 crc32(u32 *pData,u32 uLen)
 ����������    crc32�������
 ���������    *pData ָ�������crc������
			   uLen  ���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u32 crc32(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, val_REFOUT, val_REFIN, POLY_SIZE;
	u32 val_XOROUT, val_POLY, val_INIT;

	val_POLY = 0x04c11db7;
	val_INIT = 0xffffffff;
	val_XOROUT = 0xffffffff;

	val_REFOUT = ENABLE; //������ݷ�תʹ��
	val_REFIN = ENABLE;	 //�������ݷ�תʹ��

	REV_OUT_TYPE = val_REFOUT;
	REV_IN_TYPE = (val_REFIN) ? 3 : 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 0;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = val_POLY;
	CRC_INIT = val_INIT;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	return (REG32(&CRC_DR)) ^ val_XOROUT;
}

/*******************************************************************************
 �������ƣ�    u32 crc32_MPEG_2(u32 *pData,u32 uLen)
 ����������    crc32_MPEG_2����
 ���������    *pData ָ�������crc������
			   uLen  ���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u32 crc32_MPEG_2(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, val_REFOUT, val_REFIN, POLY_SIZE;
	u32 val_XOROUT, val_POLY, val_INIT;

	val_POLY = 0x04c11db7;	 //����ʽ
	val_INIT = 0xffffffff;	 //��ʼֵ
	val_XOROUT = 0x00000000; //������ֵ

	val_REFOUT = DISABLE; //������ݷ�תʹ��
	val_REFIN = DISABLE;  //�������ݷ�תʹ��

	REV_OUT_TYPE = val_REFOUT;
	REV_IN_TYPE = (val_REFIN) ? 3 : 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 0;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = val_POLY;
	CRC_INIT = val_INIT;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	return (REG32(&CRC_DR)) ^ val_XOROUT;
}
u16 crc16_IBM(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;
	out_XOR = 0x0000;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��
	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x8005; // crc16���㹫ʽ
	CRC_INIT = 0x0000;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			if (REV_IN_TYPE)
			{
				REG8(&CRC_DR) = *pData;
			}
			else
			{
				REG8(&CRC_DR) = (((*pData & 0xff00) >> 8) | ((*pData & 0x00ff) << 8));
			}
			// crc_dr��ַ��32λ������
			//����CRC8��CRC16ʱ��Ҫ����8/16λд��
			//����32λд�����ݻ���ɴ���Ľ��
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_MAXIM(u16 *pData,u16 uLen)
 ����������    crc16_MAXIM�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_MAXIM(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;
	out_XOR = 0xffff;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��
	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x8005; // crc16���㹫ʽ
	CRC_INIT = 0x0000;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_USB(u16 *pData,u16 uLen)
 ����������    crc16_USB�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_USB(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0xffff;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x8005; // crc16���㹫ʽ
	CRC_INIT = 0xffff;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_MODBUS(u16 *pData,u16 uLen)
 ����������    crc16_MODBUS�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_MODBUS(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0x0000;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x8005; // crc16���㹫ʽ
	CRC_INIT = 0xffff;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_CCITT(u16 *pData,u16 uLen)
 ����������    crc16_CCITT�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_CCITT(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0x0000;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x1021; // crc16���㹫ʽ
	CRC_INIT = 0x0000;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_CCITT_FALSE(u16 *pData,u16 uLen)
 ����������    crc16_CCITT_FALSE�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_CCITT_FALSE(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0x0000;
	REV_OUT_TYPE = DISABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x1021; // crc16���㹫ʽ
	CRC_INIT = 0xffff;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_X25(u16 *pData,u16 uLen)
 ����������    crc16_X25�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_X25(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0xffff;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x1021; // crc16���㹫ʽ
	CRC_INIT = 0xffff;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_XMODEM(u16 *pData,u16 uLen)
 ����������    crc16_XMODEM�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_XMODEM(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;
	out_XOR = 0x0000;
	REV_OUT_TYPE = DISABLE; //������ݷ�תʹ��
	REV_IN_TYPE = 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x1021; // crc16���㹫ʽ
	CRC_INIT = 0x0000;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc16_DNP(u16 *pData,u16 uLen)
 ����������    crc16_DNP�㷨�ο�
 ���������    *pDataָ�������crc������
			   uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u16 crc16_DNP(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u16 out_XOR;

	out_XOR = 0xffff;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 2;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 1;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0;	  //��λ
	CRC_POL = 0x3d65; // crc16���㹫ʽ
	CRC_INIT = 0x0000;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u16)(CRC_DR >> 16)) ^ out_XOR;
	}
	else
	{
		return (REG16(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u8 crc8(u8 *pData,u32 uLen)
 ����������    crc8�������
 ���������    *pDataָ�������crc������
			        uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 crc8(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u8 out_XOR;
	out_XOR = 0x00;
	REV_OUT_TYPE = DISABLE; //������ݷ�תʹ��
	REV_IN_TYPE = 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 2;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = 0x07; // crc8���㹫ʽ
	CRC_INIT = 0x00;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u8)(CRC_DR >> 24)) ^ out_XOR;
	}
	else
	{
		return (REG8(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u8 crc8_ITU(u8 *pData,u32 uLen)
 ����������    crc8_ITU�������
 ���������    *pDataָ�������crc������
			        uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 crc8_ITU(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u8 out_XOR;
	out_XOR = 0x55;
	REV_OUT_TYPE = DISABLE; //������ݷ�תʹ��
	REV_IN_TYPE = 0;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 2;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = 0x07; // crc8���㹫ʽ
	CRC_INIT = 0x00;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u8)(CRC_DR >> 24)) ^ out_XOR;
	}
	else
	{
		return (REG8(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc8_ROHC(u16 *pData,u16 uLen)
 ����������    crc8_ROHC�㷨�ο�
 ���������    *pDataָ�������crc������
			  uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 crc8_ROHC(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u8 out_XOR;

	out_XOR = 0x00;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 1;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 2;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = 0x07; // crc8���㹫ʽ
	CRC_INIT = 0xff;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			// crc_dr��ַ��32λ������
			//����CRC8��CRC16ʱ��Ҫ����8/16λд��
			//����32λд�����ݻ���ɴ���Ľ��
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u8)(CRC_DR >> 24)) ^ out_XOR;
	}
	else
	{
		return (REG8(&CRC_DR)) ^ out_XOR;
	}
}
/*******************************************************************************
 �������ƣ�    u16 crc8_MAXIM(u16 *pData,u16 uLen)
 ����������    crc8_MAXIM�㷨�ο�
 ���������    *pDataָ�������crc������
			        uLen���鳤��
 ���������    ��
 �� �� ֵ��    ��
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------
 *******************************************************************************/
u8 crc8_MAXIM(u8 *pData, u32 uLen)
{
	u8 REV_OUT_TYPE, REV_IN_TYPE, POLY_SIZE;
	u8 out_XOR;

	out_XOR = 0x00;
	REV_OUT_TYPE = ENABLE; //������ݷ�תʹ��

	REV_IN_TYPE = 1;
	/*
		REV_IN_TYPE
		���������ݷ�ת����
		0:����ת
		1:���ֽڷ�ת���� b[31]=b[24], b[30]=b[25], ��, b[24]=b[31], ��, b[7]=b[0], b[6]=b[1], ��, b[0]=b[7]
		2:�����֣� 16bit �� �� ת �� �� b[31]=b[16], b[30]=b[17], ��, b[16]=b[31], ��, b[15]=b[0], b[14]=b[1], ��, b[0]=b[15]
		3:���ַ�ת���� b[31]=b[0], b[30]=b[1],�� [b0]=b[31]
	*/
	POLY_SIZE = 2;
	/*
		������루����ʽ��λ��
		0: 32bits
		1: 16bits
		2: 8bits
		3: 7bits
	*/
	CRC_CR = (REV_OUT_TYPE << 12 | REV_IN_TYPE << 8 | POLY_SIZE << 4);
	CRC_CR |= BIT0; //��λ
	CRC_POL = 0x31; // crc8���㹫ʽ
	CRC_INIT = 0x00;
	if (uLen == 0)
		return 0;
	else
		while (uLen--)
		{
			REG8(&CRC_DR) = *pData;
			// crc_dr��ַ��32λ������
			//����CRC8��CRC16ʱ��Ҫ����8/16λд��
			//����32λд�����ݻ���ɴ���Ľ��
			pData++;
		}
	if (REV_OUT_TYPE == ENABLE)
	{
		return ((u8)(CRC_DR >> 24)) ^ out_XOR;
	}
	else
	{
		return (REG8(&CRC_DR)) ^ out_XOR;
	}
}
