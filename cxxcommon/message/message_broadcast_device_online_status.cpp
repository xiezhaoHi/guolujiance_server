#include "message_broadcast_device_online_status.h"




CMessageBroadcastDeviceOnlineStatus::CMessageBroadcastDeviceOnlineStatus()
{

}

CMessageBroadcastDeviceOnlineStatus::~CMessageBroadcastDeviceOnlineStatus()
{

}

std::size_t CMessageBroadcastDeviceOnlineStatus::Length()
{
    return CMessageGetDeviceOnlineStatusRep::Length();
}

bool CMessageBroadcastDeviceOnlineStatus::WriteToOutputByteArray(OutputByteArray & out)
{
    return CMessageGetDeviceOnlineStatusRep::WriteToOutputByteArray(out);
}

bool CMessageBroadcastDeviceOnlineStatus::ReadFromInputByteArray(InputByteArray & in)
{
    return CMessageGetDeviceOnlineStatusRep::ReadFromInputByteArray(in);
}

QString CMessageBroadcastDeviceOnlineStatus::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageBroadcastDeviceOnlineStatus]{ ";
    stream << CMessageGetDeviceOnlineStatusRep::ToString();
    stream << "}";

    return qstr;
}

IMessage * CMessageBroadcastDeviceOnlineStatus::CreateFromHeader(const CMessageHeader & header)
{
    return CMessageGetDeviceOnlineStatusRep::CreateFromHeader(header);
}
