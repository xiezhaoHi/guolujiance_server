#ifndef _MESSAGE_GET_DEVICE_LIST_REP_H_
#define _MESSAGE_GET_DEVICE_LIST_REP_H_

#include "message/i_message.h"
#include "message/message_devices_list_item.h"
#include <QList>

class CMessageGetDeviceListRep : public IMessage
{
public:
    
    CMessageGetDeviceListRep();

    virtual ~CMessageGetDeviceListRep();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:

    T_MSG_DEVICES_LIST_REP              m_struct;
    QList<CMessageDevicesListItem *>    m_lstDevices;
};


#endif//!_MESSAGE_GET_DEVICE_LIST_REP_H_