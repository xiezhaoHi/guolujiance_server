#include "message_get_device_online_status_rep.h"
#include "message/message_header.h"
#include "message/message_get_device_online_status_rep_item.h"



CMessageGetDeviceOnlineStatusRep::CMessageGetDeviceOnlineStatusRep()
{
    static_assert(sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP) == 2, "sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP) != 2");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP));
}

CMessageGetDeviceOnlineStatusRep::~CMessageGetDeviceOnlineStatusRep()
{
    for (QList<CMessageGetDeviceOnlineStatusRepItem *>::iterator it = m_lstDeviceIDs.begin();
        it != m_lstDeviceIDs.end(); )
    {
        delete (*it);
        it = m_lstDeviceIDs.erase(it);
    }
}

std::size_t CMessageGetDeviceOnlineStatusRep::Length()
{
    m_struct.itemCount = m_lstDeviceIDs.length();
    std::size_t len = sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP);
    for (CMessageGetDeviceOnlineStatusRepItem * pItem : m_lstDeviceIDs) {
        len += pItem->Length();
    }
    return len;
}

bool CMessageGetDeviceOnlineStatusRep::WriteToOutputByteArray(OutputByteArray & out)
{
    m_struct.itemCount = m_lstDeviceIDs.length();
    out.WriteU16(m_struct.itemCount);
    for (CMessageGetDeviceOnlineStatusRepItem * pItem : m_lstDeviceIDs) {
        pItem->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDeviceOnlineStatusRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.itemCount = in.ReadU16();
    for (int i = 0; i < m_struct.itemCount; i++) {
        CMessageGetDeviceOnlineStatusRepItem * pItem = new CMessageGetDeviceOnlineStatusRepItem();
        pItem->ReadFromInputByteArray(in);
        m_lstDeviceIDs.push_back(pItem);
    }
    return true;
}

QString CMessageGetDeviceOnlineStatusRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDeviceOnlineStatusRep]{ ";
    stream << "[ itemCount:" << m_struct.itemCount<< "], ";
    for (CMessageGetDeviceOnlineStatusRepItem * pItem : m_lstDeviceIDs) {
        stream << pItem->ToString();
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceOnlineStatusRep::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICE_ONLINE_STATUS_REP)) {
        return new CMessageGetDeviceOnlineStatusRep();
    }
    return NULL;
}
