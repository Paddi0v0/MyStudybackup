/*******************************************************************************
 * ��Ȩ���� (C)2019, LINKO SEMICONDUCTOR Co.ltd
 *
 * �ļ����ƣ� ComFuncs.c
 * �ļ���ʶ��
 * ����ժҪ�� Define the commom function for motor control
 * ����˵���� ��
 * ��ǰ�汾�� V1.0
 * ��    �ߣ� LKS/MC TEAM
 * ������ڣ� 2022��8��11��
 *******************************************************************************/
#include "basic.h"
#include "PubData_DriverInstance.h"
/*******************************************************************************
 �������ƣ�    void delay(u16 cnt)
 ����������    �����ʱ����
 �����ı�    ��
 ���������    cnt ��Ҫ��ʱ��ʱ��
 ���������    ��
 �� �� ֵ��    ��
 ���������:   �ú����漰ȫ�ֱ����������������
 ����˵����
 �޸�����      �汾��          �޸���            �޸�����
 -----------------------------------------------------------------------------

 *******************************************************************************/
 
void delay(u16 cnt)
{
    u16 t_cnt;

    for (t_cnt = 0; t_cnt < cnt; t_cnt++)
    {
        __nop();
    }
}


unsigned char CountLeadZero(unsigned int val)	
{
	return (__clz(val));
}

// user 2 App trans

/*****************************************************************************
 * ������   : User2AppFreqTrans(PSTR_TransCoefElement pUser2App,float val)
 * ˵��     : User2App Ƶ��ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 User2AppFreqTrans(PSTR_TransCoefElement pUser2App,float val)
{
    s32 ax;
    ax = ((s32)(pUser2App->nuFreq * val)) >> (pUser2App->nFreqShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : User2AppCurTrans(s32 val)
 * ˵��     : struUser2App Ƶ��ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 User2AppCurTrans(PSTR_TransCoefElement pUser2AppCur,float val)
{
    s32 ax;
    ax = ((s32)(pUser2AppCur->nuCur * val)) >> (pUser2AppCur->nCurShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : User2AppVolTrans(PSTR_TransCoefElement pUser2AppVolt,float val)
 * ˵��     : User2App ��ѹת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 User2AppVolTrans(PSTR_TransCoefElement pUser2AppVolt,float val)
{
    s32 ax;
    ax = ((s32)(pUser2AppVolt->nuVol* val)) >> (pUser2AppVolt->nVolShftNum);
    return (ax);
}

// App 2 core trans

/*****************************************************************************
 * ������   : App2CoreFreqTrans(s32 val)
 * ˵��     : App2Core Ƶ��ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 App2CoreFreqTrans(PSTR_TransCoefElement pApp2CoreFreq,s32 val)
{
    s32 ax;
    ax = (s32)pApp2CoreFreq->nuFreq * val >> (pApp2CoreFreq->nFreqShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : App2CoreCurTrans(s32 val)
 * ˵��     : App2Core ����ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 App2CoreCurTrans(PSTR_TransCoefElement pApp2CoreCur,s32 val)
{
    s32 ax;
    ax = (s32)pApp2CoreCur->nuCur * val >> (pApp2CoreCur->nCurShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : App2CoreVoltTrans(PSTR_TransCoefElement pApp2CoreVolt,s32 val)
 * ˵��     : App2Core ��ѹת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 App2CoreVoltTrans(PSTR_TransCoefElement pApp2CoreVolt,s32 val)
{
    s32 ax;
    ax = (s32)pApp2CoreVolt->nuVol * val >> (pApp2CoreVolt->nVolShftNum);
    return (ax);
}

// Core 2 App trans
/*****************************************************************************
 * ������   : Core2AppFreqTrans(s32 val)
 * ˵��     : Core2App Ƶ��ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 Core2AppFreqTrans(PSTR_TransCoefElement pCore2AppFreq,s32 val)
{
    s32 ax;
    ax = (s32)pCore2AppFreq->nuFreq * val >> (pCore2AppFreq->nFreqShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : Core2AppCurTrans(PSTR_TransCoefElement pCore2AppCur,s32 val)
 * ˵��     : Core2App ����ת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 Core2AppCurTrans(PSTR_TransCoefElement pCore2AppCur,s32 val)
{
    s32 ax;
    ax = (s32)pCore2AppCur->nuCur * val >> (pCore2AppCur->nCurShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : Core2AppVolTrans(PSTR_TransCoefElement pCore2AppVolt,s32 val)
 * ˵��     : Core2App ��ѹת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 Core2AppVolTrans(PSTR_TransCoefElement pCore2AppVolt,s32 val)
{
	s32 ax;
    ax = (s32)pCore2AppVolt->nuVol * val >> (pCore2AppVolt->nVolShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : Core2AppPowerTrans(PSTR_TransCoefElement pCore2AppPower,s16 val)
 * ˵��     : Core2App ��ѹת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s32 Core2AppPowerTrans(PSTR_TransCoefElement pCore2AppPower,s16 val)
{
    s32 ax;
    ax = (s32)pCore2AppPower->nuPower * val >> (pCore2AppPower->nPowerShftNum);
    return (ax);
}

/*****************************************************************************
 * ������   : s16 App2UsrFreqTrans(PSTR_TransCoefElement pUsr2AppFreq,s32 val) 
 * ˵��     : User2Core ��ѹת�� 
 * ���˼· ��1.����ϵͳת��
 * ����     ����
 * ����ֵ   ����
 * �޸�ʱ�� ��2020.08.17
 *****************************************************************************/
s16 App2UsrFreqTrans(PSTR_TransCoefElement pUsr2AppFreq,s32 val)
{
    s16 ax;
    ax = (s16)(val/pUsr2AppFreq->nuFreq);
    return (ax);
}

/************************ (C) COPYRIGHT LINKO SEMICONDUCTOR **********************/
/* ------------------------------END OF FILE------------------------------------ */






////////////////////


