#ifndef _MESSAGE_DEVICE_REALTIME_DATA_ITEM_H_
#define _MESSAGE_DEVICE_REALTIME_DATA_ITEM_H_


#include "message/i_message.h"
#include <memory>

class CMessageDeviceRealtimeDataItem : public IMessage
{
public:
    CMessageDeviceRealtimeDataItem();

    virtual ~CMessageDeviceRealtimeDataItem();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

public:
    T_MSG_DEVICE_REALTIME_DATA_ITEM m_struct;
};


typedef std::shared_ptr<CMessageDeviceRealtimeDataItem> message_device_realtime_data_item_ptr;


#endif//!_MESSAGE_DEVICE_REALTIME_DATA_ITEM_H_