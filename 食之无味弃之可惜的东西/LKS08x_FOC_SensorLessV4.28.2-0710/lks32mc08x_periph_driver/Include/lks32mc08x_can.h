/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_can.h\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� CAN������������ͷ�ļ� \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2022��06��1�� <td>1.0     <td>HuangMG    <td>����
 * </table>
 */
#ifndef __LKS32MC08x_CAN_H_
#define __LKS32MC08x_CAN_H_
/* Includes ------------------------------------------------------------------*/
#include "lks32mc08x_lib.h"

/** 
 *@brief CAN�Ĵ����ṹ����
 */
typedef struct
{
	__IO uint32_t MOD; /**<CAN ģʽ�Ĵ��� */
	__IO uint32_t CMR; /**<CAN ����Ĵ��� */
	__IO uint32_t SR;  /**<CAN ״̬�Ĵ��� */
	__IO uint32_t IR;  /**<CAN �ж�״̬�Ĵ��� */

	__IO uint32_t EIR;  /**<CAN �жϿ��ƼĴ��� */
	__IO uint32_t null0;
	__IO uint32_t BTR0; /**<CAN ����ʱ����ƼĴ��� 0 */
	__IO uint32_t BTR1; /**<CAN ����ʱ����ƼĴ��� 1 */

	__IO uint32_t null1; 
	__IO uint32_t null2;
	__IO uint32_t null3;
	__IO uint32_t ALC;  /**<CAN �ٲö�ʧ��׽�Ĵ��� */

	__IO uint32_t ECC;  /**<CAN �����벶׽�Ĵ��� */
	__IO uint32_t EWLR; /**<CAN ����&��������ֵ���üĴ��� */
	__IO uint32_t RXERR;/**<CAN ���մ�������� */
	__IO uint32_t TXERR;/**<CAN ���ʹ�������� */

	__IO uint32_t TXRX0;/**<��������ģʽ�£�CAN ����֡��ʽ�Ĵ���0/CAN ����֡��ʽ�Ĵ���0*/
	__IO uint32_t TXRX1;/**<��������ģʽ�£�CAN �������ݼĴ���0/CAN �������ݼĴ���1*/
	__IO uint32_t TXRX2;/**<��������ģʽ�£�CAN �������ݼĴ���2/CAN �������ݼĴ���2*/
	__IO uint32_t TXRX3;/**<��������ģʽ�£�CAN �������ݼĴ���3/CAN �������ݼĴ���3*/

	__IO uint32_t TXRX4;/**<��������ģʽ�£�CAN �������ݼĴ���4/CAN �������ݼĴ���4*/
	__IO uint32_t TXRX5;/**<��������ģʽ�£�CAN �������ݼĴ���5/CAN �������ݼĴ���5*/
	__IO uint32_t TXRX6;/**<��������ģʽ�£�CAN �������ݼĴ���6/CAN �������ݼĴ���6*/
	__IO uint32_t TXRX7;/**<��������ģʽ�£�CAN �������ݼĴ���7/CAN �������ݼĴ���7*/

	__IO uint32_t TXRX8;/**<��������ģʽ�£�CAN �������ݼĴ���8/CAN �������ݼĴ���8*/
	__IO uint32_t TXRX9;/**<��������ģʽ�£�CAN �������ݼĴ���9/CAN �������ݼĴ���9*/
	__IO uint32_t TXRXA;/**<��������ģʽ�£�CAN �������ݼĴ���10/CAN �������ݼĴ���10*/
	__IO uint32_t TXRXB;/**<��������ģʽ�£�CAN �������ݼĴ���11/CAN �������ݼĴ���11*/

	__IO uint32_t TXRXC;/**<��������ģʽ�£�CAN �������ݼĴ��� 12/CAN �������ݼĴ��� 12 */
	__IO uint32_t RMC;  /**<CAN FIFO ��Ч������Ϣ������ */
	__IO uint32_t RBSA; /**<CAN ��һ����Ч������Ϣ�� FIFO �еĵ�ַ�Ĵ��� */
} CAN_TypeDef;

/**
 * CAN�ṹ�����ַ����
 */
#ifndef CAN
#define CAN             ((CAN_TypeDef *)CAN_BASE)
#endif

/** 
 *@brief CAN��ʼ���ṹ��������ʼ��ʱ�붨��þ��������������ַ������
 */
typedef struct
{
	uint32_t CAN_WorkMode;	       /**<CAN����ģʽѡ�� 1���Բ�ģʽ 2������ģʽ 3;�����շ�ģʽ*/
	uint32_t CAN_DMAEn;		       /**<ʹ��CAN��DMA����*/
	uint32_t CAN_ErrWarThre;       /**<�����뾯������ֵ*/
	uint32_t CAN_Btr0;		       /**<������ 0*/
	uint32_t CAN_Btr1;	  	       /**<������ 1*/
	uint32_t IRQEna;		       /**<�ж�ʹ��*/
} CAN_InitTypeDef;               
															                  
#define CAN_SELFTEST_MODE  BIT2    /**<CAN����ģʽѡ��,�Բ�ģʽ*/
#define CAN_MONITOR_MODE   BIT1    /**<CAN����ģʽѡ��,����ģʽ*/
#define CAN_NORMAL_MODE    0x0	   /**<CAN����ģʽѡ��,�����շ�ģʽ*/
															   
#define CAN_IE_BUSERR       BIT7   /**<�ж�ʹ�ܶ���,���ߴ����ж�ʹ��*/
#define CAN_IE_LOSTARB      BIT6   /**<�ж�ʹ�ܶ���,��ʧ�ٲ��ж�ʹ��*/
#define CAN_IE_PASSIVEERR   BIT5   /**<�ж�ʹ�ܶ���,������������ж�ʹ��*/
#define CAN_IE_WAKE         BIT4   /**<�ж�ʹ�ܶ���,CANģ��������л����ж�ʹ��*/
#define CAN_IE_RFIFOOV      BIT3   /**<�ж�ʹ�ܶ���,RFIFO���ݷ�������ж�ʹ��*/
#define CAN_IE_WERR         BIT2   /**<�ж�ʹ�ܶ���,���󱨾��ж�ʹ��*/
#define CAN_IE_TXDONE       BIT1   /**<�ж�ʹ�ܶ���,������ϵ�ǰ֡�ж�ʹ��*/
#define CAN_IE_RFIFONOEMPTY BIT0   /**<�ж�ʹ�ܶ���,RFIFI0���µ����ݱ����յ��ж�ʹ��*/


#define CAN_IF_BUSERR BIT7		   /**<���ߴ����жϱ�־*/
#define CAN_IF_LOSTARB BIT6		   /**<��ʧ�ٲ��жϱ�־*/
#define CAN_IF_PASSIVEERR BIT5	   /**<������������жϱ�־*/
#define CAN_IF_WAKE BIT4		   /**<CANģ��������л����жϱ�־*/
#define CAN_IF_RFIFOOV BIT3		   /**<RFIFO���ݷ�������жϱ�־*/
#define CAN_IF_WERR BIT2		   /**<���󱨾��жϱ�־*/
#define CAN_IF_TXDONE BIT1		   /**<������ϵ�ǰ֡�жϱ�־*/
/**<�жϱ�־����,ע��CAN_IRΪ������Ĵ���,ֻ�� BIT0--RFIFO_N_EMPTY_IF �޷��������
��λֻ��ͨ����CAN_CMR BIT2 д 1 ������� RX FIFO �ж�֡���յ����ݣ�CAN_CMR BIT2
д 1 Ҳ�޷����������RX FIFO ȫ�����꣬����һ֡����д CAN_CMR BIT2������������*/
#define CAN_IF_RFIFONOEMPTY BIT0   /**<RFIFI0���µ����ݱ����յ��жϱ�־*/

/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_ONBUS BIT7			   /**<1��CAN ģ�飬���� BUS OFF ״̬�������ݷ��ͽ��ն��� 
									 0��CAN ģ�飬���Է������ݵ� CAN ���ߣ������ CAN ��������*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_ERROV BIT6			   /**<1��CAN ��������Ĵ��������ﵽ�򳬹� CAN_EWL �涨ֵ 
									 0��CAN ��������Ĵ����������� CAN_EWL �涨ֵ*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_TXING BIT5			   /**<1��CAN ģ�����ڷ���һ֡���� 
								     0��CAN ģ��û�з������ݵĶ���*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_RXING BIT4			   /**<1��CAN ģ�����ڽ���һ֡���� 
									 0��CAN ģ��û�н������ݵĶ��� */
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_TRNASDONE BIT3	       /**<1�����һ�δ�������� 
									 0�����һ�δ���δ���*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_TFIFOEMPTY BIT2        /**<1��TFIFO �գ�����д�뷢������ 
								     0��TFIFO �ǿգ��ڲ�����û�з������*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_RFIFOEMPTY BIT1        /**<1��RFIFO �����̫֡�࣬�������������ݶ�ʧ 
								     0��RFIFO δ��*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_DATAAVAIL BIT0	       /**<1��RFIFO ����һ֡���֡���ݣ�����ͨ�� RFIFO �Ĵ�����ȡ 
									 0��RFIFO û����Ч֡����*/
/**<ѡ���ȡCAN_SR״̬����*/
#define CAN_ALLSR 0xff			   /**<��ȡȫ��״̬*/

/**<��¼�����ٲö�ʧ�ľ���λ�ö���*/
typedef enum
{
	CAN_LOST_ID0,  /**<ID�ĵ�һλ*/
	CAN_LOST_ID1,  /**<ID�ĵڶ�λ*/
	CAN_LOST_ID2,  /**<ID�ĵ���λ*/
	CAN_LOST_ID3,  /**<ID�ĵ���λ*/
	CAN_LOST_ID4,  /**<ID�ĵ���λ*/
	CAN_LOST_ID5,  /**<ID�ĵ���λ*/
	CAN_LOST_ID6,  /**<ID�ĵ���λ*/
	CAN_LOST_ID7,  /**<ID�ĵڰ�λ*/
	CAN_LOST_ID8,  /**<ID�ĵھ�λ*/
	CAN_LOST_ID9,  /**<ID�ĵ�ʮλ*/
	CAN_LOST_IDA,  /**<ID�ĵ�ʮһλ*/
	CAN_LOST_SRTR, /**<ID��SRTRλ*/
	CAN_LOST_IDE,  /**<ID��IDEλ*/
} enumLossposition;

/**<���ߴ�������*/
typedef enum
{
	CAN_BITERROR,	 /**<bit����*/
	CAN_FORMATERROR, /**<��ʽ����*/
	CAN_FILLERROR,	 /**<������*/
	CAN_OTHERERROR,	 /**<�������ʹ���*/
} enumErrortype;

/**<���ߴ�����ʱ��*/
typedef enum
{
	CAN_RECE_PROCESS, /**<���ߴ������ڷ��͹�����*/
	CAN_SEND_PROCESS, /**<���ߴ������ڽ��չ�����*/
} enumErrortime;

/**<���ߴ�������λ�ö���*/
typedef enum
{
	null0,
	null1,
	CAN_ID28_ID21,		     /**<ID.28 �� ID.21*/
	CAN_START,			     /**<��ʼ֡*/
	CAN_SRTR,			     /**<SRTR λ*/
	CAN_IDE,			     /**<IDE λ*/
	CAN_ID20_ID18,		     /**<ID.20 �� ID.18*/
	CAN_ID17_ID13,		     /**<ID.17 �� ID.13*/
	CAN_CRC,			     /**<CRC ���ݶ�*/
	CAN_KEEP0,			     /**<����λ���̶�Ϊ 0*/
	CAN_DATA_FIELD,		     /**<Data Field*/
	CAN_DATALENGTHCODE,      /**<Data Length Code*/
	CAN_RTR,			     /**<RTR λ*/
	CAN_KEEP1,			     /**<����λ���̶�Ϊ 1*/
	CAN_ID4_ID0,		     /**<ID.4 �� ID.0*/
	CAN_ID15_ID12,	     	 /**<ID.12 �� ID.5*/
	null2,
	CAN_ACTIVE_ERROR_FLAG,   /**<active error flag*/
	CAN_INTERMISSION,	     /**<Intermission*/
	CAN_TOLERATE_DOMINANT,   /**<Tolerate dominantλ*/
	null3,
	null4,
	CAN_PASSIVE_ERROR,		 /**<passive error flag*/
	CAN_ERROR_DELIMITER,	 /**<Error delimiter*/
	CAN_CRC_DELIMITER,		 /**<CRC delimiter*/
	CAN_CONNFIRM,			 /**<ȷ��λ*/
	CAN_FRAMEEND,			 /**<֡����*/
	CAN_CONNFIRM_DELIMITER,  /**<ȷ�� delimiter*/
	CAN_OVERLOAD_FLAG,		 /**<Overload flag*/
} enumErrposition;


void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct);
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct);
void SFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr, u8 Byte1_acr, u8 Byte1_amr, u8 Byte2_acr, u8 Byte2_amr);
void EFF_ID_Filter(u32 acr, u32 amr, u8 rtr_acr, u8 rtr_amr);
void ID1_Filter_Dual(u32 acr1, u32 amr1, u8 rtr_acr1, u8 rtr_amr1, u8 Byte_acr1, u8 Byte_amr1, u8 ide);
void ID2_Filter_Dual(u32 acr2, u32 amr2, u8 rtr_acr2, u8 rtr_amr2, u8 ide);
u8 My_CAN_Send_Msg(u32 id, u8 ide, u8 rtr, u8 *msg, u8 len);
u8 CAN_Receive_Msg(u32 *id, u8 *ide, u8 *rtr, u8 *buf);
uint8_t CAN_ReadState(CAN_TypeDef *CANx, uint32_t sr);
uint32_t CAN_GetIRQFlag(void);
void CAN_Sleep(u32 Baud);
void CAN_Manual_Awake(void);

#endif
