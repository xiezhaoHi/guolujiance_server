#ifndef _MESSAGE_DEVICE_REALTIME_BATCH_DATA_H_
#define _MESSAGE_DEVICE_REALTIME_BATCH_DATA_H_


#include "message/i_message.h"
#include <QVector>


class CMessageDeviceRealtimeDataItem;
typedef std::shared_ptr<CMessageDeviceRealtimeDataItem> message_device_realtime_data_item_ptr;

class CMessageDeviceRealtimeData;
typedef std::shared_ptr<CMessageDeviceRealtimeData> message_device_realtime_data_ptr;

class CMessageDeviceRealtimeBatchedData : public IMessage
{
public:
    CMessageDeviceRealtimeBatchedData();

    virtual ~CMessageDeviceRealtimeBatchedData();

    virtual std::size_t Length();

    virtual bool WriteToOutputByteArray(OutputByteArray & out);

    virtual bool ReadFromInputByteArray(InputByteArray & in);

    virtual QString ToString();

    static IMessage * CreateFromHeader(const CMessageHeader & header);

    void SplitBatchedMessage(QVector<message_device_realtime_data_ptr> & vec);

public:
    T_MSG_DEVICE_REALTIME_BATCHED_DATA          m_struct;
    QVector<message_device_realtime_data_item_ptr>   m_vecDataItems;
};



#endif//!_MESSAGE_DEVICE_REALTIME_BATCH_DATA_H_