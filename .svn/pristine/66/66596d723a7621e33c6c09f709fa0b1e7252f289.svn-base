#ifndef _MESSAGE_BROADCAST_DEVICE_REALTIME_DATA_H_
#define _MESSAGE_BROADCAST_DEVICE_REALTIME_DATA_H_

#include "message/i_message.h"

class CMessageBroadcastDeviceRealtimeData : public IMessage
{
public:

    CMessageBroadcastDeviceRealtimeData();

    virtual ~CMessageBroadcastDeviceRealtimeData();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

public:

    T_MSG_BROADCAST_DEVICE_REALTIME_DATA m_struct;
};


#endif//!_MESSAGE_BROADCAST_DEVICE_REALTIME_DATA_H_