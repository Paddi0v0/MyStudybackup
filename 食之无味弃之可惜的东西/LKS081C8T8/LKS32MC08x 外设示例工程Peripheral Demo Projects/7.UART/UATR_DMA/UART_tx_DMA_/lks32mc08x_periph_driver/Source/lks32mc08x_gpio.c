/**
 * @file 
 * @copyright (C)2015, LINKO SEMICONDUCTOR Co.ltd
 * @brief �ļ����ƣ� lks32mc08x_gpio.c\n
 * �ļ���ʶ�� �� \n
 * ����ժҪ�� GPIO������������ \n
 * ����˵���� �� \n
 *@par �޸���־:
 * <table>
 * <tr><th>Date	        <th>Version  <th>Author  <th>Description
 * <tr><td>2016��6��19�� <td>1.0     <td>cfwu       <td>����
 * </table>
 */


#include "lks32mc08x_gpio.h"


/**
 *@brief @b ��������:   void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
 *@brief @b ��������:   GPIO��ʼ������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_InitStruct: GPIO��ʼ���ṹ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			GPIO_InitTypeDef GPIO_InitStruct;
            //P2.11����ģʽ
            GPIO_StructInit(&GPIO_InitStruct);        //��ʼ���ṹ��
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; //GPIO����ģʽ
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
            GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIO2, &GPIO_InitStruct);
            //P0.2���ģʽ    
            GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
            GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
            GPIO_Init(GPIO0, &GPIO_InitStruct);
            //P3.2��©���     
            GPIO_StructInit(&GPIO_InitStruct);         //��ʼ���ṹ��
            GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //GPIO���ģʽ
            GPIO_InitStruct.GPIO_PODEna = ENABLE;      //ʹ�ܿ�©ģʽ
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
            GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//�ر�����������Ϊ����̬
            GPIO_Init(GPIO3, &GPIO_InitStruct);
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0        <td>cfwu         <td>����
 * </table>
 */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    uint32_t pinpos = 0x00, pos = 0x00, currentpin = 0x00, tempreg = 0x00;

    /*-------------------------- Configure the port pins -----------------------*/
    for (pinpos = 0x00; pinpos < 0x10; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;

        /* Get the port pins position */
        currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
        if (currentpin == pos)
        {
            if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IN)
            {
                tempreg = GPIOx->PIE;    /*ʹ������*/
                tempreg |= (BIT0 << pinpos);
                GPIOx->PIE = tempreg;

                tempreg = GPIOx->POE;    /*��ֹ���*/
                tempreg &= ~(BIT0 << pinpos);
                GPIOx->POE = tempreg;
            }
            else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT)
            {
                tempreg = GPIOx->PIE;    /*��ֹ����*/
                tempreg &= ~(BIT0 << pinpos);
                GPIOx->PIE = tempreg;

                tempreg = GPIOx->POE;    /*ʹ�����*/
                tempreg |= (BIT0 << pinpos);
                GPIOx->POE = tempreg;
            }
            else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_ANA)
            {
							  tempreg = GPIOx->PIE;    /*��ֹ����*/
                tempreg &= ~(BIT0 << pinpos);
                GPIOx->PIE = tempreg;
							  
                tempreg = GPIOx->POE;    /*��ֹ���*/
                tempreg &= ~(BIT0 << pinpos);
                GPIOx->POE = tempreg;
            }
           else if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IO)
            {
                tempreg = GPIOx->PIE;    /*ʹ������*/
                tempreg |= (BIT0 << pinpos);
                GPIOx->PIE = tempreg;
					 
                tempreg = GPIOx->POE;    /*ʹ�����*/    
                tempreg |= (BIT0 << pinpos);
                GPIOx->POE = tempreg;
            }

            if (GPIO_InitStruct->GPIO_PuPd == GPIO_PuPd_UP)
            {
                tempreg = GPIOx->PUE;
                tempreg |= (BIT0 << pinpos);
                GPIOx->PUE = tempreg;
            }
            else if (GPIO_InitStruct->GPIO_PuPd == GPIO_PuPd_NOPULL)
            {
                tempreg = GPIOx->PUE;
                tempreg &= ~(BIT0 << pinpos);
                GPIOx->PUE = tempreg;
            }

            tempreg = GPIOx->PODE;
            if(GPIO_InitStruct->GPIO_PODEna == (uint32_t)ENABLE)
            {
                tempreg |= (BIT0 << pinpos);
            }
            else
            {
                tempreg &= ~(BIT0 << pinpos);
            }
            GPIOx->PODE = tempreg;
        }
    }
}

/**
 *@brief @b ��������:   void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
 *@brief @b ��������:   GPIO�ṹ���ʼ��
 *@see���������ݣ�       ��
 *@param���������       
                        GPIO_InitStruct: GPIO��ʼ���ṹ��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			GPIO_InitTypeDef GPIO_InitStruct;
            GPIO_StructInit(&GPIO_InitStruct);        //��ʼ���ṹ��
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
    GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_NONE;
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct->GPIO_PODEna = DISABLE;
}


/**
 *@brief @b ��������:   uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
 *@brief @b ��������:   ��ȡGPIO��ָ��Pin������ֵ
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ָ��Pin��������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			u8 GPIO_IN_VALUE = 0;
            GPIO_IN_VALUE = GPIO_ReadInputDataBit(GPIO0,GPIO_Pin_0);//��ȡP0.0�����ƽ 1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;
    if ((GPIOx->PDI & GPIO_Pin) != 0)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 *@brief @b ��������:   uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
 *@brief @b ��������:   ��ȡGPIO����������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3��
 *@param���������      ��
 *@return�� �� ֵ��     ��������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			u8 GPIO_IN_VALUE = 0;
            GPIO_IN_VALUE = GPIO_ReadInputData(GPIO0);//��ȡP0.0~P0.15�����ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
    return GPIOx->PDI;
}


/**
 *@brief @b ��������:   uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
 *@brief @b ��������:   ��ȡGPIO��ָ��Pin�����ֵ
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ָ��IO�������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			u8 GPIO_OUT_VALUE = 0;
            GPIO_OUT_VALUE = GPIO_ReadOutputDataBit(GPIO0,GPIO_Pin_0);//��ȡP0.0�����ƽ 1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;
    if ((GPIOx->PDO & GPIO_Pin) != 0)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
 *@brief @b ��������:   uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
 *@brief @b ��������:   ��ȡGPIO���������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3��
 *@param���������      ��
 *@return�� �� ֵ��     �������
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
			u8 GPIO_OUT_VALUE = 0;
            GPIO_OUT_VALUE = GPIO_ReadOutputData(GPIO0);//��ȡP0.0~P0.15�����ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
    return GPIOx->PDO;
}


/**
 *@brief @b ��������:   void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
 *@brief @b ��������:   GPIOָ��Pin��1
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_SetBits(GPIO0,GPIO_Pin_0);//P0.0����ߵ�ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);
}

/**
 *@brief @b ��������:   void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
 *@brief @b ��������:   GPIOָ��Pin��1
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_ResetBits(GPIO0,GPIO_Pin_0);//P0.0����͵�ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);
}

/**
 *@brief @b ��������:   void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
 *@brief @b ��������:   ��GPIOָ����Pinд������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15�� \n
                        BitVal:д���Bitֵ ��Bit_RESET �� Bit_SET��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_WriteBit(GPIO0,GPIO_Pin_0,Bit_RESET);//P0.0����͵�ƽ
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
    if (BitVal != Bit_RESET)
    {
        GPIOx->PDO |= GPIO_Pin;
    }
    else
    {
        GPIOx->PDO &= ~GPIO_Pin;
    }
}

/**
 *@brief @b ��������:   void GPIO_Write(GPIO_TypeDef* GPIOx, uint32_t Val)
 *@brief @b ��������:   ��GPIOд������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        BitVal:д���Bitֵ ��0x00~0xffff��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ��
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_WriteBit(GPIO0,0x01);//P0.0��1,����ߵ�ƽ��P0.1~P0.15ֵ���䱣����һʱ��״̬
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint32_t Val)
{
    GPIOx->PDO |= Val;
}

/**
 *@brief @b ��������:   void GPIO_LockRegister(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
 *@brief @b ��������:   GPIO�Ĵ�����������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ������GPIO����/���/������/��©/����ѡ���ܱ��޸�
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_LockRegister(GPIO0,GPIO_Pin_0);//P0.0����
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_LockRegister(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
    LCKR_PRT = 0x5AC4;       /* �ر�д���� */
	
    GPIOx->LCKR |= GPIO_Pin; /* ����GPIOx_Pin���� */
	
	LCKR_PRT = 0xFFFF;       /* ����д���� */
}

/**
 *@brief @b ��������:   void GPIO_UnlockRegister(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
 *@brief @b ��������:   GPIO�Ĵ�����������
 *@see���������ݣ�       ��
 *@param���������       
                        GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
                        GPIO_Pin:ָ����Pin ��GPIO_Pin_0~GPIO_Pin_15��
 *@param���������      ��
 *@return�� �� ֵ��     ��
 *@note����˵����       ������GPIO����/���/������/��©/����ѡ����Ա��޸�
 *@warning             ��
 *@par ʾ�����룺
 *@code    
            GPIO_UnlockRegister(GPIO0,GPIO_Pin_0);//P0.0����
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_UnlockRegister(GPIO_TypeDef* GPIOx ,uint16_t GPIO_Pin)
{
	  LCKR_PRT = 0x5AC4;       /* �ر�д���� */
	
      GPIOx->LCKR &= (~GPIO_Pin); /* ����GPIOx_Pin���� */
	
	  LCKR_PRT = 0xFFFF;       /* ����д���� */
}

/**
 *@brief @b ��������:   void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint32_t GPIO_PinSource, uint32_t GPIO_AF)
 *@brief @b ��������:   GPIO���ù���ѡ��
 *@see���������ݣ�       ��
 *@param���������       
 *                      GPIOx:GPIO����,������ѡ��GPIO0~GPIO3�� \n
 *                      GPIO_PadSource:ָ����PadSource ��GPIO_PinSource_0~GPIO_PinSource_15�� \n
 *                      GPIO_AF:ָ�����ù��� \n
 * 
 * <table>              <tr><td>���ù�������    <td> ˵��                  
 * 						<tr><th>AF0_GPIO                 <td>ģ�⹦��
 * 						<tr><th>AF1_SYSAF                <td>SYS_AF��CMP��CLOCK�������ź�������� 
 * 						<tr><th>AF2_HALL                 <td>HALL���� 
 * 						<tr><th>AF3_MCPWM                <td>MCPWM����
 * 						<tr><th>AF4_UART                 <td>UART���� 
 * 						<tr><th>AF5_SPI                  <td>SPI����
 * 						<tr><th>AF6_I2C                  <td>I2C����
 * 						<tr><th>AF7_TIMER01              <td>Timer0��Timer1���� 
 * 						<tr><th>AF8_TIMER23              <td>Timer2��Timer3��QEP0��QEP1���� 
 * 						<tr><th>AF9_DEBUG                <td>ADC trigger debug���� 
 * 						<tr><th>AF10_CAN                 <td>CAN���� 
 * 						<tr><th>AF11_SIF                 <td>SIF����   
 * </table>                         
 *              
 *@param ���������      ��
 *@return �� �� ֵ��     ��
 *@note ����˵����       ��
 *@warning              ��
 *@par ʾ�����룺
 *@code    
            GPIO_PinAFConfig(GPIO1,GPIO_Pin_0��AF4_UART);//P1.0����Ϊ���ڹ���
  @endcode   
 *@par �޸���־:   
 * <table>
 * <tr><th>Date	        <th>Version    <th>Author      <th>Description
 * <tr><td>2016��4��18��  <td>1.0       <td>cfwu          <td>����
 * </table>
 */
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint32_t GPIO_PinSource, uint32_t GPIO_AF)
{
    uint32_t temp;
    switch(GPIO_PinSource)
    {
    /*F3210*/
    case 0:
    {
        temp = GPIOx->F3210 & 0xFFF0;  /*get F321*/
        GPIOx->F3210 = temp + GPIO_AF; /*F321 + F0*/
        break;
    }
    case 1:
    {
        temp = GPIOx->F3210 & 0xFF0F;
        GPIOx->F3210 = temp + (GPIO_AF << 4);
        break;
    }
    case 2:
    {/* 2020.8.13 Repair bug HL */
        temp = GPIOx->F3210 & 0xF0FF;
        GPIOx->F3210 = temp + (GPIO_AF << 8);
        break;
    }
    case 3:
    {
        temp = GPIOx->F3210 & 0x0FFF;
        GPIOx->F3210 = temp + (GPIO_AF << 12);
        break;
    }
    /*F7654*/
    case 4:
    {
        temp = GPIOx->F7654 & 0xFFF0;
        GPIOx->F7654 = temp + GPIO_AF;
        break;
    }
    case 5:
    {
        temp = GPIOx->F7654 & 0xFF0F;
        GPIOx->F7654 = temp + (GPIO_AF << 4);
        break;
    }
    case 6:
    {
        temp = GPIOx->F7654 & 0xF0FF;
        GPIOx->F7654 = temp + (GPIO_AF << 8);
        break;
    }
    case 7:
    {
        temp = GPIOx->F7654 & 0x0FFF;
        GPIOx->F7654 = temp + (GPIO_AF << 12);
        break;
    }
    /*FBA98*/
    case 8:
    {
        temp = GPIOx->FBA98 & 0xFFF0;
        GPIOx->FBA98 = temp + GPIO_AF;
        break;
    }
    case 9:
    {
        temp = GPIOx->FBA98 & 0xFF0F;
        GPIOx->FBA98 = temp + (GPIO_AF << 4);
        break;
    }
    case 10:
    {
        temp = GPIOx->FBA98 & 0xF0FF;
        GPIOx->FBA98 = temp + (GPIO_AF << 8);
        break;
    }
    case 11:
    {
        temp = GPIOx->FBA98 & 0x0FFF;
        GPIOx->FBA98 = temp + (GPIO_AF << 12);
        break;
    }
    /*FFEDC*/
    case 12:
    {
        temp = GPIOx->FFEDC & 0xFFF0;
        GPIOx->FFEDC = temp + GPIO_AF;
        break;
    }
    case 13:
    {
        temp = GPIOx->FFEDC & 0xFF0F;
        GPIOx->FFEDC = temp + (GPIO_AF << 4);
        break;
    }
    case 14:
    {
        temp = GPIOx->FFEDC & 0xF0FF;
        GPIOx->FFEDC = temp + (GPIO_AF << 8);
        break;
    }
    case 15:
    {
        temp = GPIOx->FFEDC & 0x0FFF;
        GPIOx->FFEDC = temp + (GPIO_AF << 12);
        break;
    }
    default:
        break;
    }
}


/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR *****END OF FILE****/
