#ifndef COMDATA_H_
#define COMDATA_H_

#include "lks32mc08x.h"


#define    APP_ADDR     0XA00 

#define MAXSEND_LEN  		8
#define MAXFILEDATA_LEN		200

#define MINRECV_LEN		    12
#define MAXRECV_LEN		    (MAXFILEDATA_LEN+10)	//���ձ�����󳤶�

#define INDEX_LEN		6				//���ĳ��ȵ��ֽ����
#define INDEX_CMD		7				//�������͵��ֽ����
#define INDEX_VERIFYCRC	8
#define INDEX_CONTI		8				//�ļ����ݺ�����־λ�ֽ����
#define INDEX_FILEDATA	9				//�ļ�������ʼ�ֽ����

#define CMD_VERIFYREQ	0x05			//У������
#define CMD_DOWNREQ		0x06			//��д����
#define CMD_REQCONF		0x07			//ȷ��

#define CMD_ERASEOK		0x17			//flash������ϣ������·��ļ�
#define CMD_DOWNFINISH	0x08			//chip�����������
#define CMD_FILE		0x2B			//
#define CMD_ERRCRC		0x09			//
#define CMD_ERRLEN		0x0A			//
#define CMD_DATACONFIRM	0x0B			//chip��������ȷ��

#define CMD_VERIFYOK	0x0C
#define CMD_VERIFYFAIL	0x0D


// ͬ����EB90 EB90 EB90
#define ISSYNC(buf)	\
	( (buf[0]==0xEB) && (buf[2]==0xEB) && (buf[4]==0xEB) && \
      (buf[1]==0x90) && (buf[3]==0x90) && (buf[5]==0x90) )
	
void InitCom(void);														//��ʼ��ͨ�Ŵ��ڲ���

void InitRecvData(void);
void InitHexData(void);
void AddRecvData(u8 data);

void CheckRecvData(void);
void HandleCmd(void);
void HandleError(void);

void HandleDownload(void);
void HandleVerify(void);
void HandleFileData(void);
void DoVerify(void);

void SendToPC(UINT8 byCMD);
void StartUserProc(void);

UINT8 CalcByteCRC(UINT8 *pData, UINT8 len);
UINT32 CharToLong(UINT8 *pData);

#endif

