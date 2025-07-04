/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� KeyScan.h
 * �ļ���ʶ��
 * ����ժҪ�� KeyScan  & Led
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� andrew kong
 * ������ڣ� 2019��12��19��
 *
 *******************************************************************************/
#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#include "basic.h"
#include "lks32mc07x.h"
#include "lks32mc07x_gpio.h"

#define KEY_START GPIO2, GPIO_Pin_11
#define KEY_STOP  GPIO2, GPIO_Pin_12

void KeyScan(void);
#endif

