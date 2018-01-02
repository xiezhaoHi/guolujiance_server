#ifndef _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_H_
#define _MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_H_

#include "message/i_message.h"
#include <QList>

class CMessageGetDeviceOnlineStatusRepItem;

class CMessageGetDeviceOnlineStatusRep : public IMessage
{
public:
    CMessageGetDeviceOnlineStatusRep();

    virtual ~CMessageGetDeviceOnlineStatusRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_DEVICE_ONLINE_STATUS_REP                  m_struct;
    QList<CMessageGetDeviceOnlineStatusRepItem *>   m_lstDeviceIDs;
};



#endif//!_MESSAGE_GET_DEVICE_ONLINE_STATUS_REP_H_