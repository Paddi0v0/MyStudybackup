/**
 * @file
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_spi.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� SPI������������ \n
 * ����˵���� �� \n
 * @par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��05��21�� <td>1.0        <td>cfwu          <td>����
 * </table>
 */

#include "lks32mc08x_spi.h"

/**
 *@brief @b ��������:   void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
 *@brief @b ��������:   SPI��ʼ������
 *@see���������ݣ�       SYS_ModuleClockCmd()
 *@param���������       SPIx��SPI , SPI_InitTypeDef *SPI_InitStruct
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			SPI_InitTypeDef SPI_InitStruct;
            SPI_StructInit(&SPI_InitStruct);              //SPI�ṹ���ʼ��
            SPI_InitStruct.Duplex = SPI_Full;             //˫��ģʽ����
            SPI_InitStruct.Mode = SPI_Master;             //master����ģʽ
            SPI_InitStruct.EN = ENABLE;                   //ʹ��SPIģ��
            SPI_InitStruct.DMA = SPI_DMA_DISABLE;         // ѡ��SPI���Ʒ�ʽ����MCU�������ݵ�SPI
            SPI_InitStruct.Trig = 0;                      //�ڲ��Զ���������
            SPI_InitStruct.DataOrder = SPI_FIRSTSEND_MSB; //���ֽ��ȷ��� MSB
            SPI_InitStruct.CPHA = 0;                      //��һ����Ϊ��������ʱ��
            SPI_InitStruct.CPOL = 0;                      //CLKĬ�ϸߵ�ƽ
            SPI_InitStruct.BaudRate = 30;                 //�����ٶ�96MHZ/(2*(30+1))= 1.548MHZ
            SPI_InitStruct.ByteLength = 0;                //��������ֽ�
            SPI_InitStruct.IRQEna = DISABLE;              //�ر�SPI���ж�
            SPI_Init(SPI, &SPI_InitStruct);               //SPI��ʼ������
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��05��21�� <td>1.0        <td>cfwu          <td>����
 * </table>
 */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
	SYS_WR_PROTECT = 0x7a83;/* ʹ��ϵͳ�Ĵ���д����*/
    SYS_SFT_RST |= BIT8;  //��λSPI
    SYS_SFT_RST &= ~BIT8; //�ͷ�SPI
	  SYS_WR_PROTECT = 0x0; /* �ر�ϵͳ�Ĵ���д����*/
    SYS_ModuleClockCmd(SYS_Module_SPI, ENABLE);

    SPIx->IE   =   SPI_InitStruct->Trig   << 3  | (SPI_InitStruct->IRQEna        );

    SPIx->DIV  =   SPI_InitStruct->BaudRate     | (SPI_InitStruct->DMA       << 7);

    SPIx->SIZE =   SPI_InitStruct->ByteLength;

    SPIx->CFG  =   SPI_InitStruct->EN           | (SPI_InitStruct->DataOrder << 1)
                   | (SPI_InitStruct->CPOL   << 2) | (SPI_InitStruct->CPHA      << 3)
                   | (SPI_InitStruct->Mode   << 4) | (SPI_InitStruct->SS        << 5)
                   | (SPI_InitStruct->Duplex << 6);

}

/**
 *@brief @b ��������:    void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
 *@brief @b ��������:    SPI�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       SPI_InitTypeDef *SPI_InitStruct
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			SPI_InitTypeDef SPI_InitStruct;
            SPI_StructInit(&SPI_InitStruct);              //SPI�ṹ���ʼ��
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��05��21�� <td>1.0        <td>cfwu          <td>����
 * </table>
 */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{

    SPI_InitStruct->Duplex = SPI_Full;
    SPI_InitStruct->SS     = 0;
    SPI_InitStruct->Mode   = SPI_Slave;
    SPI_InitStruct->CPOL   = 0;
    SPI_InitStruct->CPHA   = 0;
    SPI_InitStruct->DataOrder = DISABLE;
    SPI_InitStruct->EN        = 0;

    SPI_InitStruct->IRQEna    = DISABLE;
    SPI_InitStruct->Trig      = 0;

    SPI_InitStruct->DMA       = 0;
    SPI_InitStruct->BaudRate  = 3;

    SPI_InitStruct->ByteLength = 0;
}

/**
 *@brief @b ��������:   void SPI_SendData(SPI_TypeDef *SPIx, uint8_t n)
 *@brief @b ��������:   SPI��������
 *@see���������ݣ�       ��
 *@param���������       SPIx��SPI , n������һ�ֽ�����
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
			SPI_SendData(SPI,0x12);  //SPI����0x12һ�ֽ�����
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��05��21�� <td>1.0        <td>cfwu          <td>����
 * </table>
 */
void SPI_SendData(SPI_TypeDef *SPIx, uint8_t n)
{
    SPIx->TX_DATA = n;
}


/**
 *@brief @b ��������:   uint8_t SPI_ReadData(SPI_TypeDef *SPIx)
 *@brief @b ��������:   SPI������������
 *@see���������ݣ�       ��
 *@param���������       SPIx��SPI
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
            u8 SPI_RXValue = 0;
			SPI_RXValue = SPI_ReadData(SPI);  //SPI����һ�ֽ�����
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��05��21�� <td>1.0        <td>cfwu          <td>����
 * </table>
 */
uint8_t SPI_ReadData(SPI_TypeDef *SPIx)
{
    return SPIx->RX_DATA;
}

/**
 *@brief @b ��������:   uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
 *@brief @b ��������:   ��ȡSPI�жϱ�־λ
 *@see���������ݣ�       ��
 *@param���������       SPIx��SPI
                        IRQ_Flag��
 * <table>              <tr><th>SPI_IF_TranDone       <td>��������жϱ�־λ \n
 * 						<tr><th>SPI_IF_SSErr          <td>�����쳣�жϱ�־λ \n
 * 						<tr><th>SPI_IF_DataOver       <td>��������жϱ�־λ \n
 * </table>                       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ֻ�ж�Ӧ�ж�ʹ�ܺ󣬸�Ϊ���ܶ�ȡ�������Ӧ�ж�δʹ�ܣ���ȡ���һֱΪ0
 *@par ʾ�����룺
 *@code
            if(SPI_GetIRQFlag(SPI,SPI_IF_TranDone))//��ȡ��������жϱ�־λ
            {
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��04��20�� <td>1.0       <td>Zhu Jie       <td>����
 * </table>
 */
uint8_t SPI_GetIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
{
    u8 SPI_IRQFlag = 0;
	  if(SPIx->IE & IRQ_Flag)
		{
		    SPI_IRQFlag = 1;
		}
		else
		{
		    SPI_IRQFlag = 0;
		}
    return SPI_IRQFlag;
}

/**
 *@brief @b ��������:   void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
 *@brief @b ��������:   ���SPI�жϱ�־λ
 *@see���������ݣ�       ��
 *@param���������       SPIx��SPI
                        IRQ_Flag��
 * <table>              <tr><th>SPI_IF_TranDone       <td>��������жϱ�־λ \n
 * 						<tr><th>SPI_IF_SSErr          <td>�����쳣�жϱ�־λ \n
 * 						<tr><th>SPI_IF_DataOver       <td>��������жϱ�־λ \n
 * </table>                       
 *@param���������       ��
 *@return�� �� ֵ��      ��
 *@note����˵����        ��
 *@warning              ��
 *@par ʾ�����룺
 *@code
            if(SPI_GetIRQFlag(SPI,SPI_IF_TranDone))//��ȡ��������жϱ�־λ
            {
                SPI_ClearIRQFlag(SPI,SPI_IF_TranDone);//�����������жϱ�־λ
            }
  @endcode
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2022��04��20�� <td>1.0      <td>Zhu Jie        <td>����
 * </table>
 */
void SPI_ClearIRQFlag(SPI_TypeDef *SPIx,u8 IRQ_Flag)
{
    SPIx->IE |= IRQ_Flag;
}
