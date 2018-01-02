#ifndef _MESSAGE_DEVICE_ITEM_INFO_H_
#define _MESSAGE_DEVICE_ITEM_INFO_H_

#include "message/i_message.h"

class CMessageDeviceItemInfo : public IMessage
{
public:
    CMessageDeviceItemInfo();

    virtual ~CMessageDeviceItemInfo();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

public:

    T_MSG_DEVICE_INFO_LIST_ITEM m_struct;
};

#endif//!_MESSAGE_DEVICE_ITEM_INFO_H_