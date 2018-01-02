/*****************************************************
** �ļ�����  message_get_device_online_status_rep_item.h
** ��  Ȩ��  XXX ��Ȩ����
** ��  ����  
** ���ݼ�����
** �������ڣ�2015/08/05
** �����ˣ�  ����
** �޸ļ�¼��
   ����        �汾      �޸���    �޸����� 
    
*****************************************************/
#ifndef _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_
#define _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_

#include "message/i_message.h"


class CMessageDeviceID;

class CMessageGetDeviceOnlineStatusRepItem : public IMessage
{
public:
    CMessageGetDeviceOnlineStatusRepItem();

    virtual ~CMessageGetDeviceOnlineStatusRepItem();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

public:
    T_MSG_DEVICE_ONLINE_STATUS_REP_ITEM m_struct;
};


#endif//!_MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_ITEM_H_