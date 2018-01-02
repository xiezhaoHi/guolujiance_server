#include "message_get_device_list_rep.h"
#include "message/message_header.h"

CMessageGetDeviceListRep::CMessageGetDeviceListRep()
{
    static_assert(sizeof(T_MSG_DEVICES_LIST_REP) == 2, "sizeof(T_MSG_DEVICES_LIST_REP) != 2");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICES_LIST_REP));
}

CMessageGetDeviceListRep::~CMessageGetDeviceListRep()
{
    for (QList<CMessageDevicesListItem *>::iterator it = m_lstDevices.begin();
        it != m_lstDevices.end(); )
    {
        delete (*it);
        it = m_lstDevices.erase(it);
    }
}


std::size_t CMessageGetDeviceListRep::Length()
{
    m_struct.deviceCount = m_lstDevices.length();

    std::size_t len =  sizeof(T_MSG_DEVICES_LIST_REP);
    for (QList<CMessageDevicesListItem *>::iterator it = m_lstDevices.begin();
        it != m_lstDevices.end();
        it++)
    {
        Q_ASSERT(*it);

        len += (*it)->Length();
    }
    return len;
}

bool CMessageGetDeviceListRep::WriteToOutputByteArray(OutputByteArray & out)
{
    m_struct.deviceCount = m_lstDevices.length();

    out.WriteU16(m_struct.deviceCount);
    for (int i = 0; i < m_struct.deviceCount; i++)
    {
        m_lstDevices.at(i)->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDeviceListRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.deviceCount = in.ReadU16();
    for (int i = 0; i < m_struct.deviceCount; i++)
    {
        CMessageDevicesListItem *item = new CMessageDevicesListItem();
        item->ReadFromInputByteArray(in);
        m_lstDevices.append(item);
    }
    return true;
}

QString CMessageGetDeviceListRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDeviceListRep] { ";
    stream << "[ deviceCount:" << m_struct.deviceCount << "], ";
    for (QList<CMessageDevicesListItem *>::iterator it = m_lstDevices.begin();
        it != m_lstDevices.end();
        it++)
    {
        Q_ASSERT(*it);

        stream << (*it)->ToString();
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDeviceListRep::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICES_LIST_REP))
    {
        return new CMessageGetDeviceListRep();
    }
    return NULL;
}

