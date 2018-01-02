#ifndef _MESSAGE_DEVICE_REALTIME_DATA_H_
#define _MESSAGE_DEVICE_REALTIME_DATA_H_

#include "message/i_message.h"
#include <memory>

class CMessageDeviceRealtimeData : public IMessage
{
public:

    CMessageDeviceRealtimeData();

    CMessageDeviceRealtimeData(const CMessageDeviceRealtimeData & obj);

    virtual ~CMessageDeviceRealtimeData();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    CMessageDeviceRealtimeData& operator= (const CMessageDeviceRealtimeData & val);

public:

    T_MSG_DEVICE_REALTIME_DATA m_struct;
};

typedef std::shared_ptr<CMessageDeviceRealtimeData> message_device_realtime_data_ptr;

#endif//!_MESSAGE_DEVICE_REALTIME_DATA_H_