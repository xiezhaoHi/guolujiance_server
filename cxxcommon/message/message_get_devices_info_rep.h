#ifndef _MESSAGE_GET_DEVICE_INFO_REP_H_
#define _MESSAGE_GET_DEVICE_INFO_REP_H_

#include "message/i_message.h"
#include "message/message_device_item_info.h"
#include <QList>

class CMessageGetDevicesInfoRep : public IMessage
{
public:

    CMessageGetDevicesInfoRep();

    virtual ~CMessageGetDevicesInfoRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();
    
    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:
    T_MSG_DEVICES_INFO_LIST_REP     m_struct;
    QList<CMessageDeviceItemInfo *> m_lstDeviceItemInfo;
};

#endif//!_MESSAGE_GET_DEVICE_INFO_REP_H_