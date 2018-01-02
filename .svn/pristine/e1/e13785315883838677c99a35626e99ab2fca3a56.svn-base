#ifndef _MESSAGE_GET_DEVICE_ONLINE_STATUS_H_
#define _MESSAGE_GET_DEVICE_ONLINE_STATUS_H_

#include "message/i_message.h"
#include <QList>

class CMessageDeviceID;

class CMessageGetDeviceOnlineStatus : public IMessage
{
public:
    CMessageGetDeviceOnlineStatus();

    virtual ~CMessageGetDeviceOnlineStatus();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_DEVICE_ONLINE_STATE_REQ   m_struct;
    QList<CMessageDeviceID *>       m_lstDeviceIDs;
};

#endif//!_MESSAGE_GET_DEVICE_ONLINE_STATUS_H_