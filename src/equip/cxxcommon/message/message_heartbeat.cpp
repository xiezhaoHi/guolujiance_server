#include "message_heartbeat.h"
#include "message/message_header.h"


CMessageHeartbeat::CMessageHeartbeat()
{
    static_assert(sizeof(T_MSG_HEARTBEAT) == 8, "sizeof(T_MSG_HEARTBEAT) != 8");
    memset(&m_struct, 0x00, sizeof(m_struct));
}

CMessageHeartbeat::~CMessageHeartbeat()
{

}

std::size_t CMessageHeartbeat::Length()
{
    return sizeof(m_struct);
}

bool CMessageHeartbeat::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU64(m_struct.heartbeatTime);
    return true;
}

bool CMessageHeartbeat::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.heartbeatTime = in.ReadU64();
    return true;
}

QString CMessageHeartbeat::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageHeartbeat] { ";
    stream << "[ heartbeatTime:" << (long long)m_struct.heartbeatTime << "], ";
    stream << "}";

    return qstr;
}

IMessage * CMessageHeartbeat::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_HEARTBEAT)) {
        return new CMessageHeartbeat();
    }
    return NULL;
}
