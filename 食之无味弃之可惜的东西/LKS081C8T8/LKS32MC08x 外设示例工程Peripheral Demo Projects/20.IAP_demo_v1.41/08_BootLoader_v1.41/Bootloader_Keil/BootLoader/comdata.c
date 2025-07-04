#include "comdata.h"
#include "lks32MC08x_Flash.h"

UINT16 tickCount = 0;
UINT16 volatile recvLen = 0;				// �ѽ������ݳ���
UINT8  recvData[MAXRECV_LEN] = {0};			// ��������
UINT16 recvHex = 0;							// �ѽ���hex�ļ�WORD�� charToLong
UINT8  bStarted = FALSE;					// �����Ƿ�ʼ�ı�־

UINT32 fileLength = 0;
UINT32 fileCrc = 0;

extern volatile u8 jumpFlg;


void InitRecvData(void)
{
    UINT8 i;

    for(i = 0; i < MAXRECV_LEN; i++)
    {
        recvData[i] = 0;
    }
    recvLen = 0;
}

void InitHexData(void)
{
    recvHex = 0;
}

void SendToPC(UINT8 byCMD)
{
    UINT8 i;
    UINT8 pData[8];
    pData[0] = 0xEB;
    pData[1] = 0x90;
    pData[2] = 0xEB;
    pData[3] = 0x90;
    pData[4] = 0xEB;
    pData[5] = 0x90;
    pData[6] = 8;
    pData[7] = byCMD;

    for (i = 0; i < 8; i++)
    {
		u16 dealy = 60000;
		
        while(!(UART0_IF & BIT2))
		{
			if(dealy > 0)
			{
				dealy--;
			}
			else
			{
				break;
			}
		}
		
        UART0_IF = BIT2;
        UART0_BUFF = pData[i];
    }
}

UINT8 CalcByteCRC(UINT8 *pData, UINT8 len)
{
    UINT8 i;
    UINT8 byCRC = 0;
    for (i = 0; i < len; i++)
    {
        byCRC ^= pData[i];
    }
    return byCRC;
}

void AddRecvData(u8 data)
{
	if (recvLen == 0 && data != 0xEB)
	{
		return;
	}
	if (recvLen >= MAXRECV_LEN)
	{
		return;
	}

	recvData[recvLen] = data;
	recvLen++;							//�ѽ������ݳ���
}

void CheckRecvData(void)
{
	UINT8 byGramLen;
    if(recvLen < MINRECV_LEN)			//�ռ������ݲ�����̵���Ч���ĳ���
    {
        return;
    }

    if(ISSYNC(recvData))
    {
		UTIMER_IE = 0x00;
		byGramLen = recvData[INDEX_LEN];
		if (byGramLen > MAXRECV_LEN)	// ���ȱ�ʶ�쳣
		{
			NVIC_DisableIRQ(UART0_IRQn);
			HandleError();			
			NVIC_EnableIRQ(UART0_IRQn);
			return;
		}
		
        if(recvLen >= byGramLen)		// �չ�һ������
        {
			NVIC_DisableIRQ(UART0_IRQn);
            HandleCmd();
			NVIC_EnableIRQ(UART0_IRQn);
        }
    }
    else																														//������һ���ֽ�
    {
		NVIC_DisableIRQ(UART0_IRQn);
		HandleError();
		NVIC_EnableIRQ(UART0_IRQn);
    }
}

void HandleCmd(void)
{
	UINT8 byCMD = recvData[INDEX_CMD];
	switch (byCMD)
	{
		case CMD_DOWNREQ:
			HandleDownload();
			break;
		case CMD_VERIFYREQ:
			HandleVerify();
			break;
		case CMD_FILE:
			HandleFileData();
			break;
		default:
			break;
	}
}

void HandleError(void)
{
	InitRecvData();
}

void HandleDownload(void)
{
	if(bStarted)
	{
		InitRecvData();
		return;
	}
	
	SendToPC(CMD_REQCONF);							//CMD_DOWNCONF
	bStarted = TRUE;
	UTIMER_UNT0_CFG &= ~BIT10;						//TIM_TimerCmd(TIMER0, DISABLE);
	//erase flash
	fileLength = CharToLong(&recvData[8]);			//�ļ����ԣ��ܳ���
	fileCrc = CharToLong(&recvData[12]);			//�ļ����ԣ��ļ�CRC
	if(fileLength <= 0)
	{
		InitRecvData();
		InitHexData();
		return;
	}
	
	UINT32 nSections = fileLength / 512;
	if(fileLength % 512 > 0)	nSections += 1;
	for(UINT32 i = 0; i < nSections; i++)
	{
		EraseSector(APP_ADDR + i * 512);
		for(UINT32 i = 0; i < 5000; i++) {}
	}
	
	//send finish erase
	SendToPC(CMD_ERASEOK);
	InitRecvData();
	InitHexData();	
}

void HandleVerify(void)
{
	if(bStarted)
	{
		InitRecvData();
		return;
	}
	
	SendToPC(CMD_REQCONF);							//CMD_DOWNCONF
	bStarted = TRUE;
	UTIMER_UNT0_CFG &= ~BIT10;						//TIM_TimerCmd(TIMER0, DISABLE);
	
	//verify flash
	fileLength = CharToLong(&recvData[8]);			//�ļ����ԣ��ܳ���
	fileCrc = CharToLong(&recvData[12]);			//�ļ����ԣ��ļ�CRC
	
	for(UINT32 i = 0; i < 20000; i++) {}
	DoVerify();
	
	InitRecvData();
	InitHexData();
}

void HandleFileData(void)
{
	UINT8 t_FlashBuf[MAXFILEDATA_LEN] = {0};
	
	UINT8 byGramLen = recvData[INDEX_LEN];
	UINT32 nFileDataLen = byGramLen - 10;
	if((nFileDataLen % 4) != 0)							//�ж��ļ����ݳ����Ƿ�Ϊ4�ı���
	{
		SendToPC(CMD_ERRLEN);							//CMD_ERRLEN
		InitRecvData();
		InitHexData();
		return;
	}
	UINT8 byCRC = CalcByteCRC((UINT8*)(recvData + INDEX_FILEDATA), nFileDataLen);	//�ж�CRC
	if(byCRC != (UINT8)recvData[byGramLen - 1])
	{
		SendToPC(CMD_ERRCRC);								//CMD_ERRCRC
		InitRecvData();
		InitHexData();
		return;
	}
	for(UINT32 i = 0; i < nFileDataLen; i++)				//�ļ�����д��flash��
	{
		t_FlashBuf[i] = recvData[INDEX_FILEDATA + i];
	}

	ProgramPage_WrtieOnly(APP_ADDR + recvHex, nFileDataLen, t_FlashBuf);		// flashд��
	recvHex += nFileDataLen;
	SendToPC(CMD_DATACONFIRM);
	
	UINT8 byContinue = recvData[INDEX_CONTI];				//�жϺ�����־λ
	if(byContinue == 0)
	{
		SendToPC(CMD_DOWNFINISH);
		
		for(UINT32 i = 0; i < 20000; i++) {}				//delay����ȷ����������ʱ�����ܱ�֤����������ȷ			
		DoVerify();
	}
	InitRecvData();	
}

void DoVerify(void)
{
	UINT32 crc = 0;
	UINT32 data = 0;
	for (UINT32 addr = APP_ADDR; addr < APP_ADDR + fileLength; addr += 4)
	{
		FLASH_ADDR = addr;
		data = FLASH_RDATA;
		crc ^= data;
	}
	
	if (crc == fileCrc)
	{
		SendToPC(CMD_VERIFYOK);
		for(UINT32 i = 0; i < 50000; i++) {}
		jumpFlg = 1;
	}
	else
	{
		SendToPC(CMD_VERIFYFAIL);
		for(UINT32 i = 0; i < 20000; i++) {}
		NVIC_SystemReset();
	}
}

typedef  void (*iapfun)(void);
void StartUserProc(void)
{
	iapfun jump2app;

	GPIO0_PIE = 0x00;
	GPIO1_POE = 0x00;
	GPIO0_FBA98 = 0x00;
	GPIO1_F3210 = 0x00;

	UART0_CTRL = 0x00;
	UART0_DIVH = 0x00;
	UART0_DIVL = 0x00;
	UART0_IE = 0x00;

	NVIC_DisableIRQ(UART0_IRQn);
	NVIC_DisableIRQ(TIMER0_IRQn);
	
	__disable_irq();
	jump2app=(iapfun)*(UINT32 *)(APP_ADDR+4);		
	__set_MSP(*(volatile u32 *)APP_ADDR);
	jump2app();
}

UINT32 CharToLong(UINT8 *pData)
{
    UINT8 pTmpData[4];
    pTmpData[0] = pData[0];
    pTmpData[1] = pData[1];
    pTmpData[2] = pData[2];
    pTmpData[3] = pData[3];
    return *((UINT32*)pTmpData);
}

