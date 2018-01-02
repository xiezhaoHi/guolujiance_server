#include "message_get_devices_info_req.h"
#include "message/message_device_id.h"
#include "message/message_header.h"



CMessageGetDevicesInfoReq::CMessageGetDevicesInfoReq()
{
    static_assert(sizeof(T_MSG_DEVICES_INFO_LIST_REQ) == 1, "sizeof(T_MSG_DEVICES_INFO_LIST_REQ) != 1");
    memset(&m_struct, 0x00, sizeof(T_MSG_DEVICES_INFO_LIST_REQ));
}

CMessageGetDevicesInfoReq::~CMessageGetDevicesInfoReq()
{
    for (QList<CMessageDeviceID *>::iterator it = m_lstDeviceID.begin();
        it != m_lstDeviceID.end(); )
    {
        delete (*it);
        it = m_lstDeviceID.erase(it);
    }
}

std::size_t CMessageGetDevicesInfoReq::Length()
{
    m_struct.deviceCount = m_lstDeviceID.length();
    std::size_t len = sizeof(T_MSG_DEVICES_INFO_LIST_REQ);
    for (CMessageDeviceID * deviceID : m_lstDeviceID) {
        len += deviceID->Length();
    }
    return len;
}

bool CMessageGetDevicesInfoReq::WriteToOutputByteArray(OutputByteArray & out)
{
    out.WriteU8(m_struct.deviceCount);
    for (CMessageDeviceID * deviceID : m_lstDeviceID) {
        Q_ASSERT(deviceID);

        deviceID->WriteToOutputByteArray(out);
    }
    return true;
}

bool CMessageGetDevicesInfoReq::ReadFromInputByteArray(InputByteArray & in)
{
    m_struct.deviceCount = in.ReadU8();

    m_lstDeviceID.clear();
    for(int i = 0; i < m_struct.deviceCount; i++) {
        CMessageDeviceID * pDeviceID = new CMessageDeviceID();
        pDeviceID->ReadFromInputByteArray(in);
        m_lstDeviceID.push_back(pDeviceID);
    }
    return true;
}

QString CMessageGetDevicesInfoReq::ToString()
{
    QString qstr;
    QTextStream stream(&qstr);

    stream << "[CMessageGetDevicesInfoReq] { ";
    stream << "[ deviceCount:" << m_struct.deviceCount << "], ";
    for (int i = 0; i < m_struct.deviceCount; i++) {
        CMessageDeviceID * pMessageDeviceID = m_lstDeviceID.at(i);
        stream << pMessageDeviceID->ToString();
        stream << ", ";
    }
    stream << "}";

    return qstr;
}

IMessage * CMessageGetDevicesInfoReq::CreateFromHeader(const CMessageHeader & header)
{
    if (header.m_struct.bodyLength >= sizeof(T_MSG_DEVICES_INFO_LIST_REQ)) {
        return new CMessageGetDevicesInfoReq();
    }
    return NULL;
}
