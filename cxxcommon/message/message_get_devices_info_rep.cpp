#include "message_get_devices_info_rep.h"
#include "message/message_header.h"


CMessageGetDevicesInfoRep::CMessageGetDevicesInfoRep()
{
    static_assert(sizeof(T_MSG_DEVICES_INFO_LIST_REP) == 1, "sizeof(T_MSG_DEVICES_INFO_LIST_REP) != 1");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICES_INFO_LIST_REP));
}

CMessageGetDevicesInfoRep::~CMessageGetDevicesInfoRep()
{
    for (QList<CMessageDeviceItemInfo *>::iterator it = m_lstDeviceItemInfo.begin();
        it != m_lstDeviceItemInfo.end();)
    {
        delete (*it);
        it = m_lstDeviceItemInfo.erase(it);
    }
}


std::size_t CMessageGetDevicesInfoRep::Length()
{
    m_struct.deviceCount = m_lstDeviceItemInfo.length();
    std::size_t len = sizeof(T_MSG_DEVICES_INFO_LIST_REP);
    for (CMessageDeviceItemInfo * deviceInfo : m_lstDeviceItemInfo) {
        len += deviceInfo->Length();
    }
    return len;
}

bool CMessageGetDevicesInfoRep::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU8(m_struct.deviceCount);
    for (CMessageDeviceItemInfo * deviceInfo : m_lstDeviceItemInfo) {
        deviceInfo->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDevicesInfoRep::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.deviceCount = in.ReadU8();
    for (int i = 0; i < m_struct.deviceCount; i++) {
        CMessageDeviceItemInfo * pDeviceInfo = new CMessageDeviceItemInfo();
        pDeviceInfo->ReadFromInputByteArray(in);
        m_lstDeviceItemInfo.push_back(pDeviceInfo);
    }
    return true;
}

QString CMessageGetDevicesInfoRep::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDevicesInfoRep]{ ";
    stream << "[ deviceCount:" << m_struct.deviceCount << "] ";
    for (CMessageDeviceItemInfo * pItemInfo : m_lstDeviceItemInfo) {
        stream << pItemInfo->ToString();
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDevicesInfoRep::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICES_INFO_LIST_REP)) {
        return new CMessageGetDevicesInfoRep();
    }
    return NULL;
}
