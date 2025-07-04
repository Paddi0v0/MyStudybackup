/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2011 Freescale Semiconductor
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file   freemaster_serial.c
*
* @brief  FreeMASTER serial communication routines
*
* @version 1.1.28.0
* 
* @date Jun-6-2011
* 
*******************************************************************************/

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_protocol.h"
#include "lks32mc08x.h"

void UART2_SENDDATA(UINT8 n);

volatile unsigned char uart_tx_en;

#if FMSTR_USE_SERIAL

/***********************************
*  local variables 
***********************************/
void FMSTR_RxQueue(FMSTR_BCHR nRxChar);
FMSTR_BOOL FMSTR_Rx(FMSTR_BCHR nRxChar);
/* FreeMASTER communication buffer (in/out) plus the STS and LEN bytes */
static FMSTR_BCHR pcm_pCommBuffer[FMSTR_COMM_BUFFER_SIZE+4];    

/* FreeMASTER runtime flags */
/*lint -e{960} using union */
typedef volatile union 
{
    FMSTR_FLAGS all;
    
    struct
    {
        unsigned bTxActive : 1;         /* response is being transmitted */
#if FMSTR_USE_SCI
        unsigned bTxWaitTC : 1;         /* response sent, wait for transmission complete */
#endif
        unsigned bTxLastCharSOB   : 1;  /* last transmitted char was equal to SOB  */
        unsigned bRxLastCharSOB   : 1;  /* last received character was SOB */
        unsigned bRxMsgLengthNext : 1;  /* expect the length byte next time */
#if FMSTR_USE_JTAG
        unsigned bJtagRIEPending  : 1;  /* JTAG RIE bit failed to be set, try again later */
#endif
#if FMSTR_USE_USB_CDC || FMSTR_USE_MQX_IO || FMSTR_USE_JTAG
        unsigned bTxFirstSobSend  : 1;  /* to send SOB char at the begin of the packet */
#endif
#if FMSTR_USE_MQX_IO
        unsigned bMqxReadyToSend  : 1;  /* to send next character in transmit routine */
#endif
#if FMSTR_USE_USB_CDC
        unsigned bUsbCdcStartApp   : 1; /* FreeMASTER USB CDC Application start Init Flag */
        unsigned bUsbCdcStartTrans : 1; /* FreeMASTER USB CDC Application Carrier Activate Flag */
        unsigned bUsbReadyToDecode : 1; /* FreeMASTER packet is received, ready to decode in Poll function in Short Interrupt mode */
#endif
    } flg;
    
} FMSTR_SERIAL_FLAGS;

static FMSTR_SERIAL_FLAGS pcm_wFlags;

/* receive and transmit buffers and counters */
static FMSTR_SIZE8 pcm_nTxTodo;     /* transmission to-do counter (0 when tx is idle) */
static FMSTR_SIZE8 pcm_nRxTodo;     /* reception to-do counter (0 when rx is idle) */
static FMSTR_BPTR  pcm_pTxBuff;     /* pointer to next byte to transmit */
static FMSTR_BPTR  pcm_pRxBuff;     /* pointer to next free place in RX buffer */
static FMSTR_BCHR  pcm_nRxCheckSum; /* checksum of data being received */

/***********************************
*  local function prototypes
***********************************/

static void FMSTR_Listen(void);
static void FMSTR_SendError(FMSTR_BCHR nErrCode);

#else /* FMSTR_USE_SERIAL */

/*lint -efile(766, freemaster_protocol.h) include file is not used in this case */

#endif /* FMSTR_USE_SERIAL */

#if FMSTR_USE_SCI || FMSTR_USE_JTAG
/***********************************
*  local variables 
***********************************/

/* SHORT_INTR receive queue (circular buffer) */
#if FMSTR_SHORT_INTR
static FMSTR_BCHR  pcm_pRQueueBuffer[FMSTR_COMM_RQUEUE_SIZE];
static FMSTR_BPTR  pcm_pRQueueRP;   /* SHORT_INTR queue read-pointer */
static FMSTR_BPTR  pcm_pRQueueWP;   /* SHORT_INTR queue write-pointer */
#endif

/***********************************
*  local function prototypes
***********************************/



static void FMSTR_RxDequeue(void);


/*lint -esym(752,FMSTR_RxQueue) this may be unreferenced in some cases */
/*lint -esym(752,FMSTR_RxDequeue) this may be unreferenced in some cases */

/*******************************************************************************
*
* @brief    Routine to quick-receive a character (put to a queue only)
*
* This function puts received character into a queue and exits as soon as possible.
*
*******************************************************************************/


void FMSTR_RxQueue(FMSTR_BCHR nRxChar)
{
    /* future value of write pointer */
    FMSTR_BPTR wpnext = pcm_pRQueueWP + 1;
    
    /*lint -e{946} pointer arithmetic is okay here (same array) */
    if(wpnext >= (pcm_pRQueueBuffer + FMSTR_COMM_RQUEUE_SIZE))
    {
        wpnext = pcm_pRQueueBuffer;
    }
    
    /* any space in queue? */
    if(wpnext != pcm_pRQueueRP)
    {
        *pcm_pRQueueWP = (FMSTR_U8) nRxChar;
        pcm_pRQueueWP = wpnext;
    }
}


/*******************************************************************************
*
* @brief    Late processing of queued characters
*
* This function takes the queued characters and calls FMSTR_Rx() for each of them,
* just like as the characters would be received from SCI one by one.
*
*******************************************************************************/

#if FMSTR_SHORT_INTR

static void FMSTR_RxDequeue(void)
{ 
    FMSTR_BCHR nChar = 0U;
    
    /* get all queued characters */
    while(pcm_pRQueueRP != pcm_pRQueueWP)
    {
        nChar = *pcm_pRQueueRP++;

        /*lint -e{946} pointer arithmetic is okay here (same array) */
        if(pcm_pRQueueRP >= (pcm_pRQueueBuffer + FMSTR_COMM_RQUEUE_SIZE))
        {
            pcm_pRQueueRP = pcm_pRQueueBuffer;
        }
        
        /* emulate the SCI receive event */
        if(!pcm_wFlags.flg.bTxActive)
        {
            (void)FMSTR_Rx(nChar);
        }
    }
}

#endif /* FMSTR_SHORT_INTR */

#endif /* FMSTR_USE_SCI || FMSTR_USE_JTAG */

#if FMSTR_USE_SCI

/**************************************************************************//*!
*
* @brief    Handle SCI communication (both TX and RX)
*
* This function checks the SCI flags and calls the Rx and/or Tx functions
*
* @note This function can be called either from SCI ISR or from the polling routine
*
******************************************************************************/

void FMSTR_ProcessSCI(void)
{
    /* read & clear status     */
    /* FMSTR_SCISR wSciSR = FMSTR_SCI_RDCLRSR();*/
    FMSTR_SCISR wSciSR = UART_IF_FLG;

    /* transmitter active and empty? */
      /* if (pcm_wFlags.flg.bTxActive)*/
    if (USART_TX_EN_FLG())
       {
        /* able to accept another character? */
        /* if(wSciSR & FMSTR_SCISR_TDRE)*/
        if(wSciSR & USART_FLAG_TX_EMPT)
        {
            FMSTR_U8 ch;
            //(void)FMSTR_Tx(&ch);
            /* just put the byte into the SCI transmit buffer */
					 CLEAR_TX_EMPT();
           if(!FMSTR_Tx(&ch))
           /***FMSTR_SCI_PUTCHAR((FMSTR_U8) ch);***/
            USART_SendData(ch);
        }
        
#if FMSTR_SCI_HAS_TXQUEUE
        /* waiting for transmission complete flag? */
       /* if(pcm_wFlags.flg.bTxWaitTC && (wSciSR & FMSTR_SCISR_TC))*/
        if(wSciSR & USART_FLAG_TC)
        {
            /* after TC, we can switch to listen mode safely */
					  CLEAR_TX_TC();
            FMSTR_Listen();
        }
#endif


#if !FMSTR_SCI_TWOWIRE_ONLY
        /* read-out and ignore any received character (loopback) */
        /*if(wSciSR & FMSTR_SCISR_RDRF)*/
        if(wSciSR & USART_FLAG_RXNE)
        {
            /*lint -esym(550, nRxChar) */
            volatile FMSTR_U16 nRxChar;
            /*nRxChar = FMSTR_SCI_GETCHAR(); */
            nRxChar = USART_ReceiveData();
					  CLEAR_RXNE();
        }
#endif
    }
    /* transmitter not active, able to receive */
    else
    {
        /* data byte received? */
        /*if (wSciSR & FMSTR_SCISR_RDRF)*/
        if(wSciSR &USART_FLAG_RXNE)
        {
            FMSTR_BCHR nRxChar = 0U;
            nRxChar = USART_ReceiveData();
					  CLEAR_RXNE();
            FMSTR_RxQueue(nRxChar);
         
        }
    }
}

#endif /* FMSTR_USE_SCI */


#if FMSTR_USE_JTAG

/***********************************
*  local function prototypes
***********************************/

static void FMSTR_FlushJtagTx(void);

/**************************************************************************//*!
*
* @brief    Flush one communication JTAG word 
*
* This function gets the 4 bytes from FreeMASTER TX buffer and place them to 
* JTAG transmit register.
*
******************************************************************************/

static void FMSTR_FlushJtagTx(void)
{
    FMSTR_U8 i;
    FMSTR_U32 wJtagTxData;
    register FMSTR_U16 wJtagSR = FMSTR_JTAG_GETSR();
    /* complete word to send */
    for (i=0; i<4; i++)
    {
        FMSTR_U8 ch;
        if (FMSTR_Tx(&ch))
            ch = 0xff;
        wJtagTxData = (wJtagTxData << 8) | ch;
    }

    /* send the word just completed */
    FMSTR_JTAG_PUTDWORD(wJtagTxData);
}

/**************************************************************************//*!
*
* @brief    Handle JTAG communication (both TX and RX)
*
* This function checks the JTAG flags and calls the Rx and/or Tx functions
*
* @note This function can be called either from JTAG ISR or from the polling routine
*
******************************************************************************/

void FMSTR_ProcessJTAG(void)
{
    /* read & clear status     */
    register FMSTR_U16 wJtagSR = FMSTR_JTAG_GETSR();

    /* transmitter active? */
    if (pcm_wFlags.flg.bTxActive)
    {
        /* able to transmit a new character? (TX must be empty = read-out by PC) */
        if(!(wJtagSR & FMSTR_JTAG_OTXRXSR_TDF))
        {
        
#if FMSTR_USE_JTAG_TXFIX
            /* if TDF bit is useless due to silicon bug, use the RX flag */
            /* instead (PC sends us a dummy word to kick the RX flag on) */
            if(wJtagSR & FMSTR_JTAG_OTXRXSR_RDF)
#endif
            {
                FMSTR_FlushJtagTx();
            }               
        }

        /* ignore (read-out) the JTAG-received word */
        if(wJtagSR & FMSTR_JTAG_OTXRXSR_RDF)
        {
            /*lint -esym(550, nRxWord) */
            volatile FMSTR_U16 nRxWord;
            nRxWord = FMSTR_JTAG_GETWORD();
        }
    }
    /* transmitter not active */
    else
    {
        /* JTAG 32bit word (four bytes) received? */
        if(wJtagSR & FMSTR_JTAG_OTXRXSR_RDF)
        {
            register FMSTR_U32 nRxDWord;
            FMSTR_INDEX i;
            
            nRxDWord = FMSTR_JTAG_GETDWORD();
            
            /* process all bytes, MSB first */
            for(i=0; i<4; i++)
            {
#if FMSTR_SHORT_INTR
                FMSTR_RxQueue((FMSTR_BCHR)((nRxDWord >> 24U) & 0xffU));
                
#else
                (void)FMSTR_Rx((FMSTR_BCHR)((nRxDWord >> 24U) & 0xffU));
            
                /* ignore the rest if previous bytes triggered a transmission */
                /* (i.e. the packet was complete and only filled-up to 32bit word) */
                if(pcm_wFlags.flg.bTxActive)
                {
                    break;
                }
#endif          
                /* next byte of 32bit word */
                nRxDWord = nRxDWord << 8;
            }
        }
    }
}

#endif /* FMSTR_USE_JTAG */

/******************************************************************************
****************************************************************************//*!
*
* FreeMASTER MQX IO serial communication routines
* 
*******************************************************************************/

#if FMSTR_USE_MQX_IO

#include <mqx.h>
#include <bsp.h>

/***********************************
*  local variables 
***********************************/

static FILE_PTR devfd = NULL;       /* pointer to open FreeMASTER communication interface */

/***********************************
*  local function prototypes
***********************************/

static FMSTR_BOOL FMSTR_InitMQX(void);
static void FMSTR_ProcessMQXIO(void);

/**************************************************************************//*!
*
* @brief    MQX communication interface initialization
*
******************************************************************************/

static FMSTR_BOOL FMSTR_InitMQX(void)
{   
    /* Open communication port */
    devfd = fopen (FMSTR_MQX_IO_CHANNEL, (pointer) FMSTR_MQX_IO_PARAMETER);

    return (devfd != NULL);
}

/*******************************************************************************
*
* @brief    Handle MQX IO serial communication (both TX and RX)
*
* This function calls MQX IO fread() function to get character and process it by 
* 
* FMSTR_Rx function when FreeMASTER packet is receiving. This function also transmit
*
* FreeMASTER response. Character to be send is provided by call of FMSTR_Tx function 
* 
* and passed down to fwrite() function.
*
*******************************************************************************/

static void FMSTR_ProcessMQXIO(void)
{ 
    if (devfd != NULL)
    {
         static FMSTR_U8 TxChar = 0;
        /* transmitter not active, able to receive */
        if ((!pcm_wFlags.flg.bTxActive) && (!pcm_wFlags.flg.bMqxReadyToSend))
        {
            FMSTR_U8 nRxChar;
            /* read all available bytes from communication interface */
            while (fread(&nRxChar, 1, 1, devfd)) { 
                if(FMSTR_Rx(nRxChar))
                    break;
            }
        }

        /* transmitter active and empty? */
        if (pcm_wFlags.flg.bTxActive || pcm_wFlags.flg.bMqxReadyToSend)
        {
            while (1)
            {
                /* write character when is valid */
                if (pcm_wFlags.flg.bMqxReadyToSend)
                {
                    if (fwrite(&TxChar, 1, 1, devfd))
                        /* character was successfully send, ready to get next character */
                        pcm_wFlags.flg.bMqxReadyToSend = 0;
                    else
                        /* character write failed, needs to be send next call */
                        break;

                }
                /* is ready to get next character? */
                if (FMSTR_Tx((FMSTR_U8*)&TxChar))
                    /* FreeMASTER packet is sent, exit loop */
                    break;
                /* read next character, set its validity to be send */
                pcm_wFlags.flg.bMqxReadyToSend = 1;
            }
        }	
    }
}

#else /* FMSTR_USE_MQX_IO */

/*lint -efile(766, freemaster_protocol.h) include file is not used in this case */

#endif /* FMSTR_USE_MQX_IO */

/******************************************************************************
****************************************************************************//*!
*
* FreeMASTER USB CDC serial communication routines
* 
*******************************************************************************/

#if FMSTR_USE_USB_CDC
#include "usb_cdc.h"        /* USB CDC Class Header File */

/***********************************
*  Global variables
***********************************/
FMSTR_U8 send_buf[DIC_BULK_IN_ENDP_PACKET_SIZE]; //buffer for store packet to send

/***********************************
*  local function prototypes
***********************************/

static FMSTR_BOOL FMSTR_InitUSB(void);
static void FMSTR_ProcessUSB(void);
static void FMSTR_SendUsbPacket(void);

static void FMSTR_USB_CDC_Callback(FMSTR_U8 controller_ID,
                        FMSTR_U8 event_type, void* val);
static void FMSTR_USB_CDC_Notify_Callback(FMSTR_U8 controller_ID,
                        FMSTR_U8 event_type, void* val);

/**************************************************************************//*!
*
* @brief    USB CDC communication interface initialization
*
******************************************************************************/

static FMSTR_BOOL FMSTR_InitUSB(void)
{   
    FMSTR_U8   error;

    /* Initialize the USB interface */
    error = USB_Class_CDC_Init(FMSTR_USB_CDC_ID,FMSTR_USB_CDC_Callback,
                                NULL,FMSTR_USB_CDC_Notify_Callback);
    if(error != USB_OK)
    {
        /* Error initializing USB-CDC Class */
        return FMSTR_FALSE;
    }

    return FMSTR_TRUE;
}

/**************************************************************************//*!
*
* @brief    Handle USB CDC class periodic task and initialize FreeMASTER TX packet
*
* This function perform USB CDC periodic task. In Short Interrupt mode also decode
* received FreeMASTER packet and initialize transmit
*
******************************************************************************/
static void FMSTR_ProcessUSB(void)
{ 
    /* call the periodic task function */
    USB_Class_CDC_Periodic_Task();

#if FMSTR_SHORT_INTR
    /*check whether enumeration is complete or not */
    if((pcm_wFlags.flg.bUsbCdcStartApp) && (pcm_wFlags.flg.bUsbCdcStartTrans))
    {
    	/*transmitter not active, able to receive */
        if ((!pcm_wFlags.flg.bTxActive) && (pcm_wFlags.flg.bUsbReadyToDecode))
        {
            FMSTR_ProtocolDecoder(pcm_pCommBuffer);
            pcm_wFlags.flg.bUsbReadyToDecode = 0;
        }
    }
#endif	
}


/**************************************************************************//*!
*
* @brief    Get data from FreeMASTER TX buffer and send the data to USB device 
*
* This function checks the FreeMASTER transmit state and when FreeMASTER is 
* ready to send response to PC, copy FreeMASTER TX data to local buffer and 
* pass down the buffer to USB CDC TX function. 
*
******************************************************************************/

static void FMSTR_SendUsbPacket(void)
{
    /* transmitter active and empty? */
    if (pcm_wFlags.flg.bTxActive)
    {
        FMSTR_U8 index;

        /* fill in the transmitter buffer */
        for (index = 0; index < DIC_BULK_IN_ENDP_PACKET_SIZE; index++){
            FMSTR_U8 TxChar;
            if (FMSTR_Tx(&TxChar))
                break;
            send_buf[index] = TxChar;
        }
        
       USB_Class_CDC_Interface_DIC_Send_Data(FMSTR_USB_CDC_ID, send_buf,index);
    }
}

/******************************************************************************
 *
 *    @name        FMSTR_USB_CDC_Callback
 *
 *    @brief       This function handles Class callback
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : Value of the event
 *    @param       val              : gives the configuration value
 *
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the class layer whenever reset occurs or enum
 * is complete. After the enum is complete this function sets a variable so
 * that the application can start.
 * This function also receives DATA Send and RECEIVED Events
 *****************************************************************************/

static void FMSTR_USB_CDC_Callback (
    FMSTR_U8 controller_ID,   /* [IN] Controller ID */
    FMSTR_U8 event_type,      /* [IN] value of the event */
    void* val               /* [IN] gives the configuration value */
)
{
    UNUSED (controller_ID)
    UNUSED (val)
    if(event_type == USB_APP_BUS_RESET)
    {
        pcm_wFlags.flg.bUsbCdcStartApp = 0U;
    }
    else if(event_type == USB_APP_ENUM_COMPLETE)
    {
        pcm_wFlags.flg.bUsbCdcStartApp = 1U;
    }
    else if((event_type == USB_APP_DATA_RECEIVED) && (pcm_wFlags.flg.bUsbCdcStartTrans))
    {
        /* manage received data */
        if (pcm_wFlags.flg.bUsbCdcStartApp)
        {
            /* Copy Received Data buffer to Application Buffer */
            USB_PACKET_SIZE BytesToBeCopied;
            APP_DATA_STRUCT* dp_rcv = (APP_DATA_STRUCT*)val;
            BytesToBeCopied = dp_rcv->data_size;

            /* transmitter not active, able to receive */
            if (!pcm_wFlags.flg.bTxActive)
            {
                FMSTR_U8 index;
                for(index = 0 ; index<BytesToBeCopied ; index++){
                    if(FMSTR_Rx(dp_rcv->data_ptr[index])){
                        break;
                    }
                }
                /* continue receiving, when packet is not decoded */
                if (index == BytesToBeCopied) /* receive next packet only when are not received all bytes of packet */
                    (void)USB_Class_CDC_Interface_DIC_Recv_Data(FMSTR_USB_CDC_ID, NULL, 0);
            }
        }
    }
    else if((event_type == USB_APP_SEND_COMPLETE) && (pcm_wFlags.flg.bUsbCdcStartTrans))
    {
        if ((pcm_wFlags.flg.bUsbCdcStartApp) && (pcm_wFlags.flg.bTxActive))
            /* Finalize USB packet to transmit and send packet */
            FMSTR_SendUsbPacket();
        else
            /* Previous Send is complete. Queue next receive */
            (void)USB_Class_CDC_Interface_DIC_Recv_Data(FMSTR_USB_CDC_ID, NULL, 0);
    }

    return;
}

/******************************************************************************
 *
 *    @name        FMSTR_USB_CDC_Notify_Callback
 *
 *    @brief       This function handles PSTN Sub Class callbacks
 *
 *    @param       controller_ID    : Controller ID
 *    @param       event_type       : PSTN Event Type
 *    @param       val              : gives the configuration value
 *
 *    @return      None
 *
 *****************************************************************************
 * This function handles USB_APP_CDC_CARRIER_ACTIVATED and
 * USB_APP_CDC_CARRIER_DEACTIVATED PSTN Events
 *****************************************************************************/

static void FMSTR_USB_CDC_Notify_Callback (
    FMSTR_U8 controller_ID,   /* [IN] Controller ID */
    FMSTR_U8 event_type,      /* [IN] PSTN Event Type */
    void* val               /* [IN] gives the configuration value */
)
{
    UNUSED (controller_ID)
    UNUSED (val)
    if(pcm_wFlags.flg.bUsbCdcStartApp)
    {
        if(event_type == USB_APP_CDC_CARRIER_ACTIVATED)
        {
            pcm_wFlags.flg.bUsbCdcStartTrans = 1U;
        }
        else if(event_type == USB_APP_CDC_CARRIER_DEACTIVATED)
        {
            pcm_wFlags.flg.bUsbCdcStartTrans = 0U;
        }
    }
    return;
}

#else /* FMSTR_USE_USB_CDC */

/*lint -efile(766, freemaster_protocol.h) include file is not used in this case */

#endif /* FMSTR_USE_USB_CDC */


#if FMSTR_USE_SERIAL

/**************************************************************************//*!
*
* @brief    Start listening on a serial line
*
* Reset the receiver machine and start listening on a serial line
*
******************************************************************************/

static void FMSTR_Listen(void)
{
    pcm_nRxTodo = 0U;

    /* disable transmitter state machine */
    pcm_wFlags.flg.bTxActive = 0U;
    
    /*使能USART1的接收功能*/
/*    GPIO_WriteBit(GPIOE, GPIO_Pin_15,Bit_RESET); */

#if FMSTR_USE_SCI
    pcm_wFlags.flg.bTxWaitTC = 0U;
    /* disable transmitter, enable receiver (enables single-wire connection) */
#if !FMSTR_SCI_TWOWIRE_ONLY
     TXD_IE_DISABLE();/*USART1->CR1 &= ~USART_CR1_TE; FMSTR_SCI_TD();*/
     RXD_IE_ENABLE();/*FMSTR_SCI_RE();*/
	   USART_TX_DISABLE();
#endif
#endif /* FMSTR_USE_SCI */

    /* disable transmit, enable receive interrupts */
#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
#if FMSTR_USE_SCI
    TXD_IE_DISABLE();   /* FMSTR_SCI_DTXI(); disable SCI transmit interrupt */
    RXD_IE_ENABLE(); /* FMSTR_SCI_ERXI(); enable SCI receive interrupt */
		USART_TX_DISABLE();
#endif /* FMSTR_USE_SCI / FMSTR_USE_JTAG  */
#endif /* FMSTR_SHORT_INTR || FMSTR_LONG_INTR */
}

/**************************************************************************//*!
*
* @brief    Send response of given error code (no data) 
*
* @param    nErrCode - error code to be sent
*
******************************************************************************/

static void FMSTR_SendError(FMSTR_BCHR nErrCode)
{
    /* fill & send single-byte response */
    *pcm_pCommBuffer = nErrCode;
    FMSTR_SendResponse(pcm_pCommBuffer, 1U);
}

/**************************************************************************//*!
*
* @brief    Finalize transmit buffer before transmitting 
*
* @param    nLength - response length (1 for status + data length)
*
*
* This Function takes the data already prepared in the transmit buffer 
* (inlcuding the status byte). It computes the check sum and kicks on tx.
*
******************************************************************************/

void FMSTR_SendResponse(FMSTR_BPTR pResponse, FMSTR_SIZE8 nLength)
{
    FMSTR_U16 chSum = 0U;
    FMSTR_U8 i, c;

    /*使能USART1发送功能*/
   /* GPIO_WriteBit(GPIOE, GPIO_Pin_15,Bit_SET); */
    
    /* remember the buffer to be sent */
    pcm_pTxBuff = pResponse;
    
    /* status byte and data are already there, compute checksum only     */
    for (i=0U; i<nLength; i++)
    {
        c = 0U;
        pResponse = FMSTR_ValueFromBuffer8(&c, pResponse);
        /* add character to checksum */
        chSum += c;
        /* prevent saturation to happen on DSP platforms */
        chSum &= 0xffU;
    }
    
    /* store checksum after the message */
    pResponse = FMSTR_ValueToBuffer8(pResponse, (FMSTR_U8) (((FMSTR_U16)~(chSum)) + 1U));

    /* send the message and the checksum and the SOB */
    pcm_nTxTodo = (FMSTR_SIZE8) (nLength + 1U); 
    
    /* now transmitting the response */
    pcm_wFlags.flg.bTxActive = 1U;

#if FMSTR_USE_USB_CDC || FMSTR_USE_MQX_IO || FMSTR_USE_JTAG
    pcm_wFlags.flg.bTxFirstSobSend = 1U;
#endif

#if FMSTR_USE_SCI
    pcm_wFlags.flg.bTxWaitTC = 0U;
#endif

    /* do not replicate the initial SOB  */
    pcm_wFlags.flg.bTxLastCharSOB = 0U;
    
#if FMSTR_USE_SCI       
    {
        /*lint -esym(550, dummySR) */        
        volatile FMSTR_SCISR dummySR;

        /* disable receiver, enable transmitter (single-wire communication) */
#if !FMSTR_SCI_TWOWIRE_ONLY
        RXD_IE_DISABLE(); /*FMSTR_SCI_RD();*/
        TXD_IE_ENABLE();  /*FMSTR_SCI_TE();*/
			  USART_TX_ENABLE();
#endif        
        /* kick on the SCI transmission (also clears TX Empty flag on some platforms) */
        dummySR = UART_IF_FLG;
        USART_SendData(FMSTR_SOB);/*FMSTR_SCI_PUTCHAR(FMSTR_SOB);*/
    }
    
#elif FMSTR_USE_JTAG
    /* kick on the JTAG transmission */
    FMSTR_FlushJtagTx();
#endif

    /* TX interrupt enable, RX interrupt disable */
#if FMSTR_LONG_INTR || FMSTR_SHORT_INTR
#if FMSTR_USE_SCI       
    RXD_IE_DISABLE();/*FMSTR_SCI_DRXI();*/
    TXD_IE_ENABLE();   /*FMSTR_SCI_ETXI();*/
		USART_TX_ENABLE();
    
#elif FMSTR_USE_JTAG


#elif FMSTR_USE_USB_CDC
    if ((pcm_wFlags.flg.bUsbCdcStartApp) && (pcm_wFlags.flg.bTxActive) && (pcm_wFlags.flg.bUsbCdcStartTrans))
        /* Finalize USB packet to transmit and send packet */
        FMSTR_SendUsbPacket();
    else
        /* Previous Send is complete. Queue next receive */
        (void)USB_Class_CDC_Interface_DIC_Recv_Data(FMSTR_USB_CDC_ID, NULL, 0);
#endif
#endif /* FMSTR_LONG_INTR || FMSTR_SHORT_INTR */      
}

/**************************************************************************//*!
*
* @brief    Output buffer transmission
* 
* @param  pTxChar  The character to be transmit 
*
* get ready buffer(prepare data to send)
*
******************************************************************************/

FMSTR_BOOL FMSTR_Tx(FMSTR_U8* pTxChar)
{
    
    if (pcm_nTxTodo)
    {
        /* fetch & send character ready to transmit */
        /*lint -e{534} ignoring return value */
        FMSTR_ValueFromBuffer8(pTxChar, pcm_pTxBuff);
        
        /* first, handle the replicated SOB characters */
        if (*pTxChar == FMSTR_SOB)
        {
            pcm_wFlags.flg.bTxLastCharSOB ^= 1U;
            if ((pcm_wFlags.flg.bTxLastCharSOB))
            {
                /* yes, repeat the SOB next time */
                return FMSTR_FALSE;
            }
        }
        /* no, advance tx buffer pointer */
        pcm_nTxTodo--;
        pcm_pTxBuff = FMSTR_SkipInBuffer(pcm_pTxBuff, 1U);
        return FMSTR_FALSE;
    }
    
    /* when SCI TX buffering is enabled, we must first wait until all 
       characters are physically transmitted (before disabling transmitter) */
#if FMSTR_USE_SCI
  #if FMSTR_SCI_HAS_TXQUEUE
    pcm_wFlags.flg.bTxWaitTC = 1;

    /* wait for SCI TC interrupt */
    #if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    FMSTR_SCI_ETCI();
    #endif
  #else
    /* start listening immediately */
    FMSTR_Listen();
  #endif  
#else
    /* start listening immediately */
    FMSTR_Listen();
#endif

    return FMSTR_TRUE;
}


/**************************************************************************//*!
*
* @brief  Handle received character 
*
* @param  nRxChar  The character to be processed 
* 
* Handle the character received and -if the message is complete- call the 
* protocol decode routine. 
*
******************************************************************************/

FMSTR_BOOL FMSTR_Rx(FMSTR_BCHR nRxChar)
{
    FMSTR_SERIAL_FLAGS * pflg = &pcm_wFlags;
    /* first, handle the replicated SOB characters */
    if(nRxChar == FMSTR_SOB)
    {
        pflg->flg.bRxLastCharSOB ^= 1;
        if(pflg->flg.bRxLastCharSOB)
        {
            /* this is either the first byte of replicated SOB or a  */
            /* real Start-of-Block mark - we will decide next time in FMSTR_Rx */
            return FMSTR_FALSE;
        }
    }
    
    /* we have got a common character preceded by the SOB -  */
    /* this is the command code! */
    if(pflg->flg.bRxLastCharSOB)
    {
        /* reset receiving process */
        pcm_pRxBuff = pcm_pCommBuffer;
        *pcm_pRxBuff++ = nRxChar;

        /* start computing the checksum */
        pcm_nRxCheckSum = nRxChar;
        pcm_nRxTodo = 0U;
    
        /* if the standard command was received, the message length will come in next byte */
        pflg->flg.bRxMsgLengthNext = 1U;

        /* fast command? */
        if(!((~nRxChar) & FMSTR_FASTCMD))
        {
            /* fast command received, there will be no length information */
            pflg->flg.bRxMsgLengthNext = 0U;
            /* as it is encoded in the command byte directly */
            pcm_nRxTodo = (FMSTR_SIZE8) 
                (((((FMSTR_SIZE8)nRxChar) & FMSTR_FASTCMD_DATALEN_MASK) >> FMSTR_FASTCMD_DATALEN_SHIFT) + 1U);
        }

        /* command code stored & processed */
        pflg->flg.bRxLastCharSOB = 0U;
        return FMSTR_FALSE;
    }

    /* we are waiting for the length byte */
    if(pflg->flg.bRxMsgLengthNext)
    {
        /* this byte, total data length and the checksum */
        pcm_nRxTodo = (FMSTR_SIZE8) (1U + ((FMSTR_SIZE8)nRxChar) + 1U);
        /* now read the data bytes */
        pflg->flg.bRxMsgLengthNext = 0U;

    }

    /* waiting for a data byte? */
    if(pcm_nRxTodo)
    {
        /* add this byte to checksum */
        pcm_nRxCheckSum += nRxChar;

        /* decrease number of expected bytes */
        pcm_nRxTodo--;
        /* was it the last byte of the message (checksum)? */
        if(!pcm_nRxTodo)
        {
            /* receive buffer overflow? */
            if(pcm_pRxBuff == NULL)
            {
                FMSTR_SendError(FMSTR_STC_CMDTOOLONG);
            }
            /* checksum error? */
            else if((pcm_nRxCheckSum & 0xffU) != 0U)
            {
                FMSTR_SendError(FMSTR_STC_CMDCSERR);
            }
            /* message is okay */
            else 
            {
#if FMSTR_USE_USB_CDC && FMSTR_SHORT_INTR
                /* Decode protocol and send response in Poll function */
                pflg->flg.bUsbReadyToDecode = 1U;
#else
                /* do decode now! */
                FMSTR_ProtocolDecoder(pcm_pCommBuffer);
#endif
            }

            return FMSTR_TRUE;
        }
        /* not the last character yet */
        else 
        {   
            /* is there still a space in the buffer? */
            if(pcm_pRxBuff)
            {
                /*lint -e{946} pointer arithmetic is okay here (same array) */
                if(pcm_pRxBuff < (pcm_pCommBuffer + FMSTR_COMM_BUFFER_SIZE))
                {
                    /* store byte  */
                    *pcm_pRxBuff++ = nRxChar;
                }
                /* buffer is full! */
                else
                {
                    /* NULL rx pointer means buffer overflow - but we still need */
                    /* to receive all message characters (for the single-wire mode) */
                    /* so keep "receiving" - but throw away all characters from now */
                    pcm_pRxBuff = NULL;
                }
            }
        }
    }
    return FMSTR_FALSE;	
}

/**************************************************************************//*!
*
* @brief    Serial communication initialization
*
******************************************************************************/

FMSTR_BOOL FMSTR_InitSerial(void)
{   
    /* initialize all state variables */
    pcm_wFlags.all = 0U;
    pcm_nTxTodo = 0U;

/* Initialize SCI and JTAG interface */
#if FMSTR_USE_SCI && FMSTR_SCI_TWOWIRE_ONLY
    /* to enable TX and RX together in FreeMASTER initialization */
    FMSTR_SCI_TE_RE();
#endif
    
#if FMSTR_SHORT_INTR & (FMSTR_USE_SCI || FMSTR_USE_JTAG)
    pcm_pRQueueRP = pcm_pRQueueBuffer;
    pcm_pRQueueWP = pcm_pRQueueBuffer;
#endif

#if FMSTR_USE_MQX_IO
    if (!FMSTR_InitMQX())
        return FMSTR_FALSE;
#endif

#if FMSTR_USE_USB_CDC
	if (!FMSTR_InitUSB())
		return FMSTR_FALSE;
#endif

    /* start listening for commands */
    FMSTR_Listen();
    return FMSTR_TRUE;
}


/*******************************************************************************
*
* @brief    API: Main "Polling" call from the application main loop
*
* This function either handles all the SCI communication (polling-only mode = 
* FMSTR_POLL_DRIVEN) or decodes messages received on the background by SCI interrupt
* (short-interrupt mode = FMSTR_SHORT_INTR). 
*
* In the JTAG interrupt-driven mode (both short and long), this function also checks
* if setting the JTAG RIE bit failed recently. This may happen because of the 
* RIE is held low by the EONCE hardware until the EONCE is first accessed from host.
* FMSTR_Init (->FMSTR_Listen) is often called while the PC-side FreeMASTER is still 
* turned off. So really, the JTAG is not enabled by this time and RIE bit is not set.
* This problem is detected (see how bJtagRIEPending is set above in FSMTR_Listen)
* and it is tried to be fixed periodically here in FMSTR_Poll.
*
*******************************************************************************/

void FMSTR_Poll(void)
{ 
#if !FMSTR_POLL_DRIVEN && FMSTR_USE_JTAG
    /* in the interrupt-driven JTAG mode, the JTAG RIE may have failed to be set recently */
    if(pcm_wFlags.flg.bJtagRIEPending)
    {
        FMSTR_JTAG_ERXI();  /* try to enable JTAG receive interrupt now */

        /* succeeded? */
        if(FMSTR_JTAG_ERXI_CHECK())
            pcm_wFlags.flg.bJtagRIEPending = 0; /* yes!, enough until it fails again (never?) */
    }
#endif    

#if FMSTR_USE_MQX_IO
    /* polled MQX IO mode */
    FMSTR_ProcessMQXIO(); 
#endif

#if FMSTR_POLL_DRIVEN

#if FMSTR_USE_SCI
    /* polled SCI mode */
    FMSTR_ProcessSCI(); 
    
#elif FMSTR_USE_JTAG
    /* polled JTAG mode */
    FMSTR_ProcessJTAG(); 
#endif
    
#elif FMSTR_SHORT_INTR 

#if FMSTR_USE_USB_CDC
    /*  */
    FMSTR_ProcessUSB();
#elif (FMSTR_USE_SCI || FMSTR_USE_JTAG)

    /* process queued SCI characters */
    FMSTR_RxDequeue(); 
#endif  
  
#endif
}

#endif
