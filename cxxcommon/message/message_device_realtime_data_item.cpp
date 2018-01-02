#include "message_device_realtime_data_item.h"




CMessageDeviceRealtimeDataItem::CMessageDeviceRealtimeDataItem()
{
    static_assert(sizeof(T_MSG_DEVICE_REALTIME_DATA_ITEM) == 12, "sizeof(T_MSG_DEVICE_REALTIME_DATA_ITEM) != 12");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageDeviceRealtimeDataItem::~CMessageDeviceRealtimeDataItem()
{

}

std::size_t CMessageDeviceRealtimeDataItem::Length()
{
    return sizeof(m_struct);
}

bool CMessageDeviceRealtimeDataItem::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU16(m_struct.channelNo);
    out.WriteU16(m_struct.datatype);
    for (int i = 0; i < 7; i++) {
        out.WriteU8(m_struct.data[i]);
    }
    out.WriteU8(m_struct.reserved);
    return true;
}

bool CMessageDeviceRealtimeDataItem::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.channelNo = in.ReadU16();
    m_struct.datatype = in.ReadU16();
    for (int i = 0; i < 7; i++) {
        m_struct.data[i] = in.ReadU8();
    }
    m_struct.reserved = in.ReadU8();
    return true;
}

QString CMessageDeviceRealtimeDataItem::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDeviceRealtimeDataItem] { ";
    stream << "[ channelNo:" << m_struct.channelNo << "], ";
    stream << "[ datatype:" << m_struct.datatype << "], ";
    for (int i = 0; i < sizeof(m_struct.data); i++)
    {
        stream << QString("[ data[%1]:%2]").arg(i).arg(m_struct.data[i]) << "], ";
    }
    stream << "[ reserved:" << m_struct.reserved << "], ";
    stream << "}";

    return qstr;
}
