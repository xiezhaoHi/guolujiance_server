#include "message_device_realtime_batched_data.h"
#include "message/message_device_realtime_data_item.h"
#include "message/message_device_realtime_data.h"
#include "message/message_header.h"



CMessageDeviceRealtimeBatchedData::CMessageDeviceRealtimeBatchedData()
{
    static_assert(sizeof(T_MSG_DEVICE_REALTIME_BATCHED_DATA) == 28, "sizeof(T_MSG_DEVICE_REALTIME_BATCHED_DATA) != 28");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageDeviceRealtimeBatchedData::~CMessageDeviceRealtimeBatchedData()
{
    m_vecDataItems.clear();
}

std::size_t CMessageDeviceRealtimeBatchedData::Length()
{
    m_struct.dataItemCount = m_vecDataItems.length();

    std::size_t size = sizeof(m_struct);
    for (int i = 0; i < m_vecDataItems.length(); i++) {
        size += m_vecDataItems.at(i)->Length();
    }

    return size;
}

bool CMessageDeviceRealtimeBatchedData::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU64(m_struct.timestamp);
    out.WriteD64(m_struct.longitude);
    out.WriteD64(m_struct.latitude);
    out.WriteU32(m_struct.dataItemCount);
    for (int i = 0; i < m_struct.dataItemCount; i++)
    {
        Q_ASSERT(m_vecDataItems.length() > i);
        m_vecDataItems.at(i)->WriteToOutputByteArray(out);
    }

    return true;
}

bool CMessageDeviceRealtimeBatchedData::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.timestamp = in.ReadU64();
    m_struct.longitude = in.ReadD64();
    m_struct.latitude = in.ReadD64();
    m_struct.dataItemCount = in.ReadU32();
    for (int i = 0; i < m_struct.dataItemCount; i++) {
        message_device_realtime_data_item_ptr item = std::make_shared<CMessageDeviceRealtimeDataItem>();
        item->ReadFromInputByteArray(in);
        m_vecDataItems.push_back(item);
    }
    return true;
}

QString CMessageDeviceRealtimeBatchedData::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDeviceRealtimeBatchedData] { ";
    stream << "[ timestamp:" << m_struct.timestamp << "] ";
    stream << "[ longitude:" << m_struct.longitude << "] ";
    stream << "[ latitude:" << m_struct.latitude << "] ";
    stream << "[ dataItemCount:" << m_struct.dataItemCount << "] ";
    for (int i = 0; i < m_struct.dataItemCount; i++) {
        stream << m_vecDataItems.at(i)->ToString();
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageDeviceRealtimeBatchedData::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICE_REALTIME_BATCHED_DATA)) {
        return new CMessageDeviceRealtimeBatchedData();
    }
    return NULL;
}

void CMessageDeviceRealtimeBatchedData::SplitBatchedMessage(QVector<message_device_realtime_data_ptr> & vec)
{
    // 将存放了批量数据的消息拆分为单个消息进行存储
    for (int i = 0; i < m_struct.dataItemCount; i++)
    {
        message_device_realtime_data_item_ptr pDataItem = m_vecDataItems.at(i);
        message_device_realtime_data_ptr pRealtimeData = std::make_shared<CMessageDeviceRealtimeData>();
        pRealtimeData->m_struct.timestamp = m_struct.timestamp;
        pRealtimeData->m_struct.longitude = m_struct.longitude;
        pRealtimeData->m_struct.latitude = m_struct.latitude;
        pRealtimeData->m_struct.channelNo = pDataItem->m_struct.channelNo;
        pRealtimeData->m_struct.datatype = pDataItem->m_struct.datatype;
        memcpy(pRealtimeData->m_struct.data_, pDataItem->m_struct.data, sizeof(pRealtimeData->m_struct.data_));
        pRealtimeData->m_struct.reserved = pDataItem->m_struct.reserved;
        vec.push_back(pRealtimeData);
    }
}
