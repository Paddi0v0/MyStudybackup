/*******************************************************************************
 * ��Ȩ���� (C)2021, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� i2c_driver.h
 * �ļ���ʶ��
 * ����ժҪ�� i2c������
 * ����˵���� ��
 * ��ǰ�汾�� V 1.0
 * ��    �ߣ� YangZJ
 * ������ڣ� 220826
 *
 *******************************************************************************/
#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

#include "i2c_driver.h"
#include "lks32mc07x_i2c.h"
#include "lks32mc07x_dma.h"
#include "lks32mc07x_sys.h"

void i2c_DriverInit(I2C_TypeDef *I2Cx, DMA_RegTypeDef *DMA_CHx, u32 BaudRate, u8 ADDR); // I2C�������ʼ��
void i2c_DriverMasterRx(u8 addr, u8 *data, u8 len);                                     // ��ģʽ����
void i2c_DriverMasterTx(u8 addr, u8 *data, u8 len);                                     // ��ģʽ����
void i2c_DriverMasterSetRxCallback(void (*fun)(u8));                                    // ��ģʽ������ɻص�����
void i2c_DriverMasterSetTxCallback(void (*fun)(u8));                                    // ��ģʽ������ɻص�����
void i2c_DriverMasterWait(void);                                                        // ��ģʽ�ȴ��������
void i2c_DriverSlaveSetRxCallback(void (*fun)(void));                                   // ��ģʽ������ɻص�����
void i2c_DriverSlaveSetTxCallback(void (*fun)(void));                                   // ��ģʽ������ɻص�����
void i2c_DriverSlaveSetTxData(u8 *data, u8 len);                                        // ��ģʽ���ô����͵�����
void i2c_DriverSlaveGetRxData(u8 *data, u8 len);                                        // ��ģʽ��ȡ�ս��յ�����
#endif
