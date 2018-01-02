/*****************************************************
** �ļ�����  message_broadcast_device_online_status.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ������豸����״̬�㲥��Ϣ
** �������ڣ�2015/08/05
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/
#ifndef _MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_
#define _MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_

#include "message/message_get_device_online_status_rep.h"

class CMessageBroadcastDeviceOnlineStatus : public CMessageGetDeviceOnlineStatusRep
{
public:
    CMessageBroadcastDeviceOnlineStatus();

    virtual ~CMessageBroadcastDeviceOnlineStatus();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);
};

#endif//!_MESSAGE_BROADCAST_DEVICE_ONLINE_STATUS_H_