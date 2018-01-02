#ifndef _MESSAGE_GET_DEVICES_INFO_REQ_H_
#define _MESSAGE_GET_DEVICES_INFO_REQ_H_

#include "message/i_message.h"
#include <QList>

class CMessageDeviceID;

class CMessageGetDevicesInfoReq : public IMessage
{
public:

    CMessageGetDevicesInfoReq();

    virtual ~CMessageGetDevicesInfoReq();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_DEVICES_INFO_LIST_REQ m_struct;
    QList<CMessageDeviceID *>   m_lstDeviceID;
};

#endif//!_MESSAGE_GET_DEVICES_INFO_REQ_H_