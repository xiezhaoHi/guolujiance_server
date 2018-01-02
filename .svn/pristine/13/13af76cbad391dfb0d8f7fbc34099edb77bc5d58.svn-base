#include "message_broadcast_device_realtime_data.h"
#include "message/message_broadcast_device_realtime_data.h"
#include "message/message_device_realtime_data.h"
#include "message/message_header.h"


CMessageBroadcastDeviceRealtimeData::CMessageBroadcastDeviceRealtimeData()
{
    static_assert(sizeof(T_MSG_BROADCAST_DEVICE_REALTIME_DATA) == 116, "sizeof(T_MSG_BROADCAST_DEVICE_REALTIME_DATA) != 116");
    memset(&m_struct, 0x00, sizeof(T_MSG_BROADCAST_DEVICE_REALTIME_DATA));
}

CMessageBroadcastDeviceRealtimeData::~CMessageBroadcastDeviceRealtimeData()
{

}

std::size_t CMessageBroadcastDeviceRealtimeData::Length()
{
    return sizeof(T_MSG_BROADCAST_DEVICE_REALTIME_DATA);
}

bool CMessageBroadcastDeviceRealtimeData::WriteToOutputByteArray(OutputByteArray & out)
{
    for (int i = 0; i < sizeof(m_struct.rigId); i++) {
        out.WriteU8(m_struct.rigId[i]);
    }
    for (int i = 0; i < sizeof(m_struct.deviceId); i++) {
        out.WriteU8(m_struct.deviceId[i]);
    }

    out.WriteD64(m_struct.data);

    CMessageDeviceRealtimeData deviceRealtimeData;
    memcpy(&(deviceRealtimeData.m_struct), &(m_struct.realtimeData), deviceRealtimeData.Length());
    deviceRealtimeData.WriteToOutputByteArray(out);

    return true;
}

bool CMessageBroadcastDeviceRealtimeData::ReadFromInputByteArray(InputByteArray & in)
{
    for (int i = 0; i < sizeof(m_struct.rigId); i++) {
        m_struct.rigId[i] = in.ReadU8();
    }
    for (int i = 0; i < sizeof(m_struct.deviceId); i++) {
        m_struct.deviceId[i] = in.ReadU8();
    }

    m_struct.data = in.ReadD64();

    CMessageDeviceRealtimeData deviceRealtimeData;
    deviceRealtimeData.ReadFromInputByteArray(in);
    memcpy(&(m_struct.realtimeData), &(deviceRealtimeData.m_struct), deviceRealtimeData.Length());

    return true;
}

QString CMessageBroadcastDeviceRealtimeData::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    CMessageDeviceRealtimeData deviceRealtimeData;
    memcpy(&(deviceRealtimeData.m_struct), &(m_struct.realtimeData), deviceRealtimeData.Length());

    stream << "[CMessageBroadcastDeviceRealtimeData] { ";
    stream << "[ deviceId:" << QString::fromLocal8Bit(((char *)m_struct.rigId), sizeof(m_struct.rigId)) << "], ";
    stream << deviceRealtimeData.ToString();
    stream << "}";

    return qstr;

}

IMessage * CMessageBroadcastDeviceRealtimeData::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_BROADCAST_DEVICE_REALTIME_DATA)) {
        return new CMessageBroadcastDeviceRealtimeData();
    }
    return NULL;
}
