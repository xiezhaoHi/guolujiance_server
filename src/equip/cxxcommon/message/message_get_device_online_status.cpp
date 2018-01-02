#include "message_get_device_online_status.h"
#include "message/message_header.h"
#include "message/message_device_id.h"


CMessageGetDeviceOnlineStatus::CMessageGetDeviceOnlineStatus()
{
    static_assert(sizeof(T_MSG_DEVICE_ONLINE_STATE_REQ) == 2, "sizeof(T_MSG_DEVICE_ONLINE_STATE_REQ) != 2");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_ONLINE_STATE_REQ));
}

CMessageGetDeviceOnlineStatus::~CMessageGetDeviceOnlineStatus()
{
    for (QList<CMessageDeviceID *>::iterator it = m_lstDeviceIDs.begin();
        it != m_lstDeviceIDs.end(); )
    {
        delete (*it);
        it = m_lstDeviceIDs.erase(it);
    }
}

std::size_t CMessageGetDeviceOnlineStatus::Length()
{
    m_struct.deviceCount = m_lstDeviceIDs.length();
    std::size_t len = sizeof(T_MSG_DEVICE_ONLINE_STATE_REQ);
    for (CMessageDeviceID * pDeviceID : m_lstDeviceIDs) {
        len += pDeviceID->Length();
    }
    return len;
}

bool CMessageGetDeviceOnlineStatus::WriteToOutputByteArray(OutputByteArray & out)
{
    m_struct.deviceCount = m_lstDeviceIDs.length();
    out.WriteU16(m_struct.deviceCount);
    for (CMessageDeviceID * pDeviceID : m_lstDeviceIDs) {
        pDeviceID->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDeviceOnlineStatus::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.deviceCount = in.ReadU16();
    for (int i = 0; i < m_struct.deviceCount; i++) {
        CMessageDeviceID * pDeviceID = new CMessageDeviceID();
        pDeviceID->ReadFromInputByteArray(in);
        m_lstDeviceIDs.push_back(pDeviceID);
    }
    return true;
}

QString CMessageGetDeviceOnlineStatus::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDeviceOnlineStatus]{ ";
    stream << "[ deviceCount:" << m_struct.deviceCount << "], ";
    for (CMessageDeviceID * pDeviceID : m_lstDeviceIDs) {
        stream << pDeviceID->ToString();
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceOnlineStatus::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICE_ONLINE_STATE_REQ)) {
        return new CMessageGetDeviceOnlineStatus();
    }
    return NULL;
}
