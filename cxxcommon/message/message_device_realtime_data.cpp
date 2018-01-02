#include "message_device_realtime_data.h"
#include "message/message_header.h"



CMessageDeviceRealtimeData::CMessageDeviceRealtimeData()
{
    static_assert(sizeof(T_MSG_DEVICE_REALTIME_DATA) == 36, "sizeof(T_MSG_DEVICE_REALTIME_DATA) != 36");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_REALTIME_DATA));
}

CMessageDeviceRealtimeData::CMessageDeviceRealtimeData(const CMessageDeviceRealtimeData & obj)
{
    memcpy(&m_struct, &(obj.m_struct), sizeof(T_MSG_DEVICE_REALTIME_DATA));
}

CMessageDeviceRealtimeData::~CMessageDeviceRealtimeData()
{

}

std::size_t CMessageDeviceRealtimeData::Length()
{
    return sizeof(T_MSG_DEVICE_REALTIME_DATA);
}

bool CMessageDeviceRealtimeData::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU16(m_struct.channelNo);
    out.WriteU16(m_struct.datatype);
    out.WriteU64(m_struct.timestamp);
    out.WriteD64(m_struct.longitude);
    out.WriteD64(m_struct.latitude);
    out.WriteU8(m_struct.reserved);
    for (int i = 0; i < sizeof(m_struct.data_); i++) {
        out.WriteU8(m_struct.data_[i]);
    }

    return true;
}

bool CMessageDeviceRealtimeData::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.channelNo = in.ReadU16();
    m_struct.datatype = in.ReadU16();
    m_struct.timestamp = in.ReadU64();
    m_struct.longitude = in.ReadD64();
    m_struct.latitude = in.ReadD64();
    m_struct.reserved = in.ReadU8();
    for (int i = 0; i < sizeof(m_struct.data_); i++) {
        m_struct.data_[i] = in.ReadU8();
    }
    
    return true;
}

QString CMessageDeviceRealtimeData::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageDeviceRealtimeData] { ";
    stream << "[ channelNo:" << m_struct.channelNo << "], ";
    stream << "[ datatype:" << m_struct.datatype << "], ";
    stream << "[ timestamp:" << m_struct.timestamp << "] ";
    stream << "[ longitude:" << m_struct.longitude << "] ";
    stream << "[ latitude:" << m_struct.latitude << "] ";
    //stream << "[ data:" << m_struct.data << "] ";
    stream << "}";

    return qstr;
}

IMessage * CMessageDeviceRealtimeData::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICE_REALTIME_DATA))
    {
        return new CMessageDeviceRealtimeData();
    }
    return NULL;
}

CMessageDeviceRealtimeData& CMessageDeviceRealtimeData::operator=(const CMessageDeviceRealtimeData & val)
{
    memcpy(&m_struct, &(val.m_struct), sizeof(T_MSG_DEVICE_REALTIME_DATA));
    return *this;
}
